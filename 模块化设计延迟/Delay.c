
void Delay(unsigned int xms)		//@11.0592MHz
{
	while(xms)
	{
			unsigned char i, j;
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
		xms--;
	}

}
