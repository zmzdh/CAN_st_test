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
  
#ifndef __GPIO_H__
#define __GPIO_H__

#include "fm33fg0xxa_fl.h"

#define LED0_GPIO   			GPIOE
#define LED0_PIN    			FL_GPIO_PIN_4

#define VALVE_FB_GPIO     GPIOC
#define VALVE_FB_PIN      FL_GPIO_PIN_4

#define EVPOWER_EN_GPIO   GPIOC
#define EVPOWER_EN_PIN    FL_GPIO_PIN_5

/* Pump PWM (GPTIM0 CH3/CH4) */
#define PUMP_PULSE_PWM_GPIO   GPIOC
#define PUMP_PULSE_PWM_PIN    FL_GPIO_PIN_11
#define PUMP_PULSE_PWM_REMAP  FL_GPIO_PINREMAP_FUNCTON0

#define PUMP_MASS_PWM_GPIO    GPIOC
#define PUMP_MASS_PWM_PIN     FL_GPIO_PIN_12
#define PUMP_MASS_PWM_REMAP   FL_GPIO_PINREMAP_FUNCTON0

#define IN1_GPIO          GPIOA
#define IN1_PIN           FL_GPIO_PIN_2
#define IN2_GPIO          GPIOA
#define IN2_PIN           FL_GPIO_PIN_3
#define IN3_GPIO          GPIOA
#define IN3_PIN           FL_GPIO_PIN_4
#define IN4_GPIO          GPIOA
#define IN4_PIN           FL_GPIO_PIN_5
#define IN5_GPIO          GPIOG
#define IN5_PIN           FL_GPIO_PIN_10
#define IN6_GPIO          GPIOG
#define IN6_PIN           FL_GPIO_PIN_11
#define IN7_GPIO          GPIOB
#define IN7_PIN           FL_GPIO_PIN_4
#define IN8_GPIO          GPIOB
#define IN8_PIN           FL_GPIO_PIN_5
#define IN9_GPIO          GPIOB
#define IN9_PIN           FL_GPIO_PIN_6
#define IN10_GPIO         GPIOB
#define IN10_PIN          FL_GPIO_PIN_7
#define IN11_GPIO         GPIOB
#define IN11_PIN          FL_GPIO_PIN_10
#define IN12_GPIO         GPIOB
#define IN12_PIN          FL_GPIO_PIN_11
#define IN13_GPIO         GPIOB
#define IN13_PIN          FL_GPIO_PIN_13
#define IN14_GPIO         GPIOB
#define IN14_PIN          FL_GPIO_PIN_14
#define IN15_GPIO         GPIOE
#define IN15_PIN          FL_GPIO_PIN_0
#define IN16_GPIO         GPIOE
#define IN16_PIN          FL_GPIO_PIN_1
#define IN17_GPIO         GPIOE
#define IN17_PIN          FL_GPIO_PIN_2
#define IN18_GPIO         GPIOE
#define IN18_PIN          FL_GPIO_PIN_3
#define IN19_GPIO         GPIOH
#define IN19_PIN          FL_GPIO_PIN_12
#define IN20_GPIO         GPIOH
#define IN20_PIN          FL_GPIO_PIN_13
#define IN21_GPIO         GPIOI
#define IN21_PIN          FL_GPIO_PIN_4
#define IN22_GPIO         GPIOI
#define IN22_PIN          FL_GPIO_PIN_5
#define IN23_GPIO         GPIOF
#define IN23_PIN          FL_GPIO_PIN_1
#define IN24_GPIO         GPIOF
#define IN24_PIN          FL_GPIO_PIN_2
#define IN25_GPIO         GPIOF
#define IN25_PIN          FL_GPIO_PIN_3
#define IN26_GPIO         GPIOF
#define IN26_PIN          FL_GPIO_PIN_4
#define IN27_GPIO         GPIOA
#define IN27_PIN          FL_GPIO_PIN_6
#define IN28_GPIO         GPIOA
#define IN28_PIN          FL_GPIO_PIN_7

#define VND7140_INPUT0_GPIO  GPIOC
#define VND7140_INPUT0_PIN   FL_GPIO_PIN_14
#define VND7140_FAULTRST_GPIO GPIOC
#define VND7140_FAULTRST_PIN  FL_GPIO_PIN_15
#define VND7140_SEN_GPIO     GPIOE
#define VND7140_SEN_PIN      FL_GPIO_PIN_5
#define VND7140_SEL0_GPIO    GPIOB
#define VND7140_SEL0_PIN     FL_GPIO_PIN_15
#define VND7140_SEL1_GPIO    GPIOH
#define VND7140_SEL1_PIN     FL_GPIO_PIN_14
#define VND7140_MULTSEN_GPIO GPIOC
#define VND7140_MULTSEN_PIN  FL_GPIO_PIN_0
#define VND7140_INPUT1_GPIO  GPIOC
#define VND7140_INPUT1_PIN   FL_GPIO_PIN_1

#define LED0_ON()   			FL_GPIO_ResetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_OFF()  			FL_GPIO_SetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_TOG()  			FL_GPIO_ToggleOutputPin(LED0_GPIO, LED0_PIN)

#define EVPOWER_EN_ON()   FL_GPIO_SetOutputPin(EVPOWER_EN_GPIO, EVPOWER_EN_PIN)
#define EVPOWER_EN_OFF()  FL_GPIO_ResetOutputPin(EVPOWER_EN_GPIO, EVPOWER_EN_PIN)


extern void GPIO_Init(void);
extern void GPIO_DeInit(void);
extern void ValveOutputs_Set(uint32_t mask);
extern void VND7140_SetInputs(uint8_t input0, uint8_t input1);
extern void VND7140_SetSelect(uint8_t sel0, uint8_t sel1, uint8_t mults);
extern void VND7140_FaultResetPulse(void);

#endif
