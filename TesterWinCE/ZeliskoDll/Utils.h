#ifndef _UTIL_H_INCLUDE
#define _UTIL_H_INCLUDE
#ifdef DLLUTIL_EXPORTS
#define EXPORTS_UTIL _declspec( dllexport )
#else
#define EXPORTS_UTIL _declspec(dllimport)
#endif

void printHexString(char* sPrefix, char* baData, DWORD dataLen);
INT cmpByte(LPBYTE array1,LPBYTE array2,INT len);
void copyByte(LPBYTE des, LPBYTE src, INT len);
short crc16(unsigned char data_p[], unsigned short length);
unsigned short crc16_2(char *data_p, unsigned short length);
void printHex( char* sPrefix, unsigned char* baData, int dataLen);
//BYTE * BuildFeigCmdWithCRC(BYTE in[], USHORT cmdLen);
//VOID  BuildFeigSelectCmdWithCRC(BYTE UID[], INT UIDLen,BYTE * cmd, INT &  cmdLen );
#endif