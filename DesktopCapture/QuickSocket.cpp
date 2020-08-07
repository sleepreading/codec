
#include "StdAfx.h"
#include "QuickSocket.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_SERSOCKETCOUNT 2048

typedef struct
{
	BOOL             bIsTCPMode   ;
	DWORD            dwUDPRecvLen ;
	SOCKET           nServerSock  ;
	LPVOID           lpExtData    ;		// save the server thread handle
	ACCEPT_CALLBACK  lpfnCB_Accpet ;
}ServerSockInfo;

static BOOL   s_bSockStarup = FALSE ;
static ServerSockInfo *s_ServrSockets[MAX_SERSOCKETCOUNT]={NULL} ;

static SOCKET GetNewSock(DWORD dwType = SOCK_STREAM) ;
static DWORD WINAPI ThreadServer(LPVOID lpParam);

BOOL StartUpSock()
{
	if(s_bSockStarup)
		return TRUE ;
	WORD dwVer = 0x0202 ;
	WSADATA  WSData={0} ;
	WSAStartup(dwVer,&WSData);
	s_bSockStarup = TRUE ;
	return TRUE ;
}

VOID CleanupSock()
{
	WSACleanup() ;
	s_bSockStarup = FALSE ;
}

SOCKET GetNewSock(DWORD dwType)
{
	StartUpSock() ;
	int nSocket = socket(AF_INET,dwType,0);
	if(nSocket != INVALID_SOCKET)
	{
		SetRecvSendBufferSize(nSocket,0x10000);
		SetRecvTimeOut(nSocket,5000);
	}

	return nSocket;
}

BOOL  StopServerSocket(SOCKET nSock)
{
	if(nSock == INVALID_SOCKET)
		return TRUE ;
	for(int i=0; i<MAX_SERSOCKETCOUNT; i++)
	{
		ServerSockInfo *pInfo=s_ServrSockets[i] ;
		if(pInfo!= NULL && pInfo->nServerSock == nSock)
		{
			closesocket(nSock);
			TerminateThread((HANDLE)pInfo->lpExtData,0);
			free(pInfo);
			s_ServrSockets[i] = NULL ;
			return TRUE ;
		}
	}
	return FALSE;
}


BOOL SetRecvSendBufferSize(SOCKET hSocket, DWORD dwBufferSize)
{
	if (hSocket == INVALID_SOCKET)
		return FALSE;
	setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (LPCSTR)&dwBufferSize, sizeof(DWORD));
	setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (LPCSTR)&dwBufferSize, sizeof(DWORD));
	return TRUE;
}

BOOL SetSendTimeOut(SOCKET hSocket, DWORD dwTimeOut)
{
	if (hSocket == INVALID_SOCKET)
		return FALSE;
	setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (LPCSTR) &dwTimeOut, sizeof(DWORD));
	return TRUE;
} 

BOOL SetRecvTimeOut(SOCKET hSocket, DWORD dwTimeOut)
{
	if (hSocket == INVALID_SOCKET)
		return FALSE;
	setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (LPCSTR) &dwTimeOut, sizeof(DWORD));
	return TRUE;
}

BOOL WaitSocketData(SOCKET socket,int timeout,BOOL bWaitToRecv)
{
	int    nCount = 0;
	fd_set fdWait = {0};
	timeval tmv ;
	tmv.tv_sec = timeout/1000;
	tmv.tv_usec= timeout%1000;
	if(socket == INVALID_SOCKET)
		return FALSE ;

	FD_ZERO(&fdWait);
	FD_SET(socket,&fdWait);

	if(bWaitToRecv)
		nCount = select(socket,&fdWait,NULL,NULL,&tmv) ;
	else
		nCount = select(socket,NULL,&fdWait,NULL,&tmv) ;
	
	return (nCount <= 0 || !FD_ISSET(socket,&fdWait)) ? FALSE:TRUE;
}

