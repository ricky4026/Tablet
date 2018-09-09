/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "compiler_defs.h"
#include "C8051F340_defs.h"
#include <communication.h>
#define DisplayNum  6;

#define SYSCLK      12000000           // SYSCLK frequency in Hz
#define BAUDRATE      9600           // Baud rate of UART in bps

void Counter();
void Send_data(char txdata,char num );
void Delay(float time);
void InitTimer();
void DisPlay();
void Wait_timer0();
void putchar(unsigned char cData);
void Timer2_Init();
void SYSCLK_Init (void);
unsigned long Square(char n);
char TX_Buffer[] = {0xaa,0x70,0x01,0xcc,0x33,0xc3,0x3c};
char NumberBuf[] ={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x83,0xD8,0x80,0x90,0xFF};
unsigned char Tags[26];
char NumTemp=0;
char Total=20;
xdata TPacker Packer ;
sbit SD=P1^2;
sbit TD=P1^3;
sbit HD=P1^4;
sbit tx_pin =P1^1;
sbit TX_Flag =P1^0; 
sbit SSR_Motor = P1^5;

sbit CP = P0^3;
sbit D0 = P0^5;
sbit D1 = P0^4;
sbit Senser = P0^6;
sbit Emit = P0^7;
 
unsigned int ErrorSenser= 0;
bit F_STOP  =0;
bit F_ERROR  =0;
xdata TPacker Packer ;
unsigned char RxTimeOutCount=0; // UART 接收超時時間設定
unsigned char *Pointer_Packer = &Packer.UART_Mark;
unsigned char ErrorCount =0;
bit IsBarcode = 0;
bit	IsRxTimeOut =0;	
unsigned char CasseteHigh = 0x00;
unsigned char CasseteLow  = 0x10;
unsigned char Command;
unsigned char CountNumber;
unsigned char BCC = 0x00,index =2;
char DisA,DisB,DisC;
unsigned long CasseteNumTemp=0;
unsigned int CPCount = 0x0100;
unsigned char CIndex = 0;
unsigned char STX = 2;
unsigned char ETX = 3;
bit Shining = 0;
int ss = 0;
void Timer_Init()
{
    TCON      = 0x50;
    TMOD      = 0x22;
    TL0       = 0x00;
    TH0       = 0x00;
	TH1       = 0x0C;
	TL1       = 0x0C;
	TR0 = 0;
	TR1 = 0;
}
void UART1_Init (void){

	SBRLL1    = 0x8F;
    SBRLH1    = 0xFD;
    SCON1     = 0x10;		
    SMOD1     = 0x0E;//關閉多機通訊
    SBCON1    = 0x43;  
	EIE2 = 0x02;//開啟UART1中斷
//	EIP2 = 0x02;//提高UART1中斷優先權
} 
void Port_IO_Init()
{
    P0MDOUT   = 0x8C;
    P1MDOUT   = 0x3F;
    XBR0      = 0x01;
    XBR1      = 0x40;
    XBR2      = 0x01;
	SSR_Motor = 1;
}

void Interrupts_Init()
{
    IE        = 0x82;
	ET1 = 1;
}
void SYSCLK_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector
}
void Timer3_Init(){
	TMR3CN = 0x00;
	TMR3L  = 0xFF;
	TMR3H  = 0xFF;
    EIE1  |= 0x80;
}
// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
	PCA0MD &= ~0x40; 
	SYSCLK_Init();
    Timer_Init();
	Timer3_Init();
    Port_IO_Init();
    Interrupts_Init();
	UART1_Init();
}
//-----------------------------------------------------------------------------
void Timer2_Init(){
	TMR2CN = 0x00;
	TMR2L  = 0x00;
	TMR2H  = 0x00;
    ET2 = 1;
}

void Timer2_ISR (void) interrupt INTERRUPT_TIMER2 
{
	TF2H=0;
	if (RxTimeOutCount ++ >=3)
	{
		TR2 =0;
		TMR2H = 0x00;
		TMR2L = 0x00;
		RxTimeOutCount=0;
		Pointer_Packer = & Packer.Tablet.Command ;
		IsRxTimeOut =1;
	}
}
unsigned long Square(char n)
{
	unsigned char y = 1,x;
	for(x = 0;x < n ;x++)
		y=y*2;
	return y;
}

