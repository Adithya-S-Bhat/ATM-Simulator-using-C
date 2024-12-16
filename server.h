#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#define MAX_USERS 100           // Maximum number of users
#define ACCOUNT_ID_LENGTH 20    // Maximum length of account ID
#define ACCOUNT_NAME_LENGTH 30  // Maximum length of account name
#define PASSWORD_LENGTH 20      // Maximum length of password

typedef struct AccountInfo Account;
typedef struct TransferInfo Transfer;

struct AccountInfo {
    char ID[ACCOUNT_ID_LENGTH];      // Card number
    char name[30];                   // Name
    char password[PASSWORD_LENGTH];  // Password
    double balance;                  // Balance
    Transfer* transaction_hist;
};

struct TransferInfo {
    int idx;                        // Index
    int pre_balance, post_balance;  // Balance change
    Account* receiver;              // Target
    Transfer* nxt;                  // Next transaction
    Transfer* prev;                 // Previous transaction
};

extern Account global_accounts[MAX_USERS];  // Array of user accounts
extern int global_usr_cnt;                // Current number of users

int login();                      // Login function
void mainMenu();                  // Main menu
void userMenu(Account* account);  // Main menu

// User-related functions
void deposit(Account* account, double amount);   // Deposit
void withdraw(Account* account, double amount);  // Withdraw
void queryAccountInfo(const Account* account);   // Query account information
void transfer(Account* fromAccount, Account* toAccount,
              double amount);  // Transfer funds

// Administrator-related functions
void adminLogin();  // Admin login
void adminMenu();   // Admin menu
void addUser();     // Add new user

// Clear screen function
void clearScreen();

#endif
