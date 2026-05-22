//
//  Monitor.hpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 8/5/2018.
//  Copyright, 2018, by Samuel Williams. All rights reserved.
//

#pragma once

#include <ruby.h>
#include <Time/Interval.hpp>

namespace Scheduler
{
	using Descriptor = int;
	using Timestamp = Time::Timestamp;
	
	class Monitor final
	{
	public:
		Monitor(Descriptor descriptor);
		~Monitor() {}
		
		enum Event {
			NONE = 0,
			READABLE = RB_IO_WAIT_READABLE,
			WRITABLE = RB_IO_WAIT_WRITABLE,
		};
		
		Event wait_readable(const Timestamp * timeout = nullptr);
		Event wait_writable(const Timestamp * timeout = nullptr);
		
		Event wait(Event event, const Timestamp * timeout = nullptr);
		
	protected:
		Descriptor _descriptor;
		VALUE _io = Qnil;
	};
}
