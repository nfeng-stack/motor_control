#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_Astate(uint8_t state);
void Motor_Aspeed(int8_t speed);
void Motor_Bstate(uint8_t state);
void Motor_Bspeed(int8_t speed);
unsigned char show1(void);
unsigned char show2(void);

#endif
