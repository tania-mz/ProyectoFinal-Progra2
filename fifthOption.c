#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>

void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, FILE *fpExchange, int amountOfCustomers, int flag){
 Sales saleRecord;
 Customers customerRecord;
 Products productRecord;
 ExchangeRates exchangeRecord;

 char customerName[40] = "";
 unsigned int customerKey = 0;

 for(int i = 0; i < amountOfCustomers; i++){
    fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
    fread(&customerRecord, sizeof(customerRecord), 1, fpCustomers);

    strcpy(customerName, customerRecord.Name);
    printf("\n\n/=================================================================================================================/");
    printf("\n%s\n\n", customerName);

    customerKey = customerRecord.CustomerKey;

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

            int order = saleRecord.OrderNumber;
            int limitOfSales = TellNumRecords("salesTable", sizeof(Sales));
            int numOfPurchases = 0;
            int numOfOrders = 1;
            for(int i = positionOfCustomer; customerKey == saleRecord.CustomerKey && i < limitOfSales; i++){
                numOfPurchases++;
                if(order != saleRecord.OrderNumber){
                    numOfOrders++;
                    order = saleRecord.OrderNumber;
                }
                fseek(fpSales, sizeof(Sales) * (i + 1), SEEK_SET);
                fread(&saleRecord, sizeof(Sales), 1, fpSales);
            }

            Sales *recordsPurchases = (Sales * ) calloc(numOfPurchases, sizeof(Sales));

            for(int i = 0; i < numOfPurchases; i++){
                fseek(fpSales, sizeof(Sales) * (positionOfCustomer + i), SEEK_SET);
                fread(&recordsPurchases[i], sizeof(Sales), 1, fpSales);
            }

            for(int step = 0; step < numOfPurchases - 1; step++){
                for(int i = 0; i < numOfPurchases - step - 1; i++){
                    if(recordsPurchases[i].OrderNumber > recordsPurchases[i + 1].OrderNumber){
                        Sales temp = recordsPurchases[i];
                        recordsPurchases[i] = recordsPurchases[i + 1];
						recordsPurchases[i + 1] = temp;
                    } else if (recordsPurchases[i].OrderNumber == recordsPurchases[i + 1].OrderNumber){
                        if(recordsPurchases[i].ProductKey > recordsPurchases[i + 1].ProductKey){
                            Sales temp = recordsPurchases[i];
                            recordsPurchases[i] = recordsPurchases[i + 1];
						    recordsPurchases[i + 1] = temp;
                        }
                    }
                }
            }
            
          
            int index = 0;

            int positionOfDate = BinarySearchExchangeRates(fpExchange, recordsPurchases[index]);

            if(positionOfDate == -1){
                printf("no se encontró la fecha");
            }
            
              /*fseek(fpSales, sizeof(Sales) * (positionSales - 1), SEEK_SET);
    	    fread(&saleRecord, sizeof(Sales), 1, fpSales);

    	    for( int i = positionSales - 1; i >= 0 && productKey == saleRecord.ProductKey; i -= 1){
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&saleRecord, sizeof(Sales), 1, fpSales);

    	    	positionSales = i;
    	    }*/

            fseek(fpExchange, sizeof(ExchangeRates) * positionOfDate, SEEK_SET);
            fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange);

            

            for(int i = positionOfDate;   ) {
                
            }
            

            for(int i = 0; i < numOfOrders; i++){
                printf("Order Date: %hu/%u/%u            Order Number: %li\n", recordsPurchases[index].OrderDate.MM, recordsPurchases[index].OrderDate.DD, recordsPurchases[index].OrderDate.AAAA, recordsPurchases[index].OrderNumber);
                printf("%-20s  %-60s  %-10s  %-6s %s \n", "Product Key", "Product Name", "Quantity", "Value", recordsPurchases[index].CurrencyCode);
                while(index < numOfPurchases + 1 && recordsPurchases[index].OrderNumber == recordsPurchases[index + 1].OrderNumber){
                    unsigned int productKey = recordsPurchases[index].ProductKey;
                    int positionOfProduct = BinarySearch(fpProducts, productKey, 1);
                    fseek(fpProducts, sizeof(Products) * positionOfProduct, SEEK_SET);
                    fread(&productRecord, sizeof(Products), 1, fpProducts);
                    printf("%-20d  %-60s  %-6u \n", recordsPurchases[index].ProductKey, productRecord.ProductName, recordsPurchases[index].Quantity);
                    index++;
                } 
                unsigned int productKey = recordsPurchases[index].ProductKey;
                int positionOfProduct = BinarySearch(fpProducts, productKey, 1);
                fseek(fpProducts, sizeof(Products) * positionOfProduct, SEEK_SET);
                fread(&productRecord, sizeof(Products), 1, fpProducts);
                printf("%-20d  %-60s  %-6u\n", recordsPurchases[index].ProductKey, productRecord.ProductName, recordsPurchases[index].Quantity);
                index++;
                printf("_________________________________________________________________________________________________________________\n");
            }


            fclose(fpTemporal); 
       } 
    }
}    

void BubbleSortOption5(){
    int amountOfCustomers = TellNumRecords("customersTable", sizeof(Customers));
    printf("%i", amountOfCustomers);

    BubbleSortCustomersByCustomerName();
    BubbleSortSalesByCustomerKey();

    FILE *fpProducts = fopen("productsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("customersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("salesTable", "rb+");			//Pointer to SalesTable	
    FILE *fpExchange = fopen("exchangeTable", "rb+");

    ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, fpExchange, amountOfCustomers, 1);

    fclose(fpCustomers);
    fclose(fpProducts);
    fclose(fpSales);
}

void MergeSortOption5(){
    //int amountCustomers = TellNumRecords("customersTable", sizeof(Customers));
}