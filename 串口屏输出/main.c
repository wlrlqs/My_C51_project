#include <reg52.h>  
 
#define uchar unsigned char  //宏定义 把unsigned char简写为uchar
#define uint unsigned int    //       把unsigned int 简写为uint
	
uint  i,j; 			//定义全局变量,执行电机for循环转动时间需要用到

sbit A1 = P2^0;       //定义给步进电机四相连接的IO口
sbit B1 = P2^1;
sbit C1 = P2^2;
sbit D1 = P2^3;

#define Coil_A {A1=1;B1=0;C1=0;D1=0;}     //A相通电，其它相断电
#define Coil_B {A1=0;B1=1;C1=0;D1=0;}     //B相通电，其它相断电
#define Coil_C {A1=0;B1=0;C1=1;D1=0;}     //C相通电，其它相断电
#define Coil_D {A1=0;B1=0;C1=0;D1=1;}     //D相通电，其它相断电
#define  Stop  {A1=0;B1=0;C1=0;D1=0;}     //全部断电，停止

uchar Speed=2;  		//整体调速，数字越小速度越快，而力矩也越小 

void Delay(uint xms)	//延时函数 12.000MHz Delay(1)=1ms
{                     	//延时程序 用烧录软件计算出来的 在另外一章有写
   uchar i, j;
   while(xms--) 
  {
	i = 2;
	j = 239;
	do { while (--j); }
	while (--i);
  }
	
}

   void corotation(void)  //正转 子程序 A——D
  {         
     Coil_A 
		P1 = P2;		
     Delay(Speed);  
     Coil_B
		P1 = P2;	
     Delay(Speed);
     Coil_C
		P1 = P2;	
     Delay(Speed);
     Coil_D
		P1 = P2;	
     Delay(Speed);
  }
  
   void reversal(void)    //反转 子程序 D——A
  {
	 Coil_D    
P1 = P2;			
     Delay(Speed);  
     Coil_C
		P1 = P2;	
     Delay(Speed);
     Coil_B
		P1 = P2;	
     Delay(Speed);
     Coil_A
		P1 = P2;	
     Delay(Speed);
  }
  
  
   void main()        //主程序
  {
	   Delay(30);    //启动前先缓冲一个30ms
	  
	    while(1)     //不断循环
	  { 
         for(i=0;i<500;i++)    //正转1圈(大概位置，步进电机需要编码盘才能测得比较准一点)
	     corotation();         //调用正转子函数
  
	      Stop                 //停止
		  Delay(1000);         //延时1s
		  		  
	     for(j=0;j<500;j++)    //反转1圈(大概位置)
         reversal();           //调用反转子函数
		  	  
		  Stop                 //停止
		  Delay(1000);         //延时1s
	  }
	  
  }
