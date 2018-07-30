
/* Includes ------------------------------------------------------------------*/
#include "../is42s16400j/is42s16400j.h"
#include "stm32f7xx_hal_sdram.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup IS42S16400J
  * @brief      This file provides a set of functions needed to drive the
  *             IS42S16400J SDRAM.
  * @{
  */

/** @defgroup IS42S16400J_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup IS42S16400J_Private_Defines
  * @{
  */

/**
  * @}
  */ 

/** @defgroup IS42S16400J_Private_Macros
  * @{
  */

/**
  * @}
  */  

/** @defgroup IS42S16400J_Private_Variables
  * @{
  */ 

SDRAM_DrvTypeDef is42s16400j_drv = 
{
  IS42S16400J_Init,
  IS42S16400J_WriteBuffer,
  IS42S16400J_ReadBuffer,
  IS42S16400J_Test,
};

static FMC_SDRAM_CommandTypeDef Command;

/**
* @}
*/ 

/** @defgroup IS42S16400J_Private_FunctionPrototypes
  * @{
  */

/**
* @}
*/ 

/** @defgroup IS42S16400J_Private_Functions
  * @{
  */
  
  
/**
  * @brief  �ԡ��֡�Ϊ��λ��sdramд������ 
  * @param  pBuffer: ָ�����ݵ�ָ�� 
  * @param  uwWriteAddress: Ҫд���SDRAM�ڲ���ַ
  * @param  uwBufferSize: Ҫд�����ݴ�С
  * @retval None.
  */
void IS42S16400J_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

  /* ��ֹд���� */
  HAL_SDRAM_WriteProtection_Disable(&hsdram1);
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(HAL_SDRAM_GetState(&hsdram1) != RESET)
  {
  }

  /* ѭ��д������ */
  for (; uwBufferSize != 0; uwBufferSize--) 
  {
    /* �������ݵ�SDRAM */
    *(uint32_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

    /* ��ַ����*/
    write_pointer += 4;
  }
    
}

/**
  * @brief  ��SDRAM�ж�ȡ���� 
  * @param  pBuffer: ָ��洢���ݵ�buffer
  * @param  ReadAddress: Ҫ��ȡ���ݵĵ�ʮ
  * @param  uwBufferSize: Ҫ��ȡ�����ݴ�С
  * @retval None.
  */
void IS42S16400J_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
  
   
  /* ���SDRAM��־���ȴ���SDRAM���� */  
  while ( HAL_SDRAM_GetState(&hsdram1) != RESET)
  {
  }
  
  /*��ȡ���� */
  for(; uwBufferSize != 0x00; uwBufferSize--)
  {
   *pBuffer++ = *(__IO uint32_t *)(SDRAM_BANK_ADDR + write_pointer );
    
   /* ��ַ����*/
    write_pointer += 4;
  } 
}


/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t IS42S16400J_Test(void)
{
  /*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SDRAM_INFO("���ڼ��SDRAM����8λ��16λ�ķ�ʽ��дsdram...");


  /*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SDRAM����ȫ������Ϊ0 ��IS42S16400J_SIZE����8λΪ��λ�� */
  for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* ������SDRAMд������  8λ */
  for (counter = 0; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<IS42S16400J_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //�Ӹõ�ַ��������
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("8λ���ݶ�д����");
      return 0;
    }
  }
	
  
  /*��16λ��ʽ��д���ݣ������*/
  
  /* ��SDRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SDRAMд������  16λ */
  for (counter = 0; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<IS42S16400J_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);  //�Ӹõ�ַ��������
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("16λ���ݶ�д����");

      return 0;
    }
  }

  
  SDRAM_INFO("SDRAM��д����������"); 
  /*���������return 1 */
  return 1;
  

}

/**
  * @brief  �ӳ�һ��ʱ��
  * @param  �ӳٵ�ʱ�䳤��
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}

/**
  * @brief  ��SDRAMоƬ���г�ʼ������
  * @param  None. 
  * @retval None.
  */
static void IS42S16400J_Init(SDRAM_HandleTypeDef* sdramHandle)
{
  uint32_t tmpr = 0;
  
/* Step 3 --------------------------------------------------------------------*/
  /* ������������ṩ��SDRAM��ʱ�� */
  Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
/* Send the command */
  HAL_SDRAM_SendCommand(sdramHandle, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */ 
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  SDRAM_delay(1);
    
/* Step 5 --------------------------------------------------------------------*/
  /* ������������е�bankԤ��� */ 
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
/* Send the command */
  HAL_SDRAM_SendCommand(sdramHandle, &Command, SDRAM_TIMEOUT);   
  
/* Step 6 --------------------------------------------------------------------*/
  /* ��������Զ�ˢ�� */   
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 4;
  Command.ModeRegisterDefinition = 0;
 /* Send the command */
  HAL_SDRAM_SendCommand(sdramHandle, &Command, SDRAM_TIMEOUT);
  
/* Step 7 --------------------------------------------------------------------*/
  /* ����sdram�Ĵ������� */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                   SDRAM_MODEREG_CAS_LATENCY_3           |
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  /* �����������SDRAM�Ĵ��� */
  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_COMMAND_TARGET_BANK;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = tmpr;
  /* Send the command */
  HAL_SDRAM_SendCommand(sdramHandle, &Command, SDRAM_TIMEOUT);
  
/* Step 8 --------------------------------------------------------------------*/

  /* ����ˢ�¼����� */
  /* (15.62 us x Freq) - 20 */
	  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(sdramHandle, 1386); 
//  FMC_SetRefreshCount(1386);
//  /* ������������*/
//  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
//  {
//  }
}


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/************************ (C) COPYRIGHT greedyhao *****END OF FILE****/

