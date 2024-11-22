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

        char key[11] = "";
        strcpy(key, record.Date);
        
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

//---------------------------------Bubble sort Functions -------------------------------------------------
int BubbleSortCustomersByCustomerKey(){
    int numCustomersRecords = TellNumRecords("customersTable", sizeof(Customers)); 
	FILE *fp = fopen("customersTable", "rb+");	
    Customers reg1, reg2;
	//For to sort Customers
  	for ( int step = 0; step < numCustomersRecords - 1; step += 1 ){
    	for ( int i = 0; i < numCustomersRecords - step - 1; i += 1 ){
            fseek(fp, sizeof(Customers) * i, SEEK_SET);
            fread(&reg1, sizeof(Customers), 1, fp);
            fseek(fp, sizeof(Customers) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Customers), 1, fp);
      		if (reg1.CustomerKey > reg2.CustomerKey){
				fseek(fp, sizeof(Customers) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Customers), 1, fp);
                fseek(fp, sizeof(Customers) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Customers), 1, fp);
      		}
        }
    }
	fclose(fp);
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
	FILE *fp = fopen("salesTable", "rb+");
    Sales reg1, reg2;

	for ( int step = 0; step < amountSalesRecords - 1; step += 1 ){
    	for ( int i = 0; i < amountSalesRecords - step - 1; i += 1 ){
            fseek(fp, sizeof(Sales) * i, SEEK_SET);
            fread(&reg1, sizeof(Sales), 1, fp);
            fseek(fp, sizeof(Sales) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Sales), 1, fp);
      		if (reg1.ProductKey > reg2.ProductKey){
				fseek(fp, sizeof(Sales) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Sales), 1, fp);
                fseek(fp, sizeof(Sales) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Sales), 1, fp);
      		}
        }
    }
	fclose(fp);
	return 1;
}

int BubbleSortProductsByProductName(){
	long int amountProductsRecords = TellNumRecords("productsTable", sizeof(Products));
	FILE *fp = fopen("productsTable", "rb+");
	Products reg1, reg2;

    for ( int step = 0; step < amountProductsRecords - 1; step += 1 ){
    	for ( int i = 0; i < amountProductsRecords - step - 1; i += 1 ){
            fseek(fp, sizeof(Products) * i, SEEK_SET);
            fread(&reg1, sizeof(Products), 1, fp);
            fseek(fp, sizeof(Products) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Products), 1, fp);
      		if (strcmp(reg1.ProductName, reg2.ProductName) > 0){
				fseek(fp, sizeof(Products) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Products), 1, fp);
                fseek(fp, sizeof(Products) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Products), 1, fp);
      		}
        }
    }
	fclose(fp);
	return 1;
}

int BubbleSortProductsByProductKey(){
	long int amountProductsRecords = TellNumRecords("productsTable", sizeof(Products));
	FILE *fp = fopen("productsTable", "rb+");
	Products reg1, reg2;

    for ( int step = 0; step < amountProductsRecords - 1; step += 1 ){
    	for ( int i = 0; i < amountProductsRecords - step - 1; i += 1 ){
            fseek(fp, sizeof(Products) * i, SEEK_SET);
            fread(&reg1, sizeof(Products), 1, fp);
            fseek(fp, sizeof(Products) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Products), 1, fp);
      		if (reg1.ProductKey > reg2.ProductKey){
				fseek(fp, sizeof(Products) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Products), 1, fp);
                fseek(fp, sizeof(Products) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Products), 1, fp);
      		}
        }
    }
	fclose(fp);
	return 1;
}

int BubbleSortCustomersByCustomerName(){
    int numCustomersRecords = TellNumRecords("customersTable", sizeof(Customers)); 
	FILE *fp = fopen("customersTable", "rb+");	
	Customers reg1, reg2; //

  	for ( int step = 0; step < numCustomersRecords - 1; step += 1 ){
    	for ( int i = 0; i < numCustomersRecords - step - 1; i += 1 ){
            fseek(fp, sizeof(Customers) * i, SEEK_SET);
            fread(&reg1, sizeof(Customers), 1, fp);
            fseek(fp, sizeof(Customers) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Customers), 1, fp);
      		if (strcmp(reg1.Name, reg2.Name) > 0){
				fseek(fp, sizeof(Customers) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Customers), 1, fp);
                fseek(fp, sizeof(Customers) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Customers), 1, fp);
      		}
        }
    }
	fclose(fp);
	return 1;
}

int BubbleSortSalesByCustomerKey(){
	long int amountSalesRecords = TellNumRecords("salesTable", sizeof(Sales));
	FILE *fp= fopen("salesTable", "rb+");
	Sales reg1, reg2;

	for ( int step = 0; step < amountSalesRecords - 1; step += 1 ){
    	for ( int i = 0; i < amountSalesRecords - step - 1; i += 1 ){
            fseek(fp, sizeof(Sales) * i, SEEK_SET);
            fread(&reg1, sizeof(Sales), 1, fp);
            fseek(fp, sizeof(Sales) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(Sales), 1, fp);
      		if (reg1.CustomerKey > reg2.CustomerKey){
				fseek(fp, sizeof(Sales) * i, SEEK_SET);
                fwrite(&reg2, sizeof(Sales), 1, fp);
                fseek(fp, sizeof(Sales) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(Sales), 1, fp);
      		}
        }
    }
	fclose(fp);
	return 1;
}

