
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "dma2d.h"
#include "ltdc.h"
#include "../std800480/std800480.h"
#include "../../../../Utilities/fonts/font24.c"
#include "../../../../Utilities/fonts/font20.c"
#include "../../../../Utilities/fonts/font16.c"
#include "../../../../Utilities/fonts/font12.c"
#include "../../../../Utilities/fonts/font8.c"


/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup STD800480
  * @brief      This file provides a set of functions needed to drive the
  *             STD800480 lcd.
  * @{
  */

/** @defgroup STD800480_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup STD800480_Private_Defines
  * @{
  */
  
/* Default LCD configuration with LCD Layer 1 */
static uint32_t            ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];

/**
  * @}
  */ 

/** @defgroup STD800480_Private_Macros
  * @{
  */
  
  
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y)) 

#define ABS(X)  ((X) > 0 ? (X) : -(X))
  
/*����Һ�������ֲ�Ĳ�������*/
#define HBP  46		//HSYNC�����Ч����
#define VBP  23		//VSYNC�����Ч����

#define HSW   1		//HSYNC���
#define VSW   1		//VSYNC���

#define HFP  10		//HSYNCǰ����Ч����
#define VFP  22		//VSYNCǰ����Ч����


/**
  * @}
  */  

/** @defgroup STD800480_Private_Variables
  * @{
  */ 

LCD_DrvTypeDef std800480_drv = 
{
  LCD_Init,
  LCD_Test,
  0,
  LCD_DisplayOn,
  0,
  LCD_Clear,
  LCD_SelectLayer,
  LCD_SetTransparency,
  LCD_LayerInit,
};



/**
* @}
*/ 

/** @defgroup STD800480_Private_FunctionPrototypes
  * @{
  */
//static LTDC_HandleTypeDef  hltdc;
static DMA2D_HandleTypeDef hdma2d;


static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
static void LL_ConvertLineToARGB8888(void * pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode);
/**
* @}
*/ 

/** @defgroup STD800480_Private_Functions
  * @{
  */

/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	/*��ʾ��ʾ����*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
  /* ѡ��LCD��һ�� */
  LCD_SelectLayer(0);
	
	/* ��������ʾȫ�� */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*ѡ������*/
	LCD_SetFont(&LCD_DEFAULT_FONT);

	LCD_DisplayStringLine(1,(uint8_t* )"BH 5.0 inch LCD para:");
	LCD_DisplayStringLine(2,(uint8_t* )"Image resolution:800x480 px");
	LCD_DisplayStringLine(3,(uint8_t* )"Touch pad:5 point touch supported");
	LCD_DisplayStringLine(4,(uint8_t* )"Use STM32-LTDC directed driver,");
	LCD_DisplayStringLine(5,(uint8_t* )"no need extern driver,RGB888,24bits data bus");
	LCD_DisplayStringLine(6,(uint8_t* )"Touch pad use IIC to communicate");
	//while(1)
	{		
	testCNT++;
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(7);
	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine(7,(uint8_t* )dispBuff);


	/* ��ֱ�� */
	LCD_SetTextColor(LCD_COLOR_BLUE);

	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw line:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_DrawLine(50,250,750,250);  
	LCD_DrawLine(50,300,750,300);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_DrawLine(300,250,400,400);  
	LCD_DrawLine(600,250,600,400);

//	Delay(0xFFFFFF);
  HAL_Delay(3000);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw Rect:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_DrawRect(200,250,200,100);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_DrawRect(350,250,200,50);

	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_DrawRect(200,350,50,200);

//	Delay(0xFFFFFF);
  HAL_Delay(3000);


	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw Full Rect:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_FillRect(200,250,200,100);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillRect(350,250,200,50);

	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_FillRect(200,350,50,200);

//	Delay(0xFFFFFF);
  HAL_Delay(3000);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

	/* ��Բ */
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw circle:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
	LCD_DrawCircle(200,350,50);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
	LCD_DrawCircle(350,350,75);

//	Delay(0xFFFFFF);
  HAL_Delay(3000);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


	/*���Բ*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Draw full circle:");

	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
	LCD_FillCircle(300,350,50);

	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillCircle(450,350,75);

//	Delay(0xFFFFFF);
  HAL_Delay(3000);

	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);

	LCD_ClearLine(8);
	
	/*͸��Ч�� ǰ�������*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(8);
	LCD_DisplayStringLine(8,(uint8_t* )"Transparency effect:");
	
	/*����ǰ���㲻͸����*/
	LCD_SetTransparency(1, 128);

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(1);
	
	/* ��������ʾȫ�� */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*��ǰ����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
	LCD_FillCircle(400,350,75);
	
//	Delay(0xFFFFFF);
  HAL_Delay(3000);
	
	/*͸��Ч�� ���������*/

	/* ѡ��LCD������ */
	LCD_SelectLayer(0);	
	LCD_Clear(LCD_COLOR_BLACK);		
	/*���ñ����㲻͸��*/
	LCD_SetTransparency(1, 0);
	

	/*�ڱ�����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	LCD_FillCircle(450,350,75);
	
	/*�ڱ�����һ����ɫԲ*/
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	LCD_FillCircle(350,350,75);
	
//	Delay(0xFFFFFF);
  HAL_Delay(3000);
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	LCD_FillRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	}

}

