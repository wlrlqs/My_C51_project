#include <REGX52.H>
#include <intrins.h>

sbit Trig= P2^1; //产生脉冲引脚
sbit Echo= P2^0; //回波引脚
float S=0;
unsigned int flag;
unsigned int measure_time;
char M_sensor; 
void  StartModule() 		         //启动超声波模块
  {
	  Trig=1;			                
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;
  }
	
/*void Conut(void)
	{
	 measure_time=TH2*256+TL2;
	 TH2=0;
	 TL2=0;
	 S=(measure_time*1.87)/100;     //算出来是CM
	 if(flag==1||S>50||S<2)		    //超出测量
	 {
	  flag=0;
	 M_sensor=1;
	 }
   else
		 {
	 M_sensor=0;
		 }
	}
*/
	