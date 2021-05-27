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

void initRegulator(double Kp, double Ti, double T_sample)
{
    // Calculate regulator parameters
    b0 = (2 + (T_sample/Ti)) * (Kp/2);
    b1 = ((T_sample/Ti) - 2) * (Kp/2);
    
    // Reset regulator memory
    outputOld = 0;
    errorOld = 0;
}

double calculateControlSignal(double processVal, double setpoint)
{    
    // Calculate error
    errorNew = setpoint - processVal;
    
    // Calculate new output
    outputNew = outputOld + (b0 * errorNew) + (b1 * errorOld);
    
    // Anti wind-up
    if(outputNew > 100)
        outputNew = 100;
    if(outputNew < 0)
        outputNew = 0;
    
    // Update delay line
    errorOld = errorNew;
    outputOld = outputNew;
    
    // Return result
    return outputNew;
}

/* [] END OF FILE */
