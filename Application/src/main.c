#include "main.h"

int main(void)
{
    board_bsp_thread_init();
    detect_obstacle_thread_init();
    bt_thread_init();
    start_system();
    car_control(CAR_CONTROL_FRONT);
}
