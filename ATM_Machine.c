#include<stdio.h>
#include<math.h>
#include<string.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define PINK "\033[95m"
#define BG_PINK "\033[105m"
#define BG_GREEN "\033[42m"
#define BG_RED "\033[41m"

#define DATAFILE "users_data_1.txt"
#define TRANSACTIONS "trans.txt"
#define MAX 20

typedef struct user
{
    char username[30];
    char account_num[MAX];
    char account_pin[MAX];
    int total_trans;
    float highest_trans;
    float avl_balance;
} user;

typedef struct trans{
    char account_num[MAX];
    float deposit_amount;
    float withdraw_amount;
    char timestamps[50];
} trans;

int create_account(char *username);
int login(char *username);

void clearscreen()
{
    system("cls");
}

void border(char *color)
{
    printf("%s", color);
    printf("===============================================================\n");
    printf("%s", RESET);
}

void centered(char *text, char *color)
{
    int len = strlen(text);
    int padding = (63 - len) / 2;
    printf("%s", color);
    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }
    printf("%s", text);
    printf("%s\n", RESET);
}

void get_timestamps(char *tmstmp)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int hour12 = (t->tm_hour % 12 == 0) ? 12 : t->tm_hour % 12;
    char *ampm = (t->tm_hour >= 12) ? "PM" : "AM";

    sprintf(tmstmp, "%02d-%s-%d, %02d:%02d %s", t->tm_mday, months[t->tm_mon], t->tm_year + 1900, hour12, t->tm_min, ampm);
}

void getmasked_password(char *pass, int maxlen)
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = getch();
        if (ch == 13)
        { // Enter key
            pass[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 8 && i > 0)
        { // Backspace key
            i--;
            printf("\b \b");
        }
        else if (i < maxlen - 1)
        {
            pass[i++] = ch;
            printf("*");
        }
    }
}

int safe_input(int min, int max)
{
    char buffer[50];
    int num;

    while (1)
    {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            continue; // try again

        // Try converting input to int
        if (sscanf(buffer, "%d", &num) != 1)
        {
            printf(RED"\nInvalid input!"GREEN" Enter a number"RESET":- ");
            continue;
        }

        // Range check
        if (num < min || num > max)
        {
            printf(GREEN"Enter a number between"RESET" %d"GREEN" and"RESET" %d:- "RESET, min, max);
            continue;
        }

        return num; // VALID INPUT
    }
}
int safe_money(){
    char buffer[50];
    float num;

    while (1)
    {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            continue; // try again

        // Try converting input to int
        if (sscanf(buffer, "%f", &num) != 1)
        {
            printf(RED "Invalid money amount!");
            printf("\n\n");
            break;
        }

        // Range check
        if (num <= 0.00)
        {
            printf(PINK"Money amount must be greater than "RESET"'0'"PINK"!");
            printf("\n\n");
            break;
        }

        return num; // VALID INPUT
    }
}

int user_exist(char *account_num)
{
    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL)
        return 0;

    char line[300];
    char userblock[50];
    sprintf(userblock, "[ACCOUNT_NUMBER           : %s]", account_num);

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, userblock))
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void save_trans(trans *att){
    FILE *fp = fopen(TRANSACTIONS, "a");
    if (fp == NULL)
        return;

    FILE *fp1 = fopen(TRANSACTIONS, "r");
    int attmptnum = 1;
    char line[200];
    char search_block[50];
    sprintf(search_block, "[ACCOUNT_NUMBER     : %s]", att->account_num);

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp1))
        {
            if (strstr(line, search_block))
            {
                attmptnum++;
            }
        }
    }
    fprintf(fp, "[ACCOUNT_NUMBER     : %s ##TRANSACTION_NO.: %d]\n", att->account_num, attmptnum);
    fprintf(fp, "DEPOSIT_AMOUNT      : Rs.%.2f\n", att->deposit_amount);
    fprintf(fp, "WITHDRAWAL_AMOUNT   : Rs.%.2f\n", att->withdraw_amount);
    fprintf(fp, "TIME OF TRANSACTION : %s\n\n", att->timestamps);

    fclose(fp);
    fclose(fp1);
}

