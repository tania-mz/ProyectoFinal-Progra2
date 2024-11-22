#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

void ShowCustomersPurchases(FILE *fpSales, FILE *fpCustomers, FILE *fpProducts, FILE *fpExchange, int amountOfCustomers, int flag){
	Sales saleRecord;				//Used to store a record of SalesTable and get its information
	Customers customerRecord;		//Used to store a record of CustomersTable and store it temporarely in TemporalFileOption2
	ExchangeRates exchangeRecord;

    char customerName[40] = ""; 				//Used to store the ProductName of each Product in ProductsTable
	unsigned int customerKey = 0; 						//Used to store the ProductKey of each Product in ProductTable

    for(int i = 0; i < amountOfCustomers; i++){
        fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
    	fread(&customerRecord, sizeof(Customers), 1, fpCustomers);

        strcpy(customerName, customerRecord.Name);
        printf("\n\n/=================================================================================================================/");
        printf("\nCustomer name: %-40s", customerName);

        customerKey = customerRecord.CustomerKey;

        int positionOfCustomer = BinarySearch(fpSales, customerKey, 4);

        if(positionOfCustomer == -1){
            printf(" - No purchases reported\n");
        } else {

			FILE *fpTemporalPurchases = fopen("recordPurchases", "wb+");

            fseek(fpSales, (positionOfCustomer - 1) * sizeof(Sales), SEEK_SET);
            fread(&saleRecord, sizeof(Sales), 1, fpSales);

            for( int i = positionOfCustomer - 1; i >= 0 && customerKey == saleRecord.CustomerKey; i--){
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&saleRecord, sizeof(Sales), 1, fpSales);
    	    	positionOfCustomer = i; //Changing positionSales to be the index of first record in sales with the current CustomerKey
    	    }

            fseek(fpSales, sizeof(Sales) * positionOfCustomer, SEEK_SET);
            fread(&saleRecord, sizeof(Sales), 1, fpSales);

            int numOfOrders = 0, numOfPurchases = 0, limitOfSales = TellNumRecords("salesTable", sizeof(Sales));
			long orderNumber = saleRecord.OrderNumber;
            //index es para moverse en sales y i es para moverse en el temporal
            for(int index = positionOfCustomer, i = 0; saleRecord.CustomerKey == customerKey && index < limitOfSales; index++, i++){
                fseek(fpTemporalPurchases, i * sizeof(Sales), SEEK_SET);
				fwrite(&saleRecord, sizeof(Sales), 1, fpTemporalPurchases);
				numOfPurchases++;
                fseek(fpSales, (index + 1) * sizeof(Sales), SEEK_SET);
                fread(&saleRecord, sizeof(Sales), 1, fpSales);
				if(orderNumber != saleRecord.OrderNumber){
					numOfOrders++;
					orderNumber = saleRecord.OrderNumber;
				}
            }

			if(flag){
				for(int step = 0; step < numOfPurchases - 1; step++){
					Sales reg1, reg2;
					for(int i = 0; i < numOfPurchases - step - 1; i++){
						fseek(fpTemporalPurchases, sizeof(Sales) * i, SEEK_SET);
						fread(&reg1, sizeof(Sales), 1, fpTemporalPurchases);

						fseek(fpTemporalPurchases, sizeof(Sales) * (i + 1), SEEK_SET);
						fread(&reg2, sizeof(Sales), 1, fpTemporalPurchases);
						int comparation = reg1.OrderNumber - reg2.OrderNumber;
						if(comparation == 0){
							comparation = reg1.ProductKey - reg2.ProductKey;
						}
						if(comparation > 0){
							fseek(fpTemporalPurchases, sizeof(Sales) * i, SEEK_SET);
							fwrite(&reg2, sizeof(Sales), 1, fpTemporalPurchases);

							fseek(fpTemporalPurchases, sizeof(Sales) * (i + 1), SEEK_SET);
							fwrite(&reg1, sizeof(Sales), 1, fpTemporalPurchases);
						}
					}
				}
			} else {
				MergeSortFile(fpTemporalPurchases, 0, numOfPurchases - 1, sizeof(Sales), CompareSalesByOrderNumber);
			}

			Sales firstPurchaseRecord, secondPurchaseRecord;//Se crean dos para compararlos en el while

			fseek(fpSales, positionOfCustomer * sizeof(Sales), SEEK_SET);
            fread(&saleRecord, sizeof(Sales), 1, fpSales); //Me vuelvo a ubicar en la primera venta del
			
			int orderIndex = 0; 			// Inicializar el índice de órdenes
			float totalValue = 0.0;      	// Total acumulado para todas las órdenes

			for (int order = 0; order < numOfOrders; order++) {
			    // Leer el primer registro de la orden actual
			    fseek(fpTemporalPurchases, sizeof(Sales) * orderIndex, SEEK_SET);
			    fread(&firstPurchaseRecord, sizeof(Sales), 1, fpTemporalPurchases);

			    // Leer el siguiente registro para comparación
			    fseek(fpTemporalPurchases, sizeof(Sales) * (orderIndex + 1), SEEK_SET);
			    fread(&secondPurchaseRecord, sizeof(Sales), 1, fpTemporalPurchases);

			    float exchange = -1.0;

				// Buscar el tipo de cambio para esta orden
				int positionExchange = BinarySearchExchangeDate(fpExchange, firstPurchaseRecord); 
				if (positionExchange != -1){
					ExchangeRates staticExchangeRecord;
					fseek(fpExchange, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
					fread(&staticExchangeRecord, sizeof(ExchangeRates), 1, fpExchange);
					int index = positionExchange;
					printf("DATE:%s   currency:%s \n", staticExchangeRecord.Date, staticExchangeRecord.Currency);
					if (strcmp("USD", staticExchangeRecord.Currency) != 0){
						fseek(fpExchange, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
						fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange);
						for (; strcmp("USD", exchangeRecord.Currency) != 0 && index > 0; index--){
							fseek(fpExchange, sizeof(ExchangeRates) * index, SEEK_SET);
							fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange);
						}
						index++;
					}

					printf("\tdate:%s   currency:%s\n" , exchangeRecord.Date , exchangeRecord.Currency );
					FILE *fpTemporalExchange = tmpfile();
					for (int i = 0; i < 5; i++, index++){
						fseek(fpExchange, sizeof(ExchangeRates) * index, SEEK_SET);
						fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpExchange);
						fseek(fpTemporalExchange, sizeof(ExchangeRates) * i, SEEK_SET);
						fwrite(&exchangeRecord, sizeof(ExchangeRates), 1, fpTemporalExchange);
						//printf("date:%s   currency:%s exchange:%f \n", exchangeRecord.Date, exchangeRecord.Currency, recordExchange.Exchange);
					}

					int indexTemoralExchange = -1;
					if (firstPurchaseRecord.CurrencyCode[0] == 'U'){
						indexTemoralExchange = 0;
					}
					else if (firstPurchaseRecord.CurrencyCode[0] == 'C'){
						indexTemoralExchange = 1;
					}
					else if (firstPurchaseRecord.CurrencyCode[0] == 'A'){
						indexTemoralExchange = 2;
					}
					else if (firstPurchaseRecord.CurrencyCode[0] == 'E'){
						indexTemoralExchange = 3;
					}
					else if (firstPurchaseRecord.CurrencyCode[0] == 'G'){
						indexTemoralExchange = 4;
					}
					//printf ("\n\t indextemporal : %i" , indexTemoralExchange);
					fseek(fpTemporalExchange, sizeof(ExchangeRates) * indexTemoralExchange, SEEK_SET);
					fread(&exchangeRecord, sizeof(ExchangeRates), 1, fpTemporalExchange);
                
					exchange = exchangeRecord.Exchange;
					//printf("\n fecha : %s     exchange: %f\n", exchangeRecord.Date ,  exchange);
                   
					fclose(fpTemporalExchange);
				}

			    // Mostrar encabezado de la orden
			    printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", 
			           firstPurchaseRecord.OrderDate.AAAA, firstPurchaseRecord.OrderDate.MM, firstPurchaseRecord.OrderDate.DD, 
			           firstPurchaseRecord.OrderNumber);
				printf("%-17s%-100s%-15s%s %s", "ProductKey", "ProductName", "Quantity", "Value", firstPurchaseRecord.CurrencyCode);
			    printf("_________________________________________________________________________________________________________________________________________________________\n");

			    float subTotal = 0.0;
			    Products tempProductRecord;

			    // Iterar sobre todos los productos de la misma orden
			    while (orderIndex < numOfPurchases - 1 && firstPurchaseRecord.OrderNumber == secondPurchaseRecord.OrderNumber) {
			        unsigned int productKey = firstPurchaseRecord.ProductKey;

			        // Buscar el producto en el archivo de productos
			        int positionProducts = BinarySearch(fpProducts, productKey, 1);
			        if (positionProducts != -1) {
			            fseek(fpProducts, sizeof(Products) * positionProducts, SEEK_SET);
			            fread(&tempProductRecord, sizeof(Products), 1, fpProducts);

			            // Calcular el valor del producto
			            float price = tempProductRecord.UnitPriceUSD * exchange * firstPurchaseRecord.Quantity;
			            printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.ProductKey, tempProductRecord.ProductName, firstPurchaseRecord.Quantity, price);
			            subTotal += price;
			        }

			        // Avanzar al siguiente registro
			        orderIndex++;

			        // Actualizar los datos para la próxima iteración
			        fseek(fpTemporalPurchases, sizeof(Sales) * orderIndex, SEEK_SET);
			        fread(&firstPurchaseRecord, sizeof(Sales), 1, fpTemporalPurchases);

			        fseek(fpTemporalPurchases, sizeof(Sales) * (orderIndex + 1), SEEK_SET);
			        fread(&secondPurchaseRecord, sizeof(Sales), 1, fpTemporalPurchases);
			    }

			    // Procesar el último producto de la orden
			    unsigned int productKey = firstPurchaseRecord.ProductKey;

			    int positionProducts = BinarySearch(fpProducts, productKey, 1);
			    if (positionProducts != -1) {
			        fseek(fpProducts, sizeof(Products) * positionProducts, SEEK_SET);
			        fread(&tempProductRecord, sizeof(Products), 1, fpProducts);

			        float price = tempProductRecord.UnitPriceUSD * exchange * firstPurchaseRecord.Quantity;
			        printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.ProductKey, tempProductRecord.ProductName, firstPurchaseRecord.Quantity, price);
			        subTotal += price;
			    }

			    orderIndex++; // Avanzar al siguiente índice
			    printf("_________________________________________________________________________________________________________________________________________________________\n");
				printf("%-117s%-15s%.2lf\n", "", "Subtotal", subTotal);
			    totalValue += subTotal;
			}

			// Mostrar el total acumulado
			printf("%-117s%-15s%.2lf\n", "", "TOTAL", totalValue);
            fclose(fpTemporalPurchases);

		}
        printf("\n/--------------------------------------------------------------------------------------------------------------------------------------------------------/\n");
	}
}


void BubbleSortOption5(){
	//int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	//Quantity of products in ProductsTable
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	//int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	
    FILE *fpExchange = fopen("exchangeTable", "rb+");

    BubbleSortProductsByProductKey();
    BubbleSortCustomersByCustomerName();
    BubbleSortSalesByCustomerKey();

	ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, fpExchange, numRecordsCustomers, 1);

    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}

void MergeSortOption5(){
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 		//Quantity of products in CustomersTable
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 					//Quantity of products in SalesTable
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 		//Quantity of products in ProductsTable

    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	
    FILE *fpProducts = fopen("ProductsTable", "rb+");
    FILE *fpExchange = fopen("exchangeTable", "rb+");

	MergeSortFile(fpCustomers, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByCustomerName);

	MergeSortFile(fpSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByCustomerKey);

	MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductKey); //To MergeSortArray the ProductsTable File

	ShowCustomersPurchases(fpSales, fpCustomers, fpProducts, fpExchange, numRecordsCustomers, 0);

    fclose(fpCustomers);
    fclose(fpSales);
    fclose(fpProducts);
    fclose(fpExchange);
}