SOCKET  QuickConnectToTCP(WORD wServerPort,LPCSTR lpServerIP,DWORD dwTimeOut/*=2000*/)
{
	SOCKET  nSocket = socket(AF_INET,SOCK_STREAM,0);
	if (nSocket == INVALID_SOCKET) return INVALID_SOCKET;
	SetRecvSendBufferSize(nSocket,0x10000);

	SOCKADDR_IN sAddress={0};
	sAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(lpServerIP != NULL)
	{
		HOSTENT *pHost = gethostbyname(lpServerIP) ;
		if(pHost != NULL)
			memcpy(&sAddress.sin_addr,pHost->h_addr_list[0],4);
	}
	sAddress.sin_family = AF_INET;
		sAddress.sin_port = htons(wServerPort);

	unsigned long noBlocking = 1;
	if (ioctlsocket(nSocket,FIONBIO,&noBlocking) != 0)
	{
		closesocket(nSocket);
		return INVALID_SOCKET;
	}

	if (connect(nSocket,(sockaddr*)&sAddress,sizeof(sAddress)) == SOCKET_ERROR)
	{
		fd_set fds;
		FD_ZERO(&fds);
		timeval tm = {dwTimeOut/1000,dwTimeOut%1000};
		FD_SET(nSocket,&fds);
		int errSelect = select(0,NULL,&fds,NULL,&tm);
		if (errSelect == 1)
		{
			noBlocking = 0;
			ioctlsocket(nSocket,FIONBIO,&noBlocking);
			return nSocket;
		}
	}
	else
	{
		noBlocking = 0;
		ioctlsocket(nSocket,FIONBIO,&noBlocking);
		return nSocket;	
	}

	return INVALID_SOCKET;
}


// s_wPortDeviceSet, (char *)&Infos,sizeof(CamNetParam), FALSE, NULL
SOCKET  QuickSendToUDP(WORD wPort, LPCSTR szDataBuffer, DWORD dwSize, BOOL bRetSock,LPCSTR  lpServerIP)
{
	sockaddr_in addr_to={0};
	SOCKET      sockUDP = SOCKET_ERROR;
	// get host IP list
	char szHostName[128];
	if( gethostname(szHostName, 128) == 0 )
	{
		struct hostent * pHost = gethostbyname(szHostName); if (!pHost) return sockUDP;
		for( int i = 0; pHost->h_addr_list[i]!= NULL; i++ )  
		{
			// 发送端套接字
			sockUDP = socket(AF_INET,SOCK_DGRAM,0);
			SetRecvSendBufferSize(sockUDP,0x10000);
			// 发送端地址		
			addr_to.sin_family = AF_INET;
			addr_to.sin_port   = htons(wPort);
			addr_to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			HOSTENT *pHostTmp = NULL ;
			if (lpServerIP != NULL)
				pHostTmp = gethostbyname(lpServerIP) ;	
			if (pHostTmp == NULL)
			{
				BOOL bEnabel = TRUE;
				setsockopt(sockUDP, SOL_SOCKET, SO_BROADCAST, (LPCSTR) &bEnabel, sizeof(bEnabel));
			}
			else
			{
				memcpy(&addr_to.sin_addr,pHostTmp->h_addr_list[0],4);
			}
			
			// 本地 地址
			sockaddr_in addr_from={0};
			addr_from.sin_family = AF_INET;
			addr_from.sin_port = 8629;	// DEVICE_GETPORT_default 监听端口
			memcpy(&addr_from.sin_addr,pHost->h_addr_list[i],4);
			// bind it
			bind(sockUDP, (sockaddr*)&addr_from, sizeof(addr_from));
			
			int         nLen    = sizeof(sockaddr_in);
			sendto(sockUDP, szDataBuffer, dwSize, 0, (sockaddr *)&addr_to, nLen);
			//////////////////////////////////////////////////////////////////////////
		}
	}

	return bRetSock ;
}

SOCKET AcceptTCPClient(SOCKET nListenSock,int timeout, SOCKADDR_IN *pInAddr)
{
	int  nAddrSize   = sizeof(SOCKADDR_IN) ;
	SOCKET nNewSock  = INVALID_SOCKET ;
	SOCKADDR_IN cin  = {0};
	if(WaitSocketData(nListenSock,timeout,TRUE))
	{
		nNewSock = accept( nListenSock, (struct sockaddr *)&cin, &nAddrSize);
		SetRecvSendBufferSize(nNewSock);
		if(pInAddr != NULL)
			memcpy(pInAddr,&cin,nAddrSize);
	}
	return nNewSock ;
}



