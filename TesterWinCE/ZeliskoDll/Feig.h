#include <string>
#include <iostream>
using namespace std;
#ifndef _FEIG_H_INCLUDE
#define _FEIG_H_INCLUDE
#ifdef DLLUTIL_EXPORTS
#define EXPORTS_FEIG _declspec( dllexport )
#else
#define EXPORTS_FEIG _declspec(dllimport)
#endif


BOOL InitPort (LPTSTR Portname,INT baudRate,INT byteSize,INT stopbits,INT parity);
BOOL ClosePort();
BOOL ScanTransponder(BYTE * UID, INT & UID_Len ,CHAR * ERR,INT & ERR_Len);
BOOL SelectTransponder(BYTE * UID,INT  UID_Len);
BOOL SelectVdvKaApp(BYTE * Result,INT & len);

#endif