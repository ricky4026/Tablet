typedef unsigned char byte ;
typedef unsigned int  word ;

typedef union
{
	word W ;
	byte B[2];
} TWord;

typedef enum
{
	Message_Heating 		=0,
	Message_PowderLocating	=1,
	Message_PackPowder		=2,
	Message_PackDrug		=3,
	Message_PackMix			=4,
	Message_PackSpace		=5,
	Message_Clearing		=6,
	Message_PackFinish		=7,
	Message_PackPause		=8,
	Message_PackCancel		=9,
	Message_SystemReady		=10,
	Message_Space			=11,

	Message_PowderLocationAlerm   =37,
	Message_PaperEntryAlerm		  =38,
	Message_CarbonEntryAlerm	  =39
} EMessage ;


typedef enum
{
	ErrorCode_NoError		=0,
	ErrorCode_ControlPanel	=1,
	ErrorCode_USBConnection	=2,
	ErrorCode_PaperMotor	=3,
	ErrorCode_Powder		=4,
	ErrorCode_Drug			=5,
	ErrorCode_PowderCover	=6,
	ErrorCode_DrugCover		=7,
	ErrorCode_PresureDown	=8,
	ErrorCode_PresureUp		=9,
	ErrorCode_Heater		=10,
	ErrorCode_HallSensor	=11,
	ErrorCode_Printer		=12
} EErrorCode ;


typedef enum
{
	UART_NoMark	=0x03,
	UART_Mark	=0x02,
} EUART ;

//--------------------------------------------------------
typedef enum 					
{
	TabletState_Fail	='A',				
	TabletState_Busy	='B',							
	TabletState_Sucess	='C',
} ETabletState;



typedef enum 
{ 
    StationState_Idel			=0,
	StationState_Heating		=1,	
	StationState_Packing		=2,	
	StationState_USBPacking  	=3,	
	StationState_PackPause		=4,	
	StationState_Clearing		=5,	
	StationState_PowderLocating =6, 
    StationState_Busy			=7,	
    StationState_Error			=8
} EStationState ;

typedef enum 
{ 
	State_Error		   	=0,
	State_Packing		=1,	
	State_Finish		=3
} E232State ;

// --------------------------------------------------------
typedef enum 					
{
	TabletCommand_Idel			='A',			
	TabletCommand_PackStart		='B',		
	TabletCommand_PackStop		='C',			
	TabletCommand_Return		='D',
	TabletCommand_LEDON			='E',
	TabletCommand_LEDOFF		='F',
	TabletCommand_RePack		='G'		
} ETabletCommand ;


typedef enum 
{
	EStationCommand_Idel,			
	EStationCommand_Return,

	EStationCommand_USBPackStart,
	EStationCommand_PackStart,	
	EStationCommand_PackPause,	
	EStationCommand_PackStop,	
	EStationCommand_PackContinue,	
	EStationCommand_PaperMotorLocate,
	EStationCommand_PaperMotorForward,
	EStationCommand_PaperMotorReverse,
	EStationCommand_AutoClear,		
	EStationCommand_PowderLocate,	
	EStationCommand_StopHeating,	
	
	EStationCommand_Ack,	
	EStationCommand_SetDateTime,
	EStationCommand_Reset,			
	EStationCommand_Again,
	EStationCommand_Cut

} EStationCommand;

typedef enum 
{
	BoardCommand_Start,
	BoardCommand_Idel,			
	BoardCommand_Busy,			
	BoardCommand_RxCasseteInfo,		
	BoardCommand_RxCasseteQTY,
	BoardCommand_ShowCarbonEntry,
	BoardCommand_ShowCarbonNoEntry,
	BoardCommand_ShowPackPause,
	BoardCommand_ShowPackContinue,
	BoardCommand_ShowPackStop,
	BoardCommand_PaperMotorMove,
	BoardCommand_PaperMotorStop
} E232Command;
// --------------------------------------------------------

typedef struct 
{
	ETabletCommand Command;			
	ETabletState	 State;				
	byte 		 PacketA;
	byte 		 PacketB;
	unsigned int CasseteNum;		
	byte 	 	 Quantity;				
} TTablet ;

typedef struct
{	
	EStationCommand Command;
	EStationState 	State;	
	byte	StationNum;	
	byte        NumberA	;
	byte 		NumberB	;
	byte		BCC;
	byte		END;
	byte        CasseteQTY;		
} TStation;

typedef struct 
{
	E232Command	Command;	
	E232State	State;	
	
} T232Board;
typedef struct
{
	byte		HNumber;
	byte		TNumber;
	byte		SNumber;
	byte		QTY;
	byte		State;
} ECasseteNum ;

typedef struct
{
	EUART  		UART_Mark; 	
	TTablet 	Tablet;		
	TStation 	Station;		
	T232Board  	Board;		
	EErrorCode  ErrorCode; 	
	ECasseteNum CasseteInfo[5];
	byte 		StationInfo[5];	
	byte	    RecevieData[80];
} TPacker;


