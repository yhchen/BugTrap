// demo1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <WinDef.h>
#include <WinBase.h>
#include "BugTrap.h"
#include <imagehlp.h>
#include <assert.h>
#include "myThread.h"

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

class MyThread : public Thread
{
protected:
	//线程主函数
	virtual int threadMain()
	{
		while (true)
		{
			if (!(rand() % 100))
			{
				dump();
			}
			Sleep(50);
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	initMiniDumpInstance();
	MyThread* pThread = new MyThread;
	pThread->start();
	while (true)
	{
		Sleep(1000);
	}
	return 0;
}

