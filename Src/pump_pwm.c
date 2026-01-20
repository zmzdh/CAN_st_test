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

#include "pump_pwm.h"
#include "gpio.h"
#include "fm33fg0xxa_fl.h"
#include "fm33fg0xxa_fl_gptim.h"

#define PUMP_PWM_TIMER              GPTIM0
#define PUMP_PWM_CHANNEL_PULSE      FL_GPTIM_CHANNEL_3
#define PUMP_PWM_CHANNEL_MASS       FL_GPTIM_CHANNEL_4

#define PUMP_PWM_TIMER_CLOCK_HZ     8000000U
#define PUMP_PWM_FREQUENCY_HZ       20000U
#define PUMP_PWM_DUTY_MAX           100U

static uint32_t PumpPwm_ComputeAutoReload(uint32_t *prescaler)
{
    uint32_t arr;
    uint32_t psc = 0U;

    if (prescaler == NULL)
    {
        return 0U;
    }

    arr = (PUMP_PWM_TIMER_CLOCK_HZ / PUMP_PWM_FREQUENCY_HZ) - 1U;
    while ((arr > 0xFFFFU) && (psc < 0xFFFFU))
    {
        psc++;
        arr = (PUMP_PWM_TIMER_CLOCK_HZ / (PUMP_PWM_FREQUENCY_HZ * (psc + 1U))) - 1U;
    }

    *prescaler = psc;
    return arr;
}

static void PumpPwm_InitPins(void)
{
    FL_GPIO_InitTypeDef gpio_init = {0};

    gpio_init.mode          = FL_GPIO_MODE_DIGITAL;
    gpio_init.pull          = FL_GPIO_BOTH_DISABLE;
    gpio_init.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    gpio_init.driveStrength = FL_GPIO_DRIVESTRENGTH_X3;

    gpio_init.pin       = PUMP_PULSE_PWM_PIN;
    gpio_init.remapPin  = PUMP_PULSE_PWM_REMAP;
    (void)FL_GPIO_Init(PUMP_PULSE_PWM_GPIO, &gpio_init);

    gpio_init.pin       = PUMP_MASS_PWM_PIN;
    gpio_init.remapPin  = PUMP_MASS_PWM_REMAP;
    (void)FL_GPIO_Init(PUMP_MASS_PWM_GPIO, &gpio_init);
}

void PumpPwm_Init(void)
{
    FL_GPTIM_InitTypeDef tim_init = {0};
    FL_GPTIM_OC_InitTypeDef oc_init = {0};
    uint32_t prescaler = 0U;
    uint32_t auto_reload;

    FL_CMU_EnableGroup4BusClock(FL_CMU_GROUP4_BUSCLK_GPTIM0);
    PumpPwm_InitPins();

    auto_reload = PumpPwm_ComputeAutoReload(&prescaler);

    tim_init.prescaler        = prescaler;
    tim_init.counterMode      = FL_GPTIM_COUNTER_DIR_UP;
    tim_init.autoReload       = auto_reload;
    tim_init.autoReloadState  = FL_ENABLE;
    tim_init.clockDivision    = FL_GPTIM_CLK_DIVISION_DIV1;
    (void)FL_GPTIM_Init(PUMP_PWM_TIMER, &tim_init);

    oc_init.OCMode      = FL_GPTIM_OC_MODE_PWM1;
    oc_init.OCFastMode  = FL_DISABLE;
    oc_init.OCPreload   = FL_ENABLE;
    oc_init.CompareValue = 0U;
    oc_init.OCPolarity  = FL_GPTIM_OC_POLARITY_NORMAL;
    oc_init.OCETRFStatus = FL_DISABLE;
    (void)FL_GPTIM_OC_Init(PUMP_PWM_TIMER, PUMP_PWM_CHANNEL_PULSE, &oc_init);
    (void)FL_GPTIM_OC_Init(PUMP_PWM_TIMER, PUMP_PWM_CHANNEL_MASS, &oc_init);

    FL_GPTIM_OC_EnableChannel(PUMP_PWM_TIMER, PUMP_PWM_CHANNEL_PULSE);
    FL_GPTIM_OC_EnableChannel(PUMP_PWM_TIMER, PUMP_PWM_CHANNEL_MASS);

    FL_GPTIM_Enable(PUMP_PWM_TIMER);
}

void PumpPwm_SetDuty(PumpPwmChannel channel, uint16_t duty_percent)
{
    uint32_t compare_value;
    uint32_t auto_reload = FL_GPTIM_ReadAutoReload(PUMP_PWM_TIMER);
    uint32_t duty = duty_percent;

    if (duty > PUMP_PWM_DUTY_MAX)
    {
        duty = PUMP_PWM_DUTY_MAX;
    }

    compare_value = ((auto_reload + 1U) * duty) / PUMP_PWM_DUTY_MAX;

    if (channel == PUMP_PWM_PULSE)
    {
        FL_GPTIM_WriteCompareCH1(PUMP_PWM_TIMER, compare_value);
    }
    else
    {
        FL_GPTIM_WriteCompareCH2(PUMP_PWM_TIMER, compare_value);
    }
}

void PumpPwm_DisableAll(void)
{
    PumpPwm_SetDuty(PUMP_PWM_PULSE, 0U);
    PumpPwm_SetDuty(PUMP_PWM_MASS, 0U);
}
