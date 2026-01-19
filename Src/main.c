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
#include "main.h"
#include "adc.h"
#include "rmu.h"
#include "iwdt.h"
#include "svd.h"
#include "can.h"
#include "clock.h"
#include "lptim16.h"
#include "gpio.h"

uint32_t GetV0;
uint32_t GetV1;
uint32_t GetV2;
uint32_t GetV3;
uint32_t GetV4;
uint32_t GetV5;
uint32_t GetV6;
uint32_t GetV7;
uint32_t GetV8;

extern uint32_t DMAResult[10];

uint16_t CAN_V[8];   // V0 ~ V7，用于CAN发送

uint8_t txBuf[8];

#define CAN_CTRL_STD_ID 0x200U

static void HandleCanControlMessage(void)
{
    uint32_t mask;

    if ((RxMessage.StdId != CAN_CTRL_STD_ID) || (RxMessage.DLC < 7U))
    {
        return;
    }

    mask = (uint32_t)RxMessage.Data[0]
           | ((uint32_t)RxMessage.Data[1] << 8)
           | ((uint32_t)RxMessage.Data[2] << 16)
           | (((uint32_t)(RxMessage.Data[3] & 0x0FU)) << 24);

    ValveOutputs_Set(mask);
    VND7140_SetInputs((uint8_t)(RxMessage.Data[4] & 0x01U),
                      (uint8_t)(RxMessage.Data[4] & 0x02U));
    VND7140_SetSelect((uint8_t)(RxMessage.Data[5] & 0x01U),
                      (uint8_t)(RxMessage.Data[5] & 0x02U),
                      (uint8_t)(RxMessage.Data[5] & 0x04U));
    if ((RxMessage.Data[6] & 0x01U) != 0U)
    {
        VND7140_FaultResetPulse();
    }
}

/**
  * @brief  ECC检错中断不可屏蔽，直接进入NMI中断服务 请保留 
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    uint32_t RAMDetectionFlag = 0, DataFlashDetectionFlag = 0, CodeFlashDetectionFlag = 0;
    uint32_t WakeUpDebugFlag = 0;
    
    /* RAM ECC 检错 必须保留 */
    RAMDetectionFlag = FL_FLASH_IsActiveFlag_ECCDetectionInRAM(FLASH);
    if (0x01U == RAMDetectionFlag)
    {   
        FL_FLASH_ClearFlag_ECCDetectionInRAM(FLASH);
        /* 客户安全处理 */
        /* ...... */
    }
    
    /* Data Flash ECC检错 必须保留 */ 
    DataFlashDetectionFlag = FL_FLASH_IsActiveFlag_ECCDetectionInDataFlash(FLASH);
    if (0x01U == DataFlashDetectionFlag)
    {
        FL_FLASH_ClearFlag_ECCDetectionInDataFlash(FLASH); 
        /* 客户安全处理 */
        /* ...... */
    }
    
    /* Code Flash ECC检错 必须保留 */ 
    CodeFlashDetectionFlag = FL_FLASH_IsActiveFlag_ECCDetectionInCodeFlash(FLASH);
    if (0x01U == CodeFlashDetectionFlag)
    {
        FL_FLASH_ClearFlag_ECCDetectionInCodeFlash(FLASH); 
        /* 客户安全处理 */
        /* ...... */
    }
    
    /* CPU Debugger唤醒 必须保留 */
    WakeUpDebugFlag = FL_PMU_IsActiveFlag_WakeupDBG(PMU);
    if (0x01U == WakeUpDebugFlag)
    {
        FL_PMU_ClearFlag_WakeupDBG(PMU);
    }
}

/**
  * @brief  HardFault 中断服务函数 请保留 
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{   /* 软件复位MCU，使用内联函数 */
    RMU_Soft_SystemReset();
}

/**
  * @brief  掉电监控，用于执行掉电事件
  * @param  None
  * @retval None
  */
static void PowerDownMonitoring(void)
{
    /* 确认SVD监测结果是否低于阈值 */
    if (true == SVD_Result_Confirmed(SVD_BELOW_THRESHOLD, 1U))
    {   /* 防抖处理 */
        if (true == SVD_Result_Confirmed(SVD_BELOW_THRESHOLD, 2000U/*us*/))
        {
            /* 电压下降到报警阈值，处理掉电事件 */
            /* 用户程序1 */
            /* ...... */
            /* 注意：用户需确保在BOR复位之前执行完毕 */
            
            /* 确认SVD监测结果是否高于阈值，如否则持续等待 */
            while(false == SVD_Result_Confirmed(SVD_HIGHER_THRESHOLD, 20U/*us*/));
            /* 由于电源有下降到报警阈值，软件复位后重新初始化 */
            RMU_Soft_SystemReset();
        }
    }
}

