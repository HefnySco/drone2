#ifndef _DRONE_MOTORS_DUE_H_
#define _DRONE_MOTORS_DUE_H_

#include "Arduino.h"

#include "Motors.h"

//#if defined (USE_400HZ_ESC)
#define PWM_PERIOD 2500   // in us
//#else
//  #define PWM_PERIOD 3300   // in us
//#endif  

#define PWM_SERVO_PERIOD 20000   // in us


void writeMotors()
{

  for (int ch = 0;ch < 8; ch++) {
    if (motorCommand[ch] <= 1000) { motorCommand[ch] = 1000;}
    if (motorCommand[ch] >= 2000) { motorCommand[ch] = 2000;}
  }


  PWMC_SetDutyCycle(PWM, 0, motorCommand[MOTOR1]);
  PWMC_SetDutyCycle(PWM, 1, motorCommand[MOTOR2]);
  PWMC_SetDutyCycle(PWM, 2, motorCommand[MOTOR3]); 
  PWMC_SetDutyCycle(PWM, 3, motorCommand[MOTOR4]);
  if (numberOfMotors == 6 || numberOfMotors == 8) {
      PWMC_SetDutyCycle(PWM, 4, motorCommand[MOTOR5]);
      PWMC_SetDutyCycle(PWM, 5, motorCommand[MOTOR6]);
    }
    if (numberOfMotors == 8) {
      PWMC_SetDutyCycle(PWM, 6, motorCommand[MOTOR7]);
      PWMC_SetDutyCycle(PWM, 7, motorCommand[MOTOR8]);    
  }
}

void commandAllMotors(int command) {
  PWMC_SetDutyCycle(PWM, 0, command);
  PWMC_SetDutyCycle(PWM, 1, command);
  PWMC_SetDutyCycle(PWM, 2, command);
  PWMC_SetDutyCycle(PWM, 3, command);
    if (numberOfMotors == 6 || numberOfMotors == 8) {
      PWMC_SetDutyCycle(PWM, 4, command);
      PWMC_SetDutyCycle(PWM, 5, command);
    }
    if (numberOfMotors == 8) {
      PWMC_SetDutyCycle(PWM, 6, command);
      PWMC_SetDutyCycle(PWM, 7, command);
    }
}

static void setPWMpin(uint32_t pin) {
  PIO_Configure(g_APinDescription[pin].pPort,
                PIO_PERIPH_B, //hack Arduino does not allow high PWM by default
                g_APinDescription[pin].ulPin,
                g_APinDescription[pin].ulPinConfiguration);
}

static void configOneMotor(uint8_t ch, uint32_t period) {
  PWMC_ConfigureChannel(PWM, ch, PWM_CMR_CPRE_CLKA, 0, 0);
  PWMC_SetPeriod(PWM, ch, period);
  PWMC_SetDutyCycle(PWM, ch, 1000);
  PWMC_EnableChannel(PWM, ch);
}

void initializeMotors(byte numbers) {

  numberOfMotors = numbers;

  setPWMpin(34); //PWM L0
  setPWMpin(36); //PWM L1
  setPWMpin(38); //PWM L2
  PWMC_DisableChannel(PWM, 0);
  PWMC_DisableChannel(PWM, 1);
  PWMC_DisableChannel(PWM, 2);

  setPWMpin(40); //PWM L3 
  PWMC_DisableChannel(PWM, 3);
  

  if (numberOfMotors == 6 || numberOfMotors == 8) {
    setPWMpin(9);  //PWM L4 
    setPWMpin(8);  //PWM L5
    PWMC_DisableChannel(PWM, 4);
    PWMC_DisableChannel(PWM, 5);
  }
  if (numberOfMotors == 8) {
    setPWMpin(7);  //PWM L6 
    setPWMpin(6);  //PWM L7 
    PWMC_DisableChannel(PWM, 6);
    PWMC_DisableChannel(PWM, 7);
  }

  pmc_enable_periph_clk(ID_PWM);

  // set PWM clock A to 1MHz
  PWMC_ConfigureClocks(1000000,0,VARIANT_MCK);

  if (numberOfMotors == 8) {
    configOneMotor(7, PWM_PERIOD);
    configOneMotor(6, PWM_PERIOD);
  }
  if ((numberOfMotors == 6) || (numberOfMotors == 8)) {
    configOneMotor(5, PWM_PERIOD);
    configOneMotor(4, PWM_PERIOD);
  }

  configOneMotor(3, PWM_PERIOD);  
  configOneMotor(2, PWM_PERIOD);
  configOneMotor(1, PWM_PERIOD);
  configOneMotor(0, PWM_PERIOD);

  commandAllMotors(1000);                                     // Initialise motors to 1000us (stopped)

}

#endif