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
#ifndef REGULATOR_H
#define REGULATOR_H

double b0, b1;                                      //Regulator koefficients
double errorNew, errorOld, outputNew, outputOld;    //Memory elements for regulator

void initRegulator(double Kp, double Ti, double f_sample);
double calculateControlSignal(double processVal, double setpoint);

#endif /* REGULATOR_H */
/* [] END OF FILE */
