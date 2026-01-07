#include "rtthread.h"
#include "motor.h"
#include "car.h"


void car_control(car_control_direction direction)
{
    switch(direction)
    {
        case CAR_CONTROL_FRONT:
            motoerA_start_positive();
            motoerB_start_positive();
            break;
        case CAR_CONTROL_FRONT_LEFT:
            motoerA_stop();
            motoerB_start_positive();
            break;
        case CAR_CONTROL_FRONT_RIGHT:
            motoerA_start_positive();
            motoerB_stop();
            break;
        case CAR_CONTROL_BACK:
            motoerA_start_negative();
            motoerB_start_negatice();
            break;
        case CAR_CONTROL_BACK_LEFT:
            motoerA_stop();
            motoerB_start_negatice();
            break;
        case CAR_CONTROL_BACK_RIGHT:
            motoerA_start_negative();
            motoerB_stop();
            break;
        case CAR_CONTROL_STOP:
            motoerA_stop();
            motoerB_stop();
        default:
            break;
    }
}
#ifdef ENABLE_DEBUG
MSH_CMD_EXPORT(car_control,control car);
#endif
