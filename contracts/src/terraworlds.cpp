#include "../include/terraworlds.hpp"

// ============================================================================================================
void terraworlds::setparams( 
                            const name& table_name,
                            uint64_t next_id, 
                            const name& next_owner,
                            uint64_t nft_count
                            ) 
{
    // permission can be active/owner or a custom
    // NOTE: custom permission is recommended
    require_auth(get_self());

    check( next_id > 0, "Stored next id must be positive" );
    check( nft_count > 0, "Stored nft count must be positive" );

    // instantiate the lastdist table
    lastdist_index lastdist_table(get_self(), get_self().value);
    auto lastdist_it = lastdist_table.find(table_name.value);

    if( lastdist_it == lastdist_table.end() ) {
        lastdist_table.emplace(get_self(), [&](auto& row) {
            row.table_name = table_name;
            row.next_id = next_id;
            row.next_owner = next_owner;
            if (nft_count != 0) row.nft_count = nft_count;
        });
    } 
    else {
        lastdist_table.modify(lastdist_it, get_self(), [&](auto& row){
            row.next_id = next_id;
            row.next_owner = next_owner;
            if (nft_count != 0) row.nft_count = nft_count;
        });
    }
}

// ============================================================================================================
void terraworlds::distribute( 
                            const name& table_name, 
                            uint32_t loop_count,
                            const string& token_transfer_memo
                            ) 
{
    // permission can be active/owner or a custom
    // NOTE: custom permission is recommended
    require_auth(get_self());

    check( loop_count > 0, "loop count must be positive" );
    check( token_transfer_memo.size() <= 256, "memo has more than 256 bytes" );

    // -----------------------------------------------------------------------------------
    // 1. match the stored (id & owner) with the next iterated available (id & owner) 
    // Get the stored id & owner
    lastdist_index lastdist_table(get_self(), get_self().value);
    auto lastdist_it = lastdist_table.find(table_name.value);

    check( lastdist_it != lastdist_table.end(), "There is no row with this table name in terraworlds::lastdist table" );

    // get the stored id, owner, nft_count
    auto next_id = lastdist_it->next_id;
    auto next_owner = lastdist_it->next_owner;
    auto nft_count = lastdist_it->nft_count;

    // Get the id & owner from "federation::landregs" table
    landregs_table landregs_table_table(federation_contract_ac, federation_contract_ac.value);
    auto landregs_it = landregs_table_table.find(next_id);

    check( landregs_it != landregs_table_table.end(), "The stored next id is not available in federation::landregs table" );
    check( next_owner == landregs_it->owner, "The owners for the 2 tables doesn\'t match" );

    // -----------------------------------------------------------------------------------
    // 2. calculate the tokens per NFT
    // read the balance from "alien.worlds::accounts" table
    accounts from_acnts( token_contract_ac, get_self().value );
    const auto& from = from_acnts.get( token_symbol.code().raw(), "no balance object found" );
    
    auto bal_self = from.balance.amount;
    
    check( bal_self > 0, "contract\'s balance must be positive" );

    // calc TLM token per NFT asset
    asset token_per_nft = asset(0, from.balance.symbol);
    token_per_nft.amount = bal_self/nft_count;

    // -----------------------------------------------------------------------------------
    // 3. iterate & store the owners & total amount to be transferred in (4)
    vector<name> owners_arr = {};
    vector<name>::iterator owners_arr_it;
    bool clear_status = false;
    map<name, asset> owners_map = {};
    map<name, asset>::iterator owners_map_it;

    while(owners_arr.size() <= loop_count) {
        // insert owner if not present into the owners_arr
        owners_arr_it = find(owners_arr.begin(), owners_arr.end(), landregs_it->owner);
        if(owners_arr_it == owners_arr.end()) {             // 'owner' not found
            owners_arr.emplace_back(landregs_it->owner);
        }

        // update owner into map with asset increased by token_per_nft
        owners_map_it = std::find_if(owners_map.begin(), owners_map.end(), 
                                    [&](auto& ms) { return ms.first == landregs_it->owner; });
        if(owners_map_it != owners_map.end()) {         // key found
            // increase the asset corresponding to the found key
            owners_map_it->second += token_per_nft;
        } else {
            // insert the key, value to the owners_map
            owners_map.insert(make_pair(landregs_it->owner, token_per_nft));
        }

        // increase the iterator if table end is not reached
        if( landregs_it != landregs_table_table.end() ) {
            ++landregs_it;
        } else {
            clear_status = true;
            break;
        }
    }

    // -----------------------------------------------------------------------------------
    // 4. transfer tokens to the owner present in map
    for( auto itr = owners_map.begin(); itr != owners_map.end(); ++itr) {
        action(
            permission_level{get_self(), "active"_n},
            "alien.worlds"_n,
            "transfer"_n,
            std::make_tuple(get_self(), itr->first, itr->second, token_transfer_memo)
        ).send();
    }
 
    // -----------------------------------------------------------------------------------
    // 5. store the next_id & next_owner into "terraworlds::lastdist" table next_row_index
    if (clear_status) {
        // erase the row in the lastdist table
        lastdist_table.erase(lastdist_it);
    } else {
        // modify the row next_id & next_owner
        lastdist_table.modify(lastdist_it, get_self(), [&](auto& row) {
            row.next_id = next_id;
            row.next_owner = next_owner;
        });
    }

}