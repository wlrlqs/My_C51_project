#include <REG52.H>
#include"pic.c"
#include <intrins.h>
#define LCD_DATA P2
#define button_delay 150  //按键延时
#define button_acceleration 65  //按键加速度阈值
#define GAME_LOCATION 30

sbit change = P3^4;
sbit OK = P3^5;
sbit up = P3^2;
sbit down = P3^0;
sbit left = P3^1;
sbit right = P3^3;
sbit speaker=P3^6; 

sbit LCD_RS=P1^0;
sbit LCD_RW=P1^1;
sbit LCD_E=P1^2;
sbit LCD_CS2=P1^3;//P1^4;		//右屏选择（左右屏有时候相反）
sbit LCD_CS1=P1^4;//P1^3;		//左屏选择
sbit LCD_RST=P3^7;

unsigned int up_reg=button_delay;       //按键up累加器
unsigned int down_reg=button_delay;     //按键down累加器
unsigned int left_reg=button_delay;     //按键left累加器
unsigned int right_reg=button_delay;    //按键right累加器
unsigned int button_a_reg=button_delay; //按键button_a累加器
unsigned int button_b_reg=button_delay; //按键button_b累加器
unsigned int right_acceleration=0;		//按键right加速度寄存器
unsigned int left_acceleration=0;		//按键left加速度寄存器

unsigned int idata Box_Ram[19];//定义游戏点阵缓存10*16
unsigned char box_down_reg;//定义方块下落累加寄存器
unsigned char time0_reg;//定义定时器0累加寄存器
unsigned char next_mode;//定义下一个方块的类型
unsigned char next_shape;//定义下一个方块的形状
unsigned int destroy_row_num=0;//定义所消的行数
unsigned char speed_num=0;//定义游戏速度等级
unsigned char level_num;//定义游戏难度等级
bit game_over_flag;//游戏结束标志位置0表示游戏未结束
bit pause_game_flag;//游戏暂停标志位置0表示游戏未暂停

struct
{
	unsigned char mode;//类型
	unsigned char shape;//形状
	unsigned char x;//x坐标
	unsigned char y;//y坐标
	unsigned int box;//定义方块缓存
}s_box;	//定义方块结构体
//LCD检测忙状态函数
void LCD_check_busy()
{
	unsigned char temp;
	LCD_RS=0;
	LCD_RW=1;
	do
	{
		LCD_DATA=0xff;
		LCD_E=1;
		temp=LCD_DATA;
		LCD_E=0;
	}while((temp&0x80)==0x80);		
}
//写指令代码（cs为0选左屏，cs为1选右屏）
void LCD_W_code(unsigned char tpcode,bit cs)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_CS2=~cs;
	LCD_CS1=cs;
	LCD_DATA=tpcode;
	LCD_E=1;
	_nop_();
	LCD_E=0;
}
//写显示数据（cs为0选左屏，cs为1选右屏）
void LCD_W_data(unsigned char tpdata,bit cs)
{
	LCD_check_busy();
	LCD_RS=1;
	LCD_RW=0;
	LCD_CS2=~cs;
	LCD_CS1=cs;	
	LCD_DATA=tpdata;
	LCD_E=1;	
	_nop_();
	LCD_E=0;
}

//LCD初始化函数
void LCD_initialize()
{
	LCD_RST=0;
	_nop_();
	_nop_();
	LCD_RST=1;
	LCD_W_code(0x3f,0);		//开显示设置	
	LCD_W_code(0xc0,0);		//设置显示起始行为第一行	
	LCD_W_code(0xb8,0);		//页面地址设置	
	LCD_W_code(0x40,0);		//列地址设为0
	LCD_W_code(0x3f,1);
	LCD_W_code(0xc0,1);	
	LCD_W_code(0xb8,1);
	LCD_W_code(0x40,1);
}
//LCD清屏函数
void LCD_clear()
{
	unsigned char i,j;
	for(j=0;j<8;j++)
	{
		LCD_W_code(0xb8+j,0);
		LCD_W_code(0x40,0);
		LCD_W_code(0xb8+j,1);
		LCD_W_code(0x40,1);
		for(i=0;i<64;i++)
			{	
				LCD_W_data(0x00,0);	
				LCD_W_data(0x00,1);
			}
	}
}
//LCD显示字符串函数（word表示要显示的字符串,
 //length表示要显示的字符串宽度,
 //x表示首字符所在行数,
 //y表示首字符所在列数）
