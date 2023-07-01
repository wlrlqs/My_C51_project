#include <REG52.H>
#include"pic.c"
#include <intrins.h>
#define LCD_DATA P2
#define button_delay 150  //������ʱ
#define button_acceleration 65  //�������ٶ���ֵ
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
sbit LCD_CS2=P1^3;//P1^4;		//����ѡ����������ʱ���෴��
sbit LCD_CS1=P1^4;//P1^3;		//����ѡ��
sbit LCD_RST=P3^7;

unsigned int up_reg=button_delay;       //����up�ۼ���
unsigned int down_reg=button_delay;     //����down�ۼ���
unsigned int left_reg=button_delay;     //����left�ۼ���
unsigned int right_reg=button_delay;    //����right�ۼ���
unsigned int button_a_reg=button_delay; //����button_a�ۼ���
unsigned int button_b_reg=button_delay; //����button_b�ۼ���
unsigned int right_acceleration=0;		//����right���ٶȼĴ���
unsigned int left_acceleration=0;		//����left���ٶȼĴ���

unsigned int idata Box_Ram[19];//������Ϸ���󻺴�10*16
unsigned char box_down_reg;//���巽�������ۼӼĴ���
unsigned char time0_reg;//���嶨ʱ��0�ۼӼĴ���
unsigned char next_mode;//������һ�����������
unsigned char next_shape;//������һ���������״
unsigned int destroy_row_num=0;//��������������
unsigned char speed_num=0;//������Ϸ�ٶȵȼ�
unsigned char level_num;//������Ϸ�Ѷȵȼ�
bit game_over_flag;//��Ϸ������־λ��0��ʾ��Ϸδ����
bit pause_game_flag;//��Ϸ��ͣ��־λ��0��ʾ��Ϸδ��ͣ

struct
{
	unsigned char mode;//����
	unsigned char shape;//��״
	unsigned char x;//x����
	unsigned char y;//y����
	unsigned int box;//���巽�黺��
}s_box;	//���巽��ṹ��
//LCD���æ״̬����
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
//дָ����루csΪ0ѡ������csΪ1ѡ������
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
//д��ʾ���ݣ�csΪ0ѡ������csΪ1ѡ������
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

