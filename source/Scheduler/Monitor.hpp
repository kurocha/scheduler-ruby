//
//  Monitor.hpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 8/5/2018.
//  Copyright, 2018, by Samuel Williams. All rights reserved.
//

#pragma once

#include <ruby.h>

namespace Scheduler
{
	using Descriptor = int;
	
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
		
		void wait_readable();
		void wait_writable();
		
		void wait(Event event);
		
	protected:
		Descriptor _descriptor;
		VALUE _io = Qnil;
	};
}
