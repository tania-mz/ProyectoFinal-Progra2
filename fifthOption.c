#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>

void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, int amountOfCustomers, int flag){
 Sales saleRecord;
 Customers customerRecord;
 //Products productRecord;

 char customerName[40] = "";
 unsigned short int customerKey = 0;

 for(int i = 0; i < amountOfCustomers; i++){
    fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
    fread(&customerRecord, sizeof(customerRecord), 1, fpCustomers);

    strcpy(customerName, customerRecord.Name);
    printf("\n%s\n", customerName);

    customerKey = customerRecord.CustomerKey;
    printf("Customer Key %i\n", customerKey);

    int positionOfCustomer = BinarySearch(fpSales, customerKey, 4);
    
    if(positionOfCustomer == -1){
    		printf(" -No purchases reported\n");
    	} else {
    		FILE *fpTemporal = fopen("TemporalFileOption5", "wb+");
			if (fpTemporal == NULL) {
			    printf("Error abriendo archivo temporal.\n");
			    return;
			}

            fseek(fpSales, sizeof(Sales) * (positionOfCustomer - 1), SEEK_SET);
    	    fread(&saleRecord, sizeof(Sales), 1, fpSales);

    	    for( int i = positionOfCustomer - 1; i >= 0 && customerKey == saleRecord.CustomerKey; i -= 1){
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&saleRecord, sizeof(Sales), 1, fpSales);

    	    	positionOfCustomer = i;
    	    }

            fseek(fpSales, sizeof(Sales) * positionOfCustomer, SEEK_SET);
            fread(&saleRecord, sizeof(Sales), 1, fpSales);

            int numOfPurchases = 0;
            for(int i = positionOfCustomer; customerKey == saleRecord.CustomerKey; i++){
                numOfPurchases++;
                fseek(fpSales, (sizeof(Sales) * i) + 1, SEEK_SET);
                fread(&saleRecord, sizeof(Sales), 1, fpSales);
            }

            printf("%s    -    %i", customerName, numOfPurchases);   
       } 
    }
}    

void BubbleSortOption5(){
    int amountOfCustomers = TellNumRecords("customersTable", sizeof(Customers));

    BubbleSortCustomersByCustomerName();
    BubbleSortSalesByCustomerKey();

    FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

    ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, amountOfCustomers, 1);

    fclose(fpCustomers);
    fclose(fpProducts);
    fclose(fpSales);
}

void MergeSortOption5(){
    //int amountCustomers = TellNumRecords("customersTable", sizeof(Customers));
}