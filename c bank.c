
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accNumber;
    char name[100];
    float balance;
};

void createAccount();
void viewAccount();
void depositMoney();
void withdrawMoney();
void deleteAccount();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while (1) {
        printf("\n===== Bank Account Management System =====\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: deleteAccount(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void createAccount() {
    struct Account acc;
    FILE *file = fopen("accounts.dat", "ab");

    printf("Enter Account Number: ");
    scanf("%d", &acc.accNumber);
    printf("Enter Name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    printf("Account Created Successfully!\n");
}

void viewAccount() {
    struct Account acc;
    FILE *file = fopen("accounts.dat", "rb");
    int accNumber, found = 0;

    printf("Enter Account Number to View: ");
    scanf("%d", &accNumber);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("\nAccount Number: %d\n", acc.accNumber);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found) printf("Account not found!\n");
}

void depositMoney() {
    struct Account acc;
    FILE *file = fopen("accounts.dat", "rb+");
    int accNumber, found = 0;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNumber);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fclose(file);

            printf("Deposit Successful! New Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found!\n");
}

void withdrawMoney() {
    struct Account acc;
    FILE *file = fopen("accounts.dat", "rb+");
    int accNumber, found = 0;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNumber);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount > acc.balance) {
                printf("Insufficient Balance!\n");
            } else {
                acc.balance -= amount;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                printf("Withdrawal Successful! New Balance: %.2f\n", acc.balance);
            }
            fclose(file);
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found!\n");
}

void deleteAccount() {
    struct Account acc;
    FILE *file = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    int accNumber, found = 0;

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accNumber);

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accNumber == accNumber) {
            found = 1;
        } else {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found) printf("Account Deleted Successfully!\n");
    else printf("Account not found!\n");
}
