/* 
*  PRJ4 Test Example
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "unistd.h"

/*******************************
*   GPIO Setup
*******************************/

// ADC Pin Settings
#define ADC_OUTPUT 12
#define ADC_OUTPUT_PIN_SEL ((1ULL << ADC_OUTPUT))
#define ADC_INPUT 13
#define ADC_INPUT_PIN_SEL ((1ULL << ADC_INPUT))
bool ADCtestFlag = true;
// R-sense pin Settings
#define SENSE_OUTPUT 33
#define SENSE_OUTPUT_PIN_SEL ((1ULL << SENSE_OUTPUT))
#define SENSE_INPUT 27
#define SENSE_INPUT_PIN_SEL ((1ULL << SENSE_INPUT))

void gpioSetup(void)
{
    // Setup GPIO ADC
    gpio_config_t io_conf_adc;
    // Output pin
    io_conf_adc.intr_type = GPIO_PIN_INTR_DISABLE; //disable interrupt
    io_conf_adc.mode = GPIO_MODE_OUTPUT;           //set as output mode
    io_conf_adc.pin_bit_mask = ADC_OUTPUT_PIN_SEL; //bit mask of the pins that you want to set,e.g. GPIO -12-
    io_conf_adc.pull_down_en = 0;                  //disable pull-down mode
    io_conf_adc.pull_up_en = 0;                    //disable pull-up mode
    gpio_config(&io_conf_adc);                     //configure GPIO with the given settings
    // Input Pin
    io_conf_adc.intr_type = GPIO_PIN_INTR_DISABLE; // GPIO_PIN_INTR_POSEDGE;  //interrupt of rising edg
    io_conf_adc.pin_bit_mask = ADC_INPUT_PIN_SEL;  //bit mask of the pins, use GPIO -13-
    io_conf_adc.mode = GPIO_MODE_INPUT;            //set as input mode
    io_conf_adc.pull_down_en = 1;                  //enable pull-down mode
    io_conf_adc.pull_up_en = 0;                    //disable pull-up mode
    gpio_config(&io_conf_adc);

    // Setup GPIO Sense
    gpio_config_t io_conf_sense;
    // Output pin
    io_conf_sense.intr_type = GPIO_PIN_INTR_DISABLE;   //disable interrupt
    io_conf_sense.mode = GPIO_MODE_OUTPUT;             //set as output mode
    io_conf_sense.pin_bit_mask = SENSE_OUTPUT_PIN_SEL; //bit mask of the pins that you want to set,e.g. GPIO -12-
    io_conf_sense.pull_down_en = 0;                    //disable pull-down mode
    io_conf_sense.pull_up_en = 0;                      //disable pull-up mode
    gpio_config(&io_conf_sense);                       //configure GPIO with the given settings
    // Input Pin
    io_conf_sense.intr_type = GPIO_PIN_INTR_DISABLE;  // GPIO_PIN_INTR_POSEDGE;  //interrupt of rising edg
    io_conf_sense.pin_bit_mask = SENSE_INPUT_PIN_SEL; //bit mask of the pins, use GPIO -13-
    io_conf_sense.mode = GPIO_MODE_INPUT;             //set as input mode
    io_conf_sense.pull_down_en = 1;                   //enable pull-down mode
    io_conf_sense.pull_up_en = 0;                     //disable pull-up mode
    gpio_config(&io_conf_sense);
}

/*******************************
 * ADC Reader
 *******************************/

// Calculation
#define SAMPLESIZE 1300000
#define MAVG 20

// ADC Variables
volatile double vIn = 0;
volatile double inputADC[MAVG] = {0};
volatile double inputSENSE[MAVG] = {0};
uint8_t mAVGcounter = 0;
volatile bool vInFlag = false;
uint32_t counterSum = 0;
uint32_t counter = 0;

void adcRead(void *pvParameters)
{
    // Initialize GPIO and variable
    gpio_set_level(ADC_OUTPUT, 1);
    gpio_set_level(SENSE_OUTPUT, 1);
    uint8_t check = 0;
    int inputPin = ADC_INPUT;
    int outputPin = ADC_OUTPUT;
    ;
    while (1)
    {
        check = gpio_get_level(inputPin); // Get Comperator value
        gpio_set_level(outputPin, check); // Set Integrator input
        counterSum += check;              // Count High signals
        counter = counter + 1;            // Count Sequence length

        if (counter >= SAMPLESIZE) // 1 milion sample reached
        {
            vIn = (float)((counterSum * 3300) / counter); // Calculate Vin -> V_out = 3.3V
            // Reset counters, set flag = new value ready
            counter = 0;
            counterSum = 0;
            vInFlag = true; // new value ready
            if (ADCtestFlag)
            {
                inputPin = SENSE_INPUT;   // Set input to R_sense ADC
                outputPin = SENSE_OUTPUT; // Set output to R_sense ADC
                ADCtestFlag = false;      // invert flag
            }
            else
            {
                inputPin = ADC_INPUT;   // Set input to PT1000 ADC
                outputPin = ADC_OUTPUT; // Set output to PT1000 ADC
                ADCtestFlag = true;     // invert flag
            }
        }
        usleep(0.1);
    }
}

