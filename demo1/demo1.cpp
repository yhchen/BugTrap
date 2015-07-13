// demo1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <WinDef.h>
#include <WinBase.h>
#include "BugTrap.h"
#include <imagehlp.h>
#include <assert.h>

static void initMiniDumpInstance()
{
	BT_InstallSehFilter();
	
	BT_SetAppName((LPCTSTR)"demo execute file");
	BT_SetSupportEMail(_T("myemail@myemail.com"));
	BT_SetFlags(BTF_DETAILEDMODE | BTF_EDITMAIL);
	BT_SetSupportServer(_T("bug.mydomain.org"), 9999);
	BT_SetDumpType(MiniDumpNormal);
	BT_SetSupportURL(_T(""));
}

void dump()
{
	while (true)
	{
		throw "help me";
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	initMiniDumpInstance();
	dump();
	return 0;
}

