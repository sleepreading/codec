#include "StdAfx.h"
#include "QueueBuf.h"

CQueueBuf::CQueueBuf()
{
	m_dwTotal = 40;
}

CQueueBuf::~CQueueBuf()
{
	Empty();
}

void CQueueBuf::Empty()
{
	if (m_listBuf.IsEmpty() == FALSE)
	{
		EncData_s* pBuf = NULL;
		POSITION pos = m_listBuf.GetHeadPosition();
		while (pos)
		{
			pBuf = (EncData_s*)m_listBuf.GetNext(pos);
			if (pBuf)
			{
				free(pBuf->pDataEnc);
				free(pBuf);
			}
		}
	}
	m_listBuf.RemoveAll();
}

bool CQueueBuf::GetBuf(EncData_s* pBuf)
{
	bool bRet = false;
	m_csListBuf.Lock();
	int iCount = m_listBuf.GetCount();

	if( iCount > 0)
	{
		EncData_s* pRet = (EncData_s*)m_listBuf.RemoveHead();
		if (pRet)
		{
			memcpy(pBuf->pDataEnc,pRet->pDataEnc,pRet->length);
			free(pRet->pDataEnc);
			free(pRet);
		}
		bRet = true;
	}
	m_csListBuf.Unlock();
	
	return bRet;
}

bool CQueueBuf::AddBuf(EncData_s* pBuf)
{
	bool bRet = false;
	m_csListBuf.Lock();
	int iCount = m_listBuf.GetCount();
	//TRACE("--- video buf len:%d ---\n",iCount);			// usually, 0 - 2
	if(iCount>=0 && iCount<m_dwTotal)
	{
		m_listBuf.AddTail(pBuf);
		bRet = true;
	}
	m_csListBuf.Unlock();
	
	return bRet;
}

void CQueueBuf::Rest( DWORD dwTime )
{
	int tmp = m_listBuf.GetCount();
	if (tmp < m_dwTotal && tmp > m_dwTotal/4)
	{
		Sleep(dwTime);
	}
	else
		Sleep(10);
}