// #=== SAVE USER DATA THROUGH USER STRUCTURE ===#
void save_userdata(struct user *u)
{
    FILE *fp = fopen(DATAFILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL)
    {
        fp = fopen(DATAFILE, "w");
        fprintf(fp, "[ACCOUNT_NUMBER           : %s]\n", u->account_num);
        fprintf(fp, "OWNER'S NAME              : %s\n", u->username);
        fprintf(fp, "PASSWORD                  : %s\n", u->account_pin);
        fprintf(fp, "TOTAL_TRANSACTIONS        : %d\n", u->total_trans);
        fprintf(fp, "AVL. BALANCE              : Rs.%0.2f\n", u->avl_balance);
        fprintf(fp, "HIGHEST_WITHDRAWAL_AMOUNT : Rs.%0.2f\n\n", u->highest_trans);
        fclose(fp);
        if(temp != NULL) fclose(temp);
        remove("temp.txt");
        return;
    }

    char line[200];
    char user_block[50];
    sprintf(user_block, "[ACCOUNT_NUMBER           : %s]", u->account_num);
    int found = 0;
    int in_user_block = 0;

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, user_block))
        {
            found = 1;
            in_user_block = 1;

            fprintf(temp, "[ACCOUNT_NUMBER           : %s]\n", u->account_num);
            fprintf(temp, "OWNER'S NAME              : %s\n", u->username);
            fprintf(temp, "PASSWORD                  : %s\n", u->account_pin);
            fprintf(temp, "TOTAL_TRANSACTIONS        : %d\n", u->total_trans);
            fprintf(temp, "AVL. BALANCE              : Rs.%0.2f\n", u->avl_balance);
            fprintf(temp, "HIGHEST_WITHDRAWAL_AMOUNT : Rs.%0.2f\n\n", u->highest_trans);

            continue;
        }
        if(in_user_block){
            if(line[0] == '['){
                in_user_block = 0;
                fprintf(temp, "%s", line);
            }
            continue;
        }
        fprintf(temp, "%s", line);
    }

    if (!found)
    {
        fprintf(temp, "[ACCOUNT_NUMBER           : %s]\n", u->account_num);
        fprintf(temp, "OWNER'S NAME              : %s\n", u->username);
        fprintf(temp, "PASSWORD                  : %s\n", u->account_pin);
        fprintf(temp, "TOTAL_TRANSACTIONS        : %d\n", u->total_trans);
        fprintf(temp, "AVL. BALANCE              : Rs.%0.2f\n", u->avl_balance);
        fprintf(temp, "HIGHEST_WITHDRAWAL_AMOUNT : Rs.%0.2f\n\n", u->highest_trans);
    }

    fclose(fp);
    fclose(temp);
    remove(DATAFILE);
    rename("temp.txt", DATAFILE);
}

void load_userdata(char *account_num, user *details)
{
    strcpy(details->account_num, account_num);
    details->total_trans = 0;
    details->highest_trans = 0.0;

    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL)
        return;

    char line[200];
    char user_block[50];

    sprintf(user_block, "[ACCOUNT_NUMBER           : %s]", details->account_num);
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, user_block))
        {
            while (fgets(line, sizeof(line), fp))
            {
                if (line[0] == '[')
                    break;
                if (strstr(line, "OWNER'S NAME              : "))
                {
                    sscanf(line, "OWNER'S NAME              : %s", details->username);
                }
                else if (strstr(line, "PASSWORD                  : "))
                {
                    sscanf(line, "PASSWORD                  : %s", details->account_pin);
                }
                else if (strstr(line, "TOTAL_TRANSACTIONS        : "))
                {
                    sscanf(line, "TOTAL_TRANSACTIONS        : %d", &details->total_trans);
                }
                else if (strstr(line, "AVL. BALANCE              : "))
                {
                    sscanf(line, "AVL. BALANCE              : Rs.%f", &details->avl_balance);
                }
                else if (strstr(line, "HIGHEST_WITHDRAWAL_AMOUNT : "))
                {
                    sscanf(line, "HIGHEST_WITHDRAWAL_AMOUNT : Rs.%f\n\n", &details->highest_trans);
                }
            }
            break;
        }
    }
    fclose(fp);
}

