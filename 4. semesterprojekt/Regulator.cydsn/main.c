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
#include "heatController.h"
#include "regulator.h"
#include "temperatureMeasurement.h"
#include <stdio.h>
#include <stdint.h>

CY_ISR_PROTO(UART_RX_HANDLER);

/* -- Regulatorparametre -- */
const double Kp = 3.8063;       //Times gain
const double Ti = 2503.4;       //Sek
const double f_sample = 3.0;    //Hz

extern double V_sense, V_PT1000, current, R_PT1000;

int main(void)
{
    CyDelay(2000);
    UART_Start();
    UART_PutString("Regulator is running.. \n\r");
    CyGlobalIntEnable; /* Enable global interrupts. */
    rx_int_StartEx(UART_RX_HANDLER);
    
    initHeatController();
    initRegulator(Kp, Ti, f_sample);
    initTempMeasure();
    
    double temp = 20;
    double controlSignal = 0;
    char buffer[256];
    
    for(;;)
    {
        temp = getProcessTemp();
        controlSignal = calculateControlSignal(temp, 60);
        setControlSignal(controlSignal);
        //CyDelay(10);
        //temp += (controlSignal - 10) / 100;
        
        sprintf(buffer, "Temperatur: %f     Control signal: %f      V_sense: %f V     V_PT1000: %f V \n\r", temp, controlSignal, V_sense, V_PT1000);
        UART_PutString(buffer);
    }
}

CY_ISR(UART_RX_HANDLER)
{
       
}




/* [] END OF FILE */
