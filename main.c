#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

typedef struct card_details {
    char name[50];
    char nr_card[30];
    char expiration_date[10];
    char cvc[8];
    float balance;  
} card;

card* ptr_array[5] = {NULL, NULL, NULL, NULL, NULL};

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool is_valid_date(const char *date) {
    if (strlen(date) != 5 || date[2] != '/') return false;
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (!isdigit(date[i])) return false;
    }
    int month = atoi(date);
    return month >= 1 && month <= 12;
}

bool is_valid_card_number(const char *number) {
    if (strlen(number) != 16) return false;
    for (int i = 0; i < 16; i++) {
        if (!isdigit(number[i])) return false;
    }
    return true;
}

bool is_valid_cvc(const char *cvc) {
    if (strlen(cvc) != 3) return false;
    for (int i = 0; i < 3; i++) {
        if (!isdigit(cvc[i])) return false;
    }
    return true;
}

void welcome() {
    printf("-----------------------------------------------------------------------------\n");
    for(int i = 0; i < 7; i++) {
        if (i == 3) {
            printf("|" COLOR_CYAN "                           WELCOME TO C-BANK!                              " COLOR_RESET "|\n");
        }
        else {
            printf("|                                                                           |\n");
        }
    }
    printf("_____________________________________________________________________________\n");

    printf(COLOR_YELLOW "1. Login\n" COLOR_RESET);
    printf(COLOR_YELLOW "2. Register\n" COLOR_RESET);
    printf(COLOR_YELLOW "3. Delete Account (Clear All Data)\n" COLOR_RESET);
    printf(COLOR_YELLOW "4. Exit\n" COLOR_RESET);
    printf(COLOR_BLUE "Select an option (1-4): " COLOR_RESET);
}

void delete_account() {
    FILE *fp1 = fopen("credentials.txt", "w");
    if (fp1 != NULL) {
        fclose(fp1);
    }

    FILE *fp2 = fopen("card_detail.txt", "w");
    if (fp2 != NULL) {
        fclose(fp2);
    }

    for (int i = 0; i < 5; i++) {
        if (ptr_array[i] != NULL) {
            free(ptr_array[i]);
            ptr_array[i] = NULL;
        }
    }

    printf(COLOR_GREEN "\nAccount data successfully deleted.\n" COLOR_RESET);
    int input;
    printf("Enter 1 to continue: ");
    scanf("%d", &input);
    system("clear");
}

void delete() {
    printf(COLOR_YELLOW "\nPlease enter the name of the card you want to delete:\n" COLOR_RESET);
    char delete_name[50];
    scanf("%s", delete_name);
    card *p = ptr_array[0];
    int index = 0;
    bool found = false;
    
    while (index < 5) {
        if (p != NULL && strcmp(p->name, delete_name) == 0) {
            free(p);
            ptr_array[index] = NULL;
            found = true;
            printf(COLOR_GREEN "\nCard '%s' successfully deleted.\n" COLOR_RESET, delete_name);
            break;
        }
        p = ptr_array[++index];
    }
    
    if (!found) {
        printf(COLOR_RED "\nCard '%s' not found.\n" COLOR_RESET, delete_name);
    }
    
    int input;
    printf("\nEnter 1 to continue: ");
    scanf("%d", &input);
    system("clear");
}

card* create_card(char *nr_card, char *exp_date, char *Cvc, char *card_name, float balance) {
    card *p = (card*)malloc(sizeof(card));
    strcpy(p->name, card_name);
    strcpy(p->nr_card, nr_card);
    strcpy(p->expiration_date, exp_date);
    strcpy(p->cvc, Cvc);
    p->balance = balance;
    return p;
}

void new_card() {
    char ncard[30];
    char exp_d[10];
    char cvc[9];
    char name[50];
    float balance = 0.0;
    
    printf(COLOR_YELLOW "\nEnter the card name (max 49 chars):\n" COLOR_RESET);
    scanf("%49s", name);
    clear_input_buffer();
    
    while (1) {
        printf(COLOR_YELLOW "\nEnter card number (16 digits, no spaces):\n" COLOR_RESET);
        scanf("%19s", ncard);
        clear_input_buffer();
        if (is_valid_card_number(ncard)) break;
        printf(COLOR_RED "\nInvalid card number. Please enter exactly 16 digits.\n" COLOR_RESET);
    }
    
    while (1) {
        printf(COLOR_YELLOW "\nEnter expiration date (MM/YY):\n" COLOR_RESET);
        scanf("%9s", exp_d);
        clear_input_buffer();
        if (is_valid_date(exp_d)) break;
        printf(COLOR_RED "\nInvalid date format. Please use MM/YY format (e.g., 12/25).\n" COLOR_RESET);
    }
    
    while (1) {
        printf(COLOR_YELLOW "\nEnter CVC (3 digits):\n" COLOR_RESET);
        scanf("%8s", cvc);
        clear_input_buffer();
        if (is_valid_cvc(cvc)) break;
        printf(COLOR_RED "\nInvalid CVC. Please enter exactly 3 digits.\n" COLOR_RESET);
    }

    printf(COLOR_YELLOW "\nEnter initial balance:\n" COLOR_RESET);
    scanf("%f", &balance);
    clear_input_buffer();

    card *new_card = create_card(ncard, exp_d, cvc, name, balance);

    for (int i = 0; i < 5; i++) {
        if (ptr_array[i] == NULL) {
            ptr_array[i] = new_card;
            printf(COLOR_GREEN "\nCard '%s' successfully added!\n" COLOR_RESET, name);
            break;
        }
        if (i == 4) {
            printf(COLOR_RED "\nMaximum number of cards (5) reached. Cannot add more cards.\n" COLOR_RESET);
            free(new_card);
        }
    }
    
    int input;
    printf("\nEnter 1 to continue: ");
    scanf("%d", &input);
    system("clear");
}

