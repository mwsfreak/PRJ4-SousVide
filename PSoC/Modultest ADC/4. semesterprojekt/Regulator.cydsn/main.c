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
#include <stdlib.h>
#include <stdint.h>

CY_ISR_PROTO(UART_RX_HANDLER);
CY_ISR_PROTO(CLOCK_HANDLER);

/* -- Regulatorparametre -- */
const double Kp = 3.8063;       // Times gain
const double Ti = 2536.2;       // Sek
const double T_sample = 68;     // sekunder

double controlSignal = 0;
int setpoint;
uint8_t sampleCount = 0;

/* -- ADC -- */
<<<<<<< HEAD:PSoC/4. semesterprojekt.ADC/4. semesterprojekt/Regulator.cydsn/main.c
#define SAMPLESIZE 75000
=======
#define SAMPLESIZE 300000
>>>>>>> main:PSoC/Modultest ADC/4. semesterprojekt/Regulator.cydsn/main.c
#define MAVG 20
#define SIZE 68
uint8_t sampleFlag = 0;
uint8_t inputCount = 0;
uint8_t outputCount = 0;
double input[MAVG] = {0};
double temp = 0;

extern double V_sense, V_PT1000, current, R_PT1000;

//uint8_t bitStream[2*SAMPLESIZE] = {0};
uint32_t sum = 0;
uint32_t streamCount = 0;

/* -- UART -- */
char buffer[256];
char rxChar = 0;

enum status {stopped, started};
enum status regulatorState = stopped;



int main(void)
{
    CyDelay(2000);
    UART_Start();
    UART_PutString("Regulator is running.. \n\r");
    CyGlobalIntEnable; /* Enable global interrupts. */
    rx_int_StartEx(UART_RX_HANDLER);
    clock_int_StartEx(CLOCK_HANDLER);
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
                    heatPWM_Start();
                    UART_PutString("Regulator started\n\r");
                    //UART_PutString("setpoint, temperatur, fejl, controlsignal, V_sense, V_PT1000\n\r");
                    regulatorState = started;
                    rxChar = 0;
                    break;
                }
                case 'e':
                {
                    heatPWM_Stop();
                    UART_PutString("Regulator ended\n\r");
                    regulatorState = stopped;
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
        if(streamCount >= SAMPLESIZE)
        {
            streamCount = 0;
            /* -- Counter for Regulation -- */
            sampleCount++;
            /* -- Counter ADC Input/Output -- */
            inputCount++;

            /* -- Get Input Value -- */
            inputCount = (inputCount == SIZE ? 0 : inputCount);

            input[inputCount%MAVG] = (double)sum/SAMPLESIZE;
            /* -- Set Temperature -- */
            temp = getProcessTemp(input);
            sum = 0;
        
            if(sampleCount >= SIZE) // ~ 68 sekunder
            {
                controlSignal = calculateControlSignal(temp, setpoint);
                setControlSignal(controlSignal);
                sampleCount = 0;
            }
            
            if(regulatorState == started)
            {
                sprintf(buffer, "%d,  %f,  %f,  %f,  %f,  %f\n\r", setpoint, (float)temp, (float)(setpoint-temp), controlSignal, V_sense, V_PT1000);
                UART_PutString(buffer);
            }
<<<<<<< HEAD:PSoC/4. semesterprojekt.ADC/4. semesterprojekt/Regulator.cydsn/main.c
            sprintf(buffer, "%f\n\r",(V_PT1000)*1000);//sprintf(buffer, "Voltage delta sigma: %f mV\n\r",(V_PT1000-0.0994)*1000);
=======
            sprintf(buffer, "Voltage delta sigma: %f\n\r",V_PT1000);
>>>>>>> main:PSoC/Modultest ADC/4. semesterprojekt/Regulator.cydsn/main.c
            UART_PutString(buffer);
        }
    }
}

CY_ISR(UART_RX_HANDLER)
{
    rxChar = UART_GetChar();
    rx_int_ClearPending();
}

CY_ISR(CLOCK_HANDLER)
{
    sum += Bit_Stream_Read();
    streamCount++;
    clock_int_ClearPending();
}

/* [] END OF FILE */