void LCD_Init(void)
{

    /* ��ʼ������ */
    LCD_SetFont(&LCD_DEFAULT_FONT);
}

/**
  * @brief  ��ȡLCD��ǰ��Ч��X��Ĵ�С
  * @retval X��Ĵ�С
  */
uint32_t LCD_GetXSize(void)
{
  return hltdc.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  ��ȡLCD��ǰ��Ч��Y��Ĵ�С
  * @retval Y��Ĵ�С
  */
uint32_t LCD_GetYSize(void)
{
  return hltdc.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  ����LCD��ǰ��Ч��Y��Ĵ�С
  * @param  imageWidthPixels��ͼ�������ظ���
  * @retval ��
  */
void LCD_SetXSize(uint32_t imageWidthPixels)
{
  hltdc.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  ����LCD��ǰ��Ч��Y��Ĵ�С
  * @param  imageHeightPixels��ͼ��߶����ظ���
  * @retval None
  */
void LCD_SetYSize(uint32_t imageHeightPixels)
{
  hltdc.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  ��ʼ��LCD�� 
  * @param  LayerIndex:  ǰ����(��1)���߱�����(��0)
  * @param  FB_Address:  ÿһ���Դ���׵�ַ
  * @param  PixelFormat: ������ظ�ʽ
  * @retval ��
  */
void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat)
{     
  LTDC_LayerCfgTypeDef  layer_cfg;

  /* ���ʼ�� */
  layer_cfg.WindowX0 = 0;				//������ʼλ��X����
  layer_cfg.WindowX1 = LCD_GetXSize();	//���ڽ���λ��X����
  layer_cfg.WindowY0 = 0;				//������ʼλ��Y����
  layer_cfg.WindowY1 = LCD_GetYSize();  //���ڽ���λ��Y����
  layer_cfg.PixelFormat = PixelFormat;	//���ظ�ʽ
  layer_cfg.FBStartAdress = FB_Address; //���Դ��׵�ַ
  layer_cfg.Alpha = 255;				//���ڻ�ϵ�͸���ȳ�������Χ��0��255��0Ϊ��ȫ͸��
  layer_cfg.Alpha0 = 0;					//Ĭ��͸���ȳ�������Χ��0��255��0Ϊ��ȫ͸��
  layer_cfg.Backcolor.Blue = 0;			//�㱳����ɫ��ɫ����
  layer_cfg.Backcolor.Green = 0;		//�㱳����ɫ��ɫ����
  layer_cfg.Backcolor.Red = 0;			//�㱳����ɫ��ɫ����
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;//����ϵ��1
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;//����ϵ��2
  layer_cfg.ImageWidth = LCD_GetXSize();//����ͼ����
  layer_cfg.ImageHeight = LCD_GetYSize();//����ͼ��߶�
  
  HAL_LTDC_ConfigLayer(&hltdc, &layer_cfg, LayerIndex); //����ѡ�еĲ����

  DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;//���ò��������ɫ
  DrawProp[LayerIndex].pFont     = &LCD_DEFAULT_FONT;//���ò����������
  DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK; //���ò�����屳����ɫ
  
  __HAL_LTDC_RELOAD_CONFIG(&hltdc);//���ز�����ò���
}
/**
  * @brief  ѡ��LCD��
  * @param  LayerIndex: ǰ����(��1)���߱�����(��0)
  * @retval ��
  */
void LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
} 

/**
  * @brief  ����LCD��Ŀ��ӻ�
  * @param  LayerIndex: ǰ����(��1)���߱�����(��0)
  * @param  State: ���ܻ���ʹ��
  * @retval ��
  */
void LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&hltdc, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex);
  }
  __HAL_LTDC_RELOAD_CONFIG(&hltdc);
} 

