#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "servo.h"

extern "C" void app_main(void)
{
    xTaskCreate(servo_task, "servo_task", 4096, NULL, 5, NULL); 
}


void servo_task(void *pvParameters)
{
    Servo servo(33);

    while (1) //continuosly cycle through servo values
    {
        printf("setting 0\n");
        servo.set_angle(0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("setting 90\n");
        servo.set_angle(90);
        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("setting 180\n"); 
        servo.set_angle(180);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}