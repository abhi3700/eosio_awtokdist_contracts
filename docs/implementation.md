## Implementation

### Deployment
* Deploy the `terra.worlds` contract.
* Run the execution script as mentioned [here](#execution).

### SC System Design
#### Constructor
* Initialize the below params:
	- `token_symbol`
	- `token_contract_ac`
	- `federation_contract_ac`

#### Tables
* `lastdist`
	- `table_name`
	- `next_id`
	- `next_owner`
	- `nft_count`

#### Actions
* `distribute` has params:
	- `table_name`
	- `loop_count`
	- `token_transfer_memo`

### Execution
A script in `NodeJS` would perform these operations (mentioned below) in sequence:

1. Read the 1st row with fields `id` & `owner` from `federation::landregs` table using `eosjs` `get_table_rows` method. [Source](https://developers.eos.io/manuals/eosjs/v21.0/how-to-guides/how-to-get-table-information/#get-table-rows)
1. Calculate the "landreg" row/nft count via off-chain by counting the row_array length obtained by `get_table_rows` method.
1. Store these into on-chain table - "lastdist" into `table_name`, `next_id`, `next_owner`, `nft_count` fields respectively.
1. distribute()
	- calc the no. of tokens per NFT
	- iterate across id till some count (say 10) depending on contract's CPU/NET
	- extract the owner name
	- transfer tokens to owner & repeat
	- clear the `lastdist` table's row when reaches the `landregs` table end.