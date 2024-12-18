#include "server.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_STR_LEN 50

// Initialization
Account global_accounts[MAX_USERS];
int global_usr_cnt = 0;

bool verifyPassword(Account* account, const char* inputPassword) {
    return strcmp(account->password, inputPassword) == 0;
}

void mainMenu() {
    int c = -1;

    while (true) {
        clearScreen();

        printf("\x1B[32m");
        printf(
            "\n\t\t***************************  Welcome to AVL ATM  "
            "***************************\n\n\n\n");
        printf("\x1B[0m");
        printf("\x1B[34mEnter:\n\x1B[0m");
        printf("\x1B[31m [0] - sudo\n\x1B[0m");
        printf(" [1] - Login\n [2] - Registration\n [3] - Exit\n");
        printf("-------------------------\n");
        scanf("%d", &c);

        // validate the input choice
        if (c < 0 || c > 3) {
            invalidInputWarning();
            continue;
        }

        // Load user data file
        FILE* db;
        db = fopen("userdbase.dat", "ab+");
        if (db == NULL) {  // File not found
            db = fopen("userdbase.dat", "wb+");
            if (db == NULL) {
                printf(
                    "\x1B[31mError: Failed to load user data file.\n\x1B[0m");
                Sleep(1500);
                exit(0);
            } else {
                printf(
                    "\x1B[34m\nINFO - Loading file in Writing "
                    "Mode.\n\n\x1B[0m");
                Sleep(1500);
            }
        }

        if (c == 0) {
            // SuperUser
            printf(
                "\x1B[31mBy login as SuperUser, your operations will have "
                "consequences...\n"
                "Are you sure you want to continue? [Y/n]: \n\x1B[0m");
            char ans = '\0';
            scanf(" %c", &ans);
            if (ans == 'y' || ans == 'Y') {
                adminLogin();
            } else if (ans == 'n' || ans == 'N') {
                continue;
            } else {
                invalidInputWarning();
            }
        } else if (c == 1) {
            // User Login
            login(db);
        } else if (c == 2) {
            // Registration
            registration(db);
        } else if (c == 3) {
            // Exit
            printf("\x1B[31m\nQuitting, please wait...\n\x1B[0m");
            Sleep(1500);
            fclose(db);
            clearScreen();
            return;
        } else {
            invalidInputWarning();
        }

        fclose(db);
    }
}

void _init_acc_transinfo(Account* ptr_usr_data) {
    // Initialize transaction history
    for (int i = 0; i < MAX_TRANS_NUM; i++) {
        ptr_usr_data->transaction_hist[i].pre_balance = -1;
        ptr_usr_data->transaction_hist[i].post_balance = -1;
    }
}

int _get_acc_transnum(Account* ptr_usr_data) {
    int i = 0;
    while (i < MAX_TRANS_NUM &&
           ptr_usr_data->transaction_hist[i].pre_balance != -1)
        i++;
    return i;
}

