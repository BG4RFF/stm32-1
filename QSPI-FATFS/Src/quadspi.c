/**
  ******************************************************************************
  * File Name          : QUADSPI.c
  * Description        : This file provides code for the configuration
  *                      of the QUADSPI instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */
    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();
  
    /**QUADSPI GPIO Configuration    
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    PB6     ------> QUADSPI_BK1_NCS 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();
  
    /**QUADSPI GPIO Configuration    
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    PB6     ------> QUADSPI_BK1_NCS 
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_6);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/**
  * @brief  ��ʼ��QSPI�洢��
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Init(void)
{ 
	QSPI_CommandTypeDef s_command;
	uint8_t value = W25Q128FV_FSR_QE;
	
	/* QSPI�洢����λ */
	if (QSPI_ResetMemory() != QSPI_OK)
	{
		return QSPI_NOT_SUPPORTED;
	}
	
	/* ʹ��д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}	
	/* ������·ʹ�ܵ�״̬�Ĵ�����ʹ����ͨ��IO2��IO3���� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_STATUS_REG2_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* �������� */
	if (HAL_QSPI_Transmit(&hqspi, &value, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* �Զ���ѯģʽ�ȴ��洢������ */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ��QSPI�洢���ж�ȡ��������.
  * @param  pData: ָ��Ҫ��ȡ�����ݵ�ָ��
  * @param  ReadAddr: ��ȡ��ʼ��ַ
  * @param  Size: Ҫ��ȡ�����ݴ�С    
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ�������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = ReadAddr;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = Size;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* �������� */
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ����������д��QSPI�洢��
  * @param  pData: ָ��Ҫд�����ݵ�ָ��
  * @param  WriteAddr: д��ʼ��ַ
  * @param  Size: Ҫд������ݴ�С    
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	QSPI_CommandTypeDef s_command;
	uint32_t end_addr, current_size, current_addr;
	/* ����д���ַ��ҳ��ĩβ֮��Ĵ�С */
	current_addr = 0;

	while (current_addr <= WriteAddr)
	{
		current_addr += W25Q128FV_PAGE_SIZE;
	}
	current_size = current_addr - WriteAddr;

	/* ������ݵĴ�С�Ƿ�С��ҳ���е�ʣ��λ�� */
	if (current_size > Size)
	{
		current_size = Size;
	}

	/* ��ʼ����ַ���� */
	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;

	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = QUAD_INPUT_PAGE_PROG_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_4_LINES;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* ��ҳִ��д�� */
	do
	{
		s_command.Address = current_addr;
		s_command.NbData  = current_size;

		/* ����д���� */
		if (QSPI_WriteEnable() != QSPI_OK)
		{
			return QSPI_ERROR;
		}

		/* �������� */
		if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		/* �������� */
		if (HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		/* �����Զ���ѯģʽ�ȴ�������� */  
		if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
		{
			return QSPI_ERROR;
		}

		/* ������һҳ��̵ĵ�ַ�ʹ�С���� */
		current_addr += current_size;
		pData += current_size;
		current_size = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128FV_PAGE_SIZE;
	} while (current_addr < end_addr);
	return QSPI_OK;
}

/**
  * @brief  ����QSPI�洢����ָ���� 
  * @param  BlockAddress: ��Ҫ�����Ŀ��ַ  
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Erase_Block(uint32_t BlockAddress)
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = SECTOR_ERASE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = BlockAddress;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* ����д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* �����Զ���ѯģʽ�ȴ��������� */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ��������QSPI�洢��
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_Erase_Chip(void)
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ���������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = CHIP_ERASE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* ����д���� */
	if (QSPI_WriteEnable() != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	} 
	/* �����Զ���ѯģʽ�ȴ��������� */  
	if (QSPI_AutoPollingMemReady(W25Q128FV_BULK_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ��ȡQSPI�洢���ĵ�ǰ״̬
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_GetStatus(void)
{
	QSPI_CommandTypeDef s_command;
	uint8_t reg;
	/* ��ʼ����ȡ״̬�Ĵ������� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 1;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	/* �������� */
	if (HAL_QSPI_Receive(&hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	} 
	/* ���Ĵ�����ֵ */
	if((reg & W25Q128FV_FSR_BUSY) != 0)
	{
		return QSPI_BUSY;
	}
	else
	{
		return QSPI_OK;
	}
}

/**
  * @brief  ����QSPI�洢��������
  * @param  pInfo: �����ýṹ�ϵ�ָ��  
  * @retval QSPI�洢��״̬
  */
uint8_t BSP_QSPI_GetInfo(QSPI_Info* pInfo)
{
	/* ���ô洢�����ýṹ */
	pInfo->FlashSize          = W25Q128FV_FLASH_SIZE;
	pInfo->EraseSectorSize    = W25Q128FV_SUBSECTOR_SIZE;
	pInfo->EraseSectorsNumber = (W25Q128FV_FLASH_SIZE/W25Q128FV_SUBSECTOR_SIZE);
	pInfo->ProgPageSize       = W25Q128FV_PAGE_SIZE;
	pInfo->ProgPagesNumber    = (W25Q128FV_FLASH_SIZE/W25Q128FV_PAGE_SIZE);
	return QSPI_OK;
}

/**
  * @brief  ��λQSPI�洢����
  * @param  hqspi: QSPI���
  * @retval ��
  */
static uint8_t QSPI_ResetMemory()
{
	QSPI_CommandTypeDef s_command;
	/* ��ʼ����λʹ������ */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = RESET_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* �������� */
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* ���͸�λ�洢������ */
	s_command.Instruction = RESET_MEMORY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* �����Զ���ѯģʽ�ȴ��洢������ */  
	if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}


/**
  * @brief  ����д��ʹ�ܣ��ȴ�����Ч.
  * @param  hqspi: QSPI���
  * @retval ��
  */
static uint8_t QSPI_WriteEnable()
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	/* ����д���� */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* �����Զ���ѯģʽ�ȴ�д���� */  
	s_config.Match           = W25Q128FV_FSR_WREN;
	s_config.Mask            = W25Q128FV_FSR_WREN;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	s_command.Instruction    = READ_STATUS_REG1_CMD;
	s_command.DataMode       = QSPI_DATA_1_LINE;
	s_command.NbData         = 1;

	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

/**
  * @brief  ��ȡ�洢����SR���ȴ�EOP
  * @param  hqspi: QSPI���
  * @param  Timeout ��ʱ
  * @retval ��
  */
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;
	/* �����Զ���ѯģʽ�ȴ��洢��׼������ */  
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_STATUS_REG1_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	s_config.Match           = 0x00;
	s_config.Mask            = W25Q128FV_FSR_BUSY;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, Timeout) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	return QSPI_OK;
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t QSPI_FLASH_ReadID(void)
{
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[3];
	/* ��ȡJEDEC ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_JEDEC_ID_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	  s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 3;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}

	Temp = ( pData[2] | pData[1]<<8 )| ( pData[0]<<16 );

	return Temp;
}

 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
  * @retval FLASH Device ID
  */
uint32_t QSPI_FLASH_ReadDeviceID(void)
{  
	QSPI_CommandTypeDef s_command;
	uint32_t Temp = 0;
	uint8_t pData[3];
	/*##-2-��ȡ�豸ID����    ###########################################*/
	/* ��ȡ����/�豸 ID */
	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = READ_ID_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.Address           = 0x000000;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.NbData            = 2;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}
	if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		printf("something wrong ....\r\n");
		/* �û��������������һЩ����������������� */
		while(1)
		{
			
		}
	}

	Temp = pData[1] |( pData[0]<<8 ) ;

	return Temp;
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
