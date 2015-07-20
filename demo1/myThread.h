#include "StdAfx.h"

#ifndef __SG2D_FD_THREAD_H__
#define __SG2D_FD_THREAD_H__

#include "Object.h"
#include <Windows.h>
#include <winbase.h>
#include <processthreadsapi.h>

/**
*
* SG2DFD::Thread 线程类
*/
class Thread : public Object
{
public:
	typedef Object super;

protected:
	void* m_pThread;//系统线程句柄
	volatile bool m_boStop;//线程停止标志
private:
	bool  m_boAutoRelease;//线程停止后自动销毁标志

public:
	Thread() :super()
	{
		m_pThread = NULL;
		m_boStop = false;
		m_boAutoRelease = false;
	}

	virtual ~Thread()
	{
		stop();
		wait();
	}

	//返回系统线程句柄
	inline const void* hostThread() const { return m_pThread; }

	//启动线程
	void start()
	{
		if (!m_pThread)
		{
			m_pThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread::WinThreadRoutine, this, CREATE_SUSPENDED, NULL);
			::ResumeThread((HANDLE)m_pThread);
		}
	}

	//停止线程
	void stop()
	{
		m_boStop = true;
	}

	//等待线程退出
	void wait()
	{
		if (m_pThread)
		{
			::WaitForSingleObject((HANDLE)m_pThread, INFINITE);
			::CloseHandle((HANDLE)m_pThread);
			m_pThread = NULL;
		}
	}

	//设置线程停止后自动销毁
	void setAutoRelease(bool value){ m_boAutoRelease = value; }

	//休眠函数
	static void sleep(unsigned int milSec)
	{
		::Sleep(milSec);
	}

	//获取当前线程ID（注意：非本线程类的线程ID，而是当前调用此函数的线程的ID）
	static int currentThreadId()
	{
		return GetCurrentThreadId();
	}

protected:
	//线程主函数
	virtual int threadMain() = 0;

private:
	static void WinThreadRoutine(Thread *thread)
	{
		int ret = thread->threadMain();
		if (thread->m_boAutoRelease)
		{
			::CloseHandle((HANDLE)thread->m_pThread);
			thread->m_pThread = NULL;
			thread->release();
		}
		::ExitThread(ret);
	}
	static void PosixThreadRoutine(Thread *thread);
};

#endif
