/**
  *******************************************************************************************************
  * @attention
  * Copyright (c) 2023, SHANGHAI FUDAN MICROELECTRONICS GROUP CO., LTD.(FUDAN MICROELECTRONICS./ FUDAN MICRO.)
  * All rights reserved.
  *
  * Processor:                   FM33FG0xxA
  * http:                        http://www.fmdevelopers.com.cn/
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met
  *
  * 1. Redistributions of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  *
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  *
  * 3. Neither the name of the copyright holder nor the names of its contributors
  *    may be used to endorse or promote products derived from this software
  *    without specific prior written permission.
  *
  * 4. To provide the most up-to-date information, the revision of our documents
  *    on the World Wide Web will be the most Current. Your printed copy may be
  *    an earlier revision. To verify you have the latest information avaliable,
  *    refer to: http://www.fmdevelopers.com.cn/.
  *
  * THIS SOFTWARE IS PROVIDED BY FUDAN MICRO "AS IS" AND ANY EXPRESSED:
  * ORIMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  * OF MERCHANTABILITY NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE
  * ARE DISCLAIMED.IN NO EVENT SHALL FUDAN MICRO OR ITS CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISINGIN ANY WAY OUT OF THE
  * USE OF THIS SOFTWARE, EVEN IF ADVISED OFTHE POSSIBILITY OF SUCH DAMAGE.
  *
  *******************************************************************************************************
  */

#include "valve_pwm.h"
#include "gpio.h"

void FL_GPTIM_OC_StructInit(FL_GPTIM_OC_InitTypeDef *oc_init);

#define VALVE_PWM_GPT_TIMER            GPTIM1
#define VALVE_PWM_ATIM_TIMER           ATIM
#define VALVE_PWM_TAU_TIMER            TAU00

#define VALVE_PWM_TIMER_CLOCK_HZ       8000000U
#define VALVE_PWM_MAX_ARR              0xFFFFU

#define VALVE_PWM_MASK_ALL             0x0FFFFFFFUL
#define VALVE_PWM_MASK_GPT             ((1UL << 0)  | (1UL << 1)  | (1UL << 2)  | (1UL << 3) | \
                                        (1UL << 10) | (1UL << 11) | (1UL << 12) | (1UL << 13))
#define VALVE_PWM_MASK_ATIM            ((1UL << 6) | (1UL << 7) | (1UL << 8) | (1UL << 9))
#define VALVE_PWM_MASK_TAU             (VALVE_PWM_MASK_ALL & ~(VALVE_PWM_MASK_GPT | VALVE_PWM_MASK_ATIM))

static volatile uint32_t g_valve_pwm_mask = 0U;
static volatile uint8_t g_valve_pwm_duty = VALVE_PWM_DEFAULT_DUTY;

static void ValvePwm_UpdateCompare(void)
{
    uint32_t auto_reload = FL_GPTIM_ReadAutoReload(VALVE_PWM_TIMER);
    uint32_t compare_value;
    uint8_t duty = g_valve_pwm_duty;

    if (duty >= VALVE_PWM_DUTY_MAX)
    {
        FL_GPTIM_DisableIT_CC(VALVE_PWM_TIMER, FL_GPTIM_CHANNEL_1);
        return;
    }

    compare_value = ((auto_reload + 1U) * duty) / VALVE_PWM_DUTY_MAX;
    FL_GPTIM_WriteCompareCH1(VALVE_PWM_TIMER, compare_value);
    FL_GPTIM_EnableIT_CC(VALVE_PWM_TIMER, FL_GPTIM_CHANNEL_1);
}

void ValvePwm_Init(void)
{
    FL_GPTIM_InitTypeDef tim_init = {0};
    FL_GPTIM_OC_InitTypeDef oc_init = {0};
    FL_NVIC_InitTypeDef interrupt_config = {0};
    uint32_t prescaler = 0U;
    uint32_t auto_reload;

    auto_reload = (VALVE_PWM_TIMER_CLOCK_HZ / VALVE_PWM_FREQUENCY_HZ) - 1U;
    while (auto_reload > VALVE_PWM_MAX_ARR)
    {
        prescaler++;
        auto_reload = (VALVE_PWM_TIMER_CLOCK_HZ / (VALVE_PWM_FREQUENCY_HZ * (prescaler + 1U))) - 1U;
    }

    tim_init.prescaler        = prescaler;
    tim_init.autoReload       = auto_reload;
    tim_init.counterMode      = FL_GPTIM_COUNTER_DIR_UP;
    tim_init.clockDivision    = FL_GPTIM_CLK_DIVISION_DIV1;
    tim_init.autoReloadState  = FL_ENABLE;
    (void)FL_GPTIM_Init(VALVE_PWM_TIMER, &tim_init);

    FL_GPTIM_OC_StructInit(&oc_init);
    oc_init.OCMode = FL_GPTIM_OC_MODE_FROZEN;
    (void)FL_GPTIM_OC_Init(VALVE_PWM_TIMER, FL_GPTIM_CHANNEL_1, &oc_init);

    ValvePwm_UpdateCompare();
    FL_GPTIM_EnableIT_Update(VALVE_PWM_TIMER);

    interrupt_config.preemptPriority = 2U;
    interrupt_config.subPriority = 0U;
    interrupt_config.enable = FL_ENABLE;
    FL_NVIC_Init(&interrupt_config, GPTIMX_IRQn);

    FL_GPTIM_Enable(VALVE_PWM_TIMER);

    ValvePwm_SetDutyPercent(VALVE_PWM_DEFAULT_DUTY);
    ValvePwm_SetMask(0U);
}

