//
//  Semaphore.cpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 26/4/2023.
//  Copyright, 2023, by Samuel Williams. All rights reserved.
//

#include "Semaphore.hpp"

namespace Scheduler
{
	extern "C" {
		// Ruby Queue C API
		VALUE rb_queue_pop(VALUE queue, VALUE timeout);
		void rb_queue_push(VALUE queue, VALUE value);
	}
	
	Semaphore::Semaphore(std::size_t count) : _count(count)
	{
		// Create a Ruby Queue for signaling
		VALUE rb_cQueue = rb_const_get(rb_cObject, rb_intern("Queue"));
		_queue = rb_funcall(rb_cQueue, rb_intern("new"), 0);
		rb_gc_register_address(&_queue);
	}
	
	Semaphore::~Semaphore()
	{
		rb_gc_unregister_address(&_queue);
	}
	
	bool Semaphore::acquire(const Timestamp * timeout)
	{
		while (_count == 0) {
			if (!wait(timeout)) return false;
		}
		
		_count -= 1;
		return true;
	}
	
	void Semaphore::release()
	{
		_count += 1;
		signal();
	}
	
	bool Semaphore::wait(const Timestamp * timeout)
	{
		VALUE timeout_value = Qnil;
		
		if (timeout) {
			Timestamp now;
			auto duration = *timeout - now;
			
			if (duration.as_seconds() <= 0) {
				// Already timed out
				return false;
			}
			
			timeout_value = rb_float_new(duration.as_seconds());
		}
		
		// Pop from queue with timeout
		VALUE result = rb_funcall(_queue, rb_intern("pop"), 1, timeout_value);
		
		// nil return means timeout
		return result != Qnil;
	}
	
	void Semaphore::broadcast()
	{
		// Close and reopen the queue to wake all waiters
		rb_funcall(_queue, rb_intern("close"), 0);
		
		VALUE rb_cQueue = rb_const_get(rb_cObject, rb_intern("Queue"));
		_queue = rb_funcall(rb_cQueue, rb_intern("new"), 0);
	}
	
	void Semaphore::signal(std::size_t count)
	{
		for (std::size_t i = 0; i < count; ++i) {
			rb_funcall(_queue, rb_intern("push"), 1, Qtrue);
		}
	}
}
