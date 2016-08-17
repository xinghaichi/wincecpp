#include "stdafx.h"
#include "Feig.h"
#include "ComPort.h"
#include "Utils.h"

#define BUF_SIZE 1024
BYTE CmdInvertory[] = {0x02, 0x00 , 0x09 , 0xFF , 0xB0 , 0x01 , 0x00, 0x18, 0x43};
BYTE CmdApduSelectApp[] = {0x02 ,0x00 ,0x1B ,0xFF ,0xB2 ,0xBE ,0x81 ,0x00 ,0xA4 ,0x04 ,0x0C ,0x0C ,0xD2 ,0x76 ,0x00 ,0x01 ,0x35 ,0x4B ,0x41 ,0x4E ,0x4D ,0x30 ,0x31 ,0x00 ,0x00 ,0x48 ,0x66 };


BOOL ParseInventory(BYTE * invertoryResult, INT invertoryResultLen ,  BYTE * uId, INT & uIdLen  ,CHAR * error, INT & errorLength,BOOL & Detected);

BOOL InitPort (LPTSTR Portname,INT baudRate,INT byteSize,INT stopbits,INT parity)
{
	return  PortInitialize (Portname, baudRate, byteSize, stopbits, parity)	;
}

BOOL ClosePort()
{
	return PortClose();
}


BOOL ScanTransponder(BYTE * uid, INT & uidLen ,CHAR * err,INT & errLen)
{
	int len = 0;
	BYTE Result[1024];
	BOOL TransponderDetected;
	if(!WriteBytesToPort("CmdInvertory",CmdInvertory,9,200,Result,len))
	{return false;}
	
	if(!ParseInventory(Result,len,uid,uidLen,err,errLen,TransponderDetected))
	{return false;}

	return true;
}


VOID  BuildFeigSelectCmdWithCRC(BYTE * UID, INT UIDLen, BYTE * cmd, INT &  cmdLen )
{

	/*02 00 11 FF   B0  
	CMD Select:25 
	Mode: 21  
	UID 00 00 00 00 99 F1 A6 91    
	CRC:92 77 */
	BYTE cmdAdr=0xFF;
	BYTE cmdTyp = 0xB0;//ISO Standard Host Commands
	BYTE cmdFeig= 0x25;//SELECT
	BYTE cmdMode= 0x21;//Mode
	cmdLen = 17;

	BYTE  tmp[] = {(BYTE)0x02,(BYTE)0x00,(BYTE)0x11,cmdAdr,cmdTyp,cmdFeig,cmdMode};
	memcpy(cmd,tmp,sizeof(tmp));
	cmdLen = sizeof(tmp);
	//printf("\ncmdLen:%d",cmdLen);
	//printHex("\ntmp0:",cmd,cmdLen);
	memcpy(cmd+cmdLen,UID,UIDLen);
	cmdLen += UIDLen;
	//printf("\ncmdLen:%d",cmdLen);
	//printHex("\ntmp1:",cmd,cmdLen);
	short crc =  crc16(cmd,cmdLen);
	BYTE CRCValue[] = {crc & 0xff,(crc >> 8) & 0xff};
	//printHex("\nCRCValue:",CRCValue,sizeof(CRCValue));
	memcpy(cmd+cmdLen,CRCValue,sizeof(CRCValue));
	cmdLen+=sizeof(CRCValue);
	//printf("\ncmdLen:%d",cmdLen);
	//printHex("\nSelectCommand:",cmd,cmdLen);
}

BOOL SelectTransponder(BYTE * UID,INT  UID_Len)
{
	int len = 0;
	BYTE CmdSelect[BUF_SIZE];// pointer memory fordern
	BuildFeigSelectCmdWithCRC(UID,UID_Len,CmdSelect,len);
  
	printHex("\nBuildFeigSelectCmdWithCRC: ",CmdSelect,len);



	BYTE Result[1024];
	WriteBytesToPort("CmdSelect",CmdSelect,len,500,Result,len);
	
	
	printf("\nLength: %d\n",len);
	printHex("\nRuckgabe",Result,len);
	return true;
}



BOOL ParseInventory(BYTE * invertoryResult, INT invertoryResultLen ,  BYTE * uId, INT & uIdLen  ,CHAR * error, INT & errorLength,BOOL & Detected)
{
	Detected=false;


	CHAR * errMsg="OK";

	if(invertoryResultLen<1)
	{
		errMsg =  "Inventory error!";
		errorLength = strlen(errMsg);
		strcpy_s(error, errorLength,errMsg);
		return false;
	}
	if(*(invertoryResult+2)==0x08)
	{
		if(*(invertoryResult+5)==0x01)
		{	errMsg = "Transponder not on the Reader!";
		errorLength = strlen(errMsg);
		strcpy_s(error,errorLength, errMsg);
		return false;
		}
		if(*(invertoryResult+5)==0x10)
		{
			errMsg = "Reader: Fatal error(EPROM), restart device!";
			errorLength = strlen(errMsg);
			strcpy_s(error,errorLength,errMsg );
			return false;
		}
	}

	if(*(invertoryResult+2)>0x08 && *(invertoryResult+5)==0x00)
	{

		if(*(invertoryResult+7)!=0x04)
		{
			errMsg = "Transponder not on the Reader!";
			errorLength = strlen(errMsg);
			strcpy_s(error,errorLength, errMsg);
			return false;
		}
		else
		{
			uIdLen=8;
			Detected=true;
			memcpy(uId,invertoryResult+9,8);
			return true;
		}

	}

	errMsg = "Unknown error!";
	errorLength = strlen(errMsg);
	strcpy_s(error,errorLength, errMsg);
	return false;
}



BOOL SelectVdvKaApp(BYTE * result,INT & len)
{
	BYTE Result[1024];
	int Len;
	if(!WriteBytesToPort("CmdApduSelectApp",CmdApduSelectApp,27,500,Result,Len))
		return false;
	// Verify if status word SW1SW2 is equal 0x9000.
	if( Result[Len - 2] == 0x90 &&
		Result[Len - 1] == 0x00)
	{
		len = Len;
		memcpy(result,Result,Len);
		printHex("Select File KM): 0x", Result, len - 2);
		return true;
	}
	return false;
}
