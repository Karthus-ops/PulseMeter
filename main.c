#include <REGX52.H>

sbit rate=P2^0;//通过P20口接收方波脉冲
unsigned int count,R;    //count记录相邻脉冲之间中断次数从而得出时间
unsigned int number=1;   //暂存count值
unsigned char code table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//共阴极数码管段码0~9

void delay(unsigned int xms) // 延时x毫秒
	{
		unsigned int x,y;
		for(x=xms;x>0;x--)
		for(y=110;y>0;y--);

	}

void time_init()  //定时器初始化10ms
	{
		TMOD=0x01;
		TH0=(65535-9216)/256;
		TL0=(65535-9216)%256;
		TR0=0;
		TF0=0;
		EA=1;
		ET0=1;
		PT0=0;
	}

void seg(unsigned char num)  //数码管显示函数
	{
		unsigned char a,b,c,d;
		d=num/1000;
		c=(num%1000)/100;
		b=(num%100)/10;
		a=num%10;
		P2_4=0;P2_5=0;P0=table[a];P0_7=0;delay(5);P0=0x00;
		P2_4=1;P2_5=0;P0=table[b];P0_7=0;delay(5);P0=0x00;
		P2_4=0;P2_5=1;P0=table[c];P0_7=0;delay(5);P0=0x00;
		P2_4=1;P2_5=1;P0=table[d];P0_7=0;delay(5);P0=0x00;
	}
	
void main()
	{
		time_init();
		while(1)
		{  
		
			if(rate==1)                  //检测到高电平
			{
					TR0=~TR0;                //开关定时器
					if(TR0==0)               //一次相邻脉搏结束
					{
						number=count;          //将中断次数赋给number保存
						count=0;               //将count重新置零再次记录下一组脉冲
						/*重装定时器初值*/
						TH0=(65535-9216)/256;
						TL0=(65535-9216)%256;
					}
				
				while(rate)
				{
					seg(R);         //数码管显示函数
				}
			}
			R=60000/(10*number);//R为每分钟脉搏数
			seg(R);
		}
	}

void timer() interrupt 1   //定时器中断函数
	{
		TH0=(65535-9216)/256;
		TL0=(65535-9216)%256;
		count++;
	}