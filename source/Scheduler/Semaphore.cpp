//
//  Semaphore.cpp
//  This file is part of the "Scheduler" project and released under the .
//
//  Created by Samuel Williams on 26/4/2023.
//  Copyright, 2023, by Samuel Williams. All rights reserved.
//

#include "Semaphore.hpp"

#include "Reactor.hpp"
#include "Defer.hpp"

namespace Scheduler
{
	Semaphore::~Semaphore()
	{
		while (!_waiting.empty()) {
			broadcast();
		}
	}
	
	void Semaphore::acquire()
	{
		while (_count == 0) {
			wait();
		}
		
		_count -= 1;
	}
	
	void Semaphore::release()
	{
		_count += 1;
		signal();
	}
	
	void Semaphore::wait()
	{
		Fiber * fiber = Fiber::current;
		
		auto iterator = _waiting.insert(_waiting.end(), fiber);
		
		auto defer_cleanup = Defer([&]{
			_waiting.erase(iterator);
		});
		
		assert(Reactor::current);
		Reactor::current->transfer();
	}
	
	void Semaphore::broadcast()
	{
		while (!_waiting.empty()) {
			auto fiber = _waiting.front();
			
			assert(Reactor::current);
			Reactor::current->transfer(fiber);
		}
	}
	
	void Semaphore::signal(std::size_t count)
	{
		while (!_waiting.empty() && count) {
			auto fiber = _waiting.front();
			
			count -= 1;
			
			assert(Reactor::current);
			Reactor::current->transfer(fiber);
		}
	}
}
