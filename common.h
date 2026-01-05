#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

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

typedef struct user {
    char username[30];
    char account_num[MAX];
    char account_pin[MAX];
    int total_trans;
    float highest_trans;
    float avl_balance;
} user;

typedef struct trans {
    char account_num[MAX];
    float deposit_amount;
    float withdraw_amount;
    char timestamps[50];
} trans;

#endif