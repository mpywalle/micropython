/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * HAL MSP.
 *
 */
#include STM32_HAL_H 
#include "mpconfigboard.h"

extern void SystemClock_Config();

void HAL_MspInit()
{
    // Set the system clock
    SystemClock_Config();

    // Config Systick
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    /* Enable GPIO clocks */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();

#if defined (STM32F769xx)
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOI_CLK_ENABLE();
    __GPIOJ_CLK_ENABLE();
    __GPIOK_CLK_ENABLE();
#endif

    GPIO_InitTypeDef  GPIO_InitStructure1;
    GPIO_InitStructure1.Pull  = GPIO_PULLUP;
    GPIO_InitStructure1.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure1.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitTypeDef  GPIO_InitStructure2;
    GPIO_InitStructure2.Pull  = GPIO_PULLUP;
    GPIO_InitStructure2.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure2.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitTypeDef  GPIO_InitStructure3;
    GPIO_InitStructure3.Pull  = GPIO_PULLUP;
    GPIO_InitStructure3.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure3.Mode  = GPIO_MODE_OUTPUT_PP;

    GPIO_InitStructure1.Pin = BOOTLDR_LED1_PIN;
    HAL_GPIO_Init(BOOTLDR_LED1_PORT, &GPIO_InitStructure1);
    HAL_GPIO_WritePin(BOOTLDR_LED1_PORT, BOOTLDR_LED1_PIN, GPIO_PIN_SET);
    GPIO_InitStructure2.Pin = BOOTLDR_LED2_PIN;
    HAL_GPIO_Init(BOOTLDR_LED2_PORT, &GPIO_InitStructure2);
    HAL_GPIO_WritePin(BOOTLDR_LED2_PORT, BOOTLDR_LED2_PIN, GPIO_PIN_SET);
    GPIO_InitStructure3.Pin = BOOTLDR_LED3_PIN;
    HAL_GPIO_Init(BOOTLDR_LED3_PORT, &GPIO_InitStructure3);
    HAL_GPIO_WritePin(BOOTLDR_LED3_PORT, BOOTLDR_LED3_PIN, GPIO_PIN_SET);
}
