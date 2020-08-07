// SimpProtocol.h: interface for the CSimpProtocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPPROTOCOL_H__E637A825_83B9_457E_A4AE_71DDE3CC224D__INCLUDED_)
#define AFX_SIMPPROTOCOL_H__E637A825_83B9_457E_A4AE_71DDE3CC224D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimpProtocol  
{
public:
	CSimpProtocol();
	virtual ~CSimpProtocol();

	// operation
public:
	void DoBuilString();
	void DecodeString();

};

#endif // !defined(AFX_SIMPPROTOCOL_H__E637A825_83B9_457E_A4AE_71DDE3CC224D__INCLUDED_)