void load_cards() {
    FILE *ptrtofile = fopen("card_detail.txt", "r");
    if (ptrtofile == NULL) return;

    char carddetail[150];
    int index = 0;

    while (fgets(carddetail, sizeof(carddetail), ptrtofile) != NULL && index < 5) {
        char namec[30];
        char nrcard[30];
        char exp_date[10];
        char cvc[9];
        float balance;
        sscanf(carddetail, "%s %s %s %s %f", namec, nrcard, exp_date, cvc, &balance);
        ptr_array[index++] = create_card(nrcard, exp_date, cvc, namec, balance);
    }

    fclose(ptrtofile);
}

void save_cards() {
    FILE *filepointer = fopen("card_detail.txt", "w");
    if (filepointer == NULL) return;

    for (int i = 0; i < 5; i++) {
        if (ptr_array[i] != NULL) {
            card *p = ptr_array[i];
            fprintf(filepointer, "%s %s %s %s %.2f\n", p->name, p->nr_card, p->expiration_date, p->cvc, p->balance);
        }
    }
    fclose(filepointer);
}

int compare_cards(const void *a, const void *b) {
    card *cardA = *(card**)a;
    card *cardB = *(card**)b;
    
    if (cardA == NULL && cardB == NULL) return 0;
    if (cardA == NULL) return 1;
    if (cardB == NULL) return -1;
    
    if (cardA->balance > cardB->balance) return -1;
    if (cardA->balance < cardB->balance) return 1;
    return 0;
}

void mycards() {
    card *temp_array[5];
    int card_count = 0;
    
    for (int i = 0; i < 5; i++) {
        if (ptr_array[i] != NULL) {
            temp_array[card_count++] = ptr_array[i];
        }
    }
    
    qsort(temp_array, card_count, sizeof(card*), compare_cards);
    
    if (card_count == 0) {
        printf(COLOR_RED "\nNo cards found. Please add a card first.\n" COLOR_RESET);
    } else {
        for (int i = 0; i < card_count; i++) {
            card *p = temp_array[i];
            printf(COLOR_CYAN "\n================================================================\n" COLOR_RESET);
            printf("| " COLOR_MAGENTA "%20s%-40s" COLOR_RESET "|\n", "", p->name);
            printf(COLOR_CYAN "---------------------------------------------------------------\n" COLOR_RESET);
            printf("| Card Number      : " COLOR_GREEN "%-40s" COLOR_RESET " |\n", p->nr_card);
            printf("| Expiration Date  : " COLOR_GREEN "%-40s" COLOR_RESET " |\n", p->expiration_date);
            printf("| CVC Code         : " COLOR_GREEN "%-40s" COLOR_RESET " |\n", p->cvc);
            printf("| Balance          : " COLOR_GREEN "$%-38.2f" COLOR_RESET " |\n", p->balance);
            printf(COLOR_CYAN "===============================================================\n\n" COLOR_RESET);
        }
    }
    
    int exit_option;
    printf("\nEnter 1 to exit: ");
    scanf("%d", &exit_option);
    system("clear");
}

void display() {
    FILE *filepointer = fopen("credentials.txt", "r");
    if (filepointer == NULL) {
        printf(COLOR_RED "\nError opening credentials.txt\n" COLOR_RESET);
        return;
    }

    char credentials[5][50];
    for (int i = 0; i < 5; i++) {
        if (fgets(credentials[i], sizeof(credentials[i]), filepointer) == NULL) {
            credentials[i][0] = '\0';
        }
        credentials[i][strcspn(credentials[i], "\n")] = '\0';
    }

    printf(COLOR_CYAN "\nPersonal Information:\n" COLOR_RESET);
    printf(COLOR_YELLOW "-------------------\n" COLOR_RESET);
    printf("Username: " COLOR_GREEN "%s\n" COLOR_RESET, credentials[0]);
    printf("Password: " COLOR_GREEN "********\n" COLOR_RESET); // Don't show actual password
    printf("First Name: " COLOR_GREEN "%s\n" COLOR_RESET, credentials[2]);
    printf("Last Name: " COLOR_GREEN "%s\n" COLOR_RESET, credentials[3]);
    printf("Phone Number: " COLOR_GREEN "%s\n" COLOR_RESET, credentials[4]);
    printf(COLOR_YELLOW "-------------------\n\n" COLOR_RESET);

    fclose(filepointer);
    
    int input;
    printf("Enter 1 to exit: ");
    scanf("%d", &input);
    system("clear");
}

