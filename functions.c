#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int TellNumRecords(char fileName[], int recordSize) {
    FILE *fp = fopen(fileName, "rb"); // Abrir en modo binario
    if (fp == NULL) {
        return -1; // Retornar -1 en caso de error al abrir el archivo
    }

    fseek(fp, 0, SEEK_END); // Mover el puntero al final del archivo
    long fileSize = ftell(fp); // Obtener el tamaño del archivo en bytes
    fclose(fp);

    // Calcular el número de registros
    if (recordSize == 0) {
        return -1; // Evitar división por cero
    }
    int numRecords = fileSize / recordSize;

    return numRecords;
}

//--------------------------------EXCHANGE RATES----------------------------------
void CreateExchangeRatesTable(char fileName[]){
    FILE *fP = fopen(fileName, "r");
    FILE *fpExchange = fopen("exchangeTable", "wb");
    if (fP == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpExchange == NULL) {
        printf("Error opening file: exchangeTable\n");
        return;
    }

    //int R = 1; 
    ExchangeRates exchangeRecord;
    char line[30] = "";
    while (fgets(line, sizeof(line), fP)) {
        char *token = strtok(line, ",");
        strcpy(exchangeRecord.Date, token);

        token = strtok(NULL, ",");
        strcpy(exchangeRecord.Currency, token);

        token = strtok(NULL, ",");
        exchangeRecord.Exchange = atof(token);
        
        fwrite(&exchangeRecord, sizeof(exchangeRecord), 1, fpExchange);
        /*printf("Record: %i\n\n", R);
        printf("Date    : %s\n", exchangeRecord.Date);
        printf("Currency: %s\n", exchangeRecord.Currency);
        printf("Exchange: %.2f\n\n", exchangeRecord.Exchange);
        R++;*/
    } 
    fclose(fP); fclose(fpExchange);
}

//-----------------------------PRODUCTS-------------------------------------
void CreateProductsTable(char fileName[]) { // Arreglos 
    FILE *fP = fopen(fileName, "r");
    FILE *fpProducts = fopen("productsTable", "wb");
    if (fP == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpProducts == NULL) {
        printf("Error opening file: productsTable\n");
        return;
    }

    Products productRecord;
    //int R = 1;
    char line[300] = "";
    while (fgets(line, sizeof(line), fP)) {
        char *token = strtok(line, ",");
        char temp[200] = "";

        productRecord.ProductKey = (unsigned short int) atoi(token);

        token = strtok(NULL, ",");
        if (token[0] == '"') {
            while (token[strlen(token) - 1] != '"') {
                strcat(token, strtok(NULL, ","));
            }
            strncpy(productRecord.ProductName, token + 1, strlen(token));
            productRecord.ProductName[strlen(productRecord.ProductName) - 1] = '\0';
        } else {
            strcpy(productRecord.ProductName, token);
        }

        token = strtok(NULL, ",");
        strcpy(productRecord.Brand, token);

        token = strtok(NULL, ",");
        strcpy(productRecord.Color, token);

        token = strtok(NULL, ",");
        if (token[0] == '"') {
            strcat(token, strtok(NULL, ","));
            productRecord.UnitCostUSD = atof(strncpy(temp, token + 2, strlen(token)));
        } else {
            productRecord.UnitCostUSD = atof(strncpy(temp, token + 1, strlen(token)));
        }

        token = strtok(NULL, ",");
        if (token[0] == '"') {
            strcat(token, strtok(NULL, ","));
            productRecord.UnitPriceUSD = atof(strncpy(temp, token + 2, strlen(token)));
        } else {
            productRecord.UnitPriceUSD = atof(strncpy(temp, token + 1, strlen(token)));
        }

        token = strtok(NULL, ",");
        strcpy(productRecord.SubcategoryKey, token);

        token = strtok(NULL, ",");
        if (token[0] == '"') {
            strcat(token, strtok(NULL, ","));
            strncpy(productRecord.Subcategory, token + 1, strlen(token) + 1);
             productRecord.Subcategory[strlen(productRecord.Subcategory) - 1] = '\0';
        } else {
            strcpy(productRecord.Subcategory, token);
        }

        token = strtok(NULL, ",");
        strcpy(productRecord.CategoryKey, token);

        token = strtok(NULL, ",");
        if (token[0] == '"') {
            strcat(token, strtok(NULL, ","));
            strncpy(productRecord.Category, token + 1, strlen(token));
            productRecord.Category[strlen(productRecord.Category)] = '\0';
        } else {
            strcpy(productRecord.Category, token);
        }

        fwrite(&productRecord, sizeof(productRecord), 1, fpProducts);
         /*printf("\nRecord : %i\n\n", R);
            printf("ProductKey    :%hu\n", productRecord.ProductKey);
            printf("ProductName   :%s\n", productRecord.ProductName);
            printf("Brand         :%s\n", productRecord.Brand);
            printf("Color         :%s\n", productRecord.Color);
            printf("UnitCostUSD   :%.2f\n", productRecord.UnitCostUSD);
            printf("UnitPriceUSD  :%.2f\n",productRecord.UnitPriceUSD);
            printf("SubCategoryKey:%s\n", productRecord.SubcategoryKey);
            printf("SubCategory   :%s\n", productRecord.Subcategory);
            printf("CategoryKey   :%s\n", productRecord.CategoryKey);
            printf("Category      :%s\n", productRecord.Category);
            R++;*/
    }
    fclose(fP);fclose(fpProducts);
}

