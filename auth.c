#include "common.h"

void clearscreen(); 
void border(char *color); 
void centered(char *text, char *color);
void getmasked_password(char *pass, int maxlen); 
int safe_input(int min, int max);
int user_exist(char *account_num); 
void save_userdata(struct user *u); 
void load_userdata(char *account_num, user *details);
int create_account(char *account_num);

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