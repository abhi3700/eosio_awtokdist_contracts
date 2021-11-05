## Implementation

### Deployment
* Deploy the `terra.worlds` contract.
* Set the 1st `id` & `owner` fields from `federation::landregs` table into `terra.worlds::lastdist` table via `setparams`

### SC System Design
#### Constructor

#### Tables

#### Actions
* `distribute` has params:

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