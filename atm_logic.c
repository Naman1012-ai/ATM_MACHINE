#include "common.h"

// Forward declarations of helper functions from utils.c
void clearscreen(); 
void border(char *color); 
void centered(char *text, char *color);
void get_timestamps(char *tmstmp); 
int safe_money();

int user_exist(char *account_num) {
    FILE *fp = fopen(DATAFILE, "r");
    if (fp == NULL) return 0;
    char line[300], userblock[50];
    sprintf(userblock, "[ACCOUNT_NUMBER           : %s]", account_num);
    
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, userblock)) 
        { 
            fclose(fp); return 1; 
        }
    }

    fclose(fp); return 0;
}

void save_trans(trans *att) {
    FILE *fp = fopen(TRANSACTIONS, "a");
    if (fp == NULL) return;

    FILE *fp1 = fopen(TRANSACTIONS, "r");

    int attmptnum = 1; char line[200], search_block[50];
    sprintf(search_block, "[ACCOUNT_NUMBER     : %s]", att->account_num);
    
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp1)) {
            if (strstr(line, search_block)) attmptnum++;
        }
    }

    fprintf(fp, "[ACCOUNT_NUMBER     : %s ##TRANSACTION_NO.: %d]\n", att->account_num, attmptnum);
    fprintf(fp, "DEPOSIT_AMOUNT      : Rs.%.2f\n", att->deposit_amount);
    fprintf(fp, "WITHDRAWAL_AMOUNT   : Rs.%.2f\n", att->withdraw_amount);
    fprintf(fp, "TIME OF TRANSACTION : %s\n\n", att->timestamps);
    fclose(fp); fclose(fp1);
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