/**
  * @brief  ����LCD��͸���ȳ���
  * @param  LayerIndex: ǰ����(��1)���߱�����(��0)
  * @param  Transparency: ͸���ȣ���Χ��0��255��0Ϊ��ȫ͸��
  * @retval ��
  */
void LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{    
  HAL_LTDC_SetAlpha(&hltdc, Transparency, LayerIndex);
}

/**
  * @brief  ����LCD����֡���׵�ַ 
  * @param  LayerIndex: ǰ����(��1)���߱�����(��0)
  * @param  Address: LCD����֡���׵�ַ     
  * @retval ��
  */
void LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
  HAL_LTDC_SetAddress(&hltdc, Address, LayerIndex);
}

/**
  * @brief  ������ʾ����
  * @param  LayerIndex: ǰ����(��1)���߱�����(��0)
  * @param  Xpos: LCD 	X����ʼλ��
  * @param  Ypos: LCD 	Y����ʼλ��
  * @param  Width: LCD  ���ڴ�С
  * @param  Height: LCD ���ڴ�С  
  * @retval None
  */
void LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* �������ô��ڴ�С */
  HAL_LTDC_SetWindowSize(&hltdc, Width, Height, LayerIndex);
  
  /* �������ô��ڵ���ʼλ�� */
  HAL_LTDC_SetWindowPosition(&hltdc, Xpos, Ypos, LayerIndex); 
}


/**
  * @brief  ����LCD��ǰ��������ɫ
  * @param  Color: ������ɫ
  * @retval ��
  */
void LCD_SetTextColor(uint32_t Color)
{
  DrawProp[ActiveLayer].TextColor = Color;
}

/**
  * @brief  ��ȡLCD��ǰ��������ɫ
  * @retval ������ɫ
  */
uint32_t LCD_GetTextColor(void)
{
  return DrawProp[ActiveLayer].TextColor;
}

/**
  * @brief  ����LCD��ǰ������ֱ�����ɫ
  * @param  Color: ���ֱ�����ɫ
  * @retval ��
  */
void LCD_SetBackColor(uint32_t Color)
{
  DrawProp[ActiveLayer].BackColor = Color;
}

/**
  * @brief  ��ȡLCD��ǰ������ֱ�����ɫ
  * @retval ���ֱ�����ɫ
  */
uint32_t LCD_GetBackColor(void)
{
  return DrawProp[ActiveLayer].BackColor;
}

/**
 * @brief  ����LCD���ֵ���ɫ�ͱ�������ɫ
 * @param  TextColor: ָ��������ɫ
 * @param  BackColor: ָ��������ɫ
 * @retval ��
 */
void LCD_SetColors(uint32_t TextColor, uint32_t BackColor)
{
     LCD_SetTextColor (TextColor);
     LCD_SetBackColor (BackColor);
}
/**
  * @brief  ����LCD��ǰ����ʾ������
  * @param  fonts: ��������
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  DrawProp[ActiveLayer].pFont = fonts;
}

/**
  * @brief  ��ȡLCD��ǰ����ʾ������
  * @retval ��������
  */
sFONT *LCD_GetFont(void)
{
  return DrawProp[ActiveLayer].pFont;
}

/**
  * @brief  ��LCD������ֵ
  * @param  Xpos: X ������λ��
  * @param  Ypos: Y ������λ��
  * @retval RGB����ֵ
  */
uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  uint32_t ret = 0;
  
  if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret = *(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret  = (*(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+2) & 0x00FFFFFF);
	ret |= (*(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+1) & 0x00FFFFFF);
	ret |= (*(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
  }
  else if((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret = *(__IO uint16_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));    
  }
  else
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret = *(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));    
  }
  
  return ret;
}

/**
  * @brief  LCD��ǰ������
  * @param  Color: ������ɫ
  * @retval None
  */
void LCD_Clear(uint32_t Color)
{ 
  /* ���� */ 
  LL_FillBuffer(ActiveLayer, (uint32_t *)(hltdc.LayerCfg[ActiveLayer].FBStartAdress), LCD_GetXSize(), LCD_GetYSize(), 0, Color);
}

/**
  * @brief  ���һ��
  * @param  Line: ��
  * @retval None
  */
