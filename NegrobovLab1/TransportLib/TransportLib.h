// TransportLib.h : main header file for the TransportLib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTransportLibApp
// See TransportLib.cpp for the implementation of this class
//

class CTransportLibApp : public CWinApp
{
public:
	CTransportLibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