void LCD_display_word(unsigned char word[],
                      unsigned int length,
					  unsigned char x,
					  unsigned char y)
{
	unsigned char i;
	for(i=0;i<length;i++)
	{
		
		LCD_W_code(0xb8+x,0);
		LCD_W_code(0xb8+x,1);
		if(y+i<64)
		{
			LCD_W_code(0x40+y+i,0);	
			LCD_W_data(word[i],0);
		}
		else
		{
			LCD_W_code(y+i,1);	
			LCD_W_data(word[i],1);
		}
	}
}
//LCD画全屏函数
void LCD_full_draw(unsigned char word[])
{
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		LCD_W_code(0xb8+i,0);
		LCD_W_code(0x40,0);	
		for(j=0;j<64;j++)
		{
			LCD_W_data(word[i*128+j],0);
		}
		LCD_W_code(0xb8+i,1);
		LCD_W_code(0x40,1);	
		for(j=0;j<64;j++)
		{
			LCD_W_data(word[i*128+64+j],1);
		}			
	}
}
//LCD显示一个字节函数（
  //x表示x坐标,
  //y表示y坐标,
  //tpdata表示要显示的数据）
void LCD_display_byte(unsigned char x,
					  unsigned char y,
					  unsigned char tpdata)
{
	if(x<64)
	{
		LCD_W_code(0xb8+y,0);
		LCD_W_code(0x40+x,0);
		LCD_W_data(tpdata,0);	
	}
	else
	{
		LCD_W_code(0xb8+y,1);
		LCD_W_code(x,1);
		LCD_W_data(tpdata,1);	
	}
} 

