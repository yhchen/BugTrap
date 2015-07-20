#include "StdAfx.h"

#ifndef __SG2D_FD_THREAD_H__
#define __SG2D_FD_THREAD_H__

#include "Object.h"
#include <Windows.h>
#include <winbase.h>
#include <processthreadsapi.h>

/**
*
* SG2DFD::Thread �߳���
*/
class Thread : public Object
{
public:
	typedef Object super;

protected:
	void* m_pThread;//ϵͳ�߳̾��
	volatile bool m_boStop;//�߳�ֹͣ��־
private:
	bool  m_boAutoRelease;//�߳�ֹͣ���Զ����ٱ�־

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

	//����ϵͳ�߳̾��
	inline const void* hostThread() const { return m_pThread; }

	//�����߳�
	void start()
	{
		if (!m_pThread)
		{
			m_pThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread::WinThreadRoutine, this, CREATE_SUSPENDED, NULL);
			::ResumeThread((HANDLE)m_pThread);
		}
	}

	//ֹͣ�߳�
	void stop()
	{
		m_boStop = true;
	}

	//�ȴ��߳��˳�
	void wait()
	{
		if (m_pThread)
		{
			::WaitForSingleObject((HANDLE)m_pThread, INFINITE);
			::CloseHandle((HANDLE)m_pThread);
			m_pThread = NULL;
		}
	}

	//�����߳�ֹͣ���Զ�����
	void setAutoRelease(bool value){ m_boAutoRelease = value; }

	//���ߺ���
	static void sleep(unsigned int milSec)
	{
		::Sleep(milSec);
	}

	//��ȡ��ǰ�߳�ID��ע�⣺�Ǳ��߳�����߳�ID�����ǵ�ǰ���ô˺������̵߳�ID��
	static int currentThreadId()
	{
		return GetCurrentThreadId();
	}

protected:
	//�߳�������
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