void LCD_ClearLine(uint32_t Line)
{
  uint32_t color_backup = DrawProp[ActiveLayer].TextColor;
  DrawProp[ActiveLayer].TextColor = DrawProp[ActiveLayer].BackColor;
  
  /* ��һ��������ɫһ�µľ��������� */
  LCD_FillRect(0, (Line * DrawProp[ActiveLayer].pFont->Height), LCD_GetXSize(), DrawProp[ActiveLayer].pFont->Height);
  
  DrawProp[ActiveLayer].TextColor = color_backup;
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);  
}

/**
  * @brief  ��ʾһ���ַ�
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ����
  * @param  Ascii: �ַ� ascii ��,��Χ�� 0x20 ��0x7E ��
  * @retval ��
  */
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
    DrawChar(Xpos, Ypos, &DrawProp[ActiveLayer].pFont->table[(Ascii-' ') *\
    DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

/**
  * @brief  ��ʾ�ַ���
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ���� 
  * @param  Text: �ַ���ָ��
  * @param  Mode: ��ʾ���뷽ʽ��������CENTER_MODE��RIGHT_MODE��LEFT_MODE
  * @retval None
  */
void LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
  uint16_t ref_column = 1, i = 0;
  uint32_t size = 0, xsize = 0; 
  uint8_t  *ptr = Text;
  
  /* ��ȡ�ַ�����С */
  while (*ptr++) size ++ ;
  
  /* ÿһ�п�����ʾ�ַ������� */
  xsize = (LCD_GetXSize()/DrawProp[ActiveLayer].pFont->Width);
  
  switch (Mode)
  {
  case CENTER_MODE:
    {
      ref_column = Xpos + ((xsize - size)* DrawProp[ActiveLayer].pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      ref_column = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      ref_column = - Xpos + ((xsize - size)*DrawProp[ActiveLayer].pFont->Width);
      break;
    }    
  default:
    {
      ref_column = Xpos;
      break;
    }
  }
  
  /*�����ʼ���Ƿ�����ʾ��Χ�� */
  if ((ref_column < 1) || (ref_column >= 0x8000))
  {
    ref_column = 1;
  }

  /* ʹ���ַ���ʾ������ʾÿһ���ַ�*/
  while ((*Text != 0) & (((LCD_GetXSize() - (i*DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF)\
			>= DrawProp[ActiveLayer].pFont->Width))
  {
    /* ��ʾһ���ַ� */
    LCD_DisplayChar(ref_column, Ypos, *Text);
    /* ���������С������һ��ƫ��λ�� */
    ref_column += DrawProp[ActiveLayer].pFont->Width;
    /* ָ��ָ����һ���ַ� */
    Text++;
    i++;
  }  
}

/**
  * @brief  ��ָ������ʾ�ַ���(���60��)
  * @param  Line: ��ʾ����
  * @param  ptr: �ַ���ָ��
  * @retval ��
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{  
  LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

/**
  * @brief  ����ˮƽ��
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ����
  * @param  Length: �ߵĳ���
  * @retval ��
  */
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t  Xaddress = 0;

  if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 3*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }
  else
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }	
  /* ������� */
  LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, Length, 1, 0, DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  ���ƴ�ֱ��
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ����
  * @param  Length: �ߵĳ���
  * @retval ��
  */
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t  Xaddress = 0;
  
  if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 3*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }
  else
  {
    Xaddress = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }	
  
  /* ������� */
  LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, 1, Length, (LCD_GetXSize() - 1), DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  ָ�����㻭һ����
  * @param  x1: ��һ��X������
  * @param  y1: ��һ��Y������
  * @param  x2: �ڶ���X������
  * @param  y2: �ڶ���Y������
  * @retval ��
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* ��x��ľ���ֵ */
  deltay = ABS(y2 - y1);        /* ��y��ľ���ֵ */
  x = x1;                       /* ��һ�����ص�x������ʼֵ */
  y = y1;                       /* ��һ�����ص�y������ʼֵ */
  
  if (x2 >= x1)                 /* x����ֵΪ���� */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* x����ֵΪ�ݼ� */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* y����ֵΪ���� */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* y����ֵΪ�ݼ� */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* ÿ�� y ����ֵ������һ��x����ֵ*/
  {
    xinc1 = 0;                  /* �����Ӵ��ڻ���ڷ�ĸʱ��Ҫ�ı� x */
    yinc2 = 0;                  /* ��ҪΪÿ�ε������� y */
    den = deltax;
    num = deltax / 2;
    num_add = deltay;
    num_pixels = deltax;         /* x��y���ֵ */
  }
  else                          /* ÿ�� x ����ֵ������һ��y����ֵ */
  {
    xinc2 = 0;                  /* ��ҪΪÿ�ε������� x */
    yinc1 = 0;                  /* �����Ӵ��ڻ���ڷ�ĸʱ��Ҫ�ı� y */
    den = deltay;
    num = deltay / 2;
    num_add = deltax;
    num_pixels = deltay;         /* y��x���ֵ */
  }
  
  for (curpixel = 0; curpixel <= num_pixels; curpixel++)
  {
    LCD_DrawPixel(x, y, DrawProp[ActiveLayer].TextColor);   /* ���Ƶ�ǰ���ص� */
    num += num_add;                            /* �ڷ����Ļ��������ӷ��� */
    if (num >= den)                           /* �����Ӵ��ڻ���ڷ�ĸ */
    {
      num -= den;                             /* �����µķ���ֵ */
      x += xinc1;                             /* xֵ���� */
      y += yinc1;                             /* yֵ���� */
    }
    x += xinc2;                               /* yֵ���� */
    y += yinc2;                               /* yֵ���� */
  }
}