int32_t rusult;
int main(void)
{
    /* 使能IWDT */
    (void)IWDT_Init(FL_IWDT_PERIOD_4000MS);
    
    /* 延时函数初始化 */
    FL_Init();
    
    /* 使能SVD,阈值4.16V(falling)~4.284V(rising) */
    (void)SVD_Init(SVD_MONTIOR_VDD,FL_SVD_WARNING_THRESHOLD_GROUP11,FL_SVD_REFERENCE_1P0V);
    
    /* 确认SVD监测结果是否高于阈值，如否则持续等待 */
    while(false == SVD_Result_Confirmed(SVD_HIGHER_THRESHOLD, 2000U/*us*/));
    
    /* 使能BOR为2.0V */
    RMU_BOR_Init(FL_RMU_BOR_THRESHOLD_2P00V);
    
    /* 系统时钟选择XTHF，具体请见初始化参数 */
    SystemClockSel_XTHF(&CMU_XTHF_Init_Data);
    
    /* 用于监控XTHF停振事件，先配置系统时钟，再使能该监控定时器, 10ms周期 */
    LPTIM16_Init();
	
	  /* LED初始化 */
    GPIO_Init();
		
		/* ADC初始化 */
    AdcInit();
    
    /* CAN外设初始化 */
    rusult = Can_Controller_Init();
    
    /* 填充发送数据 */
    CAN_SetMsg(&TxMessage);  
    /* 执行数据发送 */    
    FL_CAN_Frame_Send(FL_CAN_FORMAT_STANDARD_DATA, &TxMessage, CAN, FL_CAN_BUFFER0);   
		
		/*打开电磁阀电源*/
		EVPOWER_EN_ON();

    while(1)
    {
        /* 清狗 */
        IWDT_Clr();

        if (CanRxPending != 0U)
        {
            CanRxPending = 0U;
            HandleCanControlMessage();
        }
        
        /* 电源掉电监测处理 */
        PowerDownMonitoring();
        
        /* 功能执行 */
        if (1 == Period10msFlag) 
        {
            Period10msFlag = 0;
            /* 判断是否发生XTHF停振事件，如有及时切回主系统时钟 */
            SwtichToMainSystemClock(&CMU_XTHF_Init_Data);
        }
				
				/* 闪灯 */
        LED0_TOG();
        FL_DelayMs(10);
				
				/* DMA通道采集 */
        GetDMAChannelSample();

        /* 获取转换值 */
        GetV0  =  (uint32_t)(((uint64_t)(DMAResult[0] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV1  =  (uint32_t)(((uint64_t)(DMAResult[1] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV2  =  (uint32_t)(((uint64_t)(DMAResult[2] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV3  =  (uint32_t)(((uint64_t)(DMAResult[3] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV4  =  (uint32_t)(((uint64_t)(DMAResult[4] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV5  =  (uint32_t)(((uint64_t)(DMAResult[5] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV6  =  (uint32_t)(((uint64_t)(DMAResult[6] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV7  =  (uint32_t)(((uint64_t)(DMAResult[7] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));
				GetV8  =  (uint32_t)(((uint64_t)(DMAResult[8] & 0xFFFF) * (ADC_VREF)) / ((uint64_t)(DMAResult[9] & 0xFFFF) * 10));	
				
				/* -------- 电压值压缩为 16bit（CAN信号层） -------- */
				CAN_V[0] = (uint16_t)GetV0;
				CAN_V[1] = (uint16_t)GetV1;
				CAN_V[2] = (uint16_t)GetV2;
				CAN_V[3] = (uint16_t)GetV3;
				CAN_V[4] = (uint16_t)GetV4;
				CAN_V[5] = (uint16_t)GetV5;
				CAN_V[6] = (uint16_t)GetV6;
				CAN_V[7] = (uint16_t)GetV7;
				


				/* V0 ~ V3 */
				txBuf[0] =  CAN_V[0] & 0xFF;
				txBuf[1] =  CAN_V[0] >> 8;
				txBuf[2] =  CAN_V[1] & 0xFF;
				txBuf[3] =  CAN_V[1] >> 8;
				txBuf[4] =  CAN_V[2] & 0xFF;
				txBuf[5] =  CAN_V[2] >> 8;
				txBuf[6] =  CAN_V[3] & 0xFF;
				txBuf[7] =  CAN_V[3] >> 8;

				TxMessage.StdId = 0x100;
				TxMessage.DLC = 8;
				memcpy(TxMessage.Data, txBuf, 8);

				FL_CAN_Frame_Send(FL_CAN_FORMAT_STANDARD_DATA,
													&TxMessage,
													CAN,
													FL_CAN_BUFFER0);
									
									
				/* V4 ~ V7 */
				txBuf[0] =  CAN_V[4] & 0xFF;
				txBuf[1] =  CAN_V[4] >> 8;
				txBuf[2] =  CAN_V[5] & 0xFF;
				txBuf[3] =  CAN_V[5] >> 8;
				txBuf[4] =  CAN_V[6] & 0xFF;
				txBuf[5] =  CAN_V[6] >> 8;
				txBuf[6] =  CAN_V[7] & 0xFF;
				txBuf[7] =  CAN_V[7] >> 8;

				TxMessage.StdId = 0x101;
				TxMessage.DLC = 8;
				memcpy(TxMessage.Data, txBuf, 8);

				FL_CAN_Frame_Send(FL_CAN_FORMAT_STANDARD_DATA,
													&TxMessage,
													CAN,
													FL_CAN_BUFFER0);



    }
}

