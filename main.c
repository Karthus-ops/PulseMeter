#include <REGX52.H>

sbit rate=P2^0;//ͨ��P20�ڽ��շ�������
unsigned int count,R;    //count��¼��������֮���жϴ����Ӷ��ó�ʱ��
unsigned int number=1;   //�ݴ�countֵ
unsigned char code table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//����������ܶ���0~9

void delay(unsigned int xms) // ��ʱx����
	{
		unsigned int x,y;
		for(x=xms;x>0;x--)
		for(y=110;y>0;y--);

	}

void time_init()  //��ʱ����ʼ��10ms
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

void seg(unsigned char num)  //�������ʾ����
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
		
			if(rate==1)                  //��⵽�ߵ�ƽ
			{
					TR0=~TR0;                //���ض�ʱ��
					if(TR0==0)               //һ��������������
					{
						number=count;          //���жϴ�������number����
						count=0;               //��count���������ٴμ�¼��һ������
						/*��װ��ʱ����ֵ*/
						TH0=(65535-9216)/256;
						TL0=(65535-9216)%256;
					}
				
				while(rate)
				{
					seg(R);         //�������ʾ����
				}
			}
			R=60000/(10*number);//RΪÿ����������
			seg(R);
		}
	}

void timer() interrupt 1   //��ʱ���жϺ���
	{
		TH0=(65535-9216)/256;
		TL0=(65535-9216)%256;
		count++;
	}