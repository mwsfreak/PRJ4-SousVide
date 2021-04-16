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
#include "heatController.h"
#include "project.h"

void initHeatController()
{
    heatPWM_Start();
    heatPWM_WriteCompare(0);
}
void setControlSignal(double controlSignal)
{
    // Write desired duty cycle into PWM controller
    if(controlSignal <= 100)
    heatPWM_WriteCompare((uint16)(controlSignal * 100)); 
    else
    heatPWM_WriteCompare(10000); 
}
/* [] END OF FILE */
