This Repo contains the solution for deserializing raw Bitcoin Transaction data. 

Limitations - 
- currently the code for decoding witnesses and locktime has not been added ( will add soon)
- assumes that max txinputs is 0xFF
- double SHA256 hash of the transaction message is not done
- currently input is hardcode but that can be changed
- there is no check for presence of witness data it has been hardcoded for now 

  Input data is taken from 
https://blockchain.info/rawtx/fc5d35e8b011f3a1f8c44ee7f7c0533fd33e1fcab67c6bf1ac44e6c7a4edc4c2?format=hex

Output data is referred from - 
https://blockchain.info/rawtx/fc5d35e8b011f3a1f8c44ee7f7c0533fd33e1fcab67c6bf1ac44e6c7a4edc4c2?format=json

https://chainquery.com/bitcoin-cli/getrawtransaction


References -
https://en.bitcoin.it/wiki/Transaction#General_format_of_a_Bitcoin_transaction_.28inside_a_block.29
https://klmoney.wordpress.com/bitcoin-dissecting-transactions-part-2-building-a-transaction-by-hand/
