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
CY_ISR_PROTO(SAMPLE_HANDLER);

/* -- Regulatorparametre -- */
const double Kp = 3.8063;       //Times gain
const double Ti = 2536.2;       //Sek
const double T_sample = 68.5;    //Hz

double controlSignal = 0;
int setpoint;
uint8_t sampleCount = 0;

/* -- ADC -- */
#define SAMPLESIZE 8000
#define MAVG 5
#define SIZE 25
uint8_t sampleFlag = 0;
uint8_t inputCount = 0;
uint8_t outputCount = 0;
double input[MAVG] = {0};
double temp[SIZE] = {0};

double V_sense, V_PT1000, current, R_PT1000;

/* -- UART -- */
char buffer[256];
char rxChar = 0;




int main(void)
{
    CyDelay(2000);
    UART_Start();
    UART_PutString("Regulator is running.. \n\r");
    CyGlobalIntEnable; /* Enable global interrupts. */
    rx_int_StartEx(UART_RX_HANDLER);
    sample_int_StartEx(SAMPLE_HANDLER);
    
    initHeatController();
    initRegulator(Kp, Ti, T_sample);
    initTempMeasure();
    
    for(;;)
    {
        if(rxChar != 0)
        {
            switch(rxChar)
            {
                case 's':
                {
                    initRegulator(Kp, Ti, T_sample);
                   // sampleTimer_Start();   Bliver den brugt til noget..? - eller kun start og stop af ADC konverteringer..??
                    heatPWM_Start();
                    UART_PutString("Regulator started\n\r");
                    UART_PutString("setpoint, temperatur, fejl, controlsignal, V_sense, V_PT1000\n\r");
                    
                    rxChar = 0;
                    break;
                }
                case 'e':
                {
                   // sampleTimer_Stop();   Bliver den brugt til noget..? - eller kun start og stop af ADC konverteringer..??
                    heatPWM_Stop();
                    UART_PutString("Regulator ended\n\r");
                    
                    rxChar = 0;
                    break;
                }
                default:
                {
                    setpoint = (rxChar - 48) * 10;
                    UART_PutString("Regulator got new setpoint\n\r");
                    
                    rxChar = 0;
                    break;
                }
            }
        } 
    }
    if(sampleFlag)
    {
        sampleFlag = 0;

        /* -- Get Input Value -- */
        inputCount = (inputCount == SIZE ? 0 : inputCount);
        input[inputCount%MAVG] = (double)(Value_counter_ReadPeriod()-Value_counter_ReadCapture())/SAMPLESIZE;
        /* -- Set Temperature -- */
        temp[inputCount] = getProcessTemp(input);
        
    
        if(sampleCount >= 68)
        {
            controlSignal = calculateControlSignal(temp[inputCount], setpoint);
            setControlSignal(controlSignal);
            sampleCount = 0;
        }
                
        //sprintf(buffer, "Temperatur: %f     ,Control signal: %f      ,V_sense: %f V     ,V_PT1000: %f V \n\r", temp, controlSignal, V_sense, V_PT1000);
        // setpoint, temperatur, fejl, controlsignal, V_sense, V_PT1000
        sprintf(buffer, "%d,  %f,  %f,  %f,  %f,  %f\n\r", setpoint, (float)temp[inputCount], (float)(setpoint-temp[inputCount]), controlSignal, V_sense, V_PT1000);
        UART_PutString(buffer);
    }
}

CY_ISR(UART_RX_HANDLER)
{
    rxChar = UART_GetChar();
    rx_int_ClearPending();
}

CY_ISR(SAMPLE_HANDLER)
{ 
    sampleFlag = 1;
    /* -- Counter for Regulation -- */
    sampleCount++;
    /* -- Counter ADC Input/Output -- */
    inputCount++;

    sample_int_ClearPending();
}




/* [] END OF FILE */
