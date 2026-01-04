#ifndef     __OBSTACLE_AVOIDANCE_H
#define     __OBSTACLE_AVOIDANCE_H


#define OBSTACLE_AVOIDANCE_THREAD_STACK_SIZE        1024
#define OBSTACLE_AVOIDANCE_MB_SIZE                  4


typedef enum
{
    OBSTACLE_DETECT_STATUS_UNKOWN,
    OBSTACLE_DETECT_STATUS_STOP,
    OBSTACLE_DETECT_STATUS_RUNING,
    OBSTACLE_DETECT_STATUS_DONE,
    OBSTACLE_DETECT_STATUS_PLAN_PATH,
    OBSTACLE_DETECT_STATUS_EXECUTE_PATH
}obstacle_status;

typedef enum
{
    OBSTACLE_DETECT_DIRECTION_UNKOWN = 1,
    OBSTACLE_DETECT_DIRECTION_START = 2,
    OBSTACLE_DETECT_DIRECTION_FRONT = 90,
    OBSTACLE_DETECT_DIRECTION_RIGHT = 0,
    OBSTACLE_DETECT_DIRECTION_LEFT = 180,
    OBSTACLE_DETECT_DIRECTION_STOP = 3
}obstacle_detect_direction;


typedef struct 
{
    obstacle_status work_status;
    obstacle_detect_direction direction;
    uint32_t front_distance;
    uint32_t right_distance;
    uint32_t left_distance;
}auto_avoid_obstacle;

typedef struct 
{
	obstacle_status work_state;
	void  *message;
}obstacle_mb_message;


void detect_obstacle_thread_init(void);




#endif