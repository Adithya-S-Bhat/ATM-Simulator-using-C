# ATM-Simulator

## Original Features
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
❌ Add documents

❌ Enhance the readability of the code

❌ Plug in the query for history transactions and balance change (SuperUser)

---

✅ Deprecate the username, using card ID only

✅ Finish the SuperUser function: Create accounts

✅ Add some _cooldown_ features, like exiting to main menu after 3 consecutive wrong attempts

✅ Add the transfer feature

✅ Add the transaction-history feature

✅ Encipherment for Card IDs and Passwords

✅ Finish the access to ALL-Users-List (SuperUser)

## Special Acknowledgement
The encryption and decryption of the card ID and password is Advanced Encryption Standard, [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard), and mostly based on the code from the following [repository](https://github.com/m3y54m/aes-in-c).

Thanks to the [authors](https://github.com/m3y54m/aes-in-c/commits?author=m3y54m) for their generous sharing of knowledge.

## How to use
Steps to execute:
1. Use "make -f pr.mk" to create the executable file ATM.exe
2. To execute a.out file use "./ATM" command.
---
