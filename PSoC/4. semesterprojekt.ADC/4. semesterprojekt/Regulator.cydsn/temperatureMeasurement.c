/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "temperatureMeasurement.h"
#include <math.h>

/* -- PT1000 -- */
const double A = 0.0039083;
const double B = -0.0000005775;
const uint16_t R_SENSE = 3300;

#define SIZE 25
#define MAVG 5
const uint16_t SAMPLE_SIZE = 8000;          //Number of samples before averaging

double V_sense, V_PT1000, current, R_PT1000;

/* -- Init Settings -- */
void initTempMeasure() {
    Clock_Start();   /* Start Clock - DC 0% */

    /* -- Current Reference -- */    
    ADC_CURRENT_SENSE_Start();
}


/* -- Temp Calculation -- */
double getProcessTemp(double *input) {
    double processTemp = 0.0;
    
    /* -- Current Calculation -- */
    ADC_CURRENT_SENSE_IsEndConversion(ADC_CURRENT_SENSE_WAIT_FOR_RESULT);         
    V_sense = ADC_CURRENT_SENSE_CountsTo_Volts(ADC_CURRENT_SENSE_GetResult16());
    
    /* -- Moving Average Filter -- */
    double sum = 0;
    for(uint8_t i = 0; i < SIZE ; i++)
    {
        sum += input[i];
    }
    V_PT1000 = (sum/MAVG)*5;
    
    /* -- Temp Voltage Calculation -- */
    current = V_sense / R_SENSE;
    R_PT1000 = V_PT1000 / current;
    
    /* -- Temp Calculation - PT1000 Values -- */
    processTemp = ((sqrt(2500*pow(A,2)-10000*B+10*B*R_PT1000)/100)-A/2)/B;
    
    return processTemp;
}

/* [] END OF FILE */
