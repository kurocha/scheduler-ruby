//
//  Semaphore.hpp
//  This file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 26/4/2023.
//  Copyright, 2023, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Time/Interval.hpp>

#include <ruby.h>
#include <ruby/thread.h>
#include <cstdint>

namespace Scheduler
{
	using Timestamp = Time::Timestamp;
	
	class Semaphore final
	{
		VALUE _queue;
		std::size_t _count = 0;
		
	public:
		Semaphore(std::size_t count = 1);
		~Semaphore();
		
		struct Acquire
		{
			Semaphore & semaphore;
			
			Acquire(Semaphore & semaphore) : semaphore(semaphore) {semaphore.acquire();}
			~Acquire() {semaphore.release();}
		};
		
		std::size_t count() const noexcept {return _count;}
		
		// @returns true if the semaphore was acquired.
		// @returns false if timeout occurs.
		bool acquire(const Timestamp * timeout = nullptr);
		
		void release();
		
		// @returns true if the semaphore was signalled.
		// @returns false if timeout occurs.
		bool wait(const Timestamp * timeout = nullptr);
		
		void broadcast();
		void signal(std::size_t count = 1);
	};
}