void ValvePwm_SetDutyPercent(uint8_t duty_percent)
{
    uint8_t duty = duty_percent;
    uint32_t primask;

    if (duty > VALVE_PWM_DUTY_MAX)
    {
        duty = VALVE_PWM_DUTY_MAX;
    }

    primask = __get_PRIMASK();
    __disable_irq();
    g_valve_pwm_duty = duty;
    ValvePwm_UpdateCompare();
    if (primask == 0U)
    {
        __enable_irq();
    }

    if ((duty == 0U) || (g_valve_pwm_mask == 0U))
    {
        ValveOutputs_Set(0U);
    }
    else if (duty >= VALVE_PWM_DUTY_MAX)
    {
        ValveOutputs_Set(g_valve_pwm_mask);
    }
}

void ValvePwm_SetMask(uint32_t mask)
{
    uint32_t primask;
    uint32_t previous_mask;
    uint32_t removed_mask;
    uint32_t new_mask = mask & VALVE_PWM_MASK_ALL;

    primask = __get_PRIMASK();
    __disable_irq();
    previous_mask = g_valve_pwm_mask;
    g_valve_pwm_mask = new_mask;
    g_valve_pwm_group_mask[VALVE_PWM_GROUP_GPT] = g_valve_pwm_mask & VALVE_PWM_MASK_GPT;
    g_valve_pwm_group_mask[VALVE_PWM_GROUP_ATIM] = g_valve_pwm_mask & VALVE_PWM_MASK_ATIM;
    g_valve_pwm_group_mask[VALVE_PWM_GROUP_TAU] = g_valve_pwm_mask & VALVE_PWM_MASK_TAU;
    removed_mask = previous_mask & ~new_mask;
    if (primask == 0U)
    {
        __enable_irq();
    }

    if (removed_mask != 0U)
    {
        ValveOutputs_SetMasked(0U, removed_mask);
    }
    ValvePwm_ApplyGroupOutput(g_valve_pwm_group_mask[VALVE_PWM_GROUP_GPT], g_valve_pwm_duty[VALVE_PWM_GROUP_GPT]);
    ValvePwm_ApplyGroupOutput(g_valve_pwm_group_mask[VALVE_PWM_GROUP_ATIM], g_valve_pwm_duty[VALVE_PWM_GROUP_ATIM]);
    ValvePwm_ApplyGroupOutput(g_valve_pwm_group_mask[VALVE_PWM_GROUP_TAU], g_valve_pwm_duty[VALVE_PWM_GROUP_TAU]);
}

void ValvePwm_Disable(void)
{
    g_valve_pwm_mask = 0U;
    ValveOutputs_Set(0U);
    FL_GPTIM_DisableIT_CC(VALVE_PWM_TIMER, FL_GPTIM_CHANNEL_1);
    FL_GPTIM_DisableIT_Update(VALVE_PWM_TIMER);
    FL_GPTIM_Disable(VALVE_PWM_TIMER);
}

void GPTIMX_IRQHandler(void)
{
    if (FL_GPTIM_IsActiveFlag_Update(VALVE_PWM_TIMER))
    {
        FL_GPTIM_ClearFlag_Update(VALVE_PWM_TIMER);
        if ((g_valve_pwm_mask == 0U) || (g_valve_pwm_duty == 0U))
        {
            ValveOutputs_Set(0U);
        }
        else
        {
            ValveOutputs_Set(g_valve_pwm_mask);
        }
    }

    if (FL_GPTIM_IsActiveFlag_CC(VALVE_PWM_TIMER, FL_GPTIM_CHANNEL_1))
    {
        FL_GPTIM_ClearFlag_CC(VALVE_PWM_TIMER, FL_GPTIM_CHANNEL_1);
        if (g_valve_pwm_duty < VALVE_PWM_DUTY_MAX)
        {
            ValveOutputs_Set(0U);
        }
    }
}
