## Features
* Admin can distribute tokens on-chain via `distribute` function.
* The token distribution happens in multiple iterations of token transfer actions in single run. The no. of token transfer actions depends on CPU/NET of the smart contract.
* The Contract also stores the next `id` & `owner` fields to be started with, in the next immediate run of `distribute` ACTION.