void Timer3_ISR (void) interrupt INTERRUPT_TIMER3 
{
	double ErrorTimer = 0xF000;
	TMR3CN &= 0x7F;
	while(!CP);
	while(ErrorTimer --)
	{
		//ErrorTimer -=1;
		if(D1 == 0 )
		{
			CasseteNumTemp += Square(index -1);
			
			index--;
			Tags[index] = 1;
			while(D1 == 0);
		}
		else if(D0 == 0)
		{
			
			index--;
			Tags[index] = 0;
			while(D0 == 0);
	
		}
		if(index == 0)
		{	
			Packer.Tablet.CasseteNum = CasseteNumTemp;
			CasseteNumTemp = 0;
			IsBarcode = 1;
			index = 25;			
			break;
		}

	}
}
xdata ECasseteNum Tablet;
void RunCommand()
{
if(Packer.RecevieData[1] == DisA && Packer.RecevieData[2] == DisB && Packer.RecevieData[3] == DisC)
{
	Packer.Tablet.Command = Packer.RecevieData[0];
	switch(Packer.Tablet.Command)
	{
		case TabletCommand_PackStart:
			CountNumber = ( Packer.RecevieData[4] & 0x0F) * 10 +
				  ( Packer.RecevieData[5] & 0x0F);
			Command = TabletCommand_PackStart;
			Packer.Tablet.State = TabletState_Busy;
			BCC = UART_Mark ^ Packer.Tablet.State  ;
			putchar(UART_Mark);
			putchar(Packer.Tablet.State);
			putchar(ETX);
			break;
		case TabletCommand_RePack:
			Command = TabletCommand_RePack;
//			Packer.Tablet.State = TabletState_Busy;
			BCC = UART_Mark ^ Packer.Tablet.State  ;
			putchar(UART_Mark);
			putchar(Packer.Tablet.State);
			putchar(ETX);
			break;
		case TabletCommand_Return:
			BCC = UART_Mark ^ Packer.Tablet.State  ;
			putchar(UART_Mark);
			putchar(Packer.Tablet.State);
			putchar(ETX);
			break;
		case TabletCommand_LEDON:
			Command = TabletCommand_LEDON;
			Shining = 1;
//			Packer.Tablet.State = TabletState_Busy;
//			BCC = UART_Mark ^ Packer.Tablet.State  ;
			putchar(UART_Mark);
			putchar(Packer.Tablet.State);
			putchar(ETX);
			break;
		case TabletCommand_LEDOFF:
			Command = TabletCommand_LEDOFF;
			Shining = 0;
//			Packer.Tablet.State = TabletState_Busy;
//			BCC = UART_Mark ^ Packer.Tablet.State  ;
			putchar(UART_Mark);
			putchar(Packer.Tablet.State);
			putchar(ETX);	
			break;	
	}
}
}
//-----------------------------------------------------------------------------
byte BCCTemp,BCC;
void UART1_ISR(void)   interrupt 16 
{
	EIE2 &= ~0x02;
	if (SCON1 & 0x02)		// TI1
	{
		SCON1 &= (~0x02);
	}

	if (SCON1 & 0x01)		// RI1
	{
		SCON1 &= (~0x01);
		*(Pointer_Packer) = SBUF1;
		BCCTemp ^= SBUF1;
		if(Pointer_Packer == &Packer.UART_Mark)
		{
			if (Packer.UART_Mark == UART_Mark)
			{
				Packer.UART_Mark = UART_NoMark ;
				Pointer_Packer = &Packer.RecevieData[CIndex];
			}
			else
			{	
				Pointer_Packer = &Packer.UART_Mark;
			}
		}
		else if(Pointer_Packer == &Packer.RecevieData[CIndex])
		{
			if(CIndex == 0)
				BCCTemp = SBUF1;
			if(Packer.RecevieData[CIndex] == ETX)
			{	
				Pointer_Packer = &Packer.Station.BCC;
				BCC = BCCTemp;						
			}
			else
			{
				CIndex++;
				Pointer_Packer = &Packer.RecevieData[CIndex];
				
			}
		}
		else if(Pointer_Packer == &Packer.Station.BCC)	
		{
			Pointer_Packer = &Packer.UART_Mark;
						
   			if(BCC ==Packer.Station.BCC )
				RunCommand();
			CIndex = 0;
		}
		else 
		{
			Pointer_Packer = &Packer.UART_Mark;
		}		
	}
	EIE2 |= 0x02;
}

//-----------------------------------------------------------------------------

