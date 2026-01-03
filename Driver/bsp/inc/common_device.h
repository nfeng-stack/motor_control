#ifndef __COMMON_DEVICE__
#define __COMMON_DEVICE__

typedef void (*device_init_driver_fn)(uint8_t device_id);
typedef void (*device_open_driver_fn)(uint8_t device_id);
typedef void (*device_close_driver_fn)(uint8_t device_de);
typedef struct 
{
    device_init_driver_fn init;
    device_open_driver_fn open;
    device_close_driver_fn close;
}Driver,*Driver_t;
typedef struct 
{
    uint8_t device_id;
    Driver_t drivers;
    
}Device,*Device_t;



#endif