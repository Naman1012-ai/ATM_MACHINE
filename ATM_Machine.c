#include<stdio.h>
#include<math.h>
int main(){

    
    int actnum, actpin,entact,entpin,attempts=3,attempts1=3; // ACCOUNT DETAILS!
    
    //BALANCE DETAILS!
    int option;
    float dep,balance=0,with,newbal;

    //CREATING ACCOUNT!

    printf("Create account number : "); // ACCOUNT NUMBER!
    scanf("%d",&actnum);

    printf("Create account pin : "); // ACCOUNT PIN!
    scanf("%d",&actpin);

    printf("Account %d created successfully!\n\n",actnum);

    // LOGIN TO ACCOUNT!

    printf("Welcome to the ATM!\n\n");

    printf("Enter your account number : "); 
    scanf("%d",&entact);

    // CHECKING ACCOUNT NUMBER!

    while(entact!=actnum && attempts>1){
        
        printf("Account Number Doesn't Exist!\nRe-enter account number : ");
        scanf("%d",&entact);
        attempts--;
         if(attempts==1 && entact!=actnum){
         printf("Too Many Wrong Attempts! Exiting...");
         break;
         } 
        
    }

    // GIVING ACCOUNT PIN!

    if(entact==actnum){
        printf("Enter account pin : ");
        scanf("%d",&entpin);
         
        // CHECKING ACCOUNT PIN!

       while(entpin!=actpin && attempts1>1){
       
        printf("Account PIN Doesn't Match!\nRe-enter account pin : ");
        scanf("%d",&entpin);
        attempts1--;
        if(attempts1==1 && entpin!=actpin){
        printf(" Too Many Wrong Attempts! Exiting...");
        break;
        }
        }if(entpin==actpin){
            printf("Authentication Successful!\n\n");
        }
        

    }
        
    // CHOOSING AN OPTION -WHAT TO DO?

    for(int i=1; i<=10; i++){
    
    if(actnum==entact && actpin==entpin){
        printf("\n\n\t--ATM MENU--\t\n\n");
        printf("1.Check Balance\n2.Deposit Money\n3.Withdraw Money\n4.Exit\n");
        printf("Choose an option : ");
        scanf("%d",&option);
    }

     // CHECK BALANCE!

    if(option==1){
        printf("\nYour Balance : %.2f\n",balance);
    }

    // DEPOSIT MONEY!

    if(option==2){
        printf("\nEnter deposit amount : ");
        scanf("%f",&dep);
       
        while(dep<=0){
            printf("Deposited amount must be greater than 0!  Re-enter deposit amount : ");
            scanf("%f",&dep);
        }if(dep>0){
            balance+=dep;
            printf("Deposited Amount :Rs%.2f.\t New Balance :Rs%.2f\n",dep,balance);
        }
    }
    
   
    // WITHDRAWING MONEY!

    if(option==3){
        printf("\nEnter withdrawal amount : ");
        scanf("%f",&with);
        
        while(with>balance){
              printf("Insufficient Balance! Re-enter withdrawal amount : ");
              scanf("%f",&with);
        }
        if(with<=balance){balance-=with;}
        printf("Withdrawal Amount :Rs%.2f.\t New Balance :Rs%.2f\n",with,balance);
    }
    
    // EXITING ATM!

    if(option==4){
        printf("\nExiting ATM. Thank You!");
        break;
    }

    // INVALID OPTION!

    if(entact==actnum && entpin==actpin &&  option<1 || option>4){
        printf("Invalid option!");
        continue;
    }

    }
    

    return 0;

}
