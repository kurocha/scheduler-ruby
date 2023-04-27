//
//  Reactor.hpp
//  File file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 27/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Defer.hpp"

#include <stdexcept>

#include <vector>
#include <list>
#include <Time/Interval.hpp>

#if defined(SCHEDULER_EPOLL)
	#include <sys/epoll.h>
#elif defined(SCHEDULER_KQUEUE)
	#include <sys/types.h>
	#include <sys/event.h>
	#include <sys/time.h>
#else
	#error "Unable to determine Scheduler implementation."
#endif

#include "Handle.hpp"
#include "Fiber.hpp"

namespace Scheduler
{
	using Time::Interval;
	
	class Reactor final
	{
	public:
		static thread_local Reactor * current;
		struct Bound;

		Reactor();
		~Reactor();
		
		// Transfer to the reactor. The current fiber will be marked as waiting.
		void transfer();
		
		// Transfer to the specified fiber, mark the current fiber as ready.
		void transfer(Fiber * fiber);
		
		// Run the reactor until all fibers are completed.
		std::size_t run();
		
		// Run the reactor until all fibers are completed or the specified duration has elapsed.
		std::size_t run(Interval duration);
		
		const Handle & handle() const noexcept {return _selector;}
		Handle & handle() noexcept {return _selector;}
		
	private:
		Handle _selector;
		
		std::size_t _waiting = 0;
		std::list<Fiber *> _ready;
		std::size_t transfer_ready();
		
		// Wait indefinitely for events:
		std::size_t select();
		
		// Wait at most the specified duration for events:
		std::size_t select(Interval duration);
		
#if defined(SCHEDULER_EPOLL)
	public:
		std::size_t select_internal(int timeout);
		void append(int operation, Descriptor descriptor, int events, void * data);
		
	private:
		std::vector<struct epoll_event> _events;
#elif defined(SCHEDULER_KQUEUE)
	public:
		std::size_t select_internal(struct timespec * timeout);
		void append(const struct kevent & event, bool flush = true);
		
	private:
		std::vector<struct kevent> _changes;
		std::vector<struct kevent> _events;
#endif
	};
	
	struct Reactor::Bound {
		Reactor reactor;
		
		Bound()
		{
			if (Reactor::current != nullptr)
				throw std::runtime_error("Reactor::current is already set!");
			
			Reactor::current = &reactor;
		}
		
		~Bound()
		{
			Reactor::current = nullptr;
		}
	};
}
