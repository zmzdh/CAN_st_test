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

#include "adc.h"

volatile uint8_t ADCComplete = 0U;
uint64_t GetVSample = 0;
uint32_t GetAVREFSample = 0;
uint8_t  ADCChannelCount = 0U;

uint32_t DMAResult[10];

/**
  * @brief  ADC初始化
  * @param  void
  * @retval void
  */
void AdcInit(void)
{
    FL_GPIO_InitTypeDef         GPIO_InitStruct = {0};
    FL_ADC_CommonInitTypeDef    ADC_CommonInitStruct;
    FL_ADC_InitTypeDef          ADC_InitStruct ;
    
		
		/* 配置引脚为模拟功能 PD0 到 PD3   PD9 到 PD13 */
    GPIO_InitStruct.pin        = FL_GPIO_PIN_0|FL_GPIO_PIN_1|FL_GPIO_PIN_2|FL_GPIO_PIN_3|FL_GPIO_PIN_9
																|FL_GPIO_PIN_10|FL_GPIO_PIN_11|FL_GPIO_PIN_12|FL_GPIO_PIN_13;
    GPIO_InitStruct.mode       = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull       = FL_DISABLE;
    GPIO_InitStruct.remapPin   = FL_DISABLE;
    FL_GPIO_Init( GPIOD, &GPIO_InitStruct );
		
    
    
    /* ADC 时钟配置 */
    ADC_CommonInitStruct.clockSource             = FL_CMU_ADC_CLK_SOURCE_RCHF;  /* ADC_CLK 时钟源 */
    ADC_CommonInitStruct.clockPrescaler          = FL_CMU_ADC_PSC_DIV8;         /* ADC_CLK 分频系数 */
    ADC_CommonInitStruct.referenceSource         = FL_ADC_REF_SOURCE_VDDA;      /* ADC正基准 */
    ADC_CommonInitStruct.negativeReferenceSource = FL_ADC_REF_NEGA_VSSA;        /* ADC负基准 */
    FL_ADC_CommonInit(&ADC_CommonInitStruct);
    
    /* ADC寄存器设置 */
    ADC_InitStruct.conversionMode           = FL_ADC_CONV_MODE_SINGLE;     /* 单次模式 */
    ADC_InitStruct.autoMode                 = FL_ADC_SINGLE_CONV_MODE_AUTO;/* 自动 */
    ADC_InitStruct.waitMode                 = FL_DISABLE;                  /* 等待 */
    ADC_InitStruct.overrunMode              = FL_ENABLE;                   /* 覆盖上次数据 */
    ADC_InitStruct.scanDirection            = FL_ADC_SEQ_SCAN_DIR_FORWARD; /* 通道正序扫描 */
    ADC_InitStruct.externalTrigConv         = FL_ADC_RGTRIGGER_EDGE_NONE;  /* 禁止触发信号 */
    ADC_InitStruct.triggerSource            = FL_ADC_TRGI_LUT0;            /* 触发信号选择 */
    ADC_InitStruct.fastChannelTime          = FL_ADC_FAST_CH_SAMPLING_TIME_2_ADCCLK;  /* 快速通道采样时间 */
    ADC_InitStruct.lowChannelTime           = FL_ADC_SLOW_CH_SAMPLING_TIME_192_ADCCLK;/* 慢速通道采样时间 */
    ADC_InitStruct.oversamplingMode         = FL_ENABLE;                   /* 过采样打开 关闭可缩短采样时间 */
    ADC_InitStruct.overSampingMultiplier    = FL_ADC_OVERSAMPLING_MUL_8X;  /* 8倍过采样 */
    ADC_InitStruct.oversamplingShift        = FL_ADC_OVERSAMPLING_SHIFT_3B;/* 数据右移*/
   
    FL_ADC_Init(ADC, &ADC_InitStruct);

    FL_ADC_EnableDMAReq(ADC);
}

/**
  * @brief  ADC的DMA配置
  * @param  buffer: 数据指针 
  *         length: 传输长度
  * @retval void
  */
void ADC_DMA(uint32_t *buffer, uint32_t length)
{
    FL_DMA_InitTypeDef     DMA_InitStruct = {0};
    FL_DMA_ConfigTypeDef   DMA_ConfigStruct={0};

    DMA_InitStruct.periphAddress        = FL_DMA_PERIPHERAL_FUNCTION1;
    DMA_InitStruct.direction            = FL_DMA_DIR_PERIPHERAL_TO_RAM;
    DMA_InitStruct.memoryAddressIncMode = FL_DMA_MEMORY_INC_MODE_INCREASE;
    DMA_InitStruct.dataSize             = FL_DMA_BANDWIDTH_32B;
    DMA_InitStruct.priority             = FL_DMA_PRIORITY_HIGH;
    DMA_InitStruct.circMode             = FL_DISABLE;
    FL_DMA_Init(DMA, &DMA_InitStruct, FL_DMA_CHANNEL_4);
    
    FL_DMA_Enable(DMA);
    
    DMA_ConfigStruct.memoryAddress      = (uint32_t)buffer;
    DMA_ConfigStruct.transmissionCount  = length - 1;
    FL_DMA_StartTransmission(DMA, &DMA_ConfigStruct, FL_DMA_CHANNEL_4);
}

/**
  * @brief  获取AVREF采样值
  * @param  void
  * @retval ADCRdresult: 采样值
  */