//----------------------------------SALES--------------------------------
void CreateSalesTable(char fileName[]){ 
    FILE *fP = fopen(fileName, "r");
    FILE *fpSales = fopen("salesTable", "wb");
    if (fP == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpSales == NULL) {
        printf("Error opening file: SalesTable\n");
        return;
    }

    char line[1024] = "";
    //int R = 1;
    Sales salesRecord;
    while(fgets(line, sizeof(line), fP)){
        char *token = strtok(line, ",");
        salesRecord.OrderNumber = atol(token);

        token = strtok(NULL, ",");
        salesRecord.LineItems = (unsigned char) atoi(token);

        token = strtok(NULL, ",");
        unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            salesRecord.OrderDate.MM = (unsigned char) month;
            salesRecord.OrderDate.DD = (unsigned char) day;
            salesRecord.OrderDate.AAAA = (unsigned short int) year;

        token = strtok(NULL, ",");
        if(token[0] == ' '){
            salesRecord.DeliveryDate.MM = 0;
            salesRecord.DeliveryDate.DD = 0;
            salesRecord.DeliveryDate.AAAA = 0;
        } else{
            unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            salesRecord.DeliveryDate.MM = (unsigned char) month;
            salesRecord.DeliveryDate.DD = (unsigned char) day;
            salesRecord.DeliveryDate.AAAA = (unsigned short int) year;
        }

         token = strtok(NULL, ",");
         salesRecord.CustomerKey = (unsigned int) atoi(token);

         token = strtok(NULL, ",");
         salesRecord.StoreKey = (unsigned short int) atoi(token);

         token = strtok(NULL, ",");
         salesRecord.ProductKey = (unsigned short int) atoi(token);

         token = strtok(NULL, ",");
         salesRecord.Quantity = (unsigned short int) atoi(token);

         strcpy(salesRecord.CurrencyCode, strtok(NULL, ","));

        fwrite(&salesRecord, sizeof(salesRecord), 1, fpSales);
        /*printf("Record: %i\n\n",R);
        printf("OrderNumber :%ld\n", salesRecord.OrderNumber);
        printf("LineItem    :%u\n", salesRecord.LineItems);
        printf("OrderDate   :%u/%u/%hu\n", salesRecord.OrderDate.MM, salesRecord.OrderDate.DD, salesRecord.OrderDate.AAAA);
        printf("DeliveryDate:%u/%u/%hu\n", salesRecord.DeliveryDate.MM, salesRecord.DeliveryDate.DD, salesRecord.DeliveryDate.AAAA);
        printf("CustomerKey :%u\n", salesRecord.CustomerKey);
        printf("StoreKey    :%hu\n", salesRecord.StoreKey);
        printf("ProductKey  :%hu\n", salesRecord.ProductKey);
        printf("Quantity    :%hu\n", salesRecord.Quantity);
        printf("CurrencyCode:%s\n\n\n", salesRecord.CurrencyCode);
        R++;*/
    }
    fclose(fP); fclose(fpSales);
}
//--------------------------STORES----------------------------------
void CreateStoresTable(char fileName[]){
    FILE *fP = fopen(fileName, "r");
    FILE *fpStores = fopen("storesTable", "wb");
    if (fP == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpStores == NULL) {
        printf("Error opening file: storesTable\n");
        return;
    }

    Stores storesRecord;
    //int R = 1;
    char line[200] = "";
    while(fgets(line, sizeof(line), fP)){
        char *token = strtok(line, ",");
        storesRecord.StoreKey = (unsigned short int) atoi(token);

        token = strtok(NULL, ",");
        strcpy(storesRecord.Country, token);

        token = strtok(NULL, ",");
        strcpy(storesRecord.State, token);

        token = strtok(NULL, ",");
        if(token[1] == '/' || token[2] == '/'){
            storesRecord.SquareMeters = 0;
        }else{
            storesRecord.SquareMeters = (unsigned short int) atoi(token);
            token = strtok(NULL, ",");
        }
        unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            storesRecord.OpenDate.MM = (unsigned char) month;
            storesRecord.OpenDate.DD = (unsigned char) day;
            storesRecord.OpenDate.AAAA = (unsigned short int) year;

        fwrite(&storesRecord, sizeof(storesRecord), 1, fpStores);
        /*printf("Record: %i\n\n", R);
            printf("StoreKey    :%hu\n", storesRecord.StoreKey);
            printf("Country     :%s\n", storesRecord.Country);
            printf("State       :%s\n", storesRecord.State);
            printf("SquareMeters:%hu\n", storesRecord.SquareMeters);
            printf("OpenDate    :%u/%u/%hu\n\n", storesRecord.OpenDate.MM, storesRecord.OpenDate.DD, storesRecord.OpenDate.AAAA);
            R++;*/
    }
    fclose(fP); fclose(fpStores);
}