//LCD��ʼ������
void LCD_initialize()
{
	LCD_RST=0;
	_nop_();
	_nop_();
	LCD_RST=1;
	LCD_W_code(0x3f,0);		//����ʾ����	
	LCD_W_code(0xc0,0);		//������ʾ��ʼ��Ϊ��һ��	
	LCD_W_code(0xb8,0);		//ҳ���ַ����	
	LCD_W_code(0x40,0);		//�е�ַ��Ϊ0
	LCD_W_code(0x3f,1);
	LCD_W_code(0xc0,1);	
	LCD_W_code(0xb8,1);
	LCD_W_code(0x40,1);
}
//LCD��������
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
//LCD��ʾ�ַ���������word��ʾҪ��ʾ���ַ���,
 //length��ʾҪ��ʾ���ַ������,
 //x��ʾ���ַ���������,
 //y��ʾ���ַ�����������
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
//LCD��ȫ������
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
//LCD��ʾһ���ֽں�����
  //x��ʾx����,
  //y��ʾy����,
  //tpdata��ʾҪ��ʾ�����ݣ�
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
//����������ʾ����
void display_basic()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		LCD_display_byte(GAME_LOCATION,i,0xff);
		LCD_display_byte(GAME_LOCATION+41,i,0xff);
	}
}
//ˢ����Ϸ������
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
//������������(����0��ʾû���������£�����1��ʾdown�����£�����2��ʾup�����£�����3��ʾbutton_a������,����4��ʾleft������,����5��ʾright������)
//��Ϸ�а���ʶ����������ȼ����Ӹߵ���������button_a_reg>down>left>right>up��
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
		if(down_reg<button_delay)//����һֱ������ʱ����ʱ��������
		{
			down_reg++;
		}
		else
		{
			down_reg=0;
			tpflag=1;//����1��ʾdown������
		}		
	}
	else
	{
		down_reg=button_delay;//�ͷŰ���ʱ�ð�������Ϊbutton_delay���Ա����´ΰ���ʱ��ʱ��Ӧ
	}
	if(up==0)
	{
		if(up_reg<button_delay)//����һֱ������ʱ����ʱ��������
		{
			up_reg++;
		}
		else
		{
			up_reg=0;
			tpflag=2;//����2��ʾup������
		}		
	}
	else
	{
		up_reg=button_delay;//�ͷŰ���ʱ�ð�������Ϊbutton_delay���Ա����´ΰ���ʱ��ʱ��Ӧ
	}
	if(change==0)
	{
		if(button_a_reg<button_delay*8)//����һֱ������ʱ����ʱ��������
		{
			button_a_reg++;
		}
		else
		{
			button_a_reg=0;
			tpflag=3;//����3��ʾbutton_a������
		}		
	}
	else
	{
		button_a_reg=button_delay*8;//�ͷŰ���ʱ�ð�������Ϊbutton_delay���Ա����´ΰ���ʱ��ʱ��Ӧ
	}
	if(left==0)
	{
		if(left_reg<(button_delay))//����һֱ������ʱ����ʱ��������
		{
			left_reg++;
		}
		else
		{
			left_reg=left_acceleration*button_acceleration;
			if(left_acceleration<2)left_acceleration++;
			tpflag=4;//����4��ʾleft������
		}		
	}
	else
	{
		left_acceleration=0;
		left_reg=button_delay;//�ͷŰ���ʱ�ð�������Ϊbutton_delay���Ա����´ΰ���ʱ��ʱ��Ӧ
	}
	if(right==0)
	{
		if(right_reg<(button_delay))//����һֱ������ʱ����ʱ��������
		{
			right_reg++;
		}
		else
		{
			right_reg=right_acceleration*button_acceleration;
			if(right_acceleration<2)right_acceleration++;
			tpflag=5;//����5��ʾright������
		}		
	}
	else
	{
		right_acceleration=0;
		right_reg=button_delay;//�ͷŰ���ʱ�ð�������Ϊbutton_delay���Ա����´ΰ���ʱ��ʱ��Ӧ
	}
	return(tpflag);
}
//��鸲�Ǻ���������ʱ����Ĳ�����ȷ���ķ����Ƿ�Ḳ��ԭ��ͼ�Σ����Ḳ�Ƿ���1�����Ƿ���0��
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
	}//�Ƚ����еķ������Ϸ���󻺴���ɾ��
	temp=tpbox;
	for(i=0;i<4;i++)
	{
		if((((temp&0x000f)<<(9-tpx))&Box_Ram[3-i+tpy])!=0x0000)
		{
			tpflag=0;
		}
		temp=temp>>4;
	}//��鷽���Ƿ��ԭ��ͼ���ص�,�ص��ñ�־λtpflagΪ0,���ص����ñ�־λ����tpflagΪ1
	temp=s_box.box;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+s_box.y]|=((temp&0x000f)<<(9-s_box.x));
		temp=temp>>4;
	}//����Ϸ���󻺴��лָ�ԭ�з���
	return(tpflag);
}
//���黺�����ݺ��������뷽�����ͺ���״���ɻ�÷��黺�����ݣ�
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
//�������뺯��
void box_load()
{
	s_box.box=box_read_data(s_box.mode,s_box.shape);
}
//����ӳ����Ϸ���󻺴溯����������ԭ�������λ�á����棬����ȥԭ��λ�õķ��飩
void box_to_Box_Ram(unsigned char tpx,unsigned char tpy,unsigned int tpbox)
{
	unsigned char i;
	unsigned int temp;
	temp=tpbox;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+tpy]=Box_Ram[3-i+tpy]&(~((temp&0x000f)<<(9-tpx))); 
		temp=temp>>4;
	}//����Ϸ���󻺴���ɾ����ǰ�ķ���
	temp=s_box.box;
	for(i=0;i<4;i++)
	{
		Box_Ram[3-i+s_box.y]=((temp&0x000f)<<(9-s_box.x))|Box_Ram[3-i+s_box.y];
		temp=temp>>4;
	}//����Ϸ���󻺴��м����µķ���
}
//��ʾ���ֺ�����
  //x��ʾx����,
  //y��ʾy����,
  //tpdata��ʾҪ��ʾ�����֣�
