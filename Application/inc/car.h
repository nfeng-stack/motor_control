#ifndef     __CAR_H
#define     __CAR_H

typedef enum
{
    CAR_CONTROL_FRONT,
    CAR_CONTROL_FRONT_LEFT,
    CAR_CONTROL_FRONT_RIGHT,
    CAR_CONTROL_BACK,
    CAR_CONTROL_BACK_LEFT,
    CAR_CONTROL_BACK_RIGHT,
    CAR_CONTROL_STOP
}car_control_direction;

void car_control(car_control_direction direction);




#endif