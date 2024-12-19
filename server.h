#include <stdbool.h>
#include <stdio.h>

#ifndef SERVER_H
#define SERVER_H

#define MAX_USERS 100           // Maximum number of users
#define MAX_TRANS_NUM 1000      // Maximum number of transactions
#define ACCOUNT_ID_LENGTH 30    // Maximum length of account ID
#define ACCOUNT_NAME_LENGTH 30  // Maximum length of account name
#define PASSWORD_LENGTH 30      // Maximum length of password

typedef struct AccountInfo Account;
typedef struct TransferInfo Transfer;

struct TransferInfo {
    double pre_balance, post_balance;        // Balance change
    char target_card_id[ACCOUNT_ID_LENGTH];  // Target account ID
};

struct AccountInfo {
    int acc_no;                                // Index
    char firstname[30];                        // First name
    char lastname[30];                         // Last name
    char card_id[ACCOUNT_ID_LENGTH];           // Card number
    char password[PASSWORD_LENGTH];            // Password
    double balance;                            // Balance
    Transfer transaction_hist[MAX_TRANS_NUM];  // Transaction history
};

void login(FILE* db);         // Login menu
void registration(FILE* db);  // Registration menu

void mainMenu();                     // Main menu
bool userMenu(const char* card_id);  // Main menu

// User-related functions
void transfer(Account* fromAccount, Account* toAccount,
              double amt);  // Transfer funds

// Registering-related functions
void register_user(FILE* db);

// Administrator-related functions
void adminLogin();       // Admin login
void adminMenu();        // Admin menu
void addUser(FILE* db);  // Add users

// Clear screen function
void clearScreen();
void invalidInputWarning();

// Encryption-related
//

// API Calls
void demo();
int get_cipher_size();
int get_key_size();
void encryptor(unsigned char* input, unsigned char* cipher);
void decryptor(unsigned char* cipher, unsigned char* output);

#endif
