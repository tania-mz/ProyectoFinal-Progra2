#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>

void DeterminatedCustomersLocation(FILE *fpProducts, FILE *fpSales, FILE *fpCustomers, int numOfProducts, int flag){
  	Products productRecord;
	Customers customerRecord;
	Sales saleRecord;

	char productName[100] = "";
	unsigned short int productKey = 0;
	unsigned int customerKey = 0;

	for(int i = 0; i < numOfProducts; i++){
    	fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
    	fread(&productRecord, sizeof(productRecord), 1, fpProducts);

    	strcpy(productName, productRecord.ProductName);
    	printf("\nCustomer name: %s\n", productName);

		productKey = productRecord.ProductKey;


    	int positionSales = BinarySearch(fpSales, productKey, 3);

    	if(positionSales == -1){
    		printf(" - No sales reported\n");
    	} else {
    		FILE *fpTemporal = fopen("TemporalFileOption2", "wb+");
			if (fpTemporal == NULL) {
			    printf("Error abriendo archivo temporal.\n");
			    return;
			}

    	    printf("\n%-32s %-32s %-32s %-32s\n", "Continent", "Country", "State", "City");
    	    printf("______________________________________________________________________________________________________\n");

    	    fseek(fpSales, sizeof(Sales) * (positionSales - 1), SEEK_SET);
    	    fread(&saleRecord, sizeof(Sales), 1, fpSales);

    	    for( int i = positionSales - 1; i >= 0 && productKey == saleRecord.ProductKey; i -= 1){
    	    	fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
    	    	fread(&saleRecord, sizeof(Sales), 1, fpSales);

    	    	positionSales = i;
    	    }

			fseek(fpSales, sizeof(Sales) * positionSales, SEEK_SET);
			fread(&saleRecord, sizeof(Sales), 1, fpSales);

			int index = positionSales, numOfBuyers = 0, positionCustomers = 0;
    	    while (saleRecord.ProductKey == productKey && index <= 66283){
				customerKey = saleRecord.CustomerKey;

				positionCustomers = BinarySearch(fpCustomers, customerKey, 2);

				if (positionCustomers != -1) {
                    fseek(fpCustomers, sizeof(Customers) * positionCustomers, SEEK_SET);
                    fread(&customerRecord, sizeof(Customers), 1, fpCustomers);

                    fwrite(&customerRecord, sizeof(Customers), 1, fpTemporal);
                    numOfBuyers++;
                }

				fseek(fpSales, sizeof(Sales) * (index + 1), SEEK_SET);
				fread(&saleRecord, sizeof(Sales), 1, fpSales);

				index++;
			}
			
			Customers *recordsCustomers = (Customers *) calloc(numOfBuyers, sizeof(Customers));

			//For to store the records of CustomersTable in the "array" of Customers
			for(int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArray Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}

			if(flag){
			//For to sort Customers
  			for ( int step = 0; step < numOfBuyers - 1; step += 1 ){
				//printf("\nSort Customers %i", step + 1);
    			for ( int i = 0; i < numOfBuyers - step - 1; i += 1 ){

					int comparation = strcmp(recordsCustomers[i].Continent, recordsCustomers[i + 1].Continent);
    		  		if (comparation > 0){
						Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
						recordsCustomers[i] = recordsCustomers[i + 1];
						recordsCustomers[i + 1] = temp;
    		  		} else if(comparation == 0){

						comparation = strcmp(recordsCustomers[i].Country, recordsCustomers[i + 1].Country);
						if (comparation > 0){
							Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
							recordsCustomers[i] = recordsCustomers[i + 1];
							recordsCustomers[i + 1] = temp;
						} else if (comparation == 0){

							comparation = strcmp(recordsCustomers[i].State, recordsCustomers[i + 1].State);
							if (comparation > 0){
								Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
								recordsCustomers[i] = recordsCustomers[i + 1];
								recordsCustomers[i + 1] = temp;
							} else if (comparation == 0){

								comparation = strcmp(recordsCustomers[i].City, recordsCustomers[i + 1].City);
								if (comparation > 0){
									Customers temp = recordsCustomers[i]; //Temporal variable to exchange the records
									recordsCustomers[i] = recordsCustomers[i + 1];
									recordsCustomers[i + 1] = temp;
								}
							}
						}
					}
    		    }
    		}
			} else {
				MergeSort(recordsCustomers, 0, numOfBuyers -1, sizeof(Customers), CompareCustomersByLocation);
			}

			//For to write each record alredy ordered in the file CustomersTable
			for (int i = 0; i < numOfBuyers; i += 1){
				//printf("\nArchivo Customers %i", i + 1);
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fwrite(&recordsCustomers[i], sizeof(Customers), 1, fpTemporal);
			}

			for(int i  = 0; i < numOfBuyers ; i++){
				fseek(fpTemporal, sizeof(Customers) * i, SEEK_SET);
				fread(&customerRecord, sizeof(Customers), 1, fpTemporal);
				printf("%-32s %-32s %-32s %-32s\n", customerRecord.Continent, customerRecord.Country, customerRecord.State, customerRecord.City);
			}
			fclose(fpTemporal);
			free(recordsCustomers);
			recordsCustomers = NULL;
 		}
	}
}

void BubbleSortOption2(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	//Quantity of products in ProductsTable
	//int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	//Quantity of products in CustomersTable
	//int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

	BubbleSortProductsByProductName();	//To BubbleSort the ProductsTable File
	BubbleSortCustomersByCustomerKey();	//To BubbleSort the CustomersTable File
	BubbleSortSalesByProductKey();//To BubbleSort the SalesTable File

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	if (fpProducts == NULL){
		printf("Error opening the 'ProductsTable' File");
		return;
	}
	if (fpCustomers == NULL){
		printf("Error opening the 'CustomersTable' File");
		return;
	}
	if (fpSales == NULL){
		printf("Error opening the 'SalesTable' File");
		return;
	}
	
	DeterminatedCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 1);

    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);
}

void MergeSortOption2(){
	int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products)); 	
	int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers)); 	
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales));

	FILE *fpProducts = fopen("ProductsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	Customers *customersRecord = (Customers *) calloc(numRecordsCustomers, sizeof(Customers));

	for(int i = 0; i < numRecordsCustomers; i++){
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&customersRecord[i], sizeof(Customers), 1, fpCustomers);
	}

	MergeSort(customersRecord, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByCustomerKey);

	for(int i = 0; i < numRecordsCustomers; i++){
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&customersRecord[i], sizeof(Customers), 1, fpCustomers);
	}

	Sales *salesRecord = (Sales *) calloc(numRecordsSales, sizeof(Sales));

	for(int i = 0; i < numRecordsSales; i++){
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fread(&salesRecord[i], sizeof(Customers), 1, fpSales);
	}

	MergeSort(salesRecord, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByProductKey);

	for(int i = 0; i < numRecordsSales; i++){
		fseek(fpSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&salesRecord[i], sizeof(Sales), 1, fpSales);
	}

	Products *productsRecord = (Products *) calloc(numRecordsProducts, sizeof(Products));

	for(int i = 0; i < numRecordsProducts; i++){
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fread(&productsRecord[i], sizeof(Products), 1, fpProducts);
	}

	MergeSort(productsRecord, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductName);

	for(int i = 0; i < numRecordsProducts; i++){
		fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
		fwrite(&productsRecord[i], sizeof(Products), 1, fpProducts);
	}

	free(productsRecord);
	free(customersRecord);
	free(salesRecord);

	DeterminatedCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 0);
}