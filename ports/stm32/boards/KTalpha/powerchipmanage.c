/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

//#include "py/runtime.h"
//#include "py/mperrno.h"
//#include "py/mphal.h"
//#include "extmod/virtpin.h"
//#include "powerchipmanage.h"
//#include "pin.h"
#include "extint.h"

#include "py/mphal.h"
#include "pin.h"
#include "genhdr/pins.h"
#include "timer.h"
#include "irq.h"
#include "powerchipmanage.h"

#define POWER_ENABLE_Pin                GPIO_PIN_12
#define POWER_ENABLE_GPIO_Port          GPIOC
#define POWER_PULSE_Pin                 GPIO_PIN_2
#define POWER_PULSE_GPIO_Port           GPIOD
#define POWER_KEY_IN_Pin                GPIO_PIN_3
#define POWER_KEY_IN_GPIO_Port          GPIOB
#define POWER_KEY_IN_EXTI_IRQn          EXTI3_IRQn

#define LED_G_Pin 			GPIO_PIN_0
#define LED_G_GPIO_Port 		GPIOC
#define LED_B_Pin 			GPIO_PIN_1
#define LED_B_GPIO_Port 		GPIOC
#define LED_R_Pin 			GPIO_PIN_2
#define LED_R_GPIO_Port 		GPIOC

TIM_HandleTypeDef TIM5_Handle;

int pluse_count;
int power_key_flag;

//TIM5 is not used for Servo PWM any more
// and now, TIM5 is used for power chip control
void set_power_chip_pluse(void)
{
    power_key_flag = 0;
    // TIM5 clock enable
    __HAL_RCC_TIM5_CLK_ENABLE();

    // set up and enable interrupt
    NVIC_SetPriority(TIM5_IRQn, IRQ_PRI_TIM5);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);

    // PWM clock configuration
    TIM5_Handle.Instance = TIM5;
    //TIM5_Handle.Init.Period = 2000 - 1; // timer cycles at 50Hz
    TIM5_Handle.Init.Period = 14200 - 1; // timer cycles at 5Hz
    TIM5_Handle.Init.Prescaler = (timer_get_source_freq(5) / 100000) - 1; // timer runs at 100kHz
    TIM5_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM5_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;

    //HAL_TIM_PWM_Init(&TIM5_Handle);
    HAL_TIM_Base_Init(&TIM5_Handle);
    HAL_TIM_Base_Start_IT(&TIM5_Handle); // start timer interrupts
}


void power_chip_init0(void)
{
    pluse_count = 1;
    GPIO_InitTypeDef GPIO_InitStruct;

    //HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = POWER_ENABLE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(POWER_ENABLE_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_SET);
    //mp_hal_pin_config(pin_C0, MP_HAL_PIN_MODE_OUTPUT, GPIO_PULLUP, 0);

    //HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = POWER_PULSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(POWER_PULSE_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
    //mp_hal_pin_config(pin_D2, MP_HAL_PIN_MODE_OUTPUT, GPIO_PULLUP, 0);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = POWER_KEY_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(POWER_KEY_IN_GPIO_Port, &GPIO_InitStruct);
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);


    set_power_chip_pluse();  //Give a periodic signal to prevent power from sleeping
}


void power_chip_tick_update(void)
{
  if(0 == power_key_flag)
  {
     if(9 == pluse_count)
    {
        HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_RESET);
        pluse_count=0;
        }
    else
    {
        HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
        pluse_count++;
     }
  }
  else if(1 == power_key_flag)
  {
    if(pluse_count > 0)
    {
        HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_RESET);
        pluse_count=0;
        }
    else
    {
        HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
        pluse_count++;
     }
  }


}


void power_chip_key_press(void)
{
HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
/*
    HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
    mp_hal_delay_ms(100);
    HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_RESET);
    mp_hal_delay_ms(200);
    HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
    mp_hal_delay_ms(200);
    HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_RESET);
    mp_hal_delay_ms(200);
    HAL_GPIO_WritePin(POWER_PULSE_GPIO_Port, POWER_PULSE_Pin, GPIO_PIN_SET);
    mp_hal_delay_ms(200);
*/
    mp_hal_delay_ms(10);
    if(HAL_GPIO_ReadPin(POWER_KEY_IN_GPIO_Port,POWER_KEY_IN_Pin)==0)
    {
        power_key_flag = 1;
        pluse_count = 0;
    }
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
/*
  power_key_flag = 1;
  pluse_count = 0;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
*/
}