void LCD_draw(unsigned char word[])
{
  unsigned char i,j;
  for(i=0;i<8;i++)
  {
    LCD_W_code(0xb8+i,1);
	LCD_W_code(0x40+20,1);
	for(j=0;j<44;j++)
	{
	  LCD_W_data(word[i*44+j],1);
	}
  }
}
//基本界面显示函数
void display_basic()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		LCD_display_byte(GAME_LOCATION,i,0xff);
		LCD_display_byte(GAME_LOCATION+41,i,0xff);
	}
}
//刷新游戏区域函数
void refurbish_display()
{
	unsigned char i,j,tpdata;
	for(i=0;i<8;i++)
	{
		for(j=0;j<10;j++)
		{
			tpdata=0x00;
			if(  (Box_Ram[2*i]>>(12-j))&0x0001==1  )
			{
				tpdata=0x0f;
			}
			if(  (Box_Ram[2*i+1]>>(12-j))&0x0001==1  )
			{
				tpdata|=0xf0;
			}
			LCD_display_byte(GAME_LOCATION+1+j*4,i,tpdata);
			LCD_display_byte(GAME_LOCATION+2+j*4,i,0xbb&tpdata);
			LCD_display_byte(GAME_LOCATION+3+j*4,i,0xdd&tpdata);
			LCD_display_byte(GAME_LOCATION+4+j*4,i,tpdata);
		}
	}
}
//基本按键程序(返回0表示没按键被按下，返回1表示down被按下，返回2表示up被按下，返回3表示button_a被按下,返回4表示left被按下,返回5表示right被按下)
//游戏中按键识别程序（有优先级，从高到低依次是button_a_reg>down>left>right>up）
unsigned char basic_button()
{
	unsigned char tpflag=0;
	if(OK==0)
	{
	    if(button_b_reg<button_delay*8)
		{
		  button_b_reg++;
		}
		else
		{
		  button_b_reg=0;
		  tpflag=6;
		}
	}
	else
	{
   	     button_b_reg=button_delay*8;
	}
	if(down==0)
	{
		if(down_reg<button_delay)//按键一直被按下时设置时间间隔触发
		{
			down_reg++;
		}
		else
		{
			down_reg=0;
			tpflag=1;//返回1表示down被按下
		}		
	}
	else
	{
		down_reg=button_delay;//释放按键时置按键缓存为button_delay，以便在下次按键时及时响应
	}
	if(up==0)
	{
		if(up_reg<button_delay)//按键一直被按下时设置时间间隔触发
		{
			up_reg++;
		}
		else
		{
			up_reg=0;
			tpflag=2;//返回2表示up被按下
		}		
	}
	else
	{
		up_reg=button_delay;//释放按键时置按键缓存为button_delay，以便在下次按键时及时响应
	}
	if(change==0)
	{
		if(button_a_reg<button_delay*8)//按键一直被按下时设置时间间隔触发
		{
			button_a_reg++;
		}
		else
		{
			button_a_reg=0;
			tpflag=3;//返回3表示button_a被按下
		}		
	}
	else
	{
		button_a_reg=button_delay*8;//释放按键时置按键缓存为button_delay，以便在下次按键时及时响应
	}
	if(left==0)
	{
		if(left_reg<(button_delay))//按键一直被按下时设置时间间隔触发
		{
			left_reg++;
		}
		else
		{
			left_reg=left_acceleration*button_acceleration;
			if(left_acceleration<2)left_acceleration++;
			tpflag=4;//返回4表示left被按下
		}		
	}
	else
	{
		left_acceleration=0;
		left_reg=button_delay;//释放按键时置按键缓存为button_delay，以便在下次按键时及时响应
	}
	if(right==0)
	{
		if(right_reg<(button_delay))//按键一直被按下时设置时间间隔触发
		{
			right_reg++;
		}
		else
		{
			right_reg=right_acceleration*button_acceleration;
			if(right_acceleration<2)right_acceleration++;
			tpflag=5;//返回5表示right被按下
		}		
	}
	else
	{
		right_acceleration=0;
		right_reg=button_delay;//释放按键时置按键缓存为button_delay，以便在下次按键时及时响应
	}
	return(tpflag);
}
//检查覆盖函数（检查此时带入的参数所确定的方块是否会覆盖原有图形，不会覆盖返回1，覆盖返回0）
bit check_cover(unsigned char tpx,unsigned char tpy,unsigned int tpbox)
{
	unsigned char i;
	bit tpflag=1;
	unsigned int temp;
	temp=s_box.box;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+s_box.y]&=(~((temp&0x000f)<<(9-s_box.x))); 
		temp=temp>>4;
	}//先将现有的方块从游戏点阵缓存中删除
	temp=tpbox;
	for(i=0;i<4;i++)
	{
		if((((temp&0x000f)<<(9-tpx))&Box_Ram[3-i+tpy])!=0x0000)
		{
			tpflag=0;
		}
		temp=temp>>4;
	}//检查方块是否和原有图形重叠,重叠置标志位tpflag为0,不重叠不置标志位，即tpflag为1
	temp=s_box.box;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+s_box.y]|=((temp&0x000f)<<(9-s_box.x));
		temp=temp>>4;
	}//在游戏点阵缓存中恢复原有方块
	return(tpflag);
}
//方块缓存数据函数（输入方块类型和形状即可获得方块缓存数据）
unsigned int box_read_data(unsigned char tpmode,unsigned char tpshape)
{
	unsigned int tpbox;
	switch(tpmode)
	{
		case 0: switch(tpshape)
				{
					case 0: tpbox=0xf000;break;
					case 1: tpbox=0x4444;break;
					case 2: tpbox=0xf000;break;
					case 3: tpbox=0x4444;break;
					default:;
				}break;	
		case 1: switch(tpshape)
				{
					case 0: tpbox=0xe800;break;
					case 1: tpbox=0xc440;break;
					case 2: tpbox=0x2e00;break;
					case 3: tpbox=0x88c0;break;
					default:;
				}break;	
		case 2: switch(tpshape)
				{
					case 0: tpbox=0xe200;break;
					case 1: tpbox=0x44c0;break;
					case 2: tpbox=0x8e00;break;
					case 3: tpbox=0xc880;break;
					default:;
				}break;	
		case 3: switch(tpshape)
				{
					case 0: tpbox=0xcc00;break;
					case 1: tpbox=0xcc00;break;
					case 2: tpbox=0xcc00;break;
					case 3: tpbox=0xcc00;break;
					default:;
				}break;	
		case 4: switch(tpshape)
				{
					case 0: tpbox=0xc600;break;
					case 1: tpbox=0x4c80;break;
					case 2: tpbox=0xc600;break;
					case 3: tpbox=0x4c80;break;
					default:;
				}break;	
		case 5: switch(tpshape)
				{
					case 0: tpbox=0x6c00;break;
					case 1: tpbox=0x8c40;break;
					case 2: tpbox=0x6c00;break;
					case 3: tpbox=0x8c40;break;
					default:;
				}break;
		case 6: switch(tpshape)
				{
					case 0: tpbox=0x4e00;break;
					case 1: tpbox=0x8c80;break;
					case 2: tpbox=0xe400;break;
					case 3: tpbox=0x4c40;break;
					default:;
				}break;
		default:;
	}
	return(tpbox);
}
//方块载入函数
void box_load()
{
	s_box.box=box_read_data(s_box.mode,s_box.shape);
}
//方块映射游戏点阵缓存函数（参数是原来方块的位置、缓存，先消去原有位置的方块）
void box_to_Box_Ram(unsigned char tpx,unsigned char tpy,unsigned int tpbox)
{
	unsigned char i;
	unsigned int temp;
	temp=tpbox;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+tpy]=Box_Ram[3-i+tpy]&(~((temp&0x000f)<<(9-tpx))); 
		temp=temp>>4;
	}//从游戏点阵缓存中删除以前的方块
	temp=s_box.box;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+s_box.y]=((temp&0x000f)<<(9-s_box.x))|Box_Ram[3-i+s_box.y];
		temp=temp>>4;
	}//在游戏点阵缓存中加入新的方块
}
//显示数字函数（
  //x表示x坐标,
  //y表示y坐标,
  //tpdata表示要显示的数字）
