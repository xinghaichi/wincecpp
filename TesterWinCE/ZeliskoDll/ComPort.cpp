#include "stdafx.h"
#include "ComPort.h"
DWORD ReadThread(LPVOID pParam);


HANDLE hPort;
HANDLE hReadThread; 
static BYTE s_response[1024];
static int s_responseLen;

BOOL  WriteBytesToPort(CHAR * name,BYTE * bytes ,INT len,USHORT waitTime,BYTE * result,INT & resultLen)

{
	DWORD dwError,dwNumBytesWritten;
	//printf("\nCommand %s : " ,name);
	while(len>0)
	{	
		printf(" %02X ",*bytes);
		

		if (!WriteFile (hPort,              // Port handle
			bytes,              // Pointer to the data to write
			1,                  // Number of bytes to write
			&dwNumBytesWritten, // Pointer to the number of bytes written
			NULL))              // Must be NULL for Windows CE
		{

			dwError = GetLastError ();
			return false;
		}
		bytes++;
		len--;
	}

	Sleep(waitTime);
	memcpy(result,s_response,s_responseLen);
    resultLen=s_responseLen;
	//printf("\n");
	return true;
}



DWORD ReadThread(LPVOID pParam)
{
	DWORD dwThreadID=0;
	HANDLE hsPort =pParam;
	BYTE Byte;
	int iCounter = 0;
	DWORD dwBytes;
	char ReceiveBuf[1024];
	SetCommMask (hsPort, EV_RXCHAR);//Only receive char
	while (hsPort != INVALID_HANDLE_VALUE) 
	{DWORD dwCommStatus;
	WaitCommEvent(hsPort, &dwCommStatus, 0);
	SetCommMask (hsPort, EV_RXCHAR);//reset the wait signal
	do{	ReadFile(hsPort, &Byte, 1, &dwBytes, 0); 
	if(dwBytes == 1){
		ReceiveBuf[iCounter++] = Byte;
		if(iCounter == 1024)//buffer for receive is full!
		{
			return -1;
		}
	}
	}while (dwBytes == 1);
	memcpy(s_response+s_responseLen,ReceiveBuf,iCounter);
	s_responseLen+=iCounter;
	iCounter = 0;
	}
	return 0;
}

BOOL PortInitialize (LPTSTR lpszPortName,int baudRate,int byteSize,int stopbits,int parity)
{
	DWORD dwError,
		dwThreadID;
	DCB PortDCB;
	COMMTIMEOUTS CommTimeouts;
	hPort = CreateFile (lpszPortName, // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE,
		// Access (read-write) mode
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		OPEN_EXISTING,// How to open the serial port
		0,            // Port attributes
		NULL);        // Handle to port with attribute
	// to copy
	// If it fails to open the port, return FALSE.
	if ( hPort == INVALID_HANDLE_VALUE )
	{
		dwError = GetLastError ();
		return FALSE;
	}
	PortDCB.DCBlength = sizeof (DCB);    
	// Get the default port setting information.
	GetCommState (hPort, &PortDCB);
	// Change the DCB structure settings.
	PortDCB.BaudRate = baudRate;              // Current baud
	PortDCB.fBinary = TRUE;               // Binary mode; no EOF check
	PortDCB.fParity = TRUE;               // Enable parity checking
	PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control
	PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control
	PortDCB.fDtrControl = DTR_CONTROL_ENABLE; // DTR flow control type
	PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity
	PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx
	PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control
	PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control
	PortDCB.fErrorChar = FALSE;           // Disable error replacement
	PortDCB.fNull = FALSE;                // Disable null stripping
	PortDCB.fRtsControl = RTS_CONTROL_ENABLE;// RTS flow control
	PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on error
	PortDCB.ByteSize = byteSize;                 // Number of bits/byte, 4-8
	PortDCB.Parity = parity;            // 0-4=no,odd,even,mark,space
	PortDCB.StopBits = stopbits;        // 0,1,2 = 1, 1.5, 2
	// Configure the port according to the specifications of the DCB
	// structure.
	if (!SetCommState (hPort, &PortDCB))
	{
		// Could not create the read thread.
		// MessageBox (hMainWnd, TEXT(“Unable to configure the serial port”),
		//               TEXT(“Error”), MB_OK);
		dwError = GetLastError ();
		return FALSE;
	}
	// Retrieve the time-out parameters for all read and write operations
	// on the port.
	GetCommTimeouts (hPort, &CommTimeouts);
	// Change the COMMTIMEOUTS structure settings.
	CommTimeouts.ReadIntervalTimeout = MAXDWORD; 
	CommTimeouts.ReadTotalTimeoutMultiplier = 0; 
	CommTimeouts.ReadTotalTimeoutConstant = 0;   
	CommTimeouts.WriteTotalTimeoutMultiplier = 10; 
	CommTimeouts.WriteTotalTimeoutConstant = 1000;   
	// Set the time-out parameters for all read and write operations
	// on the port.
	if (!SetCommTimeouts (hPort, &CommTimeouts))
	{
		// Could not create the read thread.
		//MessageBox (hMainWnd, TEXT(“Unable to set the time-out parameters”),
		//           TEXT(“Error”), MB_OK);
		dwError = GetLastError ();
		return FALSE;
	}
	// Direct the port to perform extended functions SETDTR and SETRTS
	// SETDTR: Sends the DTR (data-terminal-ready) signal.
	// SETRTS: Sends the RTS (request-to-send) signal.
	EscapeCommFunction (hPort, SETDTR);
	EscapeCommFunction (hPort, SETRTS);
	// Create a read thread for reading data from the communication port.
	if (hReadThread = CreateThread (NULL, 0, ReadThread, hPort, 0, &dwThreadID))
	{	
		CloseHandle (hReadThread);

	}
	else
	{
		// Could not create the read thread.
		dwError = GetLastError ();
		return FALSE;
	}
	//	wprintf(L"Port :%s is open!\n",lpszPortName);
	//wcscpy( lpszDevName,lpszPortName);
	return TRUE;
}



BOOL PortClose ()
{
	DWORD dwError;
	if (hPort != INVALID_HANDLE_VALUE)
	{
		// Close the communication port.
		if (!CloseHandle (hPort))
		{
			dwError = GetLastError ();
			return FALSE;
		}
		else
		{
			hPort = INVALID_HANDLE_VALUE;
			//printf("\nPort closed !\n");
			return TRUE;
		}
	}
	return FALSE;
}