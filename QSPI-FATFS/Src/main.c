
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author         : greedyhao
  * @note           : ���˲�����վ��http://greedyhao.cc/
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
#include "main.h"
#include "stm32f7xx_hal.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <string.h>
//#include "log.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

//typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

UINT fnum;            			  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F767������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  SCB_EnableICache();
  SCB_EnableDCache();
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C4_Init();
  MX_QUADSPI_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  BSP_LED_Off(LED_R);
  BSP_LED_Off(LED_B);
  BSP_LED_Off(LED_G);
  
 	printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
//  printf("flash id=%d\n",QSPI_FLASH_ReadID());
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	retUSER = f_mount(&USERFatFS,"0:",1);
  
	/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(retUSER == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
		/* ��ʽ�� */
		retUSER=f_mkfs("0:",FM_ANY, 0, ReadBuffer, sizeof(ReadBuffer));							
		
		if(retUSER == FR_OK)
		{
			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
			/* ��ʽ������ȡ������ */
			retUSER = f_mount(NULL,"0:",1);			
			/* ���¹���	*/			
			retUSER = f_mount(&USERFatFS,"0:",1);
		}
		else
		{
			LED_RED;
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
	else if(retUSER!=FR_OK)
	{
		printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",retUSER);
		printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
		while(1);
	}
	else
	{
		printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
	}
  
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
	retUSER = f_open(&USERFile, "0:FatUSERFatFS��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
//  LOG_ERROR(retUSER);
	if ( retUSER == FR_OK )
	{
		printf("����/����FatUSERFatFS��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
		retUSER=f_write(&USERFile,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(retUSER==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",retUSER);
    }    
		/* ���ٶ�д���ر��ļ� */
    f_close(&USERFile);
	}
	else
	{	
		LED_RED;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	retUSER = f_open(&USERFile, "0:FatUSERFatFS��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(retUSER == FR_OK)
	{
		LED_GREEN;
		printf("�����ļ��ɹ���\r\n");
		retUSER = f_read(&USERFile, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(retUSER==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",retUSER);
    }		
	}
	else
	{
		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&USERFile);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"0:",1);
  
	/* ������ɣ�ͣ�� */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C4;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    /* Toggle LED1 */
    BSP_LED_Toggle(LED_B);
  }  
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

//    if(huart->Instance == USART1)   // �ж������ĸ����ڴ������ж�
//    {
//      HAL_UART_Transmit(&huart1,Rx_Buffer,1,100);    // ���յ���������ʹ�ô���1���ͳ�ȥ
//      HAL_UART_Receive_IT(&huart1,Rx_Buffer,1);      // ����ʹ�ܴ���1�����ж�
//    }    
}

///*
// * ��������Buffercmp
// * ����  ���Ƚ������������е������Ƿ����
// * ����  ��-pBuffer1     src������ָ��
// *         -pBuffer2     dst������ָ��
// *         -BufferLength ����������
// * ���  ����
// * ����  ��-PASSED pBuffer1 ����   pBuffer2
// *         -FAILED pBuffer1 ��ͬ�� pBuffer2
// */
//TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
//{
//  while(BufferLength--)
//  {
//    if(*pBuffer1 != *pBuffer2)
//    {
//      return FAILED;
//    }

//    pBuffer1++;
//    pBuffer2++;
//  }
//  return PASSED;
//}
void log_error(uint8_t code)
{
  switch(code)
    {
      case FR_OK:				/* (0) Succeeded */ 
        printf("FR_OK\n");break;
      case FR_DISK_ERR:			/* (1) A hard error occurred in the low level disk I/O layer */ 
        printf("FR_DISK_ERR\n");break;
      case FR_INT_ERR:				/* (2) Assertion failed */ 
        printf("FR_INT_ERR\n");break;
      case FR_NOT_READY:			/* (3) The physical drive cannot work */ 
        printf("FR_NOT_READY\n");break;
      case FR_NO_FILE:				/* (4) Could not find the file */ 
        printf("FR_NO_FILE\n");break;
      case FR_NO_PATH:				/* (5) Could not find the path */ 
        printf("FR_NO_PATH\n");break;
      case FR_INVALID_NAME:		/* (6) The path name format is invalid */ 
        printf("FR_INVALID_NAME\n");break;
      case FR_DENIED:				/* (7) Access denied due to prohibited access or directory full */ 
        printf("FR_DENIED\n");break;
      case FR_EXIST:			/* (8) Access denied due to prohibited access */ 
        printf("FR_EXIST��\n");break;
      case FR_INVALID_OBJECT:		/* (9) The file/directory object is invalid */ 
        printf("FR_INVALID_OBJECT\n");break;
      case FR_WRITE_PROTECTED:		/* (10) The physical drive is write protected */ 
        printf("FR_WRITE_PROTECTED\n");break;
      case FR_INVALID_DRIVE:		/* (11) The logical drive number is invalid */ 
        printf("FR_INVALID_DRIVE\n");break;
      case FR_NOT_ENABLED:			/* (12) The volume has no work area */ 
        printf("FR_NOT_ENABLED\n");break;
      case FR_NO_FILESYSTEM:		/* (13) There is no valid FAT volume */ 
        printf("FR_NO_FILESYSTEM\n");break;
      case FR_MKFS_ABORTED:		/* (14) The f_mkfs() aborted due to any problem */ 
        printf("FR_MKFS_ABORTED\n");break;
      case FR_TIMEOUT:				/* (15) Could not get a grant to access the volume within defined period */ 
        printf("FR_TIMEOUT\n");break;
      case FR_LOCKED:				/* (16) The operation is rejected according to the file sharing policy */ 
        printf("FR_LOCKED\n");break;
      case FR_NOT_ENOUGH_CORE:		/* (17) LFN working buffer could not be allocated */ 
        printf("FR_NOT_ENOUGH_CORE\n");break;
      case FR_TOO_MANY_OPEN_FILES:	/* (18) Number of open files > _FS_LOCK */ 
        printf("FR_TOO_MANY_OPEN_FILES\n");break;
      case FR_INVALID_PARAMETER:	/* (19) Given parameter is invalid */ 
        printf("FR_INVALID_PARAMETER\n");break;
    }
    printf("<<-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
