//
//  Monitor.hpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 8/5/2018.
//  Copyright, 2018, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Reactor.hpp"

namespace Scheduler
{
	class Monitor final
	{
	public:
		Monitor(Descriptor descriptor) : _descriptor(descriptor) {}
		~Monitor();
		
		enum Event : int16_t {
			NONE = 0,
#if defined(SCHEDULER_KQUEUE)
			READABLE = EVFILT_READ,
			WRITABLE = EVFILT_WRITE,
#elif defined(SCHEDULER_EPOLL)
			READABLE = EPOLLIN,
			WRITABLE = EPOLLOUT,
#endif
		};
		
		void wait_readable();
		void wait_writable();
		
		void wait(Event event);
		
	protected:
		void remove();
		
		Fiber *_added = nullptr;
		Reactor *_reactor = nullptr;
		Event _events = NONE;
		
		Descriptor _descriptor;
		
		void append();
	};
}
