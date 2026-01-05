#include "common.h"

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
        printf(" ");

    printf("%s", text);
    printf("%s\n", RESET);
}

void get_timestamps(char *tmstmp) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int hour12 = (t->tm_hour % 12 == 0) ? 12 : t->tm_hour % 12;
    char *ampm = (t->tm_hour >= 12) ? "PM" : "AM";
    sprintf(tmstmp, "%02d-%s-%d, %02d:%02d %s", t->tm_mday, months[t->tm_mon], t->tm_year + 1900, hour12, t->tm_min, ampm);
}

void getmasked_password(char *pass, int maxlen) {
    int i = 0; char ch;
    while (1) {
        ch = getch();
        if (ch == 13) 
        {
            pass[i] = '\0'; printf("\n"); break; 
        }
        else if (ch == 8 && i > 0) 
        { 
            i--; printf("\b \b"); 
        }
        else if (i < maxlen - 1) 
        { 
            pass[i++] = ch; printf("*"); 
        }
    }
}

int safe_input(int min, int max) {
    char buffer[50]; int num;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        if (sscanf(buffer, "%d", &num) != 1) {
            printf(RED "\nInvalid input!" GREEN " Enter a number" RESET ":- ");
            continue;
        }
        if (num < min || num > max) {
            printf(GREEN "Enter a number between" RESET " %d" GREEN " and" RESET " %d:- " RESET, min, max);
            continue;
        }
        return num;
    }
}

int safe_money() {
    char buffer[50]; float num;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        if (sscanf(buffer, "%f", &num) != 1) {
            printf(RED "Invalid money amount!");
            printf("\n\n"); break;
        }
        if (num <= 0.00) {
            printf(PINK "Money amount must be greater than " RESET "'0'" PINK "!");
            printf("\n\n"); break;
        }
        return num;
    }
    return -1;
}