#pragma once
#include <atomic>

class Spinlock
{
public:
	Spinlock() : state_(LockState::Unlocked) {}
	void lock()
	{
		while (state_.exchange(LockState::Locked, std::memory_order_acquire) == LockState::Locked) {
			/* busy-wait */
		}
	}
	void unlock()
	{
		state_.store(LockState::Unlocked, std::memory_order_release);
	}

private:
	enum class LockState { Locked, Unlocked };
	std::atomic<LockState> state_;
};


