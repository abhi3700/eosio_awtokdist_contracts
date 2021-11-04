#include "../include/terraworlds.hpp"

// ============================================================================================================
void terraworlds::setparams() {
	
}

// ============================================================================================================
void terraworlds::distribute( uint64_t next_id,
							  const name& federation_table_name
							) 
{
	// permission can be active/owner or a custom
	// NOTE: custom permission is recommended
	require_auth(get_self());

	check( next_id > 0, "Stored next id must be positive" );

	// -----------------------------------------------------------------------------------
	// 1. match the stored (id & owner) with the next iterated available (id & owner) 
	// Get the stored id & owner
	lastdist_index lastdist_table(get_self(), federation_table_name.value);
	auto lastdist_it = lastdist_table.find(next_id);

	check( lastdist_it != lastdist_table.end(), "The stored next id is not available in terraworlds::lastdist table" );

	// check the table size must be 1
	auto lastdist_table_size = std::distance(lastdist_table.cbegin(), lastdist_table.cend());
	check( lastdist_table_size == 1, "The lastdist table size must be 1" );

	// get the stored owner name
	auto stored_owner_name = lastdist_it->owner;
	auto nft_count = lastdist_it->nft_count;

	// Get the id & owner from "federation::landregs" table
	landregs_table landregs_table_table("federtion"_n, "federation"_n.value);
	auto landregs_it = landregs_table_table.find(next_id);

	check( landregs_it != landregs_table_table.end(), "The stored next id is not available in federation::landregs table" );
	check( lastdist_it->owner == landregs_it->owner, "The owners for the 2 tables doesn\'t match" );

	// -----------------------------------------------------------------------------------
	// 2. calculate the tokens per NFT
	// read the balance from "alien.worlds::accounts" table
	accounts from_acnts( get_self(), get_self().value );
	const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
	
	auto bal_self = from.balance.amount;
	
	check( bal_self > 0, "contract\'s balance must be positive" );

	// calc TLM token per NFT asset
	asset token_per_nft = asset(0, from.balance.symbol);
	token_per_nft = bal_self/nft_count;

	// -----------------------------------------------------------------------------------
	// 3. iterate & transfer tokens in for-loop

	// -----------------------------------------------------------------------------------
	// 4. store the next_id & next_owner into "terraworlds::lastdist" table next_row_index

	// -----------------------------------------------------------------------------------
	// 5. clear the "terraworlds::lastdist" table if the next_row_index == tot_size


}