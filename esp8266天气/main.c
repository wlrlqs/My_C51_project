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
		
		if(count == 13 || count == 14)    //天气和温度在第13个：后面
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

//连接核心代码```


```c
/*是否连接   用于判断是否和服务器连接*/
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


/*发送连接AT指令*/
void ES_ConnectInit()
{   
	ES_SendStr2(AT_str,37);
	ES_delayms(10000);
	//getMessage();
}

/*发送获取天气指令*/
void ES_ConnectGet()
{
	bit sta = 0;
	//连接远程服务端
	uint i = 0;
	do{
		ES_SendStr2(AT_str1,43);
		ES_delayms(100);
		i++;
		sta = isConnect();
	}while(sta == 0 && i < 10);
	
	//发送115个字节
	ES_SendStr2(AT_str2,16);

	ES_delayms(3000);
	//发送api接口信息
	ES_SendStr(AT_str3);
}


/*发送指令并获取回显信息*/
void ES_ConnectDeal()
{
//	showStr("AB");
	ES_ConnectGet();

	ES_GetStr();

}
