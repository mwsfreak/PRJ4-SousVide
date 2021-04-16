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
#define Ti 2503.4                                 //Sek
#define Kp 3.8063                                    //Times gain
#define F_SAMPLE 3.0                                   //Hz

double b0, b1;                                        //Regulator koefficients
double errorNew, errorOld, outputNew, outputOld;      //Memory elements for regulator

void initRegulator();
double calculateControlSignal(double processVal, double setpoint);

/* [] END OF FILE */
