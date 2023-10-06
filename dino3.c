
#include<htc.h>
#define RS RC0
#define RW RC1
#define EN RC2

#define _XTAL_FREQ 20000000
void fun();
void score();
int jump();
void stop();
int i=0,k=0,c=0;
__CONFIG(0X1F7A);
void cmd(unsigned char command)
{
	RS = 0;
	RW = 0;
	EN = 1;
	PORTD = command;
	__delay_ms(20);
	EN = 0;
}
void data(unsigned char data1)
{
	RS = 1;
	RW = 0;
	EN = 1;
	PORTD = data1;
	__delay_ms(20);
	EN = 0;
}

int main()
{
	TRISC = 0;
	TRISD = 0;
	TRISC3 = 1;
	


	cmd(0x01);
	cmd(0x0C);
	cmd(0x38);
	cmd(0x00);
	cmd(0xc0);
	data(0);
	cmd(0xcf);
	data(2);
	int i=0;
	__delay_ms(200);
	while(1)
	{
	
		if(RC3 == 1)
		{
			if (RC3 == 0)
			cmd (0x82);
          data (3);
		    cmd (0xc0);
			data (0);
		__delay_ms(300);
			jump();
		}
		
		
		score();
		data(3);
		cmd(0xcf - c);
		data(3);
		__delay_ms(100);
		data(3);
		cmd(0xcf - c);
		data(2);
		c++;
		if(c>15 && RC3 == 0)
		{
			stop();
			cmd(0x01);
			main();
		}
		if(c>15)
		{
			cmd(0xc0);
			data(3);
			c=0;
			cmd(0xc0);
			data(0);
		}
		if(RC3 == 1)
			jump();
	}
}


void score()
{
	char *num="0123456789";
	char *name="score : ";
	cmd(0x82);
	for(int j=0;j<7;j++)
		data(name[j]);
	cmd(0x8A);
	data(num[k]);
	cmd(0x8B);
	data(num[i]);
	__delay_ms(100);
	//i++;
	if(i>10)
	{
		i=0;
		k++;
		if(k==9)
			k=0;
	}
	else if(RC3 == 1)
		jump();
}
int jump()
{
			cmd(0xc0);
			data(3);
			cmd(0x81);
			data(1);
			__delay_ms(200);
			cmd(0x81);
			data(3);
			cmd(0xc0);
			data(0);
			return(i++);
				
}
void stop()
{
	cmd(0x01);
	cmd(0x80);
	char *stop="GAME OVER";
	for(int i=0;i<9;i++)
	{
		data(stop[i]);
		__delay_ms(300);
	}
	while(1);
}
