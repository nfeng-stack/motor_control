#include "rtthread.h"
#include "bsp_init.h"
#include "board.h"
#include "obstacle_avoidance.h"
#include "Servo_motor.h"

#define AUTO_CAR_SAFE_DISTANCE      1000000 //单位um
#define AUTO_CAR_DANGER_DISTANCE    500000

#define SERVO_NEED_DELAY            200

auto_avoid_obstacle auto_car;

struct rt_timer Obstacle_avoidance_timer;
struct rt_thread obstacle_avoidance_thread;
uint8_t obstacle_avoidance_thread_stack[OBSTACLE_AVOIDANCE_THREAD_STACK_SIZE];

struct rt_mailbox obstacle_mb;
uint32_t bostacle_mb_pool[OBSTACLE_AVOIDANCE_MB_SIZE]; 

void obstacle_send_message(obstacle_status work_state,void * parm)
{
    obstacle_mb_message * mb_message_p = (obstacle_mb_message *)rt_malloc(sizeof(obstacle_mb_message));
	if(mb_message_p == NULL){
		return ;
	}
	mb_message_p->message = parm;
	mb_message_p->work_state = work_state;
	rt_mb_send(&obstacle_mb,(rt_ubase_t)mb_message_p);   
}

void start_system(void)
{
    obstacle_send_message(OBSTACLE_DETECT_STATUS_STOP,NULL);
}

#ifdef ENABLE_DEBUG
MSH_CMD_EXPORT(start_system,start_detect);
#endif

void circye_detect_obstacle_timeout_handler(void * param)
{
#ifdef ENABLE_DEBUG
    rt_kprintf("tick is :%d\n",rt_tick_get());
#endif
    obstacle_mb_message * mb_message_p = (obstacle_mb_message *)rt_malloc(sizeof(obstacle_mb_message));
	if(mb_message_p == NULL){
		return ;
	}
	mb_message_p->message = NULL;
	mb_message_p->work_state = OBSTACLE_DETECT_STATUS_RUNING;
	rt_mb_send(&obstacle_mb,(rt_ubase_t)mb_message_p);
}

void obstacle_avoidance_send_detect_distance(uint32_t distance)
{
    obstacle_mb_message * mb_message_p = (obstacle_mb_message *)rt_malloc(sizeof(obstacle_mb_message));
	if(mb_message_p == NULL){
		return ;
	}
	mb_message_p->message = (void *)distance;
	mb_message_p->work_state = OBSTACLE_DETECT_STATUS_DONE;
	rt_mb_send(&obstacle_mb,(rt_ubase_t)mb_message_p);
}

void obstacle_audidance_rev_distance_handler(uint32_t distance)
{
#ifdef ENABLE_DEBUG
    rt_kprintf("obstacle distance is %dmm,direction:%d\n",distance / 1000,auto_car.direction);
#endif
    switch (auto_car.direction)
    {
    case OBSTACLE_DETECT_DIRECTION_UNKOWN:
        auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
        auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
        servo_motor_set_angle(auto_car.direction);
        rt_thread_delay(SERVO_NEED_DELAY);
        ultrasound_detect_distance();
        break;
    case OBSTACLE_DETECT_DIRECTION_FRONT:
        auto_car.front_distance = distance;
        if(auto_car.front_distance != 0 && \
            auto_car.left_distance != 0 && \
            auto_car.right_distance != 0) {
                /*选择车前进的方向*/
                uint32_t max_distance = 0;
               
                if(max_distance <= auto_car.front_distance) {
                    max_distance = auto_car.front_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
                }
                if(max_distance <= auto_car.left_distance) {
                    max_distance = auto_car.left_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_LEFT;
                }
                if(max_distance <= auto_car.right_distance) {
                    max_distance = auto_car.right_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_RIGHT;
                }
            servo_motor_set_angle(auto_car.direction);
            start_system();
            return;
        }
        if(distance >= AUTO_CAR_SAFE_DISTANCE)
        {
            /*不用继续检测了向该方向行驶*/
            start_system();
        } else if(distance <= AUTO_CAR_DANGER_DISTANCE) {
            /*需要立即停车*/
            auto_car.direction = OBSTACLE_DETECT_DIRECTION_LEFT;
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
        } else {
            auto_car.direction = OBSTACLE_DETECT_DIRECTION_LEFT;
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
        } 
        break;
    case OBSTACLE_DETECT_DIRECTION_LEFT:
        auto_car.left_distance = distance;
      if(auto_car.front_distance != 0 && \
        auto_car.left_distance != 0 && \
        auto_car.right_distance != 0) {
            /*选择车前进的方向*/
            uint32_t max_distance = 0;
            
             if(max_distance <= auto_car.front_distance) {
                    max_distance = auto_car.front_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
                }
                if(max_distance <= auto_car.left_distance) {
                    max_distance = auto_car.left_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_LEFT;
                }
                if(max_distance <= auto_car.right_distance) {
                    max_distance = auto_car.right_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_RIGHT;
                }
            servo_motor_set_angle(auto_car.direction);
            start_system();
            return;
        }
        if(distance >= AUTO_CAR_SAFE_DISTANCE)
        {
            /*不用继续检测了向该方向行驶*/
            start_system();
        } else if(distance <= AUTO_CAR_DANGER_DISTANCE) {
            /*需要立即停车*/
            auto_car.direction = OBSTACLE_DETECT_DIRECTION_RIGHT;
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
        } else {
            auto_car.direction = OBSTACLE_DETECT_DIRECTION_RIGHT;
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
        } 
        break;
    case OBSTACLE_DETECT_DIRECTION_RIGHT:
        auto_car.right_distance = distance;
         if(auto_car.front_distance != 0 && \
            auto_car.left_distance != 0 && \
            auto_car.right_distance != 0) {
                /*选择车前进的方向*/
                uint32_t max_distance = 0;
               
                if(max_distance <= auto_car.front_distance) {
                    max_distance = auto_car.front_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
                }
                if(max_distance <= auto_car.left_distance) {
                    max_distance = auto_car.left_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_LEFT;
                }
                if(max_distance <= auto_car.right_distance) {
                    max_distance = auto_car.right_distance;
                    auto_car.direction = OBSTACLE_DETECT_DIRECTION_RIGHT;
                }
            servo_motor_set_angle(auto_car.direction);
            start_system();
            return;
        }
        if(distance >= AUTO_CAR_SAFE_DISTANCE)
        {
            /*不用继续检测了向该方向行驶*/
            start_system();
        } else if(distance <= AUTO_CAR_DANGER_DISTANCE) {
            /*需要立即停车*/
            auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
        } else {
            auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
        } 
        break;
    default:
        break;
    }
}