void login(FILE* db) {
    char card_id[ACCOUNT_ID_LENGTH], password[PASSWORD_LENGTH],
        pass[MAX_STR_LEN];
    int returnOption, attempts = 0, acc_size = sizeof(Account);
    bool is_card_id_valid = false, is_pwd_valid = false,
         showOptionsMenu = false, chk = false;

    Account* ptr_usr_data = (Account*)malloc(acc_size);
    _init_acc_transinfo(ptr_usr_data);

    while (!is_card_id_valid) {
        clearScreen();

        printf("\x1B[32m");
        printf(
            "\n\t\t***************************     Login   Menu     "
            "***************************\n\n\n\n");
        printf("\x1B[0m");
        printf("\x1B[34mLogin:\n\n\x1B[0m");
        printf("\x1B[32mEnter Card ID: \x1B[0m");
        scanf(" %29s", card_id);

        // Search for the user account
        while (fread(ptr_usr_data, acc_size, 1, db) == 1) {
            if (strcmp(ptr_usr_data->card_id, card_id) == 0) {
                is_card_id_valid = true;
                chk = true;
                while (!is_pwd_valid) {
                    printf("\x1B[32mEnter Your Password: \x1B[0m");
                    scanf(" %29s", pass);
                    if (strcmp(ptr_usr_data->password, pass)) {
                        printf(
                            "\x1B[31mYou have supplied a WRONG "
                            "Password.\n\x1B[0m");
                    } else {
                        free(ptr_usr_data);
                        ptr_usr_data = NULL;

                        printf(
                            "\x1B[34mYou have successfully logged in the "
                            "system.\n");
                        is_pwd_valid = true;
                        showOptionsMenu = true;
                        Sleep(1000);
                    }
                }

                while (showOptionsMenu) {
                    clearScreen();

                    printf("\x1B[32m");
                    printf(
                        "\n\t\t***************************"
                        "     Options Menu     "
                        "***************************\n\n\n\n");
                    printf("\x1B[0m");
                    printf("\x1B[32mOPTIONS\n\n\x1B[0m");
                    printf("[1] - Deposit\n");
                    printf("[2] - Account Profile\n");
                    printf("[3] - Change Password\n");
                    printf("[4] - Withdraw\n");
                    printf("[5] - Close Account\n");
                    printf("[6] - Transfer\n");
                    printf("[7] - Log Out\n");
                    printf("----------------------\n");
                    printf("\x1B[32mPlease enter your choice: \x1B[0m");

                    showOptionsMenu = userMenu(card_id);
                }
            }
        }

        if (attempts == 3) {
            printf(
                "\x1B[31mYou have entered wrong credentials 3 times, "
                "exit to main menu.\n\x1B[0m");
            Sleep(2000);
            free(ptr_usr_data);
            ptr_usr_data = NULL;
            fclose(db);
            return;
        }

        if (!chk) {
            printf("\x1B[31mCard ID doesn't exist, please try again.\n\x1B[0m");
            Sleep(2000);
            rewind(db);
            attempts++;
        }
    }

    free(ptr_usr_data);
    ptr_usr_data = NULL;
    fclose(db);
    return;
}

