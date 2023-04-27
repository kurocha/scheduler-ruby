//
//  After.cpp
//  File file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 29/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "After.hpp"

#include <Concurrent/Fiber.hpp>

#include <cassert>

#if defined(SCHEDULER_EPOLL)
#include <sys/timerfd.h>
#endif

namespace Scheduler
{
	using namespace Concurrent;
	
	void After::wait()
	{
		assert(Fiber::current);
		auto reactor = Reactor::current;
		assert(reactor);

#if defined(SCHEDULER_KQUEUE)
		reactor->append({
			reinterpret_cast<uintptr_t>(this),
			EVFILT_TIMER,
			EV_ADD | EV_ONESHOT | EV_UDATA_SPECIFIC,
			// TODO this may overflow
			NOTE_USECONDS,
			static_cast<intptr_t>(static_cast<double>(_duration) * 1000 * 1000),
			Fiber::current
		});
		
		auto defer_removal = defer([&]{
			reactor->append({
				reinterpret_cast<uintptr_t>(this),
				EVFILT_TIMER,
				EV_DELETE | EV_UDATA_SPECIFIC,
				0,
				0,
				Fiber::current
			}, false);
		});
		
		reactor->transfer();
		
		defer_removal.cancel();
#elif defined(SCHEDULER_EPOLL)
		// TODO cache the timer handle:
		Handle timer_handle = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
		
		reactor->append(EPOLL_CTL_ADD, timer_handle, EPOLLIN|EPOLLET, (void*)Fiber::current);
		
		struct itimerspec value;
		value.it_value = _duration.value();
		value.it_interval = {0, 0};
		
		::timerfd_settime(timer_handle, 0, &value, nullptr);
		reactor->transfer();
#endif
	}
}
