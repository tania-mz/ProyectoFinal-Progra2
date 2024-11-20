#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>

void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, FILE *fpExchange, int amountOfCustomers, int flag){
 Sales saleRecord;
 Customers customerRecord;
 Products productRecord;
 ExchangeRates exchangeRecord;
 ExchangeRates staticExchangeRecord;

 char customerName[40] = "";
 unsigned int customerKey = 0;

 for(int i = 0; i < amountOfCustomers && i < 5; i++){
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
            
            int index = 0; // Index que se usa para moverse en cada venta 

            int positionOfDate = BinarySearchExchangeDate(fpExchange, recordsPurchases[index]); //posicion cualquiera que saco del binary

            if(positionOfDate == -1){   
                printf("no se encontró la fecha");
            } else { //Despues de comprobar que si se haya sacado una fecha del binary lo que hacemos es pasar al resto de procesbica

            //Ubicamos el puntero en donde encontramos la fecha
            fseek(fpExchange, sizeof(ExchangeRates) * positionOfDate, SEEK_SET);
            fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange);

            if(strcmp("USD", exchangeRecord.Currency) != 0){ //Verificamos si ya es el inicial, que sería en dolares, pues siempre tiene ese orden
                for(int i = positionOfDate; strcmp(exchangeRecord.Currency,"USD") != 0; i--){ //for para devolverme al primero
                    positionOfDate = i; //Voy igualando la posicion de la fecha al i, porque me estoy devolviendo
                    fseek(fpExchange, sizeof(ExchangeRates) * positionOfDate, SEEK_SET); //Y ubico el puntero para volver a comparar en la otra vuelta del for
                    fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange);
                } 
                positionOfDate++; //Le sumo una más ya que en la ultima vuelta no le sumo, puesto que encontro la posicion del primero, por eso me toca sumarle manualmente es que no alcanzo a sumar porque la condicion ya no se cumplia
            }
            
            //Reviso cual es la currency de la venta que voy a calcular, y le sumo esa cantidad a positionOfDate
            if(strcmp(recordsPurchases[index].CurrencyCode, "USD") == 0){
                positionOfDate += 0;
            } else if (strcmp(recordsPurchases[index].CurrencyCode, "CAD") == 0){
                positionOfDate += 1;
            } else if (strcmp(recordsPurchases[index].CurrencyCode, "AUD") == 0){
                positionOfDate += 2;
            } else if (strcmp(recordsPurchases[index].CurrencyCode, "EUR") == 0){
                positionOfDate += 3;
            } else if (strcmp(recordsPurchases[index].CurrencyCode, "GBP") == 0){
                positionOfDate += 4;
            }

            fseek(fpExchange, sizeof(ExchangeRates) * positionOfDate, SEEK_SET);
            fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange); //Me ubico en esa posicion final
            
            float value = exchangeRecord.Exchange; //Extraigo el valor de la moneda en esa fecha y la guardo en value

            /*El siguiente paso seria multiplicar ese value por la cantidad de procutos(recordsPurchases[index].Quantity) y luego multiplicar por cuanto vale cada producto(productrecord.unitprice) 
            eso se puede hacer en el while, y tambien crear las variables para ir guardadno los usbtotales que es por cada orden y el total que es de todas las ordenes del cliente(suma de subtotales)*/

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