/**
  * @brief  ����һ������
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  Width:  ���ο��  
  * @param  Height: ���θ߶�
  * @retval ��
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* ����ˮƽ�� */
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, (Ypos+ Height), Width);
  
  /* ���ƴ�ֱ�� */
  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine((Xpos + Width), Ypos, Height);
}

/**
  * @brief  ����һ��Բ��
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  Radius: Բ�İ뾶
  * @retval ��
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t   decision;    /* ���߱��� */ 
  uint32_t  current_x;   /* ��ǰx����ֵ */
  uint32_t  current_y;   /* ��ǰy����ֵ */
  
  decision = 3 - (Radius << 1);
  current_x = 0;
  current_y = Radius;
  
  while (current_x <= current_y)
  {
    LCD_DrawPixel((Xpos + current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos + current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos + current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos + current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);
    
    if (decision < 0)
    { 
      decision += (current_x << 2) + 6;
    }
    else
    {
      decision += ((current_x - current_y) << 2) + 10;
      current_y--;
    }
    current_x++;
  } 
}

/**
  * @brief  ����һ������
  * @param  Points: ָ��������ָ��
  * @param  PointCount: ����
  * @retval ��
  */
void LCD_DrawPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t x = 0, y = 0;
  
  if(PointCount < 2)
  {
    return;
  }
  
  LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
  
  while(--PointCount)
  {
    x = Points->X;
    y = Points->Y;
    Points++;
    LCD_DrawLine(x, y, Points->X, Points->Y);
  }
}

/**
  * @brief  ����һ����Բ
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  XRadius: ��ԲX��뾶
  * @param  YRadius: ��ԲY��뾶
  * @retval ��
  */
void LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  k = (float)(rad2/rad1);  
  
  do { 
    LCD_DrawPixel((Xpos-(uint16_t)(x/k)), (Ypos+y), DrawProp[ActiveLayer].TextColor);
    LCD_DrawPixel((Xpos+(uint16_t)(x/k)), (Ypos+y), DrawProp[ActiveLayer].TextColor);
    LCD_DrawPixel((Xpos+(uint16_t)(x/k)), (Ypos-y), DrawProp[ActiveLayer].TextColor);
    LCD_DrawPixel((Xpos-(uint16_t)(x/k)), (Ypos-y), DrawProp[ActiveLayer].TextColor);      
    
    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;     
  }
  while (y <= 0);
}

/**
  * @brief  ����һ����
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  RGB_Code: ������ɫֵ
  * @retval ��
  */
void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{

  if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    *(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) = RGB_Code;
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    *(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+2) = 0xFF&(RGB_Code>>16);
	*(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+1) = 0xFF&(RGB_Code>>8);
	*(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))) = 0xFF&RGB_Code;
  }
  else if((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    *(__IO uint16_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos))) = (uint16_t)RGB_Code;   
  }
  else
  {
    *(__IO uint16_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos))) = (uint16_t)RGB_Code;   
  }

}

/**
  * @brief  ����һ�����ڲ�flash���ص�ARGB888(32 bits per pixel)��ʽ��ͼƬ
  * @param  Xpos: Bmp��Һ����X ������
  * @param  Ypos: Bmp��Һ����Y ������
  * @param  pbmp: ָ��ָ������ڲ�flash��BmpͼƬ���׵�ַ
  * @retval ��
  */
void LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
  uint32_t address;
  uint32_t input_color_mode = 0;
  
  /* ��ȡλͼ���ݵĻ���ַ */
  index = *(__IO uint16_t *) (pbmp + 10);
  index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;
  
  /* ��ȡλͼ��� */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;
  
  /* ��ȡλͼ�߶� */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16; 
  
  /* ��ȡ���ظ��� */
  bit_pixel = *(uint16_t *) (pbmp + 28);   
  
  /* �趨��ַ */
  address = hltdc.LayerCfg[ActiveLayer].FBStartAdress + (((LCD_GetXSize()*Ypos) + Xpos)*(4));
  
  /*�жϲ��������ظ�ʽ */    
  if ((bit_pixel/8) == 4)
  {
    input_color_mode = CM_ARGB8888;
  }
  else if ((bit_pixel/8) == 2)
  {
    input_color_mode = CM_RGB565;   
  }
  else 
  {
    input_color_mode = CM_RGB888;
  }
  
  /* �ƹ�λͼ��֡ͷ */
  pbmp += (index + (width * (height - 1) * (bit_pixel/8)));  
  
  /* ��ͼƬת��Ϊ ARGB8888 ���ظ�ʽ */
  for(index=0; index < height; index++)
  {
    /* ���ظ�ʽת�� */
    LL_ConvertLineToARGB8888((uint32_t *)pbmp, (uint32_t *)address, width, input_color_mode);
    
    /* ����Դ��Ŀ�껺���� */
    address+=  (LCD_GetXSize()*4);
    pbmp -= width*(bit_pixel/8);
  } 
}

/**
  * @brief  ���һ��ʵ�ľ���
  * @param  Xpos: X����ֵ
  * @param  Ypos: Y����ֵ
  * @param  Width:  ���ο�� 
  * @param  Height: ���θ߶�
  * @retval ��
  */
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint32_t  x_address = 0;
  
  /* ����������ɫ */
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
  
  /* ���þ��ο�ʼ��ַ */
    if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    x_address = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    x_address = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 3*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    x_address = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }
  else
  {
    x_address = (hltdc.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);
  }	
  /* ������ */
  LL_FillBuffer(ActiveLayer, (uint32_t *)x_address, Width, Height, (LCD_GetXSize() - Width), DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  ���һ��ʵ��Բ
  * @param  Xpos: X����ֵ
  * @param  Ypos: Y����ֵ
  * @param  Radius: Բ�İ뾶
  * @retval ��
  */
void LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t   decision;    /* ���߱��� */ 
  uint32_t  current_x;   /* ��ǰx����ֵ */
  uint32_t  current_y;   /* ��ǰy����ֵ */
  
  decision = 3 - (Radius << 1);
  
  current_x = 0;
  current_y = Radius;
  
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
  
  while (current_x <= current_y)
  {
    if(current_y > 0) 
    {
      LCD_DrawHLine(Xpos - current_y, Ypos + current_x, 2*current_y);
      LCD_DrawHLine(Xpos - current_y, Ypos - current_x, 2*current_y);
    }
    
    if(current_x > 0) 
    {
      LCD_DrawHLine(Xpos - current_x, Ypos - current_y, 2*current_x);
      LCD_DrawHLine(Xpos - current_x, Ypos + current_y, 2*current_x);
    }
    if (decision < 0)
    { 
      decision += (current_x << 2) + 6;
    }
    else
    {
      decision += ((current_x - current_y) << 2) + 10;
      current_y--;
    }
    current_x++;
  }
  
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
  LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  ���ƶ����
  * @param  Points: ָ��������ָ��
  * @param  PointCount: ����
  * @retval ��
  */
void LCD_FillPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  image_left = 0, image_right = 0, image_top = 0, image_bottom = 0;
  
  image_left = image_right = Points->X;
  image_top= image_bottom = Points->Y;
  
  for(counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if(pixelX < image_left)
    {
      image_left = pixelX;
    }
    if(pixelX > image_right)
    {
      image_right = pixelX;
    }
    
    pixelY = POLY_Y(counter);
    if(pixelY < image_top)
    { 
      image_top = pixelY;
    }
    if(pixelY > image_bottom)
    {
      image_bottom = pixelY;
    }
  }  
  
  if(PointCount < 2)
  {
    return;
  }
  
  X_center = (image_left + image_right)/2;
  Y_center = (image_bottom + image_top)/2;
  
  X_first = Points->X;
  Y_first = Points->Y;
  
  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;    
    
    FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    FillTriangle(X_center, X2, X, Y_center, Y2, Y);   
  }
  
  FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
  FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
  FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);   
}

