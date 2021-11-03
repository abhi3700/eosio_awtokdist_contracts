#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <vector>
#include <algorithm>
#include <string>


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


CONTRACT terraworlds : public contract
{
private:

public:
	using contract::contract;

	terraworlds(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds) {}

	/**
	 * @brief - distribute tokens by self
	 * @details - on receiving tokens from federation account daily, tokens will be distributed based on the NFT owners table - "landregs"
	 * 
	 */
	
	ACTION distribute(  );


	using distribute_action = action_wrapper<"distribute"_n, &terraworlds::distribute>;

private:
	// -----------------------------------------------------------------------------------------------------------------------
	// scope - player
	TABLE lastdist {
		uint64_t id;
		name owner;

		auto primary_key() const { return id; }
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
	}

	typedef multi_index<"landregs"_n,  landreg_item> landregs_table;



};