void main()
{
	int i=0;
	Init_Device();
	Packer.Tablet.CasseteNum = DisplayNum;
	SSR_Motor = 0;
	Delay(1000);
	TR1 = 1;
	Shining = 0;
	while(1)
	{

		if(Command == TabletCommand_PackStart)
		{
			Counter();
			Command = TabletCommand_Idel ;
			Packer.Tablet.Command = TabletCommand_PackStop;
			
		}
		else if(Command == TabletCommand_RePack)
		{
			CountNumber -= ErrorCount;
			Counter();
			Command = TabletCommand_Idel ;
			Packer.Tablet.Command = TabletCommand_PackStop;
		}
		else if(Packer.Tablet.Command == TabletCommand_PackStop)
		{
			SSR_Motor = 0;	
		}
		else if(Command == TabletCommand_LEDOFF)
		{
			Shining = 0;
			Delay(200);
			Command = TabletCommand_Idel ;
			Packer.Tablet.State = TabletState_Sucess;
		}
		else if(Command == TabletCommand_LEDON)
		{
			Shining = 1;
			Delay(200);
			Command = TabletCommand_Idel ;
			Packer.Tablet.State = TabletState_Sucess;
		}
	
	}
}
void putchar(unsigned char cData)
{
	unsigned int i=10000;
	while( (SCON1 & 0x20) ==0) ;
	SBUF1 = cData;
	if(cData == Packer.ErrorCode){
		EIE2 |= 0x02;
		SCON1&=0x3F;
	}
	while(i--);	
}

void DisPlay()
{   
	DisA = Packer.Tablet.CasseteNum / 100 + 0x30;
	DisB = (Packer.Tablet.CasseteNum % 100) / 10 + 0x30;
	DisC = Packer.Tablet.CasseteNum %10 + 0x30;

	SD = 1;TD =0;HD = 0;
	NumTemp = DisC - 0x30;
    Send_data(NumberBuf[NumTemp],1);

	SD = 0;TD = 1;HD = 0;
	NumTemp = DisB - 0x30;
	Send_data(NumberBuf[NumTemp],1);

	SD = 0;TD = 0;HD = 1;
	NumTemp = DisA - 0x30;
	Send_data(NumberBuf[NumTemp],1);
		
}
/*----------------------------------------------------------------------------*/
//等待TIMER0??
void Wait_timer0()
{ 
TX_Flag=1;
//while(!TX_Flag);
TX_Flag=0;
}
/*----------------------------------------------------------------------------*/
void T0_int(void) interrupt 1
{
//R0 = 0;
	if (ErrorSenser ++ >=0x1800)
	{
		F_STOP =1;
	}
//TR0 = 1;
} 
/*----------------------------------------------------------------------------*/
void T1_int(void) interrupt 3
{
TR1 = 0;
DisPlay();
if(Shining)
{
	ss++;
	if(ss >300)
	{
	SD = 1;TD =0;HD = 0;
    Send_data(0xFF,1);

	SD = 0;TD = 1;HD = 0;
	Send_data(0xFF,1);

	SD = 0;TD = 0;HD = 1;
	Send_data(0xFF,1);
	Delay(2000);
	ss = 0;
	}
}

if(CP ==1)
{
	CPCount+=1;
	if(CPCount >= 0x0600)
	{
		Packer.Tablet.CasseteNum = DisplayNum;
		IsBarcode = 0;
	}
}
if(CP == 0)
{

	if(!IsBarcode)
		TMR3CN |= 0x80;
	CPCount =0;
}
TR1 = 1;
} 
/*----------------------------------------------------------------------------*/
void Send_data(char txdata,char num )
{
char *p,Data,i;
//TR0=1;
Data = txdata;
for(i=0;i<8;i++) //?送8位?据
{
tx_pin=Data&0x01;
Wait_timer0(); 
Data=Data>>1; 
} 
//TR0=0; 
Delay(15);
}
/*----------------------------------------------------------------------------*/
void Counter()
{
char Count=0x00;
Emit = 1;
F_STOP = 0;
ErrorSenser = 0x0000;
SCON1 &= 0xEF;
TR0 = 1;
SSR_Motor = 1;
while(Count<CountNumber)
{
 	while (!Senser & !F_STOP & !F_ERROR ); // 正緣

    if (F_STOP || F_ERROR )  break;

	while ( Senser & !F_STOP & !F_ERROR ); // 負緣

    if (F_STOP || F_ERROR )  break;

	Count++;

}
SSR_Motor = 0;
TR0=0;

if(F_STOP)
{
	Packer.Tablet.State = TabletState_Fail;
	ErrorCount = Count;	
}
else
{
	Packer.Tablet.State = TabletState_Sucess;
	ErrorCount = 0;	
}
Pointer_Packer = &Packer.UART_Mark;
SCON1 |= 0x10;

}

void Delay(float time)
{

	while(time --);

}