uint32_t GetAVREFChannelSample(void)
{
    uint16_t ADCRdresult;
    uint32_t counter;
    uint32_t EOC_Flag;

    FL_PMU_EnableAVREFBuffer(PMU);              /* 使能AVREF BUFFER */
    FL_ADC_EnableInternalSequencerChannel(ADC,FL_ADC_INTERNAL_AVREF);  /* 使能AVREF通道 */

    FL_ADC_ClearFlag_EndOfConversion(ADC);      /* 清除EOC标志 */
    FL_ADC_Enable(ADC);                         /* 使能ADC */
    FL_ADC_EnableRegularGroupSWConversion(ADC); /* 使能常规组转换 */

    /* 等待转换完成 */
    do
    {
        EOC_Flag = FL_ADC_IsActiveFlag_EndOfConversion(ADC);
        counter++;
    }while((counter != 0xFFFFFFFF) && (EOC_Flag == 0));

    /* 获取采样值 */
    ADCRdresult = FL_ADC_ReadConversionData(ADC);
 
    /* 关闭ADC */
    FL_ADC_Disable(ADC);
    /* 关闭AVREF通道 */
    FL_ADC_DisableInternalSequencerChannel(ADC,FL_ADC_INTERNAL_AVREF);
    /* 清除EOC标志 */
    FL_ADC_ClearFlag_EndOfConversion(ADC);

    /* 根据应用需求，不考虑功耗下可保持AVREF BUFFER常开，缩短采样时间 */
    //FL_PMU_DisableAVREFBuffer(PMU);
    /* 转换结果 */
    return ADCRdresult;
}

/**
  * @brief  ADC单端输入采样
  * @param  channel:
  * @retval ADCRdresult: 采样值
  */
uint32_t GetSingleChannelSample(uint32_t channel)
{
    uint16_t ADCRdresult;
    uint32_t counter = 0;
    uint32_t EOC_Flag = 0;

    FL_ADC_EnableExternalSequencerChannel(ADC, channel);     /* 通道选择ADC */

    FL_ADC_ClearFlag_EndOfConversion(ADC);                       /* 清标志 */
    FL_ADC_Enable(ADC);                                          /* 启动ADC */
    FL_ADC_EnableRegularGroupSWConversion(ADC);                  /* 开始转换 */

    /* 等待转换完成 */
    do
    {
        EOC_Flag = FL_ADC_IsActiveFlag_EndOfConversion(ADC);
        counter++;
    }while((counter != 0xFFFFFFFF) && (EOC_Flag == 0U));     

    /* 获取采样值 */
    ADCRdresult = FL_ADC_ReadConversionData(ADC);                

    /* 关闭ADC */
    FL_ADC_Disable(ADC);  
    /* 通道关闭ADC_1 */  
    FL_ADC_DisableExternalSequencerChannel(ADC, channel);                          
    /* 清标志 */        
    FL_ADC_ClearFlag_EndOfConversion(ADC);                    

    /* 转换结果 */
    return ADCRdresult;
}

/**
  * @brief  ADC的DMA通道采样
  * @param  void
  * @retval void
  */
void GetDMAChannelSample(void)
{
    FL_PMU_EnableAVREFBuffer(PMU);                                     /* 使能AVREF BUFFER */
    FL_PMU_EnableAVREFBufferOutput(PMU);                               /* 使能AVREF输出缓冲输出使能 */
    FL_ADC_EnableInternalSequencerChannel(ADC,FL_ADC_INTERNAL_AVREF);  /* 通道选择AVREF */
    FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH0);    /* 通道选择ADC_0 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH1);    /* 通道选择ADC_1 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH2);    /* 通道选择ADC_2 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH3);    /* 通道选择ADC_3 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH4);    /* 通道选择ADC_4 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH11);    /* 通道选择ADC_11 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH12);    /* 通道选择ADC_12 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH13);    /* 通道选择ADC_13 */
		FL_ADC_EnableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH14);    /* 通道选择ADC_14 */
    ADC_DMA(DMAResult, 10);

    FL_ADC_ClearFlag_EndOfConversion(ADC);                      /* 清标志 */
    FL_ADC_Enable(ADC);                                         /* 启动ADC */
    FL_ADC_EnableRegularGroupSWConversion(ADC);                 /* 开始转换 */
    /* 等待转换完成 */
    while(!FL_DMA_IsActiveFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4));
    FL_DMA_ClearFlag_TransferComplete(DMA, FL_DMA_CHANNEL_4);

    FL_ADC_Disable(ADC);                                                /* 关闭ADC */
    FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH0);    /* 通道关闭ADC_0 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH1);    /* 通道关闭ADC_1 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH2);    /* 通道关闭ADC_2 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH3);    /* 通道关闭ADC_3 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH4);    /* 通道关闭ADC_4 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH11);    /* 通道关闭ADC_11 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH12);    /* 通道关闭ADC_12 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH13);    /* 通道关闭ADC_13 */
		FL_ADC_DisableExternalSequencerChannel(ADC,FL_ADC_EXTERNAL_CH14);    /* 通道关闭ADC_14 */
    FL_ADC_DisableInternalSequencerChannel(ADC,FL_ADC_INTERNAL_AVREF);  /* 通道关闭AVREF */
    /*根据应用需求，在不考虑功耗的情况下可保持AVREF BUFFER常开，可缩短采样时间*/
    //FL_PMU_DisableAVREFBuffer(PMU);                             /* 关闭AVREF BUFFER */
    /* 转换结果 */          
}



/**
  * @brief  ADC复位函数
  * @param  void
  * @retval void
  */
void ADC_DeInit(void)
{
    /* ADC外设寄存器复位 */
    FL_ADC_DeInit(ADC);
    /* ADC时钟复位 */
    FL_ADC_CommonDeInit();
    /* 清除NVIC Pending标志 */
    NVIC_ClearPendingIRQ(ADC_IRQn);
    /* 关闭NVIC中断 */
    NVIC_DisableIRQ(ADC_IRQn);
}
