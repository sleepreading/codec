
#ifndef __QUICKSOCKET__H__
#define __QUICKSOCKET__H__

#ifdef __cplusplus
extern "C" {
#endif

#define  MAX_QS_TIMEOUT    0x7FFFFFFF

#include <winsock.h>

typedef void (*ACCEPT_CALLBACK)(SOCKET sListenSock, SOCKET nNewSock,SOCKADDR_IN *pInAddrCl=NULL, void *pUPDInData=NULL, DWORD dwUDPInDataLen=NULL);

BOOL    StartUpSock();
VOID    CleanupSock();
BOOL    StopServerSocket(SOCKET nSock) ;

SOCKET  QuickStartTCPServer(WORD wListenPort,ACCEPT_CALLBACK lpfnCB_Accept=NULL);
SOCKET  QuickStartUDPServer(WORD wListenPort,BOOL bBoradCast=TRUE,ACCEPT_CALLBACK lpfnCB_Accept=NULL,DWORD dwProtoHeadLen=0);
BOOL    WaitSocketData(SOCKET socket,int timeout,BOOL bWaitToRecv=TRUE);
SOCKET  QuickConnectToTCP(WORD wServerPort,LPCSTR lpServerIP=NULL,DWORD dwTimeOut=2000);
SOCKET  QuickSendToUDP(WORD wPort, LPCSTR szDataBuffer, DWORD dwSize, BOOL bRetSock=FALSE,LPCSTR  lpServerIP=NULL);
SOCKET  AcceptTCPClient(SOCKET nListenSock,int timeout, SOCKADDR_IN *pInAddr=NULL);

BOOL    SetRecvSendBufferSize(SOCKET nSocket, DWORD dwBufferSize=0x10000);
BOOL    SetRecvTimeOut(SOCKET nSocket, DWORD dwTimeOut=MAX_QS_TIMEOUT);
BOOL    SetSendTimeOut(SOCKET nSocket, DWORD dwTimeOut=MAX_QS_TIMEOUT);
DWORD   ForceSend(SOCKET sock,void *pszBuf,DWORD dwLen=0,DWORD dwTimeOut=MAX_QS_TIMEOUT,BOOL bUDPFlag=FALSE, SOCKADDR_IN *pCLAddr=NULL);
DWORD   ForceRecv(SOCKET sock,void *pszBuf,DWORD dwLen,DWORD dwTimeOut=5000,BOOL bUDPFlag=FALSE, SOCKADDR_IN *pCLAddr=NULL);

VOID    NotifySocketToEnd(WORD wPort, BOOL bIsTcpMode=FALSE);

#ifdef __cplusplus
}
#endif

#endif