void myaccount() {
    int options = 0;

    while (1) {
        printf(COLOR_CYAN "\nMy Account Menu:\n" COLOR_RESET);
        printf(COLOR_YELLOW "1. My Cards\n" COLOR_RESET);
        printf(COLOR_YELLOW "2. Add new cards\n" COLOR_RESET);
        printf(COLOR_YELLOW "3. Delete cards\n" COLOR_RESET);
        printf(COLOR_YELLOW "4. Display personal information\n" COLOR_RESET);
        printf(COLOR_YELLOW "5. Exit\n" COLOR_RESET);
        printf(COLOR_BLUE "Select an option (1-5): " COLOR_RESET);
        scanf("%d", &options);

        system("clear");

        switch (options) {
            case 1: mycards(); break;
            case 2: new_card(); break;
            case 3: delete(); break;
            case 4: display(); break;
            case 5: return;
            default: printf(COLOR_RED "\nInvalid option. Please try again.\n" COLOR_RESET);
        }
    }
}

void Login() {
    FILE *ptr = fopen("credentials.txt", "r");
    if (ptr == NULL) {
        printf(COLOR_RED "\nError opening credentials file. Please register first.\n" COLOR_RESET);
        return;
    }

    char password1[20];
    char username1[50];
    
    printf(COLOR_CYAN "\nLogin\n" COLOR_RESET);
    printf(COLOR_YELLOW "--------\n" COLOR_RESET);
    
    printf("Enter your username: ");
    scanf("%49s", username1);
    
    printf("Enter your password: ");
    scanf("%19s", password1);

    char file_pass[20];
    char file_user[50];

    fscanf(ptr, "%49s", file_user);
    fscanf(ptr, "%19s", file_pass);

    if (strcmp(username1, file_user) == 0 && strcmp(password1, file_pass) == 0) {
        system("clear");
        printf(COLOR_GREEN "\nLogin successful! Welcome back, %s!\n" COLOR_RESET, username1);
        myaccount();
    }
    else {
        printf(COLOR_RED "\nInvalid credentials. Please try again.\n" COLOR_RESET);
    }
    fclose(ptr);
}

void Register() {
    FILE *fileptr = fopen("credentials.txt", "w");
    if (fileptr == NULL) {
        printf(COLOR_RED "\nError creating credentials file!\n" COLOR_RESET);
        return;
    }

    char credentials[5][50];
    
    printf(COLOR_CYAN "\nRegistration\n" COLOR_RESET);
    printf(COLOR_YELLOW "------------\n" COLOR_RESET);
    
    printf("Enter your First Name (max 49 chars): ");
    scanf("%49s", credentials[0]);
    
    printf("Enter your Last Name (max 49 chars): ");
    scanf("%49s", credentials[1]);

    while (1) {
        printf("Enter Phone Number (digits only, max 15): ");
        scanf("%15s", credentials[2]);
        bool valid = true;
        for (int i = 0; credentials[2][i]; i++) {
            if (!isdigit(credentials[2][i])) {
                valid = false;
                break;
            }
        }
        if (valid) break;
        printf(COLOR_RED "\nInvalid phone number. Please enter digits only.\n" COLOR_RESET);
    }
    
    char username[50];
    char password[20];
    
    printf("Choose a Username (max 49 chars): ");
    scanf("%49s", username);
    
    while (1) {
        printf("Choose a Password (min 6 chars, max 19): ");
        scanf("%19s", password);
        if (strlen(password) >= 6) break;
        printf(COLOR_RED "\nPassword must be at least 6 characters long.\n" COLOR_RESET);
    }

    fprintf(fileptr, "%s\n%s\n%s\n%s\n%s\n", 
            username, password, credentials[0], credentials[1], credentials[2]);
    fclose(fileptr);
    
    printf(COLOR_GREEN "\nRegistration successful! Welcome to C-Bank, %s!\n" COLOR_RESET, username);
}

int main(void) {
    int option = 0;

    while (1) {
        welcome();
        scanf("%d", &option);
        clear_input_buffer();

        switch (option) {
            case 1:
                system("clear");
                load_cards();
                Login();
                break;
            case 2:
                system("clear");
                Register();
                system("clear");
                Login();
                break;
            case 3:
                system("clear");
                delete_account();
                break;
            case 4:
                save_cards();
                printf(COLOR_CYAN "\nThank you for using " COLOR_MAGENTA "C-Bank" COLOR_CYAN "! Goodbye!\n" COLOR_RESET);
                return 0;
            default:
                printf(COLOR_RED "\nInvalid option. Please choose between 1-4.\n" COLOR_RESET);
                break;
        }
    }
}
