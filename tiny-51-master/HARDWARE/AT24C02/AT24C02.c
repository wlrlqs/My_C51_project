#include"at24c02.h"

/*******************************************************************************
* ������         : Delay10us()
* ��������		   : ��ʱ10us
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Delay10us()
{
	unsigned char a,b;
	for(b=1;b>0;b--)
		for(a=2;a>0;a--);
}
/*******************************************************************************
* ������         : I2cStart()
* ��������		 : ��ʼ�źţ���SCLʱ���ź��ڸߵ�ƽ�ڼ�SDA�źŲ���һ���½���
* ����           : ��
* ���         	 : ��
* ��ע           : ��ʼ֮��SDA��SCL��Ϊ0
*******************************************************************************/
void I2cStart()
{
	SDA1=1;
	Delay10us();
	SCL1=1;
	Delay10us();//����ʱ����SDA����ʱ��>4.7us
	SDA1=0;
	Delay10us();//����ʱ����>4us
	SCL1=0;			
	Delay10us();		
}
/*******************************************************************************
* ������         : I2cStop()
* ��������		 : ��ֹ�źţ���SCLʱ���źŸߵ�ƽ�ڼ�SDA�źŲ���һ��������
* ����           : ��
* ���         	 : ��
* ��ע           : ����֮�󱣳�SDA��SCL��Ϊ1����ʾ���߿���
*******************************************************************************/

void I2cStop()
{
	SDA1=0;
	Delay10us();
	SCL1=1;
	Delay10us();//����ʱ�����4.7us
	SDA1=1;
	Delay10us();		
}
/*******************************************************************************
* ������         : I2cSendByte(unsigned char dat)
* ��������		 : ͨ��I2C����һ���ֽڡ���SCLʱ���źŸߵ�ƽ�ڼ䣬���ַ����ź�SDA�����ȶ�
* ����           : num
* ���         	 : 0��1�����ͳɹ�����1������ʧ�ܷ���0
* ��ע           : ������һ���ֽ�SCL=0,SDA1=1
*******************************************************************************/

unsigned char I2cSendByte(unsigned char dat)
{
	unsigned char a=0,b=0;//���255��һ����������Ϊ1us�������ʱ255us��		
	for(a=0;a<8;a++)//Ҫ����8λ�������λ��ʼ
	{
		SDA1=dat>>7;	 //��ʼ�ź�֮��SCL=0�����Կ���ֱ�Ӹı�SDA�ź�
		dat=dat<<1;
		Delay10us();
		SCL1=1;
		Delay10us();//����ʱ��>4.7us
		SCL1=0;
		Delay10us();//ʱ�����4us		
	}
	SDA1=1;
	Delay10us();
	SCL1=1;
	while(SDA1)//�ȴ�Ӧ��Ҳ���ǵȴ����豸��SDA����
	{
		b++;
		if(b>200)	 //�������2000usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
		{
			SCL1=0;
			Delay10us();
			return 0;
		}
	}
	SCL1=0;
	Delay10us();
 	return 1;		
}
/*******************************************************************************
* ������         : I2cReadByte()
* ��������		   : ʹ��I2c��ȡһ���ֽ�
* ����           : ��
* ���         	 : dat
* ��ע           : ������һ���ֽ�SCL=0,SDA1=1.
*******************************************************************************/

unsigned char I2cReadByte()
{
	unsigned char a=0,dat=0;
	SDA1=1;			//��ʼ�ͷ���һ���ֽ�֮��SCL����0
	Delay10us();
	for(a=0;a<8;a++)//����8���ֽ�
	{
		SCL1=1;
		Delay10us();
		dat<<=1;
		dat|=SDA1;
		Delay10us();
		SCL1=0;
		Delay10us();
	}
	return dat;		
}


/*******************************************************************************
* ������         : void At24c02Write(unsigned char addr,unsigned char dat)
* ��������		   : ��24c02��һ����ַд��һ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/

void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2cStart();
	I2cSendByte(0xa0);//����д������ַ
	I2cSendByte(addr);//����Ҫд���ڴ��ַ
	I2cSendByte(dat);	//��������
	I2cStop();
}
/*******************************************************************************
* ������         : unsigned char At24c02Read(unsigned char addr)
* ��������		   : ��ȡ24c02��һ����ַ��һ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	I2cStart();
	I2cSendByte(0xa0); //����д������ַ
	I2cSendByte(addr); //����Ҫ��ȡ�ĵ�ַ
	I2cStart();
	I2cSendByte(0xa1); //���Ͷ�������ַ
	num=I2cReadByte(); //��ȡ����
	I2cStop();
	return num;	
}
