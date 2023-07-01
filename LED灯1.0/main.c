#include <REGX52.H>
sbit LED0 = P1^0;
/**
  * @brief ���ڳ�ʼ��
  * @param  ��
  * @retval ��
  */
void Uart_Init()	//4800bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xC7;		//�趨��ʱ��ֵ
	TH1 = 0xFE;		//�趨��ʱ��ֵ
	ET1 = 1;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	
	//����������ɺ�ʹ����ж�
	ES=1;
	EA=1;
}

/**
  * @brief ���ڷ���һλ�ֽ�����
  * @param  Byte Ҫ���͵��ֽ�����
  * @retval ��
  */
void Uart_SendByte(unsigned char Byte)
{
	SBUF=Byte;   //�򻺴�����д������
	while(TI==0);   //�����жϱ�־λ �����⵽�˼Ĵ�����TIλ ���Ϊ0 �ͱ�ʾ����δ�������  ����ִ�б��������TIλ
	//���TIΪ1 �ͱ�ʾSUBF�������Ѿ����������  ����ִ��������佫TIλ����
	TI=0;
}
void main()
{
	Uart_Init();
	
	while(1)
	{
		Uart_SendByte('a');
	}
}
// �����жϺ���ģ��
void Uart_Routine() interrupt 4
{
	if(RI==1)   //������յ����� ������ɺ� �����ж� ����ǽ����ж�  
		//��������Ҳ������ж�
	{
		LED0 = ~LED0;  //��SUBF�������ж�ȡ����
		//Uart_SendByte(SBUF);
		RI=0;
	}
}


