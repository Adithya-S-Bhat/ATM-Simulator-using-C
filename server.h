#include <stdbool.h>
#include <stdio.h>

#ifndef SERVER_H
#define SERVER_H

#define MAX_USERS 100           // Maximum number of users
#define MAX_TRANSACTIONS 1000   // Maximum number of transactions
#define ACCOUNT_ID_LENGTH 30    // Maximum length of account ID
#define ACCOUNT_NAME_LENGTH 30  // Maximum length of account name
#define PASSWORD_LENGTH 30      // Maximum length of password

typedef struct AccountInfo Account;
typedef struct TransferInfo Transfer;

struct TransferInfo {
    int pre_balance, post_balance;  // Balance change
    Account* receiver;              // Target
};

struct AccountInfo {
    int acc_no;                                   // Index
    char firstname[30];                           // First name
    char lastname[30];                            // Last name
    char card_id[ACCOUNT_ID_LENGTH];              // Card number
    char password[PASSWORD_LENGTH];               // Password
    double balance;                               // Balance
    Transfer transaction_hist[MAX_TRANSACTIONS];  // Transaction history
};

extern Account global_accounts[MAX_USERS];  // Array of user accounts
extern int global_usr_cnt;                  // Current number of users

void login(FILE* db);         // Login menu
void registration(FILE* db);  // Registration menu

void mainMenu();               // Main menu
bool userMenu(char* card_id);  // Main menu

// User-related functions
void deposit(Account* account, double amount);   // Deposit
void withdraw(Account* account, double amount);  // Withdrawal
void queryAccountInfo(const Account* account);   // Query account information
void transfer(Account* fromAccount, Account* toAccount,
              double amt);  // Transfer funds

// Registering-related functions
// None

// Administrator-related functions
void adminLogin();  // Admin login
void adminMenu();   // Admin menu
void addUser();     // Add users

// Clear screen function
void clearScreen();
void invalidInputWarning();

#endif