// -------------------- CUSTOMERS ----------------------------------
void CreateCustomersTable(char fileName[]){
    FILE *fP = fopen(fileName, "r");
    FILE *fpCustomers = fopen("customersTable", "wb"); 
    if (fP == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpCustomers == NULL) {
        printf("Error opening file: customersTable\n");
        return;
    }

    Customers customersRecord;
    char line[400] = "";
    //int R = 1;
    while(fgets(line, sizeof(line), fP)){
        char *token = strtok(line, ";");
        customersRecord.CustomerKey = (unsigned int) atoi(token);

        token = strtok(NULL, ";");
        strcpy(customersRecord.Gender, token);

        token = strtok(NULL, ";");
        strcpy(customersRecord.Name, token);

        token = strtok(NULL, ";");
        strcpy(customersRecord.City, token);

        token = strtok(NULL, ";");
        strcpy(customersRecord.StateCode, token);

        token = strtok(NULL, ";");
        strcpy(customersRecord.State, token);

        token = strtok(NULL, ";");
        if(token[0] >= 48 && token[0] <= 57){
            customersRecord.ZipCode = (unsigned int) atoi(token);
            token = strtok(NULL, ";");
        } else {
            customersRecord.ZipCode = 0;
        }
        
        strcpy(customersRecord.Country, token);

        token = strtok(NULL, ";");
        strcpy(customersRecord.Continent, token);

        token = strtok(NULL, ";");
            unsigned int day = 0, month = 0, year = 0;
            sscanf(token, "%u/%u/%u", &month, &day, &year);
            customersRecord.Birthday.MM = (unsigned char) month;
            customersRecord.Birthday.DD = (unsigned char) day;
            customersRecord.Birthday.AAAA = (unsigned short int) year;

        fwrite(&customersRecord, sizeof(customersRecord), 1, fpCustomers);
        /*printf("Record: %i\n\n", R);
        printf("CustomerKey:%u\n", customersRecord.CustomerKey);
        printf("Gender     :%s\n", customersRecord.Gender);
        printf("Name       :%s\n", customersRecord.Name);
        printf("City       :%s\n", customersRecord.City);
        printf("StateCode  :%s\n", customersRecord.StateCode);
        printf("State      :%s\n", customersRecord.State);
        printf("ZipCode    :%u\n", customersRecord.ZipCode);
        printf("Country    :%s\n", customersRecord.Country);
        printf("Continent  :%s\n", customersRecord.Continent);
        printf("Birthday   :%u/%u/%hu\n\n", customersRecord.Birthday.MM, customersRecord.Birthday.DD, customersRecord.Birthday.AAAA);
        R++;*/
    }  
    fclose(fP); fclose(fpCustomers);
}

void CreateDatasetTables(){
    CreateExchangeRatesTable("Exchange_Rates.csv");
    CreateSalesTable("Sales.csv");
    CreateCustomersTable("Customers.csv");
    CreateStoresTable("Stores.csv");
    CreateProductsTable("Products.csv");
}

