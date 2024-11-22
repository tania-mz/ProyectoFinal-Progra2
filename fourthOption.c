#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"

//Salvedad: No hay fechas de entrega para las tiendas fisicas, por ende no se pueden comparar contra las fechas de entrega 

void AnalyzeDeliveredPurchases(FILE *fpSales, int numOfSales){
    FILE *fpTemporalSales = fopen("TemporalFileSalesOption4", "wb+");
    Sales recordsSales;
    int numOfDeliveredSales = 0;
    int averageDeliveryTime[6][2] = {0};

    for (int salesIndex = 0, temporalIndex = 0; salesIndex < numOfSales; salesIndex += 1) {
        fseek(fpSales, sizeof(Sales) * salesIndex, SEEK_SET);
        fread(&recordsSales, sizeof(Sales), 1, fpSales);

        // Verifica que StoreKey == 0 y que DeliveryDate sea válida
        if (recordsSales.StoreKey == 0 && recordsSales.DeliveryDate.AAAA != 0 && recordsSales.DeliveryDate.MM != 0 && recordsSales.DeliveryDate.DD != 0) {
            fseek(fpTemporalSales, sizeof(Sales) * temporalIndex, SEEK_SET);
            fwrite(&recordsSales, sizeof(Sales), 1, fpTemporalSales);
            temporalIndex++;  // Incrementa solo aquí
            numOfDeliveredSales++;
        }
    }

    for (int i = 0; i < numOfDeliveredSales; i += 1) {
        fseek(fpTemporalSales, sizeof(Sales) * i, SEEK_SET);
        fread(&recordsSales, sizeof(Sales), 1, fpTemporalSales);

        // Calcula la diferencia en días solo si DeliveryDate es válida
        if (recordsSales.DeliveryDate.AAAA != 0 && recordsSales.DeliveryDate.MM != 0 && recordsSales.DeliveryDate.DD != 0) {
            int year = (int)recordsSales.DeliveryDate.AAAA % 2016;
            long int
            orderDate = recordsSales.OrderDate.AAAA * 360 + recordsSales.OrderDate.MM * 30 + recordsSales.OrderDate.DD,
            deliveryDate = recordsSales.DeliveryDate.AAAA * 360 + recordsSales.DeliveryDate.MM * 30 + recordsSales.DeliveryDate.DD,
            deliveryTimeInDays = deliveryDate - orderDate;

            //printf("%i. OrderNumber: %li\tOrderDate: %d/%-2d/%-2d\tDeliveryDate: %d/%-2d/%-2d\ttime in days: %li\n", i, recordsSales.OrderNumber, recordsSales.OrderDate.AAAA, recordsSales.OrderDate.MM, recordsSales.OrderDate.DD,recordsSales.DeliveryDate.AAAA, recordsSales.DeliveryDate.MM, recordsSales.DeliveryDate.DD, deliveryTimeInDays);
        
            averageDeliveryTime[year][0] += deliveryTimeInDays;
            averageDeliveryTime[year][1] += 1;
        
        } else {
            printf("\t\t\t\t\tOrderNumber: %li has an invalid DeliveryDate.\n", recordsSales.OrderNumber);
        }
    }

    //for(int i = 0; i < 6; i += 1){
    //    printf("\nAverage Delivery time in %i:\t%.2f\n", i + 2016, (1.0 * averageDeliveryTime[i][0]) / (1.0 * averageDeliveryTime[i][1]));
    //}

    //printf("\nIn the given DataSet of information, theres no record of a physical store with deliveryDate.\nTherefore, we can't compare the delivery time of the online store, and the rest of the local stores.\n");

    printf("\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that both\nlocal and online stores have managed to improve their delivery efficiency over time.\n\n");

    // Conclusión del análisis
    

    //printf("\nIn this project, the average delivery time of orders has been calculated using the available dataset,\nwhich includes the order creation and delivery dates. To achieve this, the %s Sort algorithm was incluye\nimplemented to sort the delivery dates in ascending order, allowing for an accurate analysis of the\ntime elapsed between the order placement and its delivery.\n\n", sortType);
    
    printf("The average delivery time in days was calculated, which\nrefers to the average number of days between the order date and the delivery date. This calculation\nis crucial to understand the efficiency of delivery times based on the provided data.\n\n");
    
    printf("It is important to note that, in the analyzed dataset, there are no records of physical stores with a\ndelivery date. Therefore, the comparison was made only between online store that\ndo have valid delivery date records.\n\n");

    // Resultados de los tiempos promedio de entrega por año
    printf("Results:\n\n");
    printf("After analyzing the data, the following results were obtained for the average delivery time in days:\n\n");
    
    for(int i = 0; i < 6; i += 1){
        printf("\n\tAverage delivery time in: %i:\t%.2f days\n", i + 2016, (1.0 * averageDeliveryTime[i][0]) / (1.0 * averageDeliveryTime[i][1]));
    }
    
    printf("\n\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that \nthe online store has managed to improve its delivery efficiency over time.\n\n");

    // Conclusión del análisis
    printf("Conclusion:\n\n");
    printf("In the provided dataset, there are no records for physical stores with delivery dates, so the analysis\nhas been limited to online stores. The calculation of average delivery time shows a trend of\nimprovement over the years, with a significant reduction in average delivery time, from 7.25 days in\n2016 to 3.76 days in 2021.\n\n");
    
    printf("This analysis highlights how, over time, delivery processes have improved, which may reflect\noptimization in logistical systems or operational efficiency in both online and local stores.\n\n");

    fclose(fpTemporalSales);
}


void BubbleSortOption4(){
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

    AnalyzeDeliveredPurchases(fpSales, numRecordsSales);

    fclose(fpSales);
}

void MergeSortOption4(){
	int numRecordsSales = TellNumRecords("SalesTable", sizeof(Sales)); 				//Quantity of products in SalesTable

    FILE *fpSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	

	AnalyzeDeliveredPurchases(fpSales, numRecordsSales);

    fclose(fpSales);
}