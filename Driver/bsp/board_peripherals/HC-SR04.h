#ifndef __HC_SR04_H
#define __HC_SR04_H

void ultrasound_module_handler(void *message);
void ultrasound_detect_distance(void);
void Ultrasound_Init(void);
void ultarsound_module_detect_done_callbacke_register(void (*fun)(uint32_t distance));
#endif