int BinarySearch(FILE *fp, unsigned long int valueToSearch, int file){
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = 0;
    unsigned long int key = 0;

    if(file == 1){
		sizeOfRecord = sizeof(Products);
	} else if(file == 2){
		sizeOfRecord = sizeof(Customers);	
	} else if(file == 3){
		sizeOfRecord = sizeof(Sales);
	} else if(file == 4){
        sizeOfRecord = sizeof(Sales);
    } else {
		return -1;
	}

    fseek(fp, 0 ,SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1;
    while(start <= end){
        middle = start + ((end - start) / 2);
        fseek(fp, middle * sizeOfRecord, SEEK_SET);

		if (file == 1){
			Products recordProduct;
			fread(&recordProduct, sizeOfRecord, 1, fp);
			key = (unsigned long int) recordProduct.ProductKey;
            //printf("productsKey que agarro: %u\t", recordProduct.ProductKey);
		} else if (file == 2){
			Customers recordCustomer;
			fread(&recordCustomer, sizeOfRecord, 1, fp);
			key = (unsigned long int) recordCustomer.CustomerKey;
		} else if (file == 3){
			Sales recordSale;
			fread(&recordSale, sizeOfRecord, 1, fp);
			key = (unsigned short int) recordSale.ProductKey;
		}  else if(file == 4){
            Sales recordSale;
			fread(&recordSale, sizeOfRecord, 1, fp);
			key = (unsigned int) recordSale.CustomerKey;
        } else {
		    return -1;
	    }
        //printf("Start: %u\tEnd: %u\tMiddle: %u\tValueToSearch: %lu\tKey: %lu\n", start, end, middle, valueToSearch, key);
        
        if(key == valueToSearch){
            //printf("\nPOR FIN\n");
            return middle;
        }else if(key < valueToSearch){
			start = middle + 1;
        }else{
            end = middle - 1;
        }
    }
	return -1;
}

int BinarySearchExchangeDate(FILE *fp, Sales recordSale){
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = sizeof(ExchangeRates);
    ExchangeRates record;
    fseek(fp, 0 ,SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1;
    while (start <= end){
        middle = start + ((end - start) / 2);
        fseek(fp, middle * sizeOfRecord, SEEK_SET);
        fread(&record, sizeOfRecord, 1, fp);

        char key[11] = "", currencyCode[3] = "";
        strcpy(key, record.Date);
        strcpy(currencyCode, record.Currency);

        //printf("Start: %u, Middle: %u, End: %u, Clave: '%s', Buscando: %d/%d/%d ", start, middle, end, key, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.OrderDate.AAAA);
        //printf(",KeyAux: %s\n", keyAux);
        
        int currentMonth = 0, currentDay = 0, currentYear = 0;
        int month = 0, day = 0, year = 0;
        month = recordSale.OrderDate.MM;
        day = recordSale.OrderDate.DD;
        year = recordSale.OrderDate.AAAA;

        //Leer el formato flexible (con o sin ceros iniciales)
        sscanf(key, "%d/%d/%d", &currentMonth, &currentDay, &currentYear);
        if(currentYear == year){
            if(currentMonth == month){
                if(currentDay == day){
                    return middle;
                } else if(currentDay > day){
                    end = middle - 1;
                } else {
                    start = middle + 1;
                }
            } else if(currentMonth > month){
                end = middle - 1;
            } else {
                start = middle + 1;
            }
        } else if(currentYear > year){
            end = middle - 1;
        } else {
            start = middle + 1;
        }
    }
	return -1;
}

int BubbleSortCustomersByCustomerKey(){
    int numCustomersRecords = TellNumRecords("customersTable", sizeof(Customers)); 
	FILE *fpCustomers = fopen("customersTable", "rb+");	

	Customers *customersRecords = (Customers *) calloc(numCustomersRecords, sizeof(Customers)); //

	for(int i = 0; i < numCustomersRecords; i += 1){
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&customersRecords[i], sizeof(Customers), 1, fpCustomers);
	}

	//For to sort Customers
  	for ( int step = 0; step < numCustomersRecords - 1; step += 1 ){
    	for ( int i = 0; i < numCustomersRecords - step - 1; i += 1 ){
      		if (customersRecords[i].CustomerKey > customersRecords[i + 1].CustomerKey){
				Customers temp = customersRecords[i];
				customersRecords[i] = customersRecords[i + 1];
				customersRecords[i + 1] = temp;
      		}
        }
    }

	//For to write each record alredy ordered in the file CustomersTable
	for (int i = 0; i < numCustomersRecords; i += 1){
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&customersRecords[i], sizeof(Customers), 1, fpCustomers);
	}
    free(customersRecords);
	customersRecords = NULL;
	fclose(fpCustomers);
	return 1;
}