/**
  * @brief  ���һ��ʵ����Բ
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  XRadius: ��ԲX��뾶
  * @param  YRadius: ��ԲY��뾶
  * @retval ��
  */
void LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  k = (float)(rad2/rad1);
  
  do 
  {       
    LCD_DrawHLine((Xpos-(uint16_t)(x/k)), (Ypos+y), (2*(uint16_t)(x/k) + 1));
    LCD_DrawHLine((Xpos-(uint16_t)(x/k)), (Ypos-y), (2*(uint16_t)(x/k) + 1));
    
    e2 = err;
    if (e2 <= x) 
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}

/**
  * @brief  ʹ����ʾ
  * @retval ��
  */
void LCD_DisplayOn(void)
{
  /* ����ʾ */
  __HAL_LTDC_ENABLE(&hltdc);
  HAL_GPIO_WritePin(LTDC_DISP_GPIO_Port, LTDC_DISP_Pin, GPIO_PIN_SET);/* LCD_DISPʹ��*/
  HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_SET);  /* ������*/
}

/**
  * @brief  ������ʾ
  * @retval ��
  */
void LCD_DisplayOff(void)
{
  /* ����ʾ */
  __HAL_LTDC_DISABLE(&hltdc);
  HAL_GPIO_WritePin(LTDC_DISP_GPIO_Port, LTDC_DISP_Pin, GPIO_PIN_RESET); /* LCD_DISP����*/
  HAL_GPIO_WritePin(LTDC_BL_GPIO_Port, LTDC_BL_Pin, GPIO_PIN_RESET);/*�ر���*/
}

/**
  * @brief  ʱ������
  * @param  hltdc: LTDC���
  * @param  Params
  * @retval ��
  */
void LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
  static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

  /* LCDʱ������ */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
  /* LTDCʱ��Ƶ�� = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
  periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  periph_clk_init_struct.PLLSAI.PLLSAIN = 400;
  periph_clk_init_struct.PLLSAI.PLLSAIR = 4;
  periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
}


/*******************************************************************************
                            �ֲ�����
*******************************************************************************/

/**
  * @brief  ��ʾһ���ַ�
  * @param  Xpos: ��ʾ�ַ�����λ��
  * @param  Ypos: ����ʼλ��
  * @param  c: ָ���������ݵ�ָ��
  * @retval ��
  */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t  offset;
  uint8_t  *pchar;
  uint32_t line;
  
  height = DrawProp[ActiveLayer].pFont->Height;//��ȡ����ʹ������߶�
  width  = DrawProp[ActiveLayer].pFont->Width; //��ȡ����ʹ��������
  
  offset =  8 *((width + 7)/8) -  width ;//�����ַ���ÿһ�����ص�ƫ��ֵ��ʵ�ʴ洢��С-������
  
  for(i = 0; i < height; i++)//��������߶Ȼ��
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);//�����ַ���ÿһ�����ص�ƫ�Ƶ�ַ
    
    switch(((width + 7)/8))//��������������ȡ��ͬ�����ʵ������ֵ
    {
      
    case 1:
      line =  pchar[0];      //��ȡ������С��8���ַ�������ֵ
      break;
      
    case 2:
      line =  (pchar[0]<< 8) | pchar[1]; //��ȡ�����ȴ���8С��16���ַ�������ֵ     
      break;
      
    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2]; //��ȡ�����ȴ���16С��24���ַ�������ֵ     
      break;
    } 
    
    for (j = 0; j < width; j++)//���������Ȼ��
    {
      if(line & (1 << (width- j + offset- 1))) //����ÿһ�е�����ֵ��ƫ��λ�ð��յ�ǰ������ɫ���л��
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
      }
      else//�����һ��û�������������ձ�����ɫ���
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
      } 
    }
    Ypos++;
  }
}

