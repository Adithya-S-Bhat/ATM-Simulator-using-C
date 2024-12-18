# ATM-Simulator

## Features
A basic ATM simulator that can be run on a terminal window, which is written in C. It has the following funtionalities:
1. Create Account
2. User Authentication
3. Change Password
4. Deposit, Withdraw money
5. View Account details

#### Update (12-18-2024)
1. Reconstruct the basic architecture of the program
2. Add a SuperUser option for administrator to add/delete/modify users

#### __TODO__
❌ Encipherment for Card IDs and Passwords

❌ Plug in a forecast function based on the balance change

❌ Plug in the query for history transactions and balance change

❌ Encode and Decode the card ID and password

❌ Finish the SuperUser function: List ALL users

---

✅ Deprecate the username, using card ID only

✅ Finish the SuperUser function: Create accounts

✅ Add some _cooldown_ features, like exiting to main menu after 3 consecutive wrong attempts

✅ Add the transfer feature

✅ Add the transaction-history feature

## How to use
Steps to execute:
1. Use "make -f pr.mk" to create the executable file ATM.exe
2. To execute a.out file use "./ATM" command.
---
