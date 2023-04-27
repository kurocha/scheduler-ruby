//
//  After.cpp
//  File file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 29/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "After.hpp"

#include <ruby.h>

namespace Scheduler
{
	void After::wait()
	{
		rb_funcall(rb_mKernel, rb_intern("sleep"), 1, rb_float_new(_duration.seconds()));
	}
}
