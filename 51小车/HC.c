#include <car.h>

float  S=0;
extern bit  flag;
unsigned int  measure_time;
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
	
 void Conut(void)
	{
	 measure_time=TH1*256+TL1;
	 TH1=0;
	 TL1=0;
	 S=(measure_time*1.87)/100;     //算出来是CM
	 if(flag==1||S>50||S<2)		    //超出测量
	 {
	  flag=0;
	 LED=1;
	 M_sensor=1;
	 }
   else
		 {
	 LED=0;
	 M_sensor=0;
		 }
	}
