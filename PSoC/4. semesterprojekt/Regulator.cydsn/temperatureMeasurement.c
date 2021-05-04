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

const double A = 0.0039083;
const double B = -0.0000005775;
const uint16_t R_SENSE = 3300;

double V_sense, V_PT1000, current, R_PT1000;

void initTempMeasure() {
    ADC_CURRENT_SENSE_Start();
    ADC_TEMP_VOLTAGE_Start();
    
    //ADC_CURRENT_SENSE_StartConvert();
    ADC_TEMP_VOLTAGE_StartConvert();
}

double getProcessTemp(void) {
    double processTemp = 0.0;
    
    ADC_CURRENT_SENSE_IsEndConversion(ADC_CURRENT_SENSE_WAIT_FOR_RESULT); 
    V_sense = ADC_CURRENT_SENSE_CountsTo_Volts(ADC_CURRENT_SENSE_GetResult16());
    
    ADC_TEMP_VOLTAGE_IsEndConversion(ADC_CURRENT_SENSE_WAIT_FOR_RESULT);
    V_PT1000 = ADC_TEMP_VOLTAGE_CountsTo_Volts(ADC_TEMP_VOLTAGE_GetResult16());
    
    current = V_sense / R_SENSE;
    R_PT1000 = V_PT1000 / current;

    processTemp = ((sqrt(2500*pow(A,2)-10000*B+10*B*R_PT1000)/100)-A/2)/B;
    
    return processTemp;
}

/* [] END OF FILE */