bool userMenu(char* card_id) {
    int c, n = 0, i = 0, ret = true;
    scanf(" %d", &c);

    FILE* db;
    db = fopen("userdbase.dat", "rb+");
    int acc_size = sizeof(Account);
    Account* ptr_usr_data = (Account*)malloc(acc_size);
    _init_acc_transinfo(ptr_usr_data);

    // Calculate user count
    while (fread(ptr_usr_data, acc_size, 1, db) == 1) n++;
    rewind(db);
    free(ptr_usr_data);
    ptr_usr_data = NULL;

    // Load all user data
    Account* ptr_usr_dlst = (Account*)malloc(n * acc_size);
    fread(ptr_usr_dlst, acc_size, n, db);
    rewind(db);

    // Search for the user account
    for (; i < n; i++) {
        // printf("USR CARD ID: %s\n", (ptr_usr_dlst + i)->card_id);
        if (strcmp(card_id, (ptr_usr_dlst + i)->card_id) == 0) break;
    }

    double amt;
    char newpasswd[30], confirmPassword[30], curpasswd[30];
    char confirm, pass[30];
    bool is_usr_pwd_opt_valid = false;
    switch (c) {
        case 1:  // Deposit
            printf("\x1B[32mEnter the amount to be deposited: \x1B[0m");
            scanf(" %lf", &amt);
            (ptr_usr_dlst + i)->balance += amt;
            printf("\x1B[34mAmount deposited successfully\n\x1B[0m");
            printf("Your new balance is Rs.%.2lf\n",
                   (ptr_usr_dlst + i)->balance);

            printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
            scanf(" %c", &confirm);
            confirm = '\0';
            break;

        case 2:  // Balance Statement and Account information
            printf("\x1B[34mAccount information:\n\n\x1B[0m");
            printf("Account Number: \t%d\n", (ptr_usr_dlst + i)->acc_no);
            printf("Card ID:        \t%s\n", (ptr_usr_dlst + i)->card_id);
            printf("First name:     \t%s\n", (ptr_usr_dlst + i)->firstname);
            printf("Last name:      \t%s\n", (ptr_usr_dlst + i)->lastname);
            printf("Balance:        \t%.2lf .RMB\n\n",
                   (ptr_usr_dlst + i)->balance);

            printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
            scanf(" %c", &confirm);
            confirm = '\0';
            break;

        case 3:  // Change Password
            printf("\x1B[32mEnter your current password: \x1B[0m");
            scanf(" %29s", curpasswd);
            if (strcmp(curpasswd, (ptr_usr_dlst + i)->password) == 0) {
                printf("\x1B[32mEnter new password: \x1B[0m");
                scanf(" %29s", newpasswd);
                printf("\x1B[32mConfirm Password: \x1B[0m");
                scanf(" %29s", confirmPassword);
                if (strcmp(newpasswd, confirmPassword)) {
                    printf("\x1B[31mPasswords do not match.\n\x1B[0m");
                } else if (strcmp(newpasswd, (ptr_usr_dlst + i)->password) == 0)
                    printf(
                        "\x1B[31mYour new password is same as old "
                        "password.\n\x1B[0m");
                else {
                    strcpy((ptr_usr_dlst + i)->password, newpasswd);
                    printf(
                        "\x1B[34mYour password has been changed "
                        "successfully.\n\x1B[0m");
                }
            } else {
                printf("\x1B[31mIncorrect Password.\n\x1B[0m");
            }

            printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
            scanf(" %c", &confirm);
            confirm = '\0';
            break;

        case 4:  // Withdrawal
            printf("\x1B[32mEnter the amount you want to withdraw: \x1B[0m");
            scanf(" %lf", &amt);
            if ((ptr_usr_dlst + i)->balance >= amt) {
                (ptr_usr_dlst + i)->balance -= amt;
                printf("\x1B[34mPlease collect the cash.\n\x1B[0m");
                printf("Your new balance is Rs.%.2lf\n",
                       (ptr_usr_dlst + i)->balance);
            } else {
                printf("\x1B[31mInsufficient balance.\n\x1B[0m");
            }

            // TODO: Add transaction history
            printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
            scanf(" %c", &confirm);
            confirm = '\0';
            break;

        case 5:  // Close Account
            printf(
                "\x1B[31mConfirm:Do you want to delete account "
                "permanently? (Enter [Y/y] to confirm) \n\x1B[0m");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                while (!is_usr_pwd_opt_valid) {  // Confirm password
                    printf("\x1B[32mConfirm Your Password: \x1B[0m");
                    scanf(" %29s", pass);
                    if (strcmp((ptr_usr_dlst + i)->password, pass)) {
                        printf(
                            "\x1B[31mYou have supplied a WRONG "
                            "Password.\n\x1B[0m");
                        printf(
                            "\x1B[32mPlease re-enter your password: \n\x1B[0m");
                    } else {  // Delete account
                        fclose(db);
                        db = fopen("userdbase.dat", "wb+");
                        for (int j = 0; j < n; j++) {
                            if (j == i) {  // Skip the account to be deleted
                                continue;
                            } else {
                                fwrite(ptr_usr_dlst + j, acc_size, 1, db);
                            }
                        }
                        // printf("%d\n", i);
                        printf(
                            "\x1B[34mYou can collect your balance amount "
                            "%.2lf .RMB\n",
                            (ptr_usr_dlst + i)->balance);
                        printf(
                            "Your account has been successfully "
                            "deleted.\n\x1B[0m");
                        is_usr_pwd_opt_valid = true;
                        ret = false;  // return to main menu
                    }
                }
            }

            printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
            scanf(" %c", &confirm);
            confirm = '\0';
            break;

        case 6:  // Transferring
            int j = 0;
            char target_card_id[ACCOUNT_ID_LENGTH];
            double amt = 0.0;
            while (true) {
                printf("\x1B[32mEnter the target Card ID: \x1B[0m");
                scanf(" %29s", target_card_id);
                printf(
                    "Please confirm your target Card ID: \x1B[34m%s\n\x1B[0m",
                    target_card_id);

                if (strcmp(target_card_id, (ptr_usr_dlst + i)->card_id) == 0) {
                    printf(
                        "\x1B[31mYou cannot transfer money to your own "
                        "account.\n\x1B[0m");
                    continue;
                }

                // Search for the target user account
                bool is_found = false;
                for (; j < n; j++) {
                    if (strcmp(target_card_id, (ptr_usr_dlst + j)->card_id) ==
                        0) {
                        is_found = true;
                        break;
                    }
                }
                if (!is_found) {
                    printf(
                        "\x1B[31mGot invalid card ID, please try "
                        "again.\n\x1B[0m");
                    continue;
                } else {
                    printf("Your target person is: \x1B[34m%s %s\n\x1B[0m",
                           (ptr_usr_dlst + j)->firstname,
                           (ptr_usr_dlst + j)->lastname);
                    printf("His/her Card ID is: \x1B[34m%s\n\x1B[0m",
                           (ptr_usr_dlst + j)->card_id);
                    break;
                }
            }

            printf("\x1B[32mEnter the amount you want to transfer: \x1B[0m");
            scanf("%lf", &amt);

            transfer(ptr_usr_dlst + i, ptr_usr_dlst + j, amt);

            printf("\x1B[32mEnter a character to continue: \n\x1B[0m");
            scanf(" %c", &confirm);
            confirm = '\0';
            break;

        case 7:  // Log out
            ret = false;
            printf("\x1B[31m\nLogging out, please wait...\n\x1B[0m");
            Sleep(1500);
            break;

        default:
            invalidInputWarning();
            Sleep(2000);
            break;
    }
    rewind(db);
    if (c != 5) fwrite(ptr_usr_dlst, acc_size, n, db);

    free(ptr_usr_dlst);
    ptr_usr_dlst = NULL;
    fclose(db);
    return ret;
}

