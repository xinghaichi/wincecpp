#ifndef _COMPORT_H_INCLUDE
#define _COMPORT_H_INCLUDE
#ifdef DLLCOMPORT_EXPORTS
#define EXPORTS_COMPORT _declspec( dllexport )
#else
#define EXPORTS_COMPORT _declspec(dllimport)
#endif

BOOL PortInitialize (LPTSTR lpszPortName,int baudRate,int byteSize,int stopbits,int parity);
BOOL WriteBytesToPort(CHAR * name,BYTE * bytes ,INT len,USHORT waitTime,BYTE * result,INT & resultLen);
BOOL PortClose ();
#endif