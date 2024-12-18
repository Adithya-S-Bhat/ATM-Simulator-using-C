# ATM-Simulator

## Features
A basic ATM simulator that can be run on a terminal window, which is written in C. It has the following funtionalities:
1. Create Account
2. User Authentication
3. Change Password
4. Deposit, Withdraw money
5. View Account details

---
#### Update (12-18-2024)
1. Reconstruct the basic architecture of the program
2. Add a SuperUser option for administrator to add/delete/modify users

---
#### TODO
\+ Add the transaction-history feature

\+ Plug in a forecast function based on the balance change

\+ Plug in the query for history transactions and balance change

\+ Encode and Decode the card ID and password

\- Deprecate the username, using card ID only

## How to use
Steps to execute:
1. Use "make -f pr.mk" to create the executable file a.out
2. To execute a.out file use "./ATM" command.
