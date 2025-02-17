#ifndef __SPINLOCK_HPP__
#define __SPINLOCK_HPP__

#include <atomic>
#include <thread>

class SpinLock {
	std::atomic_flag m_atomic;
public:
	SpinLock() = default;

	bool TryAcquire()
	{
		//acquire fence so all subsequent reads by this thread valid
		bool alreadyLocked = m_atomic.test_and_set(std::memory_order_acquire);
		return !alreadyLocked;
	}

	void Acquire()
	{
		while (!TryAcquire())
		{
			//check difference with _mm_pause()
			std::this_thread::yield();
			//_mm_pause()
		}
	}

	void Release()
	{
		//ensure all prior writes fully vommited
		m_atomic.clear(std::memory_order_release);
	}
};

template<class LOCK>
class ScopedLock {
	typedef LOCK lock_t;
	lock_t* m_pLock;
public:
	explicit ScopedLock(lock_t& lock) : m_pLock(&lock)
	{
		m_pLock->Acquire();
	}
	~ScopedLock()
	{
		m_pLock->Release();
	}
};

SpinLock g_lock;

void TestLocks()
{
	ScopedLock<decltype(g_lock)> janitor(g_lock);
	
	int a = 1;
	bool check = a < 1 ? false : true;

	if (!a)
	{
		//if condition would be true, lock will be released here
		return;
	}

	/*
	imagine some processing here
	*/

	//otherwise here
	return;
}


#endif