//--------------------------------Funciones de comparacion para el Merge--------------------------

int CompareCustomersByCustomerKey(void *a, void *b){
    Customers *firstCust = (Customers *) a;
    Customers *secondCust = (Customers *) b;
    return firstCust->CustomerKey - secondCust->CustomerKey;
}

int CompareCustomersByCustomerName(void *a, void *b){
    Customers *custA = (Customers *)a;
    Customers *custB = (Customers *)b;
    return strcmp(custA->Name, custB->Name);
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

int CompareSalesByOrderNumber(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    long int comparation = saleA->OrderNumber - saleB->OrderNumber;
    
    if(comparation != 0){
        return (int)comparation;
    } else{
        return (int)(saleA->ProductKey - saleB->ProductKey);
    }
    
}

int CompareSalesByCustomerKey(void *a, void *b){
    Sales *saleA = (Sales *)a;
    Sales *saleB = (Sales *)b;
    return saleA->CustomerKey - saleB->CustomerKey;
}

int CompareProductsByProductKey(void *a, void *b){
    Products *prodA = (Products *)a;
    Products *prodB = (Products *)b;
    return prodA->ProductKey - prodB->ProductKey;
}

void MergeArray(void* array, int left, int right, int middle, int recordSize, int (*compare)(void*, void*)){
    int firstArray = middle - left + 1;
    int secondArray = right - middle;
    void *temporalLeft = calloc(firstArray,  recordSize);
    void *temporalRight = calloc(secondArray,  recordSize);

    //Copy the values of the first half
    for (int i = 0; i < firstArray; i++) {
        memcpy((char*)temporalLeft + i * recordSize, (char*)array + (left + i) * recordSize, recordSize);
    }

    //Copy the values of the second half
    for (int j = 0; j < secondArray; j++) {
        memcpy((char*)temporalRight + j * recordSize, (char*)array + (middle + 1 + j) * recordSize, recordSize);
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

void MergeSortArray(void* array, int left, int right, int recordSize, int (*compare)(void*, void*)){
    if (left < right) {
        int middle = left + ((right - left) / 2);

        //Recursive call for the first half
        MergeSortArray(array, left, middle, recordSize, compare);

        //Recursive call for the second half
        MergeSortArray(array, middle + 1, right, recordSize, compare);

        //Order and combine the two halfs
        MergeArray(array, left, right, middle, recordSize, compare);
    }
}

void MergeFile(FILE *file, int left, int right, int medium, int recordSize, int (*compare)(void*, void*)) {
    int firstArray = medium - left + 1;
    int secondArray = right - medium;

    // Crear archivos temporales
    FILE *leftFile = fopen("tempLeft.bin", "wb+");
    FILE *rightFile = fopen("tempRight.bin", "wb+");

    void *buffer = malloc(recordSize);

    // Copiar los valores de la primera mitad al archivo temporal izquierdo
    for (int i = 0; i < firstArray; i++) {
        fseek(file, (left + i) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, leftFile);
    }

    // Copiar los valores de la segunda mitad al archivo temporal derecho
    for (int j = 0; j < secondArray; j++) {
        fseek(file, (medium + 1 + j) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, rightFile);
    }

    fseek(leftFile, 0, SEEK_SET);
    fseek(rightFile, 0, SEEK_SET);

    int i = 0, j = 0;
    int posicion = left;

    // Mezclar y ordenar los registros de los archivos temporales de vuelta al archivo original
    while (i < firstArray && j < secondArray) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fseek(rightFile, j * recordSize, SEEK_SET);

        void *leftRecord = malloc(recordSize);
        void *rightRecord = malloc(recordSize);

        fread(leftRecord, recordSize, 1, leftFile);
        fread(rightRecord, recordSize, 1, rightFile);

        if (compare(leftRecord, rightRecord) <= 0) {
            fseek(file, posicion * recordSize, SEEK_SET);
            fwrite(leftRecord, recordSize, 1, file);
            i++;
        } else {
            fseek(file, posicion * recordSize, SEEK_SET);
            fwrite(rightRecord, recordSize, 1, file);
            j++;
        }

        posicion++;
        free(leftRecord);
        free(rightRecord);
    }

    // Copiar los elementos restantes del archivo temporal izquierdo
    while (i < firstArray) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, leftFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        i++;
        posicion++;
    }

    // Copiar los elementos restantes del archivo temporal derecho
    while (j < secondArray) {
        fseek(rightFile, j * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, rightFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        j++;
        posicion++;
    }

    // Liberar memoria y cerrar archivos temporales
    free(buffer);
    fclose(leftFile);
    fclose(rightFile);
    remove("tempLeft.bin");
    remove("tempRight.bin");
}

void MergeSortFile(FILE *file, int left, int right, int recordSize, int (*compare)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);

        // Llamada recursiva para la primera mitad
        MergeSortFile(file, left, medium, recordSize, compare);

        // Llamada recursiva para la segunda mitad
        MergeSortFile(file, medium + 1, right, recordSize, compare);

        // Combinar las dos mitades
        MergeFile(file, left, right, medium, recordSize, compare);
    }
}