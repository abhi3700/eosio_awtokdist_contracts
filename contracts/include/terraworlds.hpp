#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <vector>
#include <algorithm>
#include <string>
#include <map>


using eosio::contract;
using eosio::print;
using eosio::name;
using eosio::multi_index;
using eosio::const_mem_fun;
using eosio::indexed_by;
using eosio::asset;
using eosio::check;
using eosio::permission_level;
using eosio::datastream;
using eosio::current_time_point;
using eosio::action;
using eosio::same_payer;
using eosio::symbol;
using eosio::require_recipient;
using eosio::checksum256;
using eosio::action_wrapper;

using std::string;
using std::vector;
using std::map;
using std::make_pair;


CONTRACT terraworlds : public contract
{
private:
    const symbol token_symbol;
    const name token_contract_ac;
    const name federation_contract_ac;

public:
    using contract::contract;

    terraworlds(name receiver, name code, datastream<const char*> ds) : 
                contract(receiver, code, ds),
                token_contract_ac("alien.worlds"_n),
                token_symbol("TLM", 4),
                federation_contract_ac("federation"_n)
                {}

    /**
     * @brief - Set params which helps for immediate iterations inß equal distribution of tokens to the owners
     * @details - The table size is calculated off-chain as it could be huge & impossible to 
     *          calculate on-chain due to CPU/NET issue.
     * 
     * @param table_name - table name like landregs, ...
     * @param next_id - next id
     * @param next_owner - next owner
     * @param nft_count - NFT count from "federation::landregs" table  computed off-chain. This is to be calculated once per 
     *          execution in off-chain script.
     *      NOTE: If it's not to be set, then parse as zero
     */
    ACTION setparams( 
                    const name& table_name,
                    uint64_t next_id, 
                    const name& next_owner,
                    uint64_t nft_count
                    );

    /**
     * @brief - distribute tokens by self
     * @details - on receiving tokens from federation account daily, tokens will be 
     *          distributed based on the NFT owners table - "landregs"
     * @param table_name - table name like landregs, ...
     * @param loop_count - loop count for no. of token transfers to be made.
     * @param token_transfer_memo - custom message
     */
    ACTION distribute( 
                      const name& table_name, 
                      uint32_t loop_count,
                      const string& token_transfer_memo
                      );

    using setparams_action = action_wrapper<"setparams"_n, &terraworlds::setparams>;
    using distribute_action = action_wrapper<"distribute"_n, &terraworlds::distribute>;

private:
    // -----------------------------------------------------------------------------------------------------------------------
    // scope - self
    TABLE lastdist {
        name table_name;        // landregs, ...
        uint64_t next_id;
        name next_owner;
        uint64_t nft_count;

        auto primary_key() const { return table_name.value; }
    };

    using lastdist_index = multi_index<"lastdist"_n, lastdist>;

    // -----------------------------------------------------------------------------------------------------------------------
    // View data: https://wax.bloks.io/account/federation?loadContract=true&tab=Tables&account=federation&scope=federation&limit=100&table=landregs
    // Contract - federation
    // Table - landregs
    // scope - federation
    struct landreg_item {
        uint64_t id;
        name owner;

        uint64_t primary_key() const { return id; }
    };

    typedef multi_index<"landregs"_n,  landreg_item> landregs_table;

    // -----------------------------------------------------------------------------------------------------------------------
    // View data: https://wax.bloks.io/account/alien.worlds?loadContract=true&tab=Tables&account=alien.worlds&scope=alien.worlds&limit=100
    // Contract - alien.worlds
    // Table - accounts
    // scope - terra.worlds, ....
    struct account {
        asset balance;

        uint64_t primary_key()const { return balance.symbol.code().raw(); }
    };

    typedef eosio::multi_index< "accounts"_n, account > accounts;
};