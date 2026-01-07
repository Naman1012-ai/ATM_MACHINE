#include "common.h"

// External prototypes
void clearscreen(); 
void border(char *color); 
void centered(char *text, char *color);
int safe_input(int min, int max); 
int login(char *account_num);
int create_account(char *account_num);
void load_userdata(char *account_num, user *details);
void user_profile(char *account_num); 
void withdraw_amount(char *account_num);
void deposit_amount(char *account_num); 
void past_transactions(char *account_num);
void transfer_money(char *account_num);

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
        printf(YELLOW "2." RESET "CREATE ACCOUNT\n");
        printf(YELLOW "3." RESET "EXIT ATM\n");
        printf("\n");

        printf(GREEN "Enter your choice" RESET ":- ");
        choice = safe_input(1, 3);

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
                    printf(GREEN "5." RESET "TRANSFER MONEY\n");
                    printf(GREEN "6." RESET "LOGOUT\n");
                    printf("\n");
                    printf(CYAN "Enter your choice" RESET ":- ");
                    int todo_choice = safe_input(1, 6);

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
                        past_transactions(account_num);
                    }
                    else if(todo_choice == 5)
                    {
                        transfer_money(account_num);
                    }
                    else if (todo_choice == 6)
                    {
                        break;
                    }
                }
            }
        }
        else if (choice == 2){
            create_account(account_num);
        }
        else if (choice == 3)
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