void past_trans(char *account_num)
{
    clearscreen();
    border(MAGENTA);
    centered("YOUR PAST TRANSACTIONS!", BOLD BLUE);
    border(MAGENTA);
    printf("\n");
    
    FILE *fp = fopen(TRANSACTIONS, "r");
    if (fp == NULL)
    {
        printf(BG_RED " No transactions file found!\n" RESET);
        printf("Press any key to return...\n");
        getch();
        return;
    }

    char line[200];
    char search_block[60];
    // Search for the specific account block header
    sprintf(search_block, "[ACCOUNT_NUMBER     : %s", account_num);
    
    int found_any = 0;
    int trans_count = 0;

    while (fgets(line, sizeof(line), fp))
    {
        // Only enter this block if the line starts with YOUR account number
        if (strstr(line, search_block))
        {
            found_any = 1;
            trans_count++;
            
            float dep = 0, with = 0;
            char ts[50] = "N/A";

            // Read the next 3 lines specifically tied to this header
            for (int i = 0; i < 3; i++) {
                if (fgets(line, sizeof(line), fp)) {
                    if (strstr(line, "DEPOSIT_AMOUNT")) 
                        sscanf(line, "DEPOSIT_AMOUNT      : Rs.%f", &dep);
                    else if (strstr(line, "WITHDRAWAL_AMOUNT")) 
                        sscanf(line, "WITHDRAWAL_AMOUNT   : Rs.%f", &with);
                    else if (strstr(line, "TIME OF TRANSACTION")) 
                        sscanf(line, "TIME OF TRANSACTION : %[^\n]", ts);
                }
            }

            printf(YELLOW "TRANSACTION #%d\n", trans_count);
            printf(CYAN "DEPOSIT             :- " RESET "Rs.%.2f\n", dep);
            printf(CYAN "WITHDRAWAL          :- " RESET "Rs.%.2f\n", with);
            printf(CYAN "TIME OF TRANSACTION :- " RESET "%s\n", ts);
            printf(BLUE "---------------------------------------------------------------\n" RESET);
        }
    }
    fclose(fp);

    if (!found_any)
    {
        printf(BG_RED "No transactions found for this account.\n" RESET);
    }

    printf("\nPress any key to return...\n");
    getch();
}

int login(char *account_num)
{
    clearscreen();
    border(CYAN);
    centered("LOGIN", BOLD YELLOW);
    border(CYAN);
    printf("\n");

    int choose;
   
    printf(BLUE "Enter account number" RESET ":- ");
    scanf("%19s", account_num);
    while (getchar() != '\n');

    if (!user_exist(account_num))
    {
        printf("\n");
        printf(BG_RED "Account number doesn't exist!" RESET);
        printf("\n\n");
        printf(YELLOW"--> CREATE ACCOUNT!!\n");
        printf(PINK"1."RESET"YES\n");
        printf(PINK"2."RESET"NO"RESET);
        printf("\n\n");
        printf(CYAN"Enter your choice"RESET":- ");
        choose = safe_input(1, 2);

        if(choose == 1)
        {
            int x = create_account(account_num);
            return x;
        }
        else if(choose == 2)
        {
            return 0;
        }
    }
    
    user details = {0};
    load_userdata(account_num, &details);
          
    char act_pin[MAX];
    int attempts = 0;

    while (attempts < 3)
    {
        printf(BLUE "Enter pin" RESET ":- ");
        getmasked_password(act_pin, MAX);

        if (strcmp(act_pin, details.account_pin) == 0)
        {
            printf(BOLD GREEN "\nLOGIN SUCCESSFULL!\n" RESET);
            printf("\nPress any key to continue...\n");
            getch();
            return 1;
        }

        attempts++;
        if (attempts < 3)
        {
            printf(BG_RED "Wrong Password! %d attempt(s) remaining.\n\n" RESET, 3 - attempts);
        }
    }
    printf(BG_RED "\nToo many wrong attempts! Try again later!!\n" RESET);
    printf("Press any key to continue...\n");
    getch();
    return 0;
}

