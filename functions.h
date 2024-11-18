#ifndef TABLESFUNCTIONS_H
#define TABLESFUNCTIONS_H
#include <stdio.h>

typedef struct{
    long int OrderNumber;
    unsigned char LineItems;
    struct{
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } OrderDate;
    struct{
        unsigned char DD;
        unsigned char MM;
        unsigned short int AAAA;
    } DeliveryDate;
    unsigned int CustomerKey;
    unsigned short int StoreKey;
    unsigned short int ProductKey;
    unsigned short int Quantity;
    char CurrencyCode[3];
} Sales;

typedef struct{
    unsigned int CustomerKey;
    char Gender[8];
    char Name[40];
    char City[40];
    char StateCode[30];
    char State[30];
    unsigned int ZipCode;
    char Country[20];
    char Continent[20];
    struct{
        unsigned char MM;
        unsigned char DD;
        unsigned short int AAAA;
    } Birthday;
} Customers;

typedef struct{
    unsigned short int ProductKey;
    char ProductName[100];
    char Brand[30];
    char Color[15];
    float UnitCostUSD;
    float UnitPriceUSD;
    char SubcategoryKey[5];
    char Subcategory[50];
    char CategoryKey[3];
    char Category[35];
} Products;

typedef struct{
    unsigned short int StoreKey;
    char Country[35];
    char State[35];
    unsigned short int SquareMeters;
    struct{
        unsigned char MM;
        unsigned char DD;
        unsigned short int AAAA;
    } OpenDate;
} Stores;

typedef struct{
    char Date[11];
    char Currency[3];
    float Exchange;
} ExchangeRates;

int TellNumRecords(char fileName[], int recordSize);
void CreateSalesTable(char FileName[]);
void CreateCustomersTable(char FileName[]);
void CreateProductsTable(char FileName[]);
void CreateStoresTable(char FileName[]);
void CreateExchangeRatesTable(char FileName[]);
void CreateDatasetTables();
int BinarySearch(FILE *fp, unsigned long int valueToSearch, int file);
int BubbleSortCustomersByCustomerKey();
int BubbleSortSalesByProductKey();
int BubbleSortProductsByProductName();
int BubbleSortCustomersByCustomerName();
int BubbleSortSalesByCustomerKey();
int CompareCustomersByCustomerKey(void *a, void *b);
int CompareSalesByProductKey(void *a, void *b);
int CompareProductsByProductName(void *a, void *b);
int CompareCustomersByLocation(void *a, void *b);
void Merge(void *array, int left, int right, int medium, int recordSize, int (*compare)(void*, void*));
void MergeSort(void *array, int left, int right, int recordSize, int (*compare)(void*, void*));

#endif // TABLESFUNCTIONS_H