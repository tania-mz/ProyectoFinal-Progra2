#include <stdio.h>
#include "functions.c"
#include <time.h>
#include "secondOption.c"
#include "fifthOption.c"

void ShowDateTime() {
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    char formattedTime[50] = "";

    strftime(formattedTime, 50, "Valid to %Y-%b-%d at %H:%M hours", local_time);
    printf("%s\n", formattedTime);
}

void PrintExecutionTime(double time){
    int minutes = 0, seconds = 0;
    char totalTime[6] = "";

    minutes = (int) time / 60;
    seconds = (int) time % 60;

    sprintf(totalTime, "%02d'%02d''", minutes, seconds);
    printf("%s", totalTime);
}

//Function to start the second option of th menu, this one calls another function depending on the flag
//flag is a parameter that represents if the user wants the 2.1(1) or 2.2(2) option
void SecondOption(int flag){
    int start = 0, finish = 0;
    double totalSeconds = 0.0;
    start = clock();
    printf("-------------------------------------------------------------------------------------------\n");
    printf("Company Global Electronics Retailer\n");
    ShowDateTime();
    printf("Products list ordered by ProductName + Continent + Country + State + City\n");
    printf("-------------------------------------------------------------------------------------------\n");
    if(flag){
      BubbleSortOption2();
    }else{
      MergeSortOption2(); 
    }
    finish = clock();
    totalSeconds = ((double)(finish - start))/CLOCKS_PER_SEC;
    printf("\n-------------------------------------------------------------------------------------------\n");
    printf("Time used to produce this listing: "); PrintExecutionTime(totalSeconds);
    printf("\n*********************************LAST LINE OF THE REPORT***********************************\n");
    printf("-------------------------------------------------------------------------------------------\n\n\n");
}

void FifthOption(int flag){
    int start = 0, finish = 0;
    double totalSeconds = 0.0;
    start = clock();
    printf("-------------------------------------------------------------------------------------------\n");
    printf("Company Global Electronics Retailer\n");
    ShowDateTime();
    printf("Customer list ordered by Costumer name + Order date for sale + ProductKey\n");
    if(flag){
      BubbleSortOption5();
    }else{
      MergeSortOption5(); 
    }
    finish = clock();
    totalSeconds = ((double)(finish - start))/CLOCKS_PER_SEC;
    
    printf("-------------------------------------------------------------------------------------------\n");
    printf("Time used to produce this listing: "); PrintExecutionTime(totalSeconds);
    printf("\n*********************************LAST LINE OF THE REPORT***********************************\n");
    printf("-------------------------------------------------------------------------------------------\n\n\n");
}

void PrintMenu(){
    printf("Company Global Electronics Retailer\n"
       "Options menu\n"
       "0. Exit program\n"
       "1. Construction of the Database with the dataset tables\n"
       "2. List of: What types of products does the company sell, and where are customers located?\n"
       "   2.1 Utility  bubbleSort\n"
       "   2.2 Utility  mergeSort\n"
       "3. List of: Are there any seasonal patterns or trends for order volume or revenue?\n"
       "   3.1 Utility  bubbleSort\n"
       "   3.2 Utility  mergeSort\n"
       "4. List of: How long is the average delivery time in days? Has that changed over time?\n"
       "   4.1 Utility  bubbleSort\n"
       "   4.2 Utility  mergeSort\n"
       "5. List of sales order by \"Customer Name\" + \"Order Date\" + \"ProductKey\"\n"
       "   5.1 Utility  bubbleSort\n"
       "   5.2 Utility  mergeSort\n"
       "What is your option: ");
}

int main(){
    float option = 0.0; //option: keeps the option selected by the user
    do{ //This do while allows to keep showing the menu until the user select 0
        PrintMenu();
        scanf("%f", &option);
        option = (option * 10); //Necessary change in the user input to be able to compare it and know what the user chosed
        if (option == 10) {                                         
            CreateDatasetTables();
            printf("\nTables Created Succesfully\n\n\n");
        } else if (option == 21) {                       
            SecondOption(1);
        } else if (option == 22) { 
            SecondOption(0);                               
        } else if (option == 31) {                       
            printf("Executing option 3.1\n");
        } else if (option == 32) {                       
            printf("Executing option 3.2\n");
        } else if (option == 41) {                       
            printf("Executing option 4.1\n");
        } else if (option == 42) {                       
            printf("Executing option 4.2\n");
        } else if (option == 51) {                       
            FifthOption(1);
        } else if (option == 52) {                       
            FifthOption(0);
        } else if (option == 0) {                                   
            printf("Exiting the program...\n");
        } else                                          
            printf("Invalid option. Please try again.\n\n\n");

    } while(option != 0);

    return 0;
}
