#ifndef __SPINLOCK_HPP__
#define __SPINLOCK_HPP__

#include <atomic>
#include <thread>
#include <cassert>

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

class ReentrantLock32 {
	std::atomic<std::size_t> m_atomic;
	std::int32_t m_refCount;
public:
	ReentrantLock32() : m_atomic(0), m_refCount(0) {}
	void Acquire()
	{
		std::hash<std::thread::id> hasher;
		std::size_t tid = hasher(std::this_thread::get_id());
		//if this thread does not hold lock
		if (m_atomic.load(std::memory_order_relaxed) != tid) {
			//spin until it holds
			std::size_t unlockVal = 0;
			while (!m_atomic.compare_exchange_weak(
				unlockVal,
				tid,
				std::memory_order_relaxed, //fence below
				std::memory_order_relaxed
				))
			{
				unlockVal = 0;
				_mm_pause();
			}
		}
		//increment ref count
		++m_refCount;
		//ensure next valid reads by fence acquire
		std::atomic_thread_fence(std::memory_order_acquire);
	}

	void Release()
	{
		//use release semantics - fence to ensure prior writes
		std::atomic_thread_fence(std::memory_order_release);
		std::hash<std::thread::id> hasher;
		std::size_t tid = hasher(std::this_thread::get_id());
		std::size_t actual = m_atomic.load(std::memory_order_relaxed);
		assert(actual == tid);
		--m_refCount;
		if (m_refCount == 0)
		{
			//release safely, cause we own it
		}
		m_atomic.store(0, std::memory_order_relaxed);
	}

	bool TryAcquire()
	{
		std::atomic_thread_fence(std::memory_order_release);
		std::hash<std::thread::id> hasher;
		std::size_t tid = hasher(std::this_thread::get_id());
		bool acquired = false;
		if (m_atomic.load(std::memory_order_relaxed) == tid)
		{
			acquired = true;
		}
		else
		{
			std::size_t unlockValue = 0;
			acquired = m_atomic.compare_exchange_strong(unlockValue, tid,
				std::memory_order_relaxed, std::memory_order_relaxed);
		}
		if (acquired)
		{
			++m_refCount;
			std::atomic_thread_fence(std::memory_order_acquire);
		}
		return acquired;
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