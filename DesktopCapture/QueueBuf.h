/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
** �ļ�����	QueueBuf.h
** ���ߣ�	����
** ���ڣ�	11/25 ����  17:07:42
** ���ܣ�	play queue
\*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <afxcoll.h>

typedef struct EncData_s
{
	uint8_t *pDataEnc;
	int		length;
} EncData_s;


class CQueueBuf
{
public:
//////////////////////////////////////////////////////////////////////////
	CQueueBuf();
	virtual ~CQueueBuf();
	
	// operations	
	bool GetBuf(EncData_s* pBuf);
	bool AddBuf(EncData_s* pBuf);
	void Empty();
	void Rest(DWORD dwTime);

//////////////////////////////////////////////////////////////////////////
	CPtrList			m_listBuf;
	DWORD				m_dwTotal;	
	CCriticalSection	m_csListBuf;
};



#endif
