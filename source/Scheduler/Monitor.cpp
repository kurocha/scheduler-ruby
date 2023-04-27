//
//  Monitor.cpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 8/5/2018.
//  Copyright, 2018, by Samuel Williams. All rights reserved.
//

#include "Monitor.hpp"

#include <ruby/io.h>

namespace Scheduler
{
	Monitor::Monitor(Descriptor descriptor) : _descriptor(descriptor)
	{
		_io = rb_io_fdopen(_descriptor, 0, NULL);
	}
	
	void Monitor::wait_readable()
	{
		rb_io_wait(_io, RB_INT2NUM(RB_IO_WAIT_READABLE), Qnil);
	}
	
	void Monitor::wait_writable()
	{
		rb_io_wait(_io, RB_INT2NUM(RB_IO_WAIT_READABLE), Qnil);
	}
	
	void Monitor::wait(Event event)
	{
		rb_io_wait(_io, RB_INT2NUM(event), Qnil);
	}
}
