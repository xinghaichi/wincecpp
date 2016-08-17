#include <windows.h>
#include <stdio.h>
#include "stdafx.h"
#define CRC_PRESET 0xFFFF;
#define CRC_POLYNOM  0x8408;


void printHex( char* sPrefix, unsigned char* baData, int dataLen);
short crc16(unsigned char data_p[], unsigned short length);
void printHexString(char* sPrefix, char* baData, DWORD dataLen);

void BuildFeigCmdWithCRC(BYTE in[], USHORT cmdLen,BYTE out[])
{
	short crc =  crc16(in,cmdLen-2);
	

	for(int i=0;i<cmdLen-2;i++)
	{
		out[i]=in[i];
	}
	out[cmdLen-2]=crc & 0xff;
	out[cmdLen-1] = (crc >> 8) & 0xff;
	printHexString("\nCommand:",(char *)out,9);
	
}


void printHexString(char* sPrefix, char* baData, DWORD dataLen)
{
	DWORD i;

	printf("%s  ", sPrefix);

	for(i = 0; i < dataLen;i++)		
	{
		printf("%02X ",baData[i]);
	}

	printf("\n");
}


void printHex( char* sPrefix, unsigned char* baData, int dataLen)
{
	int  i;

	printf("%s  ", sPrefix);

	for(i = 0; i < dataLen;i++)		
	{
		printf("%02X ",baData[i]);
	}

	printf("\n");
}


//void printHexString(CHAR* sPrefix, LPBYTE baData, DWORD dataLen)
//{
//	DWORD i;
//
//	printf("%s", sPrefix);
//
//	for(i = 0; i < dataLen;i++)		
//	{
//		printf("%02X ",baData[i]);
//	}
//	
//	printf("\n");
//}

INT cmpByte(LPBYTE array1,LPBYTE array2,INT len)
{
	INT i;

	for(i = 0; i < len; i++)
	{
		if(*array1++ != *array2++)
		{
			return i ? i: -1;
		}
	}
	return 0;
}

void copyByte(LPBYTE des, LPBYTE src, INT len)
{
	INT i;

	for(i = 0; i < len; i++)
	{
		*des++ = *src++;
	}
}


short crc16(unsigned char data_p[], unsigned short length){
	unsigned int crc = CRC_PRESET;
	for (int i = 0; i < length; i++) // cnt = number of protocol bytes without CRC
	{
		crc ^= data_p[i];
		for (int j = 0; j < 8; j++)
		{
			if (crc & 0x0001)
			{crc = (crc >> 1) ^ CRC_POLYNOM;}
			else
			{crc = (crc >> 1);}
		}
	}
	return crc;
}

unsigned short crc16_2(char *data_p, unsigned short length)
{
	unsigned char i;
	unsigned int data;
	unsigned int crc = CRC_PRESET;

	if (length == 0)
		return (~crc);

	do
	{
		for (i=0, data=(unsigned int)0xff & *data_p++;
			i < 8; 
			i++, data >>= 1)
		{
			if ((crc & 0x0001) ^ (data & 0x0001))
			{  crc = (crc >> 1) ^ CRC_POLYNOM;}
			else  crc >>= 1;
		}
	} while (--length);

	crc = ~crc;
	data = crc;
	crc = (crc << 8) | (data >> 8 & 0xff);

	return (crc);
}