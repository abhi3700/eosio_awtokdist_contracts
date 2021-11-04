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

1. Read the 1st row with fields `id` & `owner` from `federation::landregs` table.
1. Calculate the "landreg" table size via off-chain
1. Store these into on-chain table - "lastdist" into `next_id`, `next_owner`, `tot_size` fields respectively.
1. distribute()
	- calc the no. of tokens per NFT
	- iterate across id till some count (say 10) depending on contract's CPU/NET
	- extract the owner name
	- transfer tokens to owner & repeat