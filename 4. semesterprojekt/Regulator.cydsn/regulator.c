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
#include "regulator.h"
#include "project.h"
#include <stdio.h>
#include <stdint.h>

void initRegulator()
{
    // Calculate regulator parameters
    b0 = (2 + ((1/F_SAMPLE)/Ti)) * (Kp/2);
    b1 = (((1/F_SAMPLE)/Ti) - 2) * (Kp/2);
    
    char buffer2[256];
    sprintf(buffer2, "b0 = %f\n\rb1 = %f\n\n\r",b0, b1);
    UART_PutString(buffer2);
}

double calculateControlSignal(double processVal, double setpoint)
{
    char buffer3[256];
    
    // Calculate error
    errorNew = setpoint - processVal;
    
    // Calculate new output
    outputNew = outputOld + (b0 * errorNew) + (b1 * errorOld);
    
    //sprintf(buffer3, "errorNew = %f\n\rerrorOld = %f\n\routputNew = %f\n\routputOld = %f\n\r", errorNew, errorOld, outputNew, outputOld);
    //UART_PutString(buffer3);
    
    // Update delay line
    errorOld = errorNew;
    outputOld = outputNew;
    
    
    // Return result
    return outputNew;
}

/* [] END OF FILE */