//��ʾ�ٶȺ���
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
//��ʾ�÷ֺ���
void show_score_num(unsigned char x,unsigned char y)
{
	show_num(x,y,destroy_row_num/10000);
	show_num(x+=5,y,(destroy_row_num%10000)/1000);
	show_num(x+=5,y,(destroy_row_num%1000)/100);
	show_num(x+=5,y,(destroy_row_num%100)/10);
	show_num(x+=5,y,destroy_row_num%10);
}
//���к���
void destroy_row()
{
	unsigned char i,j=0;
	unsigned char tpflag[4]={0,0,0,0};//���һ��ֻ�������У����������ĸ���־λ����,��ֵΪ0
	for(i=0;i<16;i++)
	{
		if((Box_Ram[i]&0x3ffc)==0x3ffc)
		{
			tpflag[j]=i+1;//tpflagΪ0��ʾ����־��1��ʾ��0�л���Ϊ0xffff��n��ʾ��n+1�л���Ϊ0xffff
			destroy_row_num++;//������������һ
			/*�粻��Box_Ram[19]�����idata���͵Ļ�������δ�����ʾ�������������*/
			if(destroy_row_num%30==0&&speed_num!=9)
			{
				speed_num++;//������ʮ����Ϸ�ٶȼ�һ
				show_speed_num(13,4);//������ʾ��Ϸ�ٶȺ���
			}
			/*�粻��Box_Ram[19]�����idata���͵Ļ�������δ�����ʾ�������������*/
			j++;
			if(j==4)
			{
				break;
			}//�����������Ҫ�������˳����ѭ��
		}
	}//���μ���Ƿ����л���Ϊ0xffff����������־tpflagΪ���е��к�
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
	}//����־�������α���һ����ȡ����������ȥ
	show_score_num(3,1);
}
//��ʾ��һ�����麯��
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
//�������ɺ���
void box_build()
{
	s_box.mode=next_mode;
	s_box.shape=next_shape;
	s_box.x=3;
	s_box.y=0;
	next_mode=TL0%7;//�����������������α�����
	next_shape=TL0%4;//�����������������α�����
	show_next_box();//�ŵ�game_execute()�����в�֪��Ϊʲô���ǲ�������ʾ(2015ע�ͣ��������������)
}
void game_button()
{
	
    switch(basic_button())
    {
        case 3: if(s_box.y!=0)//3��ʾbutton_a������
                {
                    EA=0;//���жϣ�������صĻ�����������Ϸ��ʾ����
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
                    EA=1;//���ж�
                    speaker=1;
					}break;
        case 1: if(s_box.y!=0)//1��ʾdown������
        {
            EA=0;//���жϣ�������صĻ�����������Ϸ��ʾ����
            speaker=0;
            while(check_cover(s_box.x,s_box.y+1,s_box.box))//����Ƿ����½���ָ���������½�Ϊֹ
            {
                s_box.y++;
                box_to_Box_Ram(s_box.x,s_box.y-1,s_box.box);
            }
            destroy_row();
            box_build();
            box_load();
					//	game_over_flag=check_game_over();//��Ϸ������־λ��1��ʾ��Ϸ����
					//	next_box();
            box_to_Box_Ram(s_box.x,s_box.y,s_box.box);
            EA=1;//���ж�
            speaker=1;
            }break;
        case 4: if(s_box.y!=0)//4��ʾleft������
        {
            EA=0;//���жϣ�������صĻ�����������Ϸ��ʾ����
            speaker=0;
            if(check_cover(s_box.x-1,s_box.y,s_box.box))
            {
                s_box.x--;
                box_to_Box_Ram(s_box.x+1,s_box.y,s_box.box);
            }
            EA=1;//���ж�
            speaker=1;
        }break;
        case 5: if(s_box.y!=0)//5��ʾright������
                {
                    EA=0;//���жϣ�������صĻ�����������Ϸ��ʾ����
                    speaker=0;
                    if(check_cover(s_box.x+1,s_box.y,s_box.box))
                    {
                        s_box.x++;
                        box_to_Box_Ram(s_box.x-1,s_box.y,s_box.box);
                    }
						EA=1;//���ж�
						speaker=1;
					}break;
        case 2: //2��ʾup������
            speaker=0;
            pause_game_flag=~pause_game_flag;//��Ϸ��ͣ��־ȡ��
            while(up==0);
            speaker=1;
            break;
        default:;
    }	
}
//�����Ϸ������������Ϸ��������1����Ϸû�н�������0��
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
	}//����½������Ƿ��ԭ��ͼ���ص�,�ص��ñ�־λtpflagΪ1,���ص����ñ�־λ����tpflagΪ0
	return(tpflag);
}
//��Ϸִ�к��������Ʒ������䣬����Ƿ񵽵ף�������׵������к�����
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
		}//����Ƿ񻹿����½�����������½�������½�
		else
		{
			destroy_row();
			box_build();
			box_load();
			game_over_flag=check_game_over();//��Ϸ������־λ��1��ʾ��Ϸ����
			box_to_Box_Ram(s_box.x,s_box.y,s_box.box);
			box_down_reg=(20-(speed_num<<1)-1);//Ϊ��ʹ����һ���־��ܱ任��״��������Ҫ����ʹ�÷����½�һ�У���֪��Ϊʲô����в��ܱ任��״
		}//��������½���������к�������Ƿ�������У�֮�����½�������
	}	
}
//ѡ����Ϸ�ٶȺ���
void select_speed()
{
	unsigned char i;
	bit tpflag=1;//��ѭ����־λΪ1
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
	}//ѡ����Ϸ�ٶ�ѭ��
}
//��Ϸ��ʼ��ʾ����
void game_start_show()
{
	bit tpflag=1;//��ѭ����־λΪ1
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
	}//game_start_showѭ��
}
//��Ϸ��ʼ������
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
//��ʱ��0��ʼ������
void time0_initialize()
{
	TMOD=0x03;//��ʱ��0��16λ������ʽ
	TR0=1; //������ʱ��
	ET0=1; //�򿪶�ʱ��0�ж�
			//Ĭ���ж����ȼ�Ϊ��
	EA=1; //�����ж�
}
//����˹������Ϸ������
void Tetris_main()
{
	unsigned char i;
	for(i=0;i<19;i++)
	{
		Box_Ram[i]=Box_Ram_data[i];
	};//������Ϸ��ʼ��ʾ����
	LCD_draw(mpic);
	game_over_flag=0;//��Ϸ������־λ��0��ʾ��Ϸδ����
	box_build();
	box_load();
//	next_box();
	box_to_Box_Ram(s_box.x,s_box.y,s_box.box);
	box_down_reg=(20-(speed_num<<1)-1);//Ϊ��ʹ����һ���־��ܱ任��״��������Ҫ����ʹ�÷����½�һ�У���֪��Ϊʲô����в��ܱ任��״
	time0_initialize();
	while(!game_over_flag)//�����Ϸ������־λ��1����ʾ��Ϸ����������ѭ����������Ϸ����������ʾ����
	{
		game_button();
	}
	EA=0;//��Ϸ��������ж�
}
//��Ϸ����������ʾ����
void game_over_show()
{
	unsigned char i;
	bit tpflag=1;//��ѭ����־λΪ1
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
	}//game over����ѭ��
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
	}//��Ϸ�÷���ʾѭ��
}

void main()
{
	LCD_initialize();
	LCD_clear();
	while(1)
	{
		game_start_show();
		select_speed();
		game_initialize();//������Ϸ��ʼ����������ʼ����Ϸ���б����Լ���Һ��������ʾ��������Ϣ
		Tetris_main();
		game_over_show();	
	}	
}
//��ʱ��0�жϷ���
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
