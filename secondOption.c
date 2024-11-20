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

    for (int i = 0; i < numOfProducts; i++) {
        fseek(fpProducts, sizeof(Products) * i, SEEK_SET);
        fread(&productRecord, sizeof(productRecord), 1, fpProducts);

        strcpy(productName, productRecord.ProductName);
        printf("\nProduct name: %s\n", productName);

        productKey = productRecord.ProductKey;

        int positionSales = BinarySearch(fpSales, productKey, 3);

        if (positionSales == -1) {
            printf(" - No sales reported\n");
        } else {
            FILE *fpTemporal = fopen("TemporalFileOption2", "wb+");
            if (fpTemporal == NULL) {
                printf("Error abriendo archivo temporal.\n");
                return;
            }

            printf("\n%-32s %-32s %-32s %-32s\n", "Continent", "Country", "State", "City");
            printf("______________________________________________________________________________________________________\n");

            // Posicionar al primer registro de venta con la clave del producto
            fseek(fpSales, sizeof(Sales) * (positionSales - 1), SEEK_SET);
            fread(&saleRecord, sizeof(Sales), 1, fpSales);

            for (int i = positionSales - 1; i >= 0 && productKey == saleRecord.ProductKey; i -= 1) {
                fseek(fpSales, sizeof(Sales) * (i - 1), SEEK_SET);
                fread(&saleRecord, sizeof(Sales), 1, fpSales);
                positionSales = i;
            }

            fseek(fpSales, sizeof(Sales) * positionSales, SEEK_SET);
            fread(&saleRecord, sizeof(Sales), 1, fpSales);

            int index = positionSales;
            int numOfBuyers = 0, positionCustomers = 0;

            // Guardar clientes que compraron el producto en archivo temporal
            while (saleRecord.ProductKey == productKey && index <= 66283) {
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

            // Ordenar el archivo temporal si es necesario
            if (flag) {
                // Ordenamiento burbuja directamente en el archivo temporal
                for (int step = 0; step < numOfBuyers - 1; step++) {
                    for (int j = 0; j < numOfBuyers - step - 1; j++) {
                        Customers customer1, customer2;
                        fseek(fpTemporal, sizeof(Customers) * j, SEEK_SET);
                        fread(&customer1, sizeof(Customers), 1, fpTemporal);
                        fread(&customer2, sizeof(Customers), 1, fpTemporal);

                        int comparation = strcmp(customer1.Continent, customer2.Continent);
                        if (comparation > 0 ||
                            (comparation == 0 && strcmp(customer1.Country, customer2.Country) > 0) ||
                            (comparation == 0 && strcmp(customer1.Country, customer2.Country) == 0 &&
                             strcmp(customer1.State, customer2.State) > 0) ||
                            (comparation == 0 && strcmp(customer1.Country, customer2.Country) == 0 &&
                             strcmp(customer1.State, customer2.State) == 0 &&
                             strcmp(customer1.City, customer2.City) > 0)) {

                            // Intercambiar registros en el archivo
                            fseek(fpTemporal, sizeof(Customers) * j, SEEK_SET);
                            fwrite(&customer2, sizeof(Customers), 1, fpTemporal);
                            fwrite(&customer1, sizeof(Customers), 1, fpTemporal);
                        }
                    }
                }
            }else{
				MergeSortFile(fpTemporal, 0, numOfBuyers -1, sizeof(Customers), CompareCustomersByLocation);
			}

            // Mostrar los registros ordenados
            rewind(fpTemporal);
            for (int i = 0; i < numOfBuyers; i++) {
                fread(&customerRecord, sizeof(Customers), 1, fpTemporal);
                printf("%-32s %-32s %-32s %-32s\n", customerRecord.Continent, customerRecord.Country, customerRecord.State, customerRecord.City);
            }

            fclose(fpTemporal);
            remove("TemporalFileOption2");
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

	FILE *fpProducts = fopen("productsTable", "rb+");	//Pointer to ProductsTable
    FILE *fpCustomers = fopen("customersTable", "rb+");	//Pointer to CustomersTable
    FILE *fpSales = fopen("salesTable", "rb+");			//Pointer to SalesTable	

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

void MergeSortOption2() {
    int numRecordsProducts = TellNumRecords("ProductsTable", sizeof(Products));     
    int numRecordsCustomers = TellNumRecords("CustomersTable", sizeof(Customers));     
    int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales));

    FILE *fpProducts = fopen("ProductsTable", "rb+");    
    FILE *fpCustomers = fopen("CustomersTable", "rb+");  
    FILE *fpSales = fopen("SalesTable", "rb+");            

    if (!fpProducts || !fpCustomers || !fpSales) {
        printf("Error al abrir uno de los archivos.\n");
        if (fpProducts) fclose(fpProducts);
        if (fpCustomers) fclose(fpCustomers);
        if (fpSales) fclose(fpSales);
        return;
    }

    // Ordenar el archivo CustomersTable por CustomerKey
    MergeSortFile(fpCustomers, 0, numRecordsCustomers - 1, sizeof(Customers), CompareCustomersByCustomerKey);

    // Ordenar el archivo SalesTable por ProductKey
    MergeSortFile(fpSales, 0, numRecordsSales - 1, sizeof(Sales), CompareSalesByProductKey);

    // Ordenar el archivo ProductsTable por ProductName
    MergeSortFile(fpProducts, 0, numRecordsProducts - 1, sizeof(Products), CompareProductsByProductName);

    // Cerrar los archivos después del ordenamiento
    fclose(fpProducts);
    fclose(fpCustomers);
    fclose(fpSales);

    // Llamar a la función DeterminatedCustomersLocation para generar el reporte o análisis
    fpProducts = fopen("ProductsTable", "rb"); // Reabrir en modo solo lectura para la función
    fpSales = fopen("SalesTable", "rb");
    fpCustomers = fopen("CustomersTable", "rb");

    if (fpProducts && fpSales && fpCustomers) {
        DeterminatedCustomersLocation(fpProducts, fpSales, fpCustomers, numRecordsProducts, 0);
    } else {
        printf("Error al reabrir los archivos para la función DeterminatedCustomersLocation.\n");
    }

    // Cerrar archivos después de su uso
    if (fpProducts) fclose(fpProducts);
    if (fpSales) fclose(fpSales);
    if (fpCustomers) fclose(fpCustomers);
}
