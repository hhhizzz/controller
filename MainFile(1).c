#pragma sfr
#pragma interrupt INTKR inter_key RB1
#pragma interrupt INTTM000 timer_INTTM000
#pragma DI
#pragma EI
#pragma ACCESS

#define STARTADDR 0XFA40
#define ENDADDR OXFA53


int KEY = 0;			//Mark number of bottom
int time;			//Duration of liting
int Ycars=0;			//Quantity of down and up's car
int Xcars=0;			//Quantity of right and left's car
int mode = 3;
int second=0;
int seconds=0;
void LCDS(char i,int position){ //Show numbers
    unsigned int addr;
	addr=STARTADDR+position*2;
	switch (i) {
		case '0':
			pokeb(addr,0x0D);
			pokeb(addr+1,0x07);
			break;
		case '1':
			pokeb(addr,0x00);
			pokeb(addr+1,0x06);
			break;
		case '2':
			pokeb(addr,0x0e);
			pokeb(addr+1,0x03);
			break;
		case '3':
			pokeb(addr,0x0a);
			pokeb(addr+1,0x07);
			break;
		case '4':
			pokeb(addr,0x03);
			pokeb(addr+1,0x06);
			break;
		case '5':
			pokeb(addr,0x0b);
			pokeb(addr+1,0x05);
			break;
		case '6':
			pokeb(addr,0x0f);
			pokeb(addr+1,0x05);
			break;
		case '7':
			pokeb(addr,0x00);
			pokeb(addr+1,0x07);
			break;
		case '8':
			pokeb(addr,0x0f);
			pokeb(addr+1,0x07);
			break;
		case '9':
			pokeb(addr,0x0b);
			pokeb(addr+1,0x07);
			break;
		case 'A':
			pokeb(addr, 0X07);
			pokeb(addr+1, 0X07);
			break;
		case 'U':
			pokeb(addr, 0X0D);
			pokeb(addr+1, 0X06);
			break;
		case '-':
			pokeb(addr,0x02);
			pokeb(addr+1,0x00);
			break;
		case 'n':
			pokeb(addr,0x05);
			pokeb(addr+1,0x07);
			break;
	}
}
void lcdA()			//Storage the "Au" for LCD display
{
	LCDS('A',0);
	LCDS('U',1);
	LCDS(Xcars+'0',6);
	LCDS(Ycars+'0',7);
}
void lcdM()			//Storage the "nU" for LCD display
{
	LCDS('n',0);
	LCDS('U',1);
	LCDS('-',6);
	LCDS('-',7);
}

void setDelayTime(int seconds){
    CR000=0x7fff*second;
}

void delay(int k)		//Delay function by software
{
	int i;
	int j;
	for(i = 0; i < k; i++)
	{
		for(j = 0; j < k; j++)
		{

		}
	}
}

void inter_key()		//Interrupt of key
{
	DI();			//Close interruption

	//Reflect when bottom is pressed
	P3.4 = 1;
	P3.3 = 0;
	delay(20);
	P3.4 = 0;
	P3.3 = 1;

	//Judge which bottom is pressed
	switch(P4 & 0x3F)
	{
		case 0x3E:
		{
			//No.1 bottom: pass a car of right and left
			if(mode==3){	//Only when the mode is auto
			    Xcars++;
			    LCDS(Xcars+'0', 6);
			}
			break;
		}
		case 0x3D:
		{
			//No.2 bottom: pass a car of down and up
			if(mode==3){
			    Ycars++;
			    LCDS(Ycars+'0', 7);
			}
			break;
		}
		case 0x3B:
		{
			//No.3 bottom: select auto model
			KEY = 3; lcdA(); break;
		}
		case 0x37:
		{
			//No.4 bottom: select manual model
			KEY = 4; lcdM();break;
		}
		case 0x2F: KEY = 5; break;			//No.5 bottom: change to right and left's going through in manual model
		case 0x1F: KEY = 6; break;			//No.6 bottom: change to down and up's going through in manual model
	}


	EI();							//Open interruption
}


