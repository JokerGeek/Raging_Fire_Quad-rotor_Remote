///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ĩ�μ��˱���������ɳ����������˼�Ļ��ͨ���������ʶ�������ø��ܶ������������������Ŷӣ�һ�����о����ᣡ
//��������������˼���û�У���˳������ֲ�͵��Զ����Լ�������ʱ���������ģ�Ҳ�������ʡ�
//������EEPW�����̳�Խ��Խ��������ڷ��׳��Լ��Ĵ��룬���Ǹ��õ�����
//��Ϊֻ�в��Ͻ���������ȡ�ø���Ľ�����������Ƥ�������������~
//    
//�����������Ҿ������Լ���ֲ���ԵĴ��뿪Դ�����������ѧϰ��ϣ�������һ�����������������������ǡ�
//��Ϊ���ǵ��׼�����һ���ģ�Ӳ������һ�µ��ˣ��Ǿ�ֻ��Ҫ�о������ϵ��㷨�����ˡ����������᷽��Ķ࣡
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
#include "rc.h"
#include "nrf24l01.h"
#include "uart1.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define RX_DR			6		//�жϱ�־
#define TX_DS			5
#define MAX_RT		4

RC_GETDATA Rc_Get;//���յ���RC����,1000~2000
u8 NRF_SEND_RC_FLAG = 0;

void NRF_SendData_AP(u8 *addr)
{
	NRF_TxPacket_AP(addr,32);
	//NRF_TxPacket(addr,32);
}

void Nrf_Check_Event(void)	//��2401���յ������ݷ��ش���
{
	u8 sta = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<RX_DR))//�����ж�
	{
		u8 rx_len = NRF_Read_Reg(R_RX_PL_WID);	//2401�Ĵ�����ֵΪ0x60
		if(rx_len<33)
		{
			NRF_Read_Buf(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);// read receive payload from RX_FIFO buffer
			Uart1_Send_Buf(NRF24L01_RXDATA,rx_len);
		}
		else 
		{
			NRF_Write_Reg(FLUSH_RX,0xff);//��ջ�����
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<TX_DS))//�����ж�
	{
		//PC_Debug_Show(1,2);
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<MAX_RT))//Ӧ���ǻ�������ж�
	{
		static u16 max_cnt = 0;
		max_cnt++;
		PC_Debug_Show(7,max_cnt);
		
		if(sta & 0x01)	//TX FIFO FULL
		{
			NRF_Write_Reg(FLUSH_TX,0xff);
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, sta);//??nrf??????
}
void NRF_Send_RC(void)
{
	NRF24L01_TXDATA_RC[0] = 0x8A;	//Դ������Ϊ8A	ң��ָ��
	NRF24L01_TXDATA_RC[1] = 0x8A;	//Դ������Ϊ8A	ң��ָ��
	NRF24L01_TXDATA_RC[2] = 0x1C;
	NRF24L01_TXDATA_RC[3] = BYTE1(Rc_Get.THROTTLE);
	NRF24L01_TXDATA_RC[4] = BYTE0(Rc_Get.THROTTLE);

	NRF24L01_TXDATA_RC[5] = BYTE1(Rc_Get.YAW);
	NRF24L01_TXDATA_RC[6] = BYTE0(Rc_Get.YAW);
	NRF24L01_TXDATA_RC[7] = BYTE1(Rc_Get.ROLL);
	NRF24L01_TXDATA_RC[8] = BYTE0(Rc_Get.ROLL);
	NRF24L01_TXDATA_RC[9] = BYTE1(Rc_Get.PITCH);
	NRF24L01_TXDATA_RC[10] = BYTE0(Rc_Get.PITCH);
	
	NRF24L01_TXDATA_RC[11] = BYTE1(Rc_Get.AUX1);
	NRF24L01_TXDATA_RC[12] = BYTE0(Rc_Get.AUX1);
	NRF24L01_TXDATA_RC[13] = BYTE1(Rc_Get.AUX2);
	NRF24L01_TXDATA_RC[14] = BYTE0(Rc_Get.AUX2);
	NRF24L01_TXDATA_RC[15] = BYTE1(Rc_Get.AUX3);
	NRF24L01_TXDATA_RC[16] = BYTE0(Rc_Get.AUX3);
	NRF24L01_TXDATA_RC[17] = BYTE1(Rc_Get.AUX4);
	NRF24L01_TXDATA_RC[18] = BYTE0(Rc_Get.AUX4);
	NRF24L01_TXDATA_RC[19] = BYTE1(Rc_Get.AUX5);
	NRF24L01_TXDATA_RC[20] = BYTE0(Rc_Get.AUX5);
	
	u8 sum = 0;
	for(u8 i=0;i<31;i++)
		sum += NRF24L01_TXDATA_RC[i];
	NRF24L01_TXDATA_RC[31] = sum;
	NRF_SEND_RC_FLAG = 1;
}