/*******************************
 * Calculate and print
 *******************************/

#include "math.h"
// PT1000 data
#define PTA 0.0039083
#define PTB -0.0000005775
#define RSENSE 1000

void readAndPrint(void *pvParameters)
{
    // Initialize Variables
    long double sum = 0;
    double outADC = 0;
    double outSense = 0;
    // double senseCurrent = 0; 
    double temp = 0;
    double pt1000 = 0;
    while (1)
    {
        if (vInFlag) // Check for new Variable
        {
            if (ADCtestFlag) // Check wich ADC have been in use
            {
                outADC = 0;                       // reset calculation
                inputADC[mAVGcounter] = vIn;      // load to input array
                vIn = 0;                          // Reset vIn
                vInFlag = false;                  // Reset 'new value' flag
                for (size_t i = 0; i < MAVG; i++) // Average input array
                {
                    sum += (float)(inputADC[i]);
                }
                outADC = (float)(sum / MAVG); // Set output
                sum = 0;                      // Reset sum
                pt1000 = outADC / (outSense / RSENSE);
            }
            else
            {
                outSense = 0;                       // reset calculation
                inputSENSE[mAVGcounter] = vIn;      // load to input array
                vIn = 0;                            // Reset vIn
                vInFlag = false;                    // Reset 'new value' flag
                for (size_t i = 0; i < MAVG; i++)   // Average input array
                {
                    sum += (float)(inputSENSE[i]); 
                }
                outSense = (float)(sum / MAVG); // Set output
                //senseCurrent = outSense/RSENSE;
                    sum = 0; // Reset sum

                mAVGcounter++;                                             // Increment input array index
                mAVGcounter = mAVGcounter == (MAVG - 1) ? 0 : mAVGcounter; // Reset index when max is reached
            }
            /*
            *   Test values for 25 degree Celcius
            */
            //  pt1000 = 1097.346563;
            //  senseCurrent = 0.000365;  
            temp = (10 * sqrt(10) * sqrt(250 * pow(PTA, 2) - 1000 * PTB + pt1000*PTB) -500*PTA)/(1000*PTB);
            printf("%.4f,  %.4f,  %.3f\n", (float)(outADC), (float)(outSense), temp);
        }
        usleep(1);
    }
}

/*******************************
 * Regulator
 *******************************/

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#define PWM_UNIT MCPWM_UNIT_0       // Choose between "MCPWM_UNIT_0" "MCPWM_UNIT_1"
#define SIGNAL_TYPE MCPWM0A         // MCPWM sginal type -> mcpwm_io_signals_t
#define PWM_TIMER MCPWM_TIMER_0     // MCPWM_TIMER_0, MCPWM_TIMER_1, MCPWM_TIMER_2
#define PWM_PIN 26                  // A0 = GPIO#26

void initPWM(void)
{
    // mcpwm_init(mcpwm_unit_tmcpwm_num, mcpwm_timer_ttimer_num, constmcpwm_config_t *mcpwm_conf)
    mcpwm_gpio_init(PWM_UNIT, SIGNAL_TYPE, PWM_PIN); // Init GPIO PIN #26

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 20;   //frequency = 10Hz
    pwm_config.cmpr_a = 0;       //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;       //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;     // 0 = Active high duty, 1 = Active low duty
    mcpwm_init(PWM_UNIT, PWM_TIMER, &pwm_config); //Configure PWM0A & PWM0B with above settings
}

void regulator(void *pvParameters)
{
    initPWM();
    while (1)
    {
        for(size_t i = 0; i < 100; i++)
        {
            mcpwm_set_duty(PWM_UNIT, MCPWM_TIMER_0, SIGNAL_TYPE, i);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        
    }
}

    /*******************************
*   Main Function
*******************************/

    void
    app_main(void)
{
    // Init GPIO
    gpioSetup();
    // Start Tasks
    xTaskCreatePinnedToCore(&adcRead, "CORE 1: ADC Reader", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(&readAndPrint, "CORE 0: Read and Print", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(&regulator, "CORE 0: Regulator", 1024, NULL, 1, NULL, 0);
}