void yellow()							//Yellow light bling one time
{
	P13.1 = 1;
	P14.0 = 1;
	P14.3 = 1;
	P15.2 = 1;
	delay(50);
	P13.1 = 0;
	P14.0 = 0;
	P14.3 = 0;
	P15.2 = 0;
}
void reverse1()						//change to down and up's going through
{
	int i;
	DI();						//Close interruption

	//Yellow light bling 3 times
	for(i = 0; i < 3; i++)
	{
		delay(100);
		P14.1 = 0;
		P15.3 = 0;
		delay(100);
		P14.1 = 1;
		P15.3 = 1;
	}


	P13.0 = 0;
	P14.2 = 0;
	P14.1 = 0;
	P15.3 = 0;
	P13.2 = 0;
	P15.0 = 0;
	P13.3 = 0;
	P15.1 = 0;

	P13.1 = 1;
	P14.0 = 1;
	P14.3 = 1;
	P15.2 = 1;
	delay(100);
	P13.1 = 0;
	P14.0 = 0;
	P14.3 = 0;
	P15.2 = 0;

	P13.0 = 0;
	P14.2 = 0;
	P14.1 = 0;
	P15.3 = 0;
	P13.2 = 1;
	P15.0 = 1;
	P13.3 = 1;
	P15.1 = 1;
	Xcars = 0;				//Reset quantity of right and left's cars
	if(mode==3){
	    lcdA();
	}
	EI();					//Open interruption
}
void reverse2() 				//change to right and left's going through
{
	int i;
	DI();					//Close interruption

	//Green light bling 3 times
	for(i = 0; i < 3; i++)
	{
		delay(100);
		P13.2 = 0;
		P15.0 = 0;
		delay(100);
		P13.2 = 1;
		P15.0 = 1;
	}

	P13.0 = 0;
	P14.2 = 0;
	P14.1 = 0;
	P15.3 = 0;
	P13.2 = 0;
	P15.0 = 0;
	P13.3 = 0;
	P15.1 = 0;

	P13.1 = 1;
	P14.0 = 1;
	P14.3 = 1;
	P15.2 = 1;
	delay(100);
	P13.1 = 0;
	P14.0 = 0;
	P14.3 = 0;
	P15.2 = 0;

	P13.0 = 1;
	P14.2 = 1;
	P14.1 = 1;
	P15.3 = 1;
	P13.2 = 0;
	P15.0 = 0;
	P13.3 = 0;
	P15.1 = 0;

	Ycars = 0;				//Reset quantity of down and up's car


	if(mode==3){
	    lcdA();
	}
	EI();
}
__interrupt void timer_INTTM000(){
    if (mode == 3)						//Select auto model
    {
        if (P15.0 == 0)
        {
            reverse1();				//change to down and up's going through
        }
        else if (P15.0 == 1)
        {
            reverse2();				//change to right and left's going through
    }
}

int main()
{

	//Initial interruption of key
	PM4 = 0x3F;
	PU4 = 0x3F;
	KRM = 0x3F;
	KRMK = 0;

	//Initial lights and buzzer
	PM3.3 = 0;
	PM3.4 = 0;
	CKS = 0B11110000;
	P3.3 = 1;
	PM13 = 0xF0;
	PM14 = 0xF0;
	PM15 = 0xF0;

	//Initial LCD
	PF2 = 0X00;
	PFALL = 0X0F;
	LCDC0 = 0X34;
	LCDMD = 0X30;
	LCDM = 0XC0;
	//INitial Timer
	CRC00.0=0;
	PRM00=0x04;
	setDelayTime(1);
	TMMK010=1;
	TMMK000=0;
	TMC00=0x0c;


	EI();					//Open interruption

	time = 250;				//Set the duration of going through in auto model

	//Intitial right and letf's going throught firstly
	P13.0 = 1;
	P14.2 = 1;
	P14.1 = 1;
	P15.3 = 1;
	P13.2 = 0;
	P15.0 = 0;
	P13.3 = 0;
	P15.1 = 0;

	lcdA();								//Display auto model on LCD
	while(1)
	{
		else if (mode == 4)					//Selet manual model
		{
			switch(KEY)
			{
				case 5:
				{
					if(P15.0 == 0)
						reverse1();		//change to down and up's going through
					break;
				}
				case 6:
				{
					if(P15.0 == 1)
						reverse2();		//change to right and left's going through
					break;
				}
			}
		}

		switch(KEY)						//Judge model
		{
			case 3: mode = 3; break;
			case 4: mode = 4; break;
		}










	}
	return 0;
}