void obstacle_avoidance_thread_entry(void *param)
{
    while(1)
    {
#ifdef ENABLE_DEBUG
        rt_kprintf("this is bostacle runing\n");
#endif
        obstacle_mb_message * mb_message_p = NULL;
        rt_err_t error = rt_mb_recv(&obstacle_mb,(rt_ubase_t *)&mb_message_p,RT_WAITING_FOREVER);
        if(error != RT_EOK && mb_message_p != NULL) {
            rt_kprintf("obstacle runing error\n");
            return ;
        }
        switch (mb_message_p->work_state)
        {
        case OBSTACLE_DETECT_STATUS_UNKOWN:
#ifdef ENABLE_DEBUG
            rt_kprintf("obstacle work state unkown\n");
#endif
            break;
        case OBSTACLE_DETECT_STATUS_STOP:
            auto_car.work_status = OBSTACLE_DETECT_STATUS_STOP;
            auto_car.front_distance = auto_car.left_distance = auto_car.right_distance = 0;
            rt_timer_start(&Obstacle_avoidance_timer);
            break;
        case OBSTACLE_DETECT_STATUS_RUNING:
#ifdef ENABLE_DEBUG
            rt_kprintf("obstacle work is detect runing\n");
#endif
            auto_car.work_status = OBSTACLE_DETECT_STATUS_RUNING;
            servo_motor_set_angle(auto_car.direction);
            rt_thread_delay(SERVO_NEED_DELAY);
            ultrasound_detect_distance();
            break;
        case OBSTACLE_DETECT_STATUS_DONE:
            auto_car.work_status = OBSTACLE_DETECT_STATUS_DONE;
            obstacle_audidance_rev_distance_handler((uint32_t)mb_message_p->message);
            break;
        case OBSTACLE_DETECT_STATUS_PLAN_PATH:
            break;
        case OBSTACLE_DETECT_STATUS_EXECUTE_PATH:
            break;
        default:
            break;
        }
        rt_free((void *)mb_message_p);
    }
}



void detect_obstacle_thread_init(void)
{
    rt_timer_init(&Obstacle_avoidance_timer,\
                "obstacle_avoid_timer",
                &circye_detect_obstacle_timeout_handler,\
                NULL,2000,RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_HARD_TIMER);
    rt_err_t error = rt_thread_init(&obstacle_avoidance_thread,\
                                    "obstacle_thread",\
                                    &obstacle_avoidance_thread_entry,\
                                    NULL,obstacle_avoidance_thread_stack,\
                                    OBSTACLE_AVOIDANCE_THREAD_STACK_SIZE,10,10);
    if(error != RT_EOK) {
        return ;
    }
    error = rt_mb_init(&obstacle_mb,\
                        "obstacle_mb",\
                        bostacle_mb_pool,\
                        OBSTACLE_AVOIDANCE_MB_SIZE,\
                        RT_IPC_FLAG_FIFO);
    if(errno != RT_EOK) {
        return ;
    }

    auto_car.work_status = OBSTACLE_DETECT_STATUS_STOP;
    auto_car.direction = OBSTACLE_DETECT_DIRECTION_FRONT;
    ultarsound_module_detect_done_callbacke_register(obstacle_avoidance_send_detect_distance);
    rt_thread_startup(&obstacle_avoidance_thread);
}