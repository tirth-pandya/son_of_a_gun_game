#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

#include "delay.h"
#include "joystick.h"

// void joystick_task(void *);

void joystick_task(void *p) {
  joystick__values_s joystick_val = {0};
  gpio_s x = {0, 25};
  gpio_s y = {1, 30};
  gpio_s s_k = {1, 31};
  joystick__initialize(x, y, s_k);

  while (1) {
    joystick_val = joystick__get_value();
    printf("X axis : %d,\tY axis : %d\n", joystick_val.x, joystick_val.y);
    vTaskDelay(100);
  }
}

int main(void) {
  puts("Starting RTOS");
  xTaskCreate(joystick_task, "read_joystick", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
