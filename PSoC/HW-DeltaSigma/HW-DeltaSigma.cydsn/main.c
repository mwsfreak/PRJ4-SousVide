/* ==========================================================
 *
 *             Optimering af ADC sampling
 *
 *  Lavet af: Magnus Franks, Emil Arthur, Emil Lunau, Allan Lassesen
 *
 *
 *                  Den. 21-04-2021
 *
 * ==========================================================
*/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const uint16_t DELTASIGMA_CLOCK_FRQ = 16000; //Set to clock frequency chosen in TopDesign
const uint16_t SAMPLE_SIZE = 32000;          //Number of samples before averaging
const double REF_VOLTAGE = 2.5;              // Delta Sigma reference voltage (must match hardware)

char string[256];
uint16_t PWM_period = 0;
uint16_t PWM_compare = 0;
uint16_t sum = 0;
double outVoltage = 0;

CY_ISR_PROTO(sampleDone);

int main(void)
{
    CyGlobalIntEnable;                  /* Enable global interrupts. */
    isr_done_sampling_StartEx(sampleDone);      /* Enable ISR Routine */
    
    Value_counter_Start();
    Sample_counter_Start();
    Value_counter_WritePeriod(SAMPLE_SIZE);   //Large enough to not overflow
    Sample_counter_WritePeriod(SAMPLE_SIZE);
    
    PWM_Start();                        /* Start PWM */
    UART_PC_Start();                    /* Start Uart PC */
    Clock_Start();                      /* Start Clock - DC 0% */
    
    PWM_period = SAMPLE_SIZE;
    PWM_WritePeriod(PWM_period);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_PC_PutString("Ready to Test\n");
    
    for(;;)
    {
        /* Place your application code here. */

    }
}

/* [] END OF FILE */

CY_ISR(sampleDone)                     /* UART Interrupt */
{  
    //UART_PC_PutString("Entered interrupt routine\n");
    sum = Value_counter_ReadPeriod()-Value_counter_ReadCapture();
    
    double result = (double)sum/SAMPLE_SIZE;
    PWM_compare = result*PWM_period;
    
    //Hvordan konverteres til en spænding ud?
    outVoltage = result*REF_VOLTAGE;
    
    //Debug info
    sprintf(string, "Sum is: %d     Average Value is: %f    ", sum, result);
    UART_PC_PutString(string);
    sprintf(string, "PWM periode: %d    PWM Compare: %d     outVoltage: %lf\n", PWM_ReadPeriod(), PWM_ReadCompare(), outVoltage);
    UART_PC_PutString(string);

    PWM_WriteCompare(PWM_compare); /* Write Duty cycle from with control signal*/    
}