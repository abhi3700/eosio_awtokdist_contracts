# Alien Worlds' Token Distribution Contract

## About
* [Documentation](./docs)

## Compile
* Open terminal in the project root directory & run the commands sequentially:
```console
mkdir contracts/build
eosio-cpp contracts/src/terraworlds.cpp -o contracts/build/terraworlds.wasm
```

## Issues
* If the receiver during the token distribution is a contract. It can be malicious. Hence, automatic distribution is not recommended as there is a chance of reentrancy attack.

## Todo
* [ ] Manual claim by the accounts
* [ ] Store the whole balance of `terra.worlds` account into a table, & the no. of nft_count in the table.
* [ ] Singleton for table with 1 row.

## Execution
* Follow [this](./docs/implementation.md#execution) to execute the task via both on-chain & off-chain combined.

## References
* [Problem statement](./docs/references/)