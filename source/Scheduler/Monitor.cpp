//
//  Monitor.cpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 8/5/2018.
//  Copyright, 2018, by Samuel Williams. All rights reserved.
//

#include "Monitor.hpp"

#include <ruby/io.h>
#include <fcntl.h>

#ifndef RUBY_IO_MODE_EXTERNAL
#define RUBY_IO_MODE_EXTERNAL FMODE_EXTERNAL
#endif

namespace Scheduler
{
	Monitor::Monitor(Descriptor descriptor) : _descriptor(descriptor)
	{
		_io = rb_io_open_descriptor(rb_cIO, _descriptor, O_RDWR | RUBY_IO_MODE_EXTERNAL, Qnil, Qnil, NULL);
	}
	
	Monitor::Event Monitor::wait_readable(const Timestamp * timeout)
	{
		return wait(Event::READABLE, timeout);
	}
	
	Monitor::Event Monitor::wait_writable(const Timestamp * timeout)
	{
		return wait(Event::WRITABLE, timeout);
	}
	
	Monitor::Event Monitor::wait(Event events, const Timestamp * timeout)
	{
		VALUE timeout_value = Qnil;
		
		if (timeout) {
			Timestamp now;
			auto duration = *timeout - now;
			
			if (duration.as_seconds() <= 0) {
				// Already timed out
				return Event::NONE;
			}
			
			timeout_value = rb_float_new(duration.as_seconds());
		}
		
		VALUE result = rb_io_wait(_io, RB_INT2NUM(events), timeout_value);
		
		if (result == Qfalse) {
			// Timeout occurred
			return Event::NONE;
		}
		
		// Return the event that occurred
		return static_cast<Event>(RB_NUM2INT(result));
	}
}