void _copy_acc_info(Account* from, Account* to) {
    if (!from || !to) {
        printf("\x1B[31mGot invalid accounts.\n\x1B[0m");
        return;
    }
    // deprecated
}

// Transfer funds
void transfer(Account* fromAccount, Account* toAccount, double amt) {
    if (!fromAccount || !toAccount) {
        printf("\x1B[31mGot invalid accounts.\n\x1B[0m");
        return;
    }

    if (fromAccount->balance < amt) {
        printf("\x1B[31mInsufficient balance.\n\x1B[0m");
        return;
    }

    printf("\x1B[34m  Source Card ID: %s\n", fromAccount->card_id);
    printf("  Target Card ID: %s\n", toAccount->card_id);
    printf("Processing, please be patient...\n\x1B[0m");
    Sleep(1000);

    int fromAccount_transcount = _get_acc_transnum(fromAccount);
    int toAccount_transcount = _get_acc_transnum(toAccount);
    if (fromAccount_transcount >= MAX_TRANS_NUM - 1 ||
        toAccount_transcount >= MAX_TRANS_NUM - 1) {
        printf("\x1B[31mMaximum transaction limit reached.\n\x1B[0m");
        return;
    }

    fromAccount->transaction_hist[fromAccount_transcount].pre_balance =
        fromAccount->balance;
    fromAccount->balance -= amt;
    fromAccount->transaction_hist[fromAccount_transcount].post_balance =
        fromAccount->balance;
    strcpy(fromAccount->transaction_hist[fromAccount_transcount].target_card_id,
           toAccount->card_id);

    toAccount->transaction_hist[toAccount_transcount].pre_balance =
        toAccount->balance;
    toAccount->balance += amt;
    toAccount->transaction_hist[toAccount_transcount].post_balance =
        toAccount->balance;
    strcpy(toAccount->transaction_hist[toAccount_transcount].target_card_id,
           fromAccount->card_id);

    printf("\x1B[32mTransaction successful.\n\x1B[0m");
}