/**
  * @brief  ��������Σ��������㣩
  * @param  x1: ��һ���X����ֵ
  * @param  y1: ��һ���Y����ֵ
  * @param  x2: �ڶ����X����ֵ
  * @param  y2: �ڶ����Y����ֵ
  * @param  x3: �������X����ֵ
  * @param  y3: �������Y����ֵ
  * @retval ��
  */
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{ 
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0,
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* ��x��ľ���ֵ */
  deltay = ABS(y2 - y1);        /* ��y��ľ���ֵ */
  x = x1;                       /* ��һ�����ص�x������ʼֵ */
  y = y1;                       /* ��һ�����ص�y������ʼֵ */
  
  if (x2 >= x1)                 /* x����ֵΪ����*/
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* x����ֵΪ�ݼ� */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* y����ֵΪ����*/
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* y����ֵΪ�ݼ� */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* ÿ�� y ����ֵ������һ��x����ֵ*/
  {
    xinc1 = 0;                  /* �����Ӵ��ڻ���ڷ�ĸʱ��Ҫ�ı� x */
    yinc2 = 0;                  /* ��ҪΪÿ�ε������� y */
    den = deltax;
    num = deltax / 2;
    num_add = deltay;
    num_pixels = deltax;         /* x��y���ֵ */
  }
  else                          /* ÿ�� x ����ֵ������һ��y����ֵ */
  {
    xinc2 = 0;                  /* ��ҪΪÿ�ε������� x */
    yinc1 = 0;                  /* �����Ӵ��ڻ���ڷ�ĸʱ��Ҫ�ı� y */
    den = deltay;
    num = deltay / 2;
    num_add = deltax;
    num_pixels = deltay;         /* y��x���ֵ */
  }
  
  for (curpixel = 0; curpixel <= num_pixels; curpixel++)
  {
    LCD_DrawLine(x, y, x3, y3);
    
    num += num_add;              /* �ڷ����Ļ��������ӷ��� */
    if (num >= den)             /* �жϷ����Ƿ���ڻ���ڷ�ĸ */
    {
      num -= den;               /* �����µķ���ֵ */
      x += xinc1;               /* xֵ���� */
      y += yinc1;               /* yֵ���� */
    }
    x += xinc2;                 /* xֵ���� */
    y += yinc2;                 /* yֵ���� */
  } 
}

/**
  * @brief  ���һ��������
  * @param  LayerIndex: ��ǰ��
  * @param  pDst: ָ��Ŀ�껺����ָ��
  * @param  xSize: ���������
  * @param  ySize: �������߶�
  * @param  OffLine: ƫ����
  * @param  ColorIndex: ��ǰ��ɫ
  * @retval None
  */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex) 
{

  hdma2d.Init.Mode         = DMA2D_R2M;
  if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  { 
    hdma2d.Init.ColorMode    = DMA2D_RGB565;
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  { 
    hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  { 
    hdma2d.Init.ColorMode    = DMA2D_RGB888;
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555)
  { 
    hdma2d.Init.ColorMode    = DMA2D_ARGB1555;
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444)
  { 
    hdma2d.Init.ColorMode    = DMA2D_ARGB4444;
  }
  hdma2d.Init.OutputOffset = OffLine;      
  
  hdma2d.Instance = DMA2D;
  
  /* DMA2D ��ʼ�� */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, LayerIndex) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* DMA��ѯ���� */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
  } 
  
//  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
//  {
//    if(HAL_DMA2D_ConfigLayer(&hdma2d, LayerIndex) == HAL_OK) 
//    {
//      if (HAL_DMA2D_Start_IT(&hdma2d, ColorIndex, (uint32_t)pDst, xSize, ySize) != HAL_OK)
//      {
////        /* DMA��ѯ���� */  
////        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
//      }
//    }
//  } 
  
}

/**
  * @brief  ת��һ��ΪARGB8888���ظ�ʽ
  * @param  pSrc: ָ��Դ��������ָ��
  * @param  pDst: �����ɫ
  * @param  xSize: ���������
  * @param  ColorMode: ������ɫģʽ   
  * @retval ��
  */
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{    
  /* ����DMA2Dģʽ,��ɫģʽ�����ƫ�� */
  hdma2d.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = ColorMode;
  hdma2d.LayerCfg[1].InputOffset = 0;
  
  hdma2d.Instance = DMA2D; 
  
  /* DMA2D ��ʼ�� */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* DMA��ѯ����*/  
        HAL_DMA2D_PollForTransfer(&hdma2d, 10);
      }
    }
  }

//  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
//  {
//    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
//    {
//      if (HAL_DMA2D_Start_IT(&hdma2d, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) != HAL_OK)
//      {
////        /* DMA��ѯ����*/  
////        HAL_DMA2D_PollForTransfer(&hdma2d, 10);
//      }
//    }
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