int create_account(char *account_num)
{
    clearscreen();
    border(CYAN);
    centered("CREATE ACCOUNT", BOLD YELLOW);
    border(CYAN);
    printf("\n");

    int attempts = 0;

    char username[30];
    printf(GREEN "Enter username" RESET ":- ");
    scanf("%29s", username);
    while (getchar() != '\n');
    
    while (strlen(username) < 3)
    {
        printf(BG_RED "Username must be atleast 3 characters long!\n" RESET);
        printf("\n");
        printf(GREEN "Enter username" RESET ":- ");
        scanf("%29s", username);
        while (getchar() != '\n');
        attempts++;     
      
        if (attempts == 2 && strlen(username) < 3)
        {
            printf(BG_RED "Too many wrong attempts! Try again later!!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return 0;
            break;
        }
    }

    user details = {0};

    int is_unique = 0;
    srand(time(NULL)); // Seed the randomizer
    
    while (!is_unique) {
        // Generates a 6-digit random number between 100000 and 999999
        int rand_num = (rand() % 900000) + 100000000;
        sprintf(details.account_num, "%d", rand_num);
        
        // Ensure this number isn't already used by another user in the file
        if (!user_exist(details.account_num)) {
            is_unique = 1;
        }
    }

    printf(PINK "\nYour Permanent Account Number is" RESET ": " BOLD YELLOW "%s\n" RESET, details.account_num);
    printf(BLUE "Please remember this number for future logins!!\n\n" RESET);
    
    char act_pin[MAX];
    char ent_pin[MAX];

    printf(GREEN "Create a pin" RESET ":- ");
    getmasked_password(act_pin, MAX);

    attempts = 0;
    while (strlen(act_pin) < 4)
    {
        printf(BG_RED "Password must be atleast 4 characters long!" RESET);
        printf("\n\n");
        printf(GREEN "Create a pin" RESET ":- ");
        getmasked_password(act_pin, MAX);
        attempts++;

        if (attempts == 2 && (strlen(act_pin) < 4))
        {
            printf("Press any key to exit...\n");
            getch();
            return 1;
            break;
        }
    }

    attempts = 0;
    printf(BLUE "Confirm pin" RESET ":- ");
    getmasked_password(ent_pin, MAX);

    while (strcmp(act_pin, ent_pin) != 0)
    {
        printf("\n");
        printf(BG_RED "Password do not match!" RESET);
        printf("\n\n");
        printf(BLUE "Re-confirm pin" RESET ":- ");
        getmasked_password(ent_pin, MAX);

        attempts++;
        if (attempts == 2 && (strcmp(act_pin, ent_pin) != 0))
        {
            printf(BG_RED "Too many wrong attempts! Try again later!!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return 0;
            break;
        }
    }
       
    printf("\n");
    printf(BG_PINK "Enter amount to be stored" RESET PINK":- " RESET BOLD"Rs. " RESET);
    scanf("%f", &details.avl_balance);
    
    strcpy(details.username, username);
       strcpy(details.account_pin, act_pin);
    details.total_trans = 0;
    details.highest_trans = 0.0;
      
    save_userdata(&details);
    load_userdata(account_num, &details);
   
    printf(BG_GREEN "\nAccount Created Successfully!\n\n" RESET);
    printf(YELLOW "Returning to loginpage..." RESET);
    printf("\nPress any key to continue...\n");
    getch();
    return 0;
}

void deposit_amount(char *account_num){
     
    clearscreen();
    float dep;
    user details = {0};
    load_userdata(account_num, &details);
    
    border(BOLD BLUE);
    centered("DEPOSIT MONEY!", BOLD YELLOW);
    border(BOLD BLUE);
    printf("\n");
        
    int atts = 0;
    while(atts < 3){
        printf(CYAN "Enter deposit amount:- " RESET BOLD "Rs. "RESET);
        dep = safe_money();
        
        if(dep > 0) break;
        if (atts == 2 && dep <= 0)
        {
            printf(BG_RED "\nToo many wrong attempts! Try again later!!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return ;
            break;
        }
        atts++;
    }
    
    
    details.avl_balance += dep;
    details.total_trans++;
    save_userdata(&details);

    trans att;
    strcpy(att.account_num, account_num);
    att.deposit_amount = dep;
    att.withdraw_amount = 0.0;
    get_timestamps(att.timestamps);
    save_trans(&att);

    printf("\n");
    printf(GREEN "Amount of :-" RESET BOLD " Rs. " RESET MAGENTA"%.2f " GREEN "deposited successfully in your account number " RESET "'%s'"GREEN"!", dep, details.account_num); 
    printf(BG_PINK "\nAvl. Balance "RESET PINK ":- " RESET BOLD "Rs. "RESET MAGENTA"%.2f " RESET, details.avl_balance);
    
    printf("\n\nPress any key to return...");
    getch();       
}

void withdraw_amount(char *account_num){
    clearscreen();
    float with;
    user details = {0};
    load_userdata(account_num, &details);
   
    border(MAGENTA);
    centered("WITHDRAW MONEY!", BOLD YELLOW);
    border(MAGENTA);
    printf("\n");
        
    int atts = 0;
    while(atts < 3){

        printf(CYAN"Enter withdrawal amount:- " RESET BOLD "Rs. " RESET);
        with = safe_money();

        if(with <= details.avl_balance && with > 0) break;
        if(with > details.avl_balance){
            printf(BG_RED "\nInsufficient Balance!" RESET );
            printf(BG_PINK "\nAvl. Balance "RESET PINK ":- " RESET BOLD "Rs. " RESET MAGENTA"%.2f " RESET, details.avl_balance);
            printf("\n\n");
        }
        
         if (atts == 2 || with < 0)
        {
            printf(BG_RED "\nToo many wrong attempts! Try again later!!" RESET);
            printf("\n");
            printf("Press any key to return...\n");
            getch();
            return ;
            break;
        }
        atts++;
    }

    if (atts == 2 && with > details.avl_balance )
    {
        printf(BG_RED "\nToo many wrong attempts! Try again later!!" RESET);
        printf("\n");
        printf("Press any key to return...\n");
        getch();
        return ;
    }

    if(with > details.highest_trans){
        details.highest_trans = with;
    }
    details.avl_balance -= with;
    details.total_trans++;
    save_userdata(&details);

    trans att;
    strcpy(att.account_num, account_num);
    if(with > att.withdraw_amount){
        att.withdraw_amount = with;
    }
    att.deposit_amount = 0.0;
    get_timestamps(att.timestamps);
    save_trans(&att);

    printf("\n");
    printf(GREEN "Amount of :-" RESET BOLD " Rs. " RESET MAGENTA"%.2f" GREEN " withdrawn successfully from your account number " RESET "'%s' "GREEN"!", with, details.account_num );
    printf(BG_PINK "\nAvl. Balance "RESET PINK ":- " RESET BOLD "Rs. " RESET MAGENTA"%.2f " RESET, details.avl_balance);

    printf("\n\nPress any key to return...");
    getch();
}

void user_profile(char *account_num){
    clearscreen();
    border(MAGENTA);
    centered("YOUR ACCOUNT DETAILS!", BOLD BLUE);
    border(MAGENTA);
    printf("\n");

    user details = {0};
    load_userdata(account_num, &details);

    printf(YELLOW"ACCOUNT NUMBER     :-"RESET" %s\n", details.account_num);
    printf(YELLOW"OWNER'S NAME       :-"RESET" %s\n", details.username);
    printf(YELLOW"TOTAL TRANSACTIONS :-"RESET" %d\n", details.total_trans);
    printf(YELLOW"YOUR AVL. BALANCE  :-"RESET BOLD" Rs." RESET MAGENTA" %.2f" RESET, details.avl_balance);
    printf("\n\n");
    printf("Press any key to return...");
    getch();
}

int main()
{
    srand(time(NULL));
    char account_num[MAX];
    int choice, x;
    user details = {0};
    load_userdata(account_num, &details);

    while (1)
    {
        // #=== LOGIN PAGE ===#
        clearscreen();
        border(BLUE);
        printf("\n");
        centered("ATM MACHINE", BOLD YELLOW);
        printf("\n");
        border(BLUE);

        printf("\n");
        printf(YELLOW "1." RESET "LOGIN\n");
        printf(YELLOW "2." RESET "EXIT ATM\n");
        printf("\n");

        printf(GREEN "Enter your choice" RESET ":- ");
        choice = safe_input(1, 2);

        if (choice == 1)
        {
            // #=== HOMEPAGE (AFTER SUCCESSFUL LOGIN) ===#
            if (login(account_num))
            {
                while (1)
                {
                    clearscreen();
                    border(MAGENTA);
                    printf("\n");
                    centered("WELCOME", BOLD YELLOW);
                    printf("\n");
                    border(MAGENTA);

                    printf("\n");
                    printf(GREEN "1." RESET "ACCOUNT DETAILS\n");
                    printf(GREEN "2." RESET "WITHDRAW AMOUNT\n");
                    printf(GREEN "3." RESET "DEPOSIT AMOUNT\n");
                    printf(GREEN "4." RESET "PAST TRANSACTIONS\n");
                    printf(GREEN "5." RESET "LOGOUT\n");
                    printf("\n");
                    printf(CYAN "Enter your choice" RESET ":- ");
                    int todo_choice = safe_input(1, 5);

                    if (todo_choice == 1)
                    {
                        user_profile(account_num);
                    }
                    else if (todo_choice == 2)
                    {
                        withdraw_amount(account_num);
                    }
                    else if (todo_choice == 3)
                    {
                        deposit_amount(account_num);
                    }
                    else if (todo_choice == 4)
                    {
                        past_trans(account_num);
                    }
                    else if (todo_choice == 5)
                    {
                        break;
                    }
                }
            }
        }
        else if (choice == 2)
        {
            clearscreen();
            border(MAGENTA);
            centered("EXITING ATM!", BOLD YELLOW);
            printf("\n");
            centered("Thanks For Coming!", BOLD GREEN);
            border(MAGENTA);
            return 0;
        }
    }
    return 0;
}