// if lpfnCB_Accept!=NULL, create a thread : ThreadServer
// 开启线程 ThreadServer,接收请求连接accept()并运行用户自定义的线程例程
SOCKET QuickStartTCPServer(WORD wListenPort,ACCEPT_CALLBACK lpfnCB_Accept)
{
	int    nReuse = 1 ;
	SOCKADDR_IN sin = {0} ;
	SOCKET nLSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

	sin.sin_family= AF_INET ;
	sin.sin_port  = htons(wListenPort) ;
	setsockopt(nLSock,SOL_SOCKET,SO_REUSEADDR,(char *)&nReuse,sizeof(int));
	if (bind(nLSock,(sockaddr *)&sin,sizeof(sin)) != INVALID_SOCKET)
	{
		// 使套接字变为被动
		// 在正在listen的套接字上执行listen不起作用
		listen(nLSock,5);

		if(lpfnCB_Accept != NULL)
		{
			DWORD dwID = 0 ;
			ServerSockInfo *pTCPInfo = (ServerSockInfo *)calloc(1,sizeof(ServerSockInfo));
			pTCPInfo->bIsTCPMode   = TRUE ;
			pTCPInfo->lpfnCB_Accpet = lpfnCB_Accept;
			pTCPInfo->nServerSock  = nLSock;
			pTCPInfo->lpExtData    = (HANDLE)CreateThread(NULL, 0, ThreadServer,pTCPInfo,0,&dwID) ;
		}

		return nLSock ;
	}
	closesocket(nLSock) ;
	return INVALID_SOCKET ;
}

SOCKET  QuickStartUDPServer(WORD wListenPort,BOOL bBoradCast,ACCEPT_CALLBACK lpfnCB_Accept,DWORD dwProtoHeadLen)
{
	int     nReuse  = 1 ;
	SOCKADDR_IN sin = {0} ;
	SOCKET nLSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 

	sin.sin_family= AF_INET ;
	sin.sin_port  = htons(wListenPort) ;

	if(bBoradCast)
		setsockopt(nLSock,SOL_SOCKET,SO_BROADCAST,(char *)&nReuse,sizeof(int));

	setsockopt(nLSock,SOL_SOCKET,SO_REUSEADDR,(char *)&nReuse,sizeof(int));
	setsockopt(nLSock,SOL_SOCKET,SO_OOBINLINE,(char *)&nReuse,sizeof(int));
	if(bind(nLSock,(sockaddr *)&sin,sizeof(sin)) != SOCKET_ERROR)
	{
		if(lpfnCB_Accept != NULL && dwProtoHeadLen>0)
		{
			DWORD dwID = 0 ;
			ServerSockInfo *pUDPInfo = (ServerSockInfo *)calloc(1,sizeof(ServerSockInfo));
			pUDPInfo->lpfnCB_Accpet = lpfnCB_Accept;
			pUDPInfo->dwUDPRecvLen = dwProtoHeadLen;
			pUDPInfo->nServerSock  = nLSock;
			pUDPInfo->lpExtData    = (HANDLE)CreateThread(NULL, 0, ThreadServer,pUDPInfo,0,&dwID) ;
		}

		return nLSock ;
	}
	TRACE("\n^^^^ QuickStartUDPServer  ERROR: %d ^^^^\n\n",GetLastError()) ;
	closesocket(nLSock) ;
	return INVALID_SOCKET ;
}

DWORD ForceSend(SOCKET sock,void *pBuf,DWORD dwLen,DWORD dwTimeOut,BOOL bUDPFlag, SOCKADDR_IN *pCLAddr)
{
	DWORD dwToSendLen = dwLen;
	DWORD dwSendSum = 0 ;
	int   nSendLen  = 0 ;
	char  *pszBuf = (char *)pBuf ;
	SetSendTimeOut(sock,dwTimeOut);
	if(dwLen == 0)
		dwToSendLen = strlen(pszBuf);
	while(1)
	{
		if(bUDPFlag)
		{
			int nLen = sizeof(sockaddr) ;
			nSendLen = sendto(sock,pszBuf + dwSendSum,dwToSendLen-dwSendSum,0,(SOCKADDR *)pCLAddr,nLen);
		}
		else
		{
			nSendLen = send(sock,pszBuf + dwSendSum,dwToSendLen-dwSendSum,0) ;
		}

		if(nSendLen != SOCKET_ERROR)
		{
			dwSendSum += nSendLen;
			if(dwToSendLen <= dwSendSum)
				break;
			continue;
		}
		break ;
	}
	
	return dwSendSum ;
}


