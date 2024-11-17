
// WirujaceKola.h: główny plik nagłówkowy aplikacji PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "dołącz nagłówek „pch.h” przed dołączeniem tego pliku na potrzeby optymalizacji PCH"
#endif

#include "resource.h"		// główne symbole


// CWirujaceKolaApp:
// Aby uzyskać implementację klasy, zobacz WirujaceKola.cpp
//

class CWirujaceKolaApp : public CWinApp
{
public:
	CWirujaceKolaApp();

// Przesłania
public:
	virtual BOOL InitInstance();

// Implementacja

	DECLARE_MESSAGE_MAP()
};

extern CWirujaceKolaApp theApp;