int BubbleSortExchangeRatesByCurrencyCode(FILE *fp){
	ExchangeRates reg1, reg2;

  	for ( int step = 0; step < 5 - 1; step += 1 ){
    	for ( int i = 0; i < 5 - step - 1; i += 1 ){
            fseek(fp, sizeof(ExchangeRates) * i, SEEK_SET);
            fread(&reg1, sizeof(ExchangeRates), 1, fp);
            fseek(fp, sizeof(ExchangeRates) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(ExchangeRates), 1, fp);
      		if (strcmp(reg1.Currency, reg2.Currency) > 0){
				fseek(fp, sizeof(ExchangeRates) * i, SEEK_SET);
                fwrite(&reg2, sizeof(ExchangeRates), 1, fp);
                fseek(fp, sizeof(ExchangeRates) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(ExchangeRates), 1, fp);
      		}
        }
    }

	return 1;
}



int BubbleSortSalesByProductKey(){
	long int amountSalesRecords = TellNumRecords("salesTable", sizeof(Sales));
	FILE *fPSales = fopen("salesTable", "rb+");

	Sales *salesRecords = (Sales *) calloc(amountSalesRecords, sizeof(Sales));

	for(int i = 0; i < amountSalesRecords; i++){
		fseek(fPSales, sizeof(Sales) * i, SEEK_SET);
		fread(&salesRecords[i], sizeof(Sales), 1, fPSales);
	}

	for (int step = 0; step < amountSalesRecords - 1; step += 1){
    	for (int i = 0; i < amountSalesRecords - step - 1; i += 1){
      		if (salesRecords[i].ProductKey > salesRecords[i + 1].ProductKey){
				Sales temp = salesRecords[i];
				salesRecords[i] = salesRecords[i + 1];
				salesRecords[i + 1] = temp;
      		}
        }
    }

	for(int i = 0; i < amountSalesRecords; i++){
		fseek(fPSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&salesRecords[i], sizeof(Sales), 1, fPSales);
	}
    free(salesRecords);
	salesRecords = NULL;
	fclose(fPSales);
	return 1;
}

int BubbleSortProductsByProductName(){
	long int amountProductsRecords = TellNumRecords("productsTable", sizeof(Products));
	FILE *fPProducts = fopen("productsTable", "rb+");
	
	Products *productsRecords = (Products * ) calloc(amountProductsRecords, sizeof(Products));

	for(int i = 0; i < amountProductsRecords; i++){
		fseek(fPProducts, sizeof(Products) * i, SEEK_SET);
		fread(&productsRecords[i], sizeof(Products), 1, fPProducts);
	}

	for (int step = 0; step < amountProductsRecords - 1; step += 1){
    	for (int i = 0; i < amountProductsRecords - step - 1; i += 1){
      		if(strcmp (productsRecords[i].ProductName, productsRecords[i + 1].ProductName) > 0){
				Products temp = productsRecords[i];
				productsRecords[i] = productsRecords[i + 1];
				productsRecords[i + 1] = temp;
      		}
        }
    }

	for (int i = 0; i < amountProductsRecords; i++){
		fseek(fPProducts, sizeof(Products) * i, SEEK_SET);
		fwrite(&productsRecords[i], sizeof(Products), 1, fPProducts);
	}
    free(productsRecords);
	productsRecords = NULL;
	fclose(fPProducts);
	return 1;
}

int BubbleSortCustomersByCustomerName(){
    int numCustomersRecords = TellNumRecords("customersTable", sizeof(Customers)); 
	FILE *fpCustomers = fopen("customersTable", "rb+");	

	Customers *customersRecords = (Customers *) calloc(numCustomersRecords, sizeof(Customers)); //

	for(int i = 0; i < numCustomersRecords; i += 1){
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fread(&customersRecords[i], sizeof(Customers), 1, fpCustomers);
	}

	//For to sort Customers
  	for ( int step = 0; step < numCustomersRecords - 1; step += 1 ){
    	for ( int i = 0; i < numCustomersRecords - step - 1; i += 1 ){
      		if (strcmp(customersRecords[i].Name, customersRecords[i + 1].Name) > 0){
				Customers temp = customersRecords[i];
				customersRecords[i] = customersRecords[i + 1];
				customersRecords[i + 1] = temp;
      		}
        }
    }

	//For to write each record alredy ordered in the file CustomersTable
	for (int i = 0; i < numCustomersRecords; i += 1){
		fseek(fpCustomers, sizeof(Customers) * i, SEEK_SET);
		fwrite(&customersRecords[i], sizeof(Customers), 1, fpCustomers);
	}
    free(customersRecords);
	customersRecords = NULL;
	fclose(fpCustomers);
	return 1;
}

