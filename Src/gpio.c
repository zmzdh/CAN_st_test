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
 
#include "gpio.h"

/**
  * @brief  配置GPIO为输出
  * @param  void
  * @retval void
  */
void GPIO_Init(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    
    GPIO_InitStruct.pin           = LED0_PIN;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_GPIO_BOTH_DISABLE;
    GPIO_InitStruct.remapPin      = FL_GPIO_PINREMAP_FUNCTON0;
    GPIO_InitStruct.driveStrength = FL_GPIO_DRIVESTRENGTH_X3;
    (void)FL_GPIO_Init(LED0_GPIO, &GPIO_InitStruct);
		
    
    GPIO_InitStruct.pin           = EVPOWER_EN_PIN;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_GPIO_BOTH_DISABLE;
    GPIO_InitStruct.remapPin      = FL_GPIO_PINREMAP_FUNCTON0;
    GPIO_InitStruct.driveStrength = FL_GPIO_DRIVESTRENGTH_X3;
    (void)FL_GPIO_Init(EVPOWER_EN_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.mode          = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_GPIO_BOTH_DISABLE;
    GPIO_InitStruct.remapPin      = FL_GPIO_PINREMAP_FUNCTON0;
    GPIO_InitStruct.driveStrength = FL_GPIO_DRIVESTRENGTH_X3;

    GPIO_InitStruct.pin           = IN1_PIN;
    (void)FL_GPIO_Init(IN1_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN2_PIN;
    (void)FL_GPIO_Init(IN2_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN3_PIN;
    (void)FL_GPIO_Init(IN3_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN4_PIN;
    (void)FL_GPIO_Init(IN4_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN5_PIN;
    (void)FL_GPIO_Init(IN5_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN6_PIN;
    (void)FL_GPIO_Init(IN6_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN7_PIN;
    (void)FL_GPIO_Init(IN7_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN8_PIN;
    (void)FL_GPIO_Init(IN8_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN9_PIN;
    (void)FL_GPIO_Init(IN9_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN10_PIN;
    (void)FL_GPIO_Init(IN10_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN11_PIN;
    (void)FL_GPIO_Init(IN11_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN12_PIN;
    (void)FL_GPIO_Init(IN12_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN13_PIN;
    (void)FL_GPIO_Init(IN13_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN14_PIN;
    (void)FL_GPIO_Init(IN14_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN15_PIN;
    (void)FL_GPIO_Init(IN15_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN16_PIN;
    (void)FL_GPIO_Init(IN16_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN17_PIN;
    (void)FL_GPIO_Init(IN17_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN18_PIN;
    (void)FL_GPIO_Init(IN18_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN19_PIN;
    (void)FL_GPIO_Init(IN19_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN20_PIN;
    (void)FL_GPIO_Init(IN20_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN21_PIN;
    (void)FL_GPIO_Init(IN21_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN22_PIN;
    (void)FL_GPIO_Init(IN22_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN23_PIN;
    (void)FL_GPIO_Init(IN23_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN24_PIN;
    (void)FL_GPIO_Init(IN24_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN25_PIN;
    (void)FL_GPIO_Init(IN25_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN26_PIN;
    (void)FL_GPIO_Init(IN26_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN27_PIN;
    (void)FL_GPIO_Init(IN27_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = IN28_PIN;
    (void)FL_GPIO_Init(IN28_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = VND7140_INPUT0_PIN;
    (void)FL_GPIO_Init(VND7140_INPUT0_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = VND7140_INPUT1_PIN;
    (void)FL_GPIO_Init(VND7140_INPUT1_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = VND7140_FAULTRST_PIN;
    (void)FL_GPIO_Init(VND7140_FAULTRST_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = VND7140_SEL0_PIN;
    (void)FL_GPIO_Init(VND7140_SEL0_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = VND7140_SEL1_PIN;
    (void)FL_GPIO_Init(VND7140_SEL1_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.pin           = VND7140_MULTSEN_PIN;
    (void)FL_GPIO_Init(VND7140_MULTSEN_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = VND7140_SEN_PIN;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_GPIO_BOTH_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.driveStrength = FL_GPIO_DRIVESTRENGTH_X3;
    (void)FL_GPIO_Init(VND7140_SEN_GPIO, &GPIO_InitStruct);

    ValveOutputs_Set(0U);
    VND7140_SetInputs(0U, 0U);
    VND7140_SetSelect(0U, 0U, 0U);
    FL_GPIO_ResetOutputPin(VND7140_FAULTRST_GPIO, VND7140_FAULTRST_PIN);
}


/**
  * @brief  配置GPIO为高阻抗状态
  * @param  void
  * @retval void
  */
void GPIO_DeInit(void)
{
    FL_GPIO_DeInit(LED0_GPIO, LED0_PIN);
		FL_GPIO_DeInit(EVPOWER_EN_GPIO, EVPOWER_EN_PIN);

    FL_GPIO_DeInit(IN1_GPIO, IN1_PIN);
    FL_GPIO_DeInit(IN2_GPIO, IN2_PIN);
    FL_GPIO_DeInit(IN3_GPIO, IN3_PIN);
    FL_GPIO_DeInit(IN4_GPIO, IN4_PIN);
    FL_GPIO_DeInit(IN5_GPIO, IN5_PIN);
    FL_GPIO_DeInit(IN6_GPIO, IN6_PIN);
    FL_GPIO_DeInit(IN7_GPIO, IN7_PIN);
    FL_GPIO_DeInit(IN8_GPIO, IN8_PIN);
    FL_GPIO_DeInit(IN9_GPIO, IN9_PIN);
    FL_GPIO_DeInit(IN10_GPIO, IN10_PIN);
    FL_GPIO_DeInit(IN11_GPIO, IN11_PIN);
    FL_GPIO_DeInit(IN12_GPIO, IN12_PIN);
    FL_GPIO_DeInit(IN13_GPIO, IN13_PIN);
    FL_GPIO_DeInit(IN14_GPIO, IN14_PIN);
    FL_GPIO_DeInit(IN15_GPIO, IN15_PIN);
    FL_GPIO_DeInit(IN16_GPIO, IN16_PIN);
    FL_GPIO_DeInit(IN17_GPIO, IN17_PIN);
    FL_GPIO_DeInit(IN18_GPIO, IN18_PIN);
    FL_GPIO_DeInit(IN19_GPIO, IN19_PIN);
    FL_GPIO_DeInit(IN20_GPIO, IN20_PIN);
    FL_GPIO_DeInit(IN21_GPIO, IN21_PIN);
    FL_GPIO_DeInit(IN22_GPIO, IN22_PIN);
    FL_GPIO_DeInit(IN23_GPIO, IN23_PIN);
    FL_GPIO_DeInit(IN24_GPIO, IN24_PIN);
    FL_GPIO_DeInit(IN25_GPIO, IN25_PIN);
    FL_GPIO_DeInit(IN26_GPIO, IN26_PIN);
    FL_GPIO_DeInit(IN27_GPIO, IN27_PIN);
    FL_GPIO_DeInit(IN28_GPIO, IN28_PIN);

    FL_GPIO_DeInit(VND7140_INPUT0_GPIO, VND7140_INPUT0_PIN);
    FL_GPIO_DeInit(VND7140_INPUT1_GPIO, VND7140_INPUT1_PIN);
    FL_GPIO_DeInit(VND7140_FAULTRST_GPIO, VND7140_FAULTRST_PIN);
    FL_GPIO_DeInit(VND7140_SEN_GPIO, VND7140_SEN_PIN);
    FL_GPIO_DeInit(VND7140_SEL0_GPIO, VND7140_SEL0_PIN);
    FL_GPIO_DeInit(VND7140_SEL1_GPIO, VND7140_SEL1_PIN);
    FL_GPIO_DeInit(VND7140_MULTSEN_GPIO, VND7140_MULTSEN_PIN);
}

static void GPIO_WritePin(GPIO_Type *GPIOx, uint32_t pin, uint8_t state)
{
    if (state != 0U)
    {
        FL_GPIO_SetOutputPin(GPIOx, pin);
    }
    else
    {
        FL_GPIO_ResetOutputPin(GPIOx, pin);
    }
}

void ValveOutputs_Set(uint32_t mask)
{
    GPIO_WritePin(IN1_GPIO, IN1_PIN,  (uint8_t)((mask >> 0) & 0x01));
    GPIO_WritePin(IN2_GPIO, IN2_PIN,  (uint8_t)((mask >> 1) & 0x01));
    GPIO_WritePin(IN3_GPIO, IN3_PIN,  (uint8_t)((mask >> 2) & 0x01));
    GPIO_WritePin(IN4_GPIO, IN4_PIN,  (uint8_t)((mask >> 3) & 0x01));
    GPIO_WritePin(IN5_GPIO, IN5_PIN,  (uint8_t)((mask >> 4) & 0x01));
    GPIO_WritePin(IN6_GPIO, IN6_PIN,  (uint8_t)((mask >> 5) & 0x01));
	  GPIO_WritePin(IN7_GPIO, IN7_PIN,  (uint8_t)((mask >> 6) & 0x01));
    GPIO_WritePin(IN8_GPIO, IN8_PIN,  (uint8_t)((mask >> 7) & 0x01));
    GPIO_WritePin(IN9_GPIO, IN9_PIN,  (uint8_t)((mask >> 8) & 0x01));
	  GPIO_WritePin(IN10_GPIO, IN10_PIN,  (uint8_t)((mask >> 9) & 0x01));
    GPIO_WritePin(IN11_GPIO, IN11_PIN,  (uint8_t)((mask >> 10) & 0x01));
	  GPIO_WritePin(IN12_GPIO, IN12_PIN,  (uint8_t)((mask >> 11) & 0x01));
    GPIO_WritePin(IN13_GPIO, IN13_PIN,  (uint8_t)((mask >> 12) & 0x01));
    GPIO_WritePin(IN14_GPIO, IN14_PIN,  (uint8_t)((mask >> 13) & 0x01));
    GPIO_WritePin(IN15_GPIO, IN15_PIN,  (uint8_t)((mask >> 14) & 0x01));
    GPIO_WritePin(IN16_GPIO, IN16_PIN,  (uint8_t)((mask >> 15) & 0x01));
	  GPIO_WritePin(IN17_GPIO, IN17_PIN,  (uint8_t)((mask >> 16) & 0x01));
    GPIO_WritePin(IN18_GPIO, IN18_PIN,  (uint8_t)((mask >> 17) & 0x01));
    GPIO_WritePin(IN19_GPIO, IN19_PIN,  (uint8_t)((mask >> 18) & 0x01));
	  GPIO_WritePin(IN20_GPIO, IN20_PIN,  (uint8_t)((mask >> 19) & 0x01));
    GPIO_WritePin(IN21_GPIO, IN21_PIN,  (uint8_t)((mask >> 20) & 0x01));
    GPIO_WritePin(IN22_GPIO, IN22_PIN,  (uint8_t)((mask >> 21) & 0x01));
	  GPIO_WritePin(IN23_GPIO, IN23_PIN,  (uint8_t)((mask >> 22) & 0x01));
    GPIO_WritePin(IN24_GPIO, IN24_PIN,  (uint8_t)((mask >> 23) & 0x01));
    GPIO_WritePin(IN25_GPIO, IN25_PIN,  (uint8_t)((mask >> 24) & 0x01));
	  GPIO_WritePin(IN26_GPIO, IN26_PIN,  (uint8_t)((mask >> 25) & 0x01));
    GPIO_WritePin(IN27_GPIO, IN27_PIN,  (uint8_t)((mask >> 26) & 0x01));
    GPIO_WritePin(IN28_GPIO, IN28_PIN,	(uint8_t)((mask >> 27) & 0x01));
}

void VND7140_SetInputs(uint8_t input0, uint8_t input1)
{
    GPIO_WritePin(VND7140_INPUT0_GPIO, VND7140_INPUT0_PIN, input0);
    GPIO_WritePin(VND7140_INPUT1_GPIO, VND7140_INPUT1_PIN, input1);
}

void VND7140_SetSelect(uint8_t sel0, uint8_t sel1, uint8_t mults)
{
    GPIO_WritePin(VND7140_SEL0_GPIO, VND7140_SEL0_PIN, sel0);
    GPIO_WritePin(VND7140_SEL1_GPIO, VND7140_SEL1_PIN, sel1);
    GPIO_WritePin(VND7140_MULTSEN_GPIO, VND7140_MULTSEN_PIN, mults);
}

void VND7140_FaultResetPulse(void)
{
    FL_GPIO_SetOutputPin(VND7140_FAULTRST_GPIO, VND7140_FAULTRST_PIN);
    FL_GPIO_ResetOutputPin(VND7140_FAULTRST_GPIO, VND7140_FAULTRST_PIN);
}