//显示速度函数
void show_num(unsigned char x,
					  unsigned char y,
					  unsigned char tpdata)
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		LCD_display_byte(x+i,y,num_data[tpdata*4+i]);	
	}
} 
void show_speed_num(unsigned char x,unsigned char y)
{
	show_num(x,y,speed_num);
}
//显示得分函数
void show_score_num(unsigned char x,unsigned char y)
{
	show_num(x,y,destroy_row_num/10000);
	show_num(x+=5,y,(destroy_row_num%10000)/1000);
	show_num(x+=5,y,(destroy_row_num%1000)/100);
	show_num(x+=5,y,(destroy_row_num%100)/10);
	show_num(x+=5,y,destroy_row_num%10);
}
//消行函数
void destroy_row()
{
	unsigned char i,j=0;
	unsigned char tpflag[4]={0,0,0,0};//最多一次只能消四行，所以设置四个标志位即可,初值为0
	for(i=0;i<16;i++)
	{
		if((Box_Ram[i]&0x3ffc)==0x3ffc)
		{
			tpflag[j]=i+1;//tpflag为0表示不标志，1表示第0行缓存为0xffff，n表示第n+1行缓存为0xffff
			destroy_row_num++;//消除的行数加一
			/*如不把Box_Ram[19]定义成idata类型的话加入这段代码显示数据区就溢出了*/
			if(destroy_row_num%30==0&&speed_num!=9)
			{
				speed_num++;//消够三十行游戏速度加一
				show_speed_num(13,4);//调用显示游戏速度函数
			}
			/*如不把Box_Ram[19]定义成idata类型的话加入这段代码显示数据区就溢出了*/
			j++;
			if(j==4)
			{
				break;
			}//检查完有四行要消除则退出检查循环
		}
	}//依次检测是否有行缓存为0xffff，如果是则标志tpflag为此行的行号
	for(j=0;j<4;j++)
	{
		if(tpflag[j]!=0)
		{
			for(i=tpflag[j]-1;i>0;i--)
			{
			Box_Ram[i]=Box_Ram[i-1];
			Box_Ram[0]=0x2004;
			}
		}
	}//被标志的行依次被上一行所取代，即被消去
	show_score_num(3,1);
}
//显示下一个方块函数
void show_next_box()
{
	unsigned char i,tpdata;
	unsigned int temp;
	temp=box_read_data(next_mode,next_shape);
	for(i=0;i<4;i++)
	{
		tpdata=0x00;
		if(  ((temp>>(15-i))&0x0001)==1  )
		{
			tpdata=0x0f;
		}
		if(  ((temp>>(11-i))&0x0001)==1  )
		{
			tpdata|=0xf0;
		}
		LCD_display_byte(7+i*4,6,tpdata);
		LCD_display_byte(8+i*4,6,0xbb&tpdata);
		LCD_display_byte(9+i*4,6,0xdd&tpdata);
		LCD_display_byte(10+i*4,6,tpdata);	
		tpdata=0x00;
		if(  ((temp>>(7-i))&0x0001)==1  )
		{
			tpdata=0x0f;
		}
		if(  ((temp>>(3-i))&0x0001)==1  )
		{
			tpdata|=0xf0;
		}
		LCD_display_byte(7+i*4,7,tpdata);
		LCD_display_byte(8+i*4,7,0xbb&tpdata);
		LCD_display_byte(9+i*4,7,0xdd&tpdata);
		LCD_display_byte(10+i*4,7,tpdata);		
	}
}
//方块生成函数
void box_build()
{
	s_box.mode=next_mode;
	s_box.shape=next_shape;
	s_box.x=3;
	s_box.y=0;
	next_mode=TL0%7;//产生随机数，但是是伪随机的
	next_shape=TL0%4;//产生随机数，但是是伪随机的
	show_next_box();//放到game_execute()函数中不知道为什么就是不正常显示(2015注释：程序可能重入了)
}
void game_button()
{
	
    switch(basic_button())
    {
        case 3: if(s_box.y!=0)//3表示button_a被按下
                {
                    EA=0;//关中断，如果不关的话可能引起游戏显示混乱
                    speaker=0;
                    if(s_box.shape==3&&check_cover(s_box.x,s_box.y,box_read_data(s_box.mode,0)))
                    {
							
                        s_box.shape=0;
                        box_load();
                        box_to_Box_Ram(s_box.x,s_box.y,box_read_data(s_box.mode,3));
                    }
                    else if(check_cover(s_box.x,s_box.y,box_read_data(s_box.mode,0)))
                    {	if(check_cover(s_box.x,s_box.y,box_read_data(s_box.mode,s_box.shape+1)))
							{	
								s_box.shape++;
								box_load();
								box_to_Box_Ram(s_box.x,s_box.y,box_read_data(s_box.mode,s_box.shape-1));
							}
                     }
                    EA=1;//开中断
                    speaker=1;
					}break;
        case 1: if(s_box.y!=0)//1表示down被按下
        {
            EA=0;//关中断，如果不关的话可能引起游戏显示混乱
            speaker=0;
            while(check_cover(s_box.x,s_box.y+1,s_box.box))//检测是否能下降，指导不能再下降为止
            {
                s_box.y++;
                box_to_Box_Ram(s_box.x,s_box.y-1,s_box.box);
            }
            destroy_row();
            box_build();
            box_load();
					//	game_over_flag=check_game_over();//游戏结束标志位置1表示游戏结束
					//	next_box();
            box_to_Box_Ram(s_box.x,s_box.y,s_box.box);
            EA=1;//开中断
            speaker=1;
            }break;
        case 4: if(s_box.y!=0)//4表示left被按下
        {
            EA=0;//关中断，如果不关的话可能引起游戏显示混乱
            speaker=0;
            if(check_cover(s_box.x-1,s_box.y,s_box.box))
            {
                s_box.x--;
                box_to_Box_Ram(s_box.x+1,s_box.y,s_box.box);
            }
            EA=1;//开中断
            speaker=1;
        }break;
        case 5: if(s_box.y!=0)//5表示right被按下
                {
                    EA=0;//关中断，如果不关的话可能引起游戏显示混乱
                    speaker=0;
                    if(check_cover(s_box.x+1,s_box.y,s_box.box))
                    {
                        s_box.x++;
                        box_to_Box_Ram(s_box.x-1,s_box.y,s_box.box);
                    }
						EA=1;//开中断
						speaker=1;
					}break;
        case 2: //2表示up被按下
            speaker=0;
            pause_game_flag=~pause_game_flag;//游戏暂停标志取反
            while(up==0);
            speaker=1;
            break;
        default:;
    }	
}
//检查游戏结束函数（游戏结束返回1，游戏没有结束返回0）
bit check_game_over()
{
	unsigned char i;
	bit tpflag=0;
	unsigned int temp;
	temp=s_box.box;
	for(i=0;i<4;i++)
	{
		if((((temp&0x000f)<<(9-s_box.x))&Box_Ram[3-i+s_box.y])!=0x0000)
		{
			tpflag=1;
		}
		temp=temp>>4;
	}//检查新建方块是否和原有图形重叠,重叠置标志位tpflag为1,不重叠不置标志位，即tpflag为0
	return(tpflag);
}
//游戏执行函数（控制方块下落，检测是否到底，如果到底调用消行函数）
void game_execute()
{
	if(box_down_reg<20-(speed_num<<1))
	{				  
		box_down_reg++;
	}
	else
	{
		box_down_reg=0;
		if(check_cover(s_box.x,s_box.y+1,s_box.box))
		{
			s_box.y++;
			box_to_Box_Ram(s_box.x,s_box.y-1,s_box.box);
		}//检测是否还可以下降，如果还能下降则继续下降
		else
		{
			destroy_row();
			box_build();
			box_load();
			game_over_flag=check_game_over();//游戏结束标志位置1表示游戏结束
			box_to_Box_Ram(s_box.x,s_box.y,s_box.box);
			box_down_reg=(20-(speed_num<<1)-1);//为了使方块一出现就能变换形状，所以需要尽快使得方块下降一行，不知道为什么最高行不能变换形状
		}//如果不能下降则调用消行函数检查是否可以消行，之后重新建立方块
	}	
}
//选择游戏速度函数
void select_speed()
{
	unsigned char i;
	bit tpflag=1;//置循环标志位为1
	LCD_clear();
	for(i=0;i<128;i++)
	{
		LCD_display_byte(i,0,0xff);
		LCD_display_byte(i,7,0xff);
	}
	LCD_display_byte(60,4,0x7f);
	LCD_display_byte(59,4,0x3e);
	LCD_display_byte(58,4,0x1c);
	LCD_display_byte(57,4,0x08);
	LCD_display_byte(67,4,0x7f);
	LCD_display_byte(68,4,0x3e);
	LCD_display_byte(69,4,0x1c);
	LCD_display_byte(70,4,0x08);
	LCD_display_word(speed_data,24,3,52);
	show_speed_num(62,4);
	while(tpflag)
	{
		switch(basic_button())
		{
			case 4: if(speed_num!=0)
					{
						speaker=0;
						speed_num--;
						show_speed_num(62,4);
						speaker=1;
					}
					while(left==0);
					break;
			case 5: if(speed_num!=9)
					{
					    speaker=0;
						speed_num++;
						show_speed_num(62,4);
						speaker=1;
					}
					while(right==0);
					break;
			case 6: tpflag=0;
		 	        speaker=0;
					while(OK==0);
					speaker=1;
					break;
			default:;
		}
	}//选择游戏速度循环
}
//游戏开始显示画面
void game_start_show()
{
	bit tpflag=1;//置循环标志位为1
	LCD_full_draw(start_pic);
	while(tpflag)
	{
		switch(basic_button())
		{
			case 6: tpflag=0;
			        speaker=0;
					while(OK==0);
					speaker=1;
					break;
			default:;
		}
	}//game_start_show循环
}
//游戏初始化函数
void game_initialize()
{
	box_down_reg=0;
	next_mode=6;
	next_shape=2;
	destroy_row_num=0;
	game_over_flag=0;
	pause_game_flag=0;
	LCD_clear();
	time0_reg=0;
	display_basic();	
	LCD_display_word(score_data,24,0,3);
	LCD_display_word(speed_data,24,3,3);
	show_score_num(3,1);
	show_speed_num(13,4);
}
//定时器0初始化函数
void time0_initialize()
{
	TMOD=0x03;//定时器0，16位工作方式
	TR0=1; //启动定时器
	ET0=1; //打开定时器0中断
			//默认中断优先级为低
	EA=1; //打开总中断
}
//俄罗斯方块游戏主函数
void Tetris_main()
{
	unsigned char i;
	for(i=0;i<19;i++)
	{
		Box_Ram[i]=Box_Ram_data[i];
	};//载入游戏初始显示画面
	LCD_draw(mpic);
	game_over_flag=0;//游戏结束标志位置0表示游戏未结束
	box_build();
	box_load();
//	next_box();
	box_to_Box_Ram(s_box.x,s_box.y,s_box.box);
	box_down_reg=(20-(speed_num<<1)-1);//为了使方块一出现就能变换形状，所以需要尽快使得方块下降一行，不知道为什么最高行不能变换形状
	time0_initialize();
	while(!game_over_flag)//如果游戏结束标志位置1，表示游戏结束，打破循环，调用游戏结束画面显示函数
	{
		game_button();
	}
	EA=0;//游戏结束后关中断
}
//游戏结束画面显示函数
void game_over_show()
{
	unsigned char i;
	bit tpflag=1;//置循环标志位为1
	LCD_full_draw(over_pic);
	while(change==0);
	while(tpflag)
	{
		switch(basic_button())
		{
			case 6: tpflag=0;
			        speaker=0;
					while(OK==0);
					speaker=1;
					break;
			default:;
		}
	}//game over画面循环
	LCD_clear();
	for(i=0;i<128;i++)
	{
		LCD_display_byte(i,0,0xff);
		LCD_display_byte(i,7,0xff);
	}
	LCD_display_word(score_data,24,3,52);
	show_score_num(52,4);
	tpflag=1;
	while(tpflag)
	{
		switch(basic_button())
		{
			case 6: tpflag=0;
			        speaker=0;
					while(OK==0);
					speaker=1;
					break;
			default:;
		}
	}//游戏得分显示循环
}

void main()
{
	LCD_initialize();
	LCD_clear();
	while(1)
	{
		game_start_show();
		select_speed();
		game_initialize();//调用游戏初始化函数，初始化游戏所有变量以及在液晶屏上显示基本的信息
		Tetris_main();
		game_over_show();	
	}	
}
//定时器0中断服务
void timer0() interrupt 1
{
	TH0=0x00;
	TL0=0x00;
	if(time0_reg<10)
	{				  
		time0_reg++;
	}
	else
	{
		time0_reg=0;
		if(pause_game_flag==0)
		{
			game_execute();
			refurbish_display();
		}
	}
}