int BubbleSortSalesByCustomerKey(){
	long int amountSalesRecords = TellNumRecords("salesTable", sizeof(Sales));
	FILE *fPSales = fopen("salesTable", "rb+");

	Sales *salesRecords = (Sales *) calloc(amountSalesRecords, sizeof(Sales));

	for(int i = 0; i < amountSalesRecords; i++){
		fseek(fPSales, sizeof(Sales) * i, SEEK_SET);
		fread(&salesRecords[i], sizeof(Sales), 1, fPSales);
	}

	for (int step = 0; step < amountSalesRecords - 1; step += 1){
    	for (int i = 0; i < amountSalesRecords - step - 1; i += 1){
      		if (salesRecords[i].CustomerKey > salesRecords[i + 1].CustomerKey){
				Sales temp = salesRecords[i];
				salesRecords[i] = salesRecords[i + 1];
				salesRecords[i + 1] = temp;
      		}
        }
    }

	for(int i = 0; i < amountSalesRecords; i++){
		fseek(fPSales, sizeof(Sales) * i, SEEK_SET);
		fwrite(&salesRecords[i], sizeof(Sales), 1, fPSales);
	}
    free(salesRecords);
	salesRecords = NULL;
	fclose(fPSales);
	return 1;
}

int CompareCustomersByCustomerKey(void *a, void *b){
    Customers *firstCust = (Customers *) a;
    Customers *secondCust = (Customers *) b;
    return firstCust->CustomerKey - secondCust->CustomerKey;
}

int CompareSalesByProductKey(void *a, void *b){
    Sales *firstSale = (Sales *) a;
    Sales *secondSale= (Sales *) b;
    return firstSale->ProductKey - secondSale->ProductKey;
}

int CompareProductsByProductName(void *a, void *b){
    Products *firstProduct = (Products *) a;
    Products *secondProduct = (Products *) b;
    return strcmp(firstProduct->ProductName, secondProduct->ProductName);
}

int CompareCustomersByLocation(void *a, void *b){
    Customers *firstCust = (Customers *) a;
    Customers *secondCust = (Customers *) b;
    int result = 0;

       result = strcmp(firstCust->Continent, secondCust->Continent);
    if(result != 0){
        return result;
    }

    result = strcmp(firstCust->Country, secondCust->Country);
    if(result != 0){
        return result;
    }

    result = strcmp(firstCust->State, secondCust->State);
    if(result != 0){
        return result;
    }

    result = strcmp(firstCust->City, secondCust->City);
    return result;
}

void Merge(void *array, int left, int right, int medium, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = medium - left + 1;
    int secondArray = right - medium;
    void *temporalLeft = calloc(firstArray,  recordSize);
    void *temporalRight = calloc(secondArray,  recordSize);

    //Copy the values of the first half
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    //Copy the values of the second half
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (medium + 1 + j) * recordSize, recordSize);
    }

    int i = 0, j = 0;
    int posicion = left;

    //Order and combine the two temporal arrays
    while (i < firstArray && j < secondArray) {
        if (compare((char*)temporalLeft + i * recordSize, (char*)temporalRight + j * recordSize) <= 0) {
            memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
            i++;
        } else {
            memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
            j++;
        }
        posicion++;
    }

    // Copiar los elementos restantes de temporalLeft
    while (i < firstArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalLeft + i * recordSize, recordSize);
        i++;
        posicion++;
    }

    // Copiar los elementos restantes de temporalRight
    while (j < secondArray) {
        memcpy((char*)array + posicion * recordSize, (char*)temporalRight + j * recordSize, recordSize);
        j++;
        posicion++;
    }

    free(temporalLeft); temporalLeft = NULL;
    free(temporalRight); temporalRight = NULL;
}

void MergeSort(void *array, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);

        //Recursive call for the first half
        MergeSort(array, left, medium, recordSize, compare);

        //Recursive call for the second half
        MergeSort(array, medium + 1, right, recordSize, compare);

        //Order and combine the two halfs
        Merge(array, left, right, medium, recordSize, compare);

    }
}