DWORD ForceRecv(SOCKET sock,void *pBuf,DWORD dwLen,DWORD dwTimeOut,BOOL bUDPFlag, SOCKADDR_IN *pCLAddr)
{
	if( SOCKET_ERROR==sock || dwLen==0 || pBuf==NULL)
		return 0 ;

	SetRecvTimeOut(sock,dwTimeOut);
//	SetRecvSendBufferSize(sock,0x40000);

	DWORD dwRecvSum = 0 ;
	int   nRecvLen  = 0 ;
	char* szBuf = (char*)pBuf;
	if (bUDPFlag)
	{
		struct sockaddr_in addrFrom = {0};
		int nLen = sizeof(addrFrom);
		while (dwRecvSum < dwLen)
		{
			nRecvLen = recvfrom(sock,szBuf+dwRecvSum,dwLen-dwRecvSum,0,(struct sockaddr*)&addrFrom,&nLen);
			if (nRecvLen == SOCKET_ERROR) return SOCKET_ERROR;
			if (pCLAddr) memcpy(pCLAddr,&addrFrom,sizeof(addrFrom));
			dwRecvSum += nRecvLen;
		}
	}
	else
	{
		while (dwRecvSum < dwLen)
		{
			nRecvLen = recv(sock,szBuf+dwRecvSum,dwLen-dwRecvSum,0);
			if (nRecvLen == SOCKET_ERROR) return SOCKET_ERROR;
			dwRecvSum += nRecvLen;
		}

	}
	
	return dwRecvSum ;
}


VOID  NotifySocketToEnd(WORD wPort, BOOL bIsTcpMode)
{
	char szBuffer[128] = "NotifySocketToEnd";

	if (bIsTcpMode)
	{
		SOCKET nSocket = QuickConnectToTCP(wPort,"127.0.0.1");
		if (nSocket != INVALID_SOCKET)
		{
			send(nSocket, szBuffer, sizeof(szBuffer), 0);
			closesocket(nSocket);
		}
	}
	else
	{
		QuickSendToUDP(wPort, szBuffer, sizeof(szBuffer),FALSE,"127.0.0.1");
	}
}


DWORD WINAPI ThreadServer(LPVOID lpParam)
{
	ServerSockInfo *pInfo = (ServerSockInfo *)lpParam ;
	if (lpParam == NULL) return 0;

	CHAR   *pszUDPDataBuf = NULL ;
	SOCKET  nListenSock   = pInfo->nServerSock ;

	if (!pInfo->bIsTCPMode)
	{
		if (pInfo->dwUDPRecvLen>0)
			pszUDPDataBuf = (CHAR *)calloc(1,pInfo->dwUDPRecvLen+1);
		else
			return 0 ;
	}

	SOCKADDR_IN addr_cl={0};
	int  nAddrSize   = sizeof(SOCKADDR_IN) ;

	while(1)
	{
		if (pInfo->bIsTCPMode)
		{
			SOCKET nNewSock  = accept(nListenSock, (sockaddr *)&addr_cl, &nAddrSize);
			if (nNewSock == INVALID_SOCKET)
				break;

			// default : 0x10000
			SetRecvSendBufferSize(nNewSock);
			if (pInfo->lpfnCB_Accpet != NULL)
				pInfo->lpfnCB_Accpet(nListenSock,nNewSock,&addr_cl) ;
		}
		else
		{
			if(ForceRecv(nListenSock,pszUDPDataBuf,pInfo->dwUDPRecvLen,MAX_QS_TIMEOUT,TRUE,&addr_cl) == 0)
				break;

			if(pInfo->lpfnCB_Accpet != NULL)
				pInfo->lpfnCB_Accpet(nListenSock,0,&addr_cl,pszUDPDataBuf,pInfo->dwUDPRecvLen) ;
		}
	}
	return 0;
}