void register_user(FILE* db) {
    int acc_size = sizeof(Account);
    Account* ptr_usr_data = (Account*)malloc(acc_size);
    _init_acc_transinfo(ptr_usr_data);

    char lastNameTemp[30], firstNameTemp[30], password[PASSWORD_LENGTH],
        card_idTemp[ACCOUNT_ID_LENGTH];
    char confirmPassword[PASSWORD_LENGTH], checkSave;

    printf("\x1B[34mCreate new account:\n\x1B[0m");
    printf(
        "\x1B[31mWarning: Password Must contain less than ten(10) Alphanumeric "
        "Digits.\n\n\x1B[0m");
    printf("\x1B[32mEnter First Name: \x1B[0m");
    scanf(" %29[^'\n']", firstNameTemp);
    printf("\x1B[32mEnter Last Name: \x1B[0m");
    scanf(" %29[^'\n']", lastNameTemp);

    bool chk = true;  // boolean value: check the existence of account
    while (chk) {
        printf("\x1B[32mEnter Card ID: \x1B[0m");
        scanf(" %29[^'\n']", card_idTemp);

        // Validate the duplication
        chk = false;
        while (fread(ptr_usr_data, acc_size, 1, db) == 1) {
            if (ptr_usr_data->card_id == NULL) {
                printf(
                    "\x1B[31mError: Memory allocation for card_id "
                    "failed!\n\x1B[0m");
                // Clean up previously allocated memory
                free(ptr_usr_data);
                ptr_usr_data = NULL;
                return;
            }
            if (strcmp(ptr_usr_data->card_id, card_idTemp) == 0) chk = true;
        }
        if (chk) {
            printf("\x1B[31mCard ID already exists.\n\x1B[0m");
            printf("\x1B[32mPlease enter a different Card ID.\n\x1B[0m");
        }
        rewind(db);
    }

    printf("\x1B[32mEnter Password: \x1B[0m");
    scanf(" %29s", password);
    printf("\x1B[32mConfirm Password: \x1B[0m");
    scanf(" %29s", confirmPassword);

    if (strcmp(password, confirmPassword)) {
        printf("\x1B[31mPasswords do not match.\n\x1B[0m");
    } else {
        printf("\x1B[31mSave User Information? [Y/n]: \n\x1B[0m");
        while (true) {
            char ans = '\0';
            scanf(" %c", &ans);

            if (ans == 'y' || ans == 'Y') {
                // Calculate the number of current accounts
                int n = 0;
                while (fread(ptr_usr_data, acc_size, 1, db) == 1) n++;

                strcpy(ptr_usr_data->card_id, card_idTemp);
                strcpy(ptr_usr_data->firstname, firstNameTemp);
                strcpy(ptr_usr_data->lastname, lastNameTemp);
                strcpy(ptr_usr_data->password, password);
                ptr_usr_data->acc_no = n + 1;
                ptr_usr_data->balance = 0.0;
                _init_acc_transinfo(ptr_usr_data);

                if (ptr_usr_data->acc_no < 0) {
                    printf(
                        "\x1B[31mError: Sorry, the max number "
                        "of accounts has been reached.\n\x1B[0m");
                    fclose(db);
                    return;
                }

                // Save the user data to the database
                fclose(db);
                db = fopen("userdbase.dat", "ab+");
                fwrite(ptr_usr_data, acc_size, 1, db);
                fflush(db);
                printf("----------------------\n");
                printf(
                    "\x1B[34mYour account has been created successfully "
                    "and your Account Number is %d\n\x1B[0m",
                    ptr_usr_data->acc_no);
                Sleep(3000);
                break;
            } else if (ans != 'n' && ans != 'N') {
                invalidInputWarning();
            }
        }
    }
    free(ptr_usr_data);
    ptr_usr_data = NULL;

    // NOTE: The db is closed here
    fclose(db);
}

// Accounts Registration
void registration(FILE* db) {
    clearScreen();
    printf("\x1B[32m");
    printf(
        "\n\t\t***************************  Registration  Menu  "
        "***************************\n\n\n\n");
    printf("\x1B[0m");
    register_user(db);  // db is closed
}
