/**
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ĩ�μ��˱���������ɳ����������˼�Ļ��ͨ���������ʶ�������ø��ܶ������������������Ŷӣ�һ�����о����ᣡ
//�������������˼���û�У���˳������ֲ�͵��Զ����Լ�������ʱ���������ģ�Ҳ�������ʡ�
//������EEPW�����̳�Խ��Խ��������ڷ��׳��Լ��Ĵ��룬���Ǹ��õ�����
//��Ϊֻ�в��Ͻ���������ȡ�ø���Ľ�����������Ƥ�������������~
//    
//�����������Ҿ������Լ���ֲ���ԵĴ��뿪Դ�����������ѧϰ��ϣ�������һ�����������������������ǡ�
//��Ϊ���ǵ��׼�����һ���ģ�Ӳ������һ�µ��ˣ��Ǿ�ֻ��Ҫ�о�����ϵ��㷨�����ˡ����������᷽��Ķ࣡
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//����DIY����飺http://www.eepw.com.cn/event/action/QuadCopter_DIY/
//
//������̳��http://forum.eepw.com.cn/forum/368/1
//
//�ҵ�����DIY��������http://forum.eepw.com.cn/thread/248747/1
//
//�Ա����̣�http://item.taobao.com/item.htm?spm=a230r.1.14.23.sYD4gY&id=35605621244
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	BY:������ɣ�2014,4,30
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp.h"
#include "uart1.h"
#include "rc.h"

#include "DMA_ADC.h"
#include "led.h"
#include "LCD5110.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
u8 aa;
u16 dianya0 = 0,dianya1 = 0;
u16 qh=0,zy=0,ph=0;
unsigned char play;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
/*=====================================================================================================*/
/*=====================================================================================================*/
void USART1_IRQHandler(void)  //�����жϺ���
{
	Uart1_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
////////////void TIM2_IRQHandler(void)		//���ڲ�׽PPM,ʹ��PA0
////////////{
//////////////	static u8 	RC_Ch = 0;
//////////////	static u16 	RC_Value[10];
////////////	
////////////	if(TIM2->SR & TIM_IT_CC1)//if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
////////////	{
////////////		u16 CC1_Value_Now = TIM2->CCR1;//��ccr,��cc1if��־
////////////		TIM2->CNT = 0;				//��cnt
////////////		//���ﲻ�����־λ�����޷��������У�����
////////////			
//////////////			if(CC1_Value_Now>3000)	//�ж�ͬ���ź�	//�ⲿ���ǲ�ץppm�ģ�����������
//////////////				RC_Ch = 0;
//////////////			RC_Value[RC_Ch] = CC1_Value_Now;
//////////////			RC_Ch++;
//////////////			
//////////////			if(RC_Ch==1)	//	��ʹ��ppm���˲������Σ�����ad�ɼ�ң���źź���
//////////////			{
//////////////				Rc_Get.THROTTLE = RC_Value[3];
//////////////				Rc_Get.YAW = RC_Value[4];
//////////////				Rc_Get.ROLL = RC_Value[1];
//////////////				Rc_Get.PITCH = RC_Value[2];
//////////////				Rc_Get.AUX1 = RC_Value[5];
//////////////				Rc_Get.AUX2 = RC_Value[6];
//////////////				Rc_Get.AUX3 = RC_Value[7];
//////////////				Rc_Get.AUX4 = RC_Value[8];
//////////////				Rc_Get.AUX5 = RC_Value[9];
////////////////				//Uart1_Send_RCdata();
//////////////				NRF_Send_RC();
//////////////			}
////////////	}	
////////////}
/*=====================================================================================================*/
/*=====================================================================================================*/

void TIM3_IRQHandler(void)		//10ms�ж�һ��
{	
	static u8 ms20 = 0,ms500 = 0;				//�жϴ���������
	if(TIM3->SR & TIM_IT_Update)		//if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{     
		TIM3->SR = ~TIM_FLAG_Update;  //TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);   //����жϱ�־
		if(!SYS_INIT_OK)		
			return;
		//ÿ���ж϶�ִ��,10ms
		ms20++;

		if(ms20==2)	  //ÿ2���ж�ִ��һ��,20ms	//�ô�����50HZң���ź�
		{
			ms20=0;	
			ms500++;
			if(ms500==25)	//2HZ
			{
				ms500=0;
				LEDALL_OFF
				
				play = 1;	 //��Ļ��ʾ
				if(play)
				{
					play = 0;
					PlayDY();
//					switch (aa%2)
//					{
//						case 0:	bl0;	break;
//						case 1:	bl1;	break;
//					}
				}
				
				if(dianya1 < 37 && dianya1 >20)		//ң�ص�ѹ
				{
					LED1_ON;
					LED2_ON;	//��
				}
				if(dianya0 < 35 && dianya0 > 20)	//�����ѹ��2200��3.75����	//Ϊ�˱�����������Խ�35�ʵ�������36
				{
					LED1_ON;
					LED3_ON;	//��
				}				
			}
		
			Rc_Get.THROTTLE = (u16)(0.25*AD_Value[0][4])+1000;	
			Rc_Get.YAW = (u16)(0.74*AD_Value[0][5]);		//ƫ����
			Rc_Get.ROLL = 3000 - (u16)(0.73*AD_Value[0][7])-11;	//�����
			Rc_Get.PITCH =(u16)(0.732*AD_Value[0][6]);//������
			
			Rc_Get.AUX1 = (u16)(AD_Value[0][0]/80); //����΢��
			Rc_Get.AUX2 = (u16)(AD_Value[0][1]/80);	//��ת΢��
			Rc_Get.AUX3 = (u16)(AD_Value[0][2]/80); //ǰ��΢��
			Rc_Get.AUX4 = (u16)(AD_Value[0][3]*6.6/410);	//��ѹֵ��ʮ��
					dianya1 = (u16)Rc_Get.AUX4;
			Rc_Get.AUX5 = ((u16)(NRF24L01_RXDATA[19])) << 8 | NRF24L01_RXDATA[20];	
					dianya0 =	(u16)((Rc_Get.AUX5)*6.6/410);

			NRF_Send_RC();	//׼��Ҫ���͵�ң������
											//װ������NRF24L01_TXDATA_RC
											//ʹNRF_SEND_RC_FLAG=1
		}
		
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
// void EXTI1_IRQHandler(void)
// {
// 	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
// 	{
// 		EXTI_ClearITPendingBit(EXTI_Line1);
// 	}
// }
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
/*=====================================================================================================*/
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
