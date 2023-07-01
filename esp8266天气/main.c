void time0() interrupt 4
{
	uchar ch;
	if(RI)
	{
		RI = 0;
		ch = SBUF;
		
		if(ch == ':')
		{
			count++;
		}

		else if(ch == '\r')
		{
//			get_Flag = 1;
			ES_pos = 0;
			count = 0;
		}
		
		if(count == 13 || count == 14)    //�������¶��ڵ�13��������
		{
			str_1[ES_pos] = ch;
			ES_pos++;
		}
		else if(count == 15)
		{
			get_Flag = 1;
		}
		else
		{
			ES_pos = 0;
		}
		
	}
}

//���Ӻ��Ĵ���```


```c
/*�Ƿ�����   �����ж��Ƿ�ͷ���������*/
bit isConnect()
{
	bit sta = 0;
	
	uchar a,b;
	
	a = ES_GetByte();
	b = ES_GetByte();
	
	if(a == 'C' || b == 'O')  //CONNECT
	{
		sta = 1;
	}
	else if(a == 'A' || b == 'L')  //ALeary
	{
		sta = 1;
	}
	return sta;
}


/*��������ATָ��*/
void ES_ConnectInit()
{   
	ES_SendStr2(AT_str,37);
	ES_delayms(10000);
	//getMessage();
}

/*���ͻ�ȡ����ָ��*/
void ES_ConnectGet()
{
	bit sta = 0;
	//����Զ�̷����
	uint i = 0;
	do{
		ES_SendStr2(AT_str1,43);
		ES_delayms(100);
		i++;
		sta = isConnect();
	}while(sta == 0 && i < 10);
	
	//����115���ֽ�
	ES_SendStr2(AT_str2,16);

	ES_delayms(3000);
	//����api�ӿ���Ϣ
	ES_SendStr(AT_str3);
}


/*����ָ���ȡ������Ϣ*/
void ES_ConnectDeal()
{
//	showStr("AB");
	ES_ConnectGet();

	ES_GetStr();

}
