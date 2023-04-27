//
//  Semaphore.hpp
//  This file is part of the "Scheduler" project and released under the .
//
//  Created by Samuel Williams on 26/4/2023.
//  Copyright, 2023, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Fiber.hpp"

#include <cstdint>
#include <list>

namespace Scheduler
{
	class Semaphore final
	{
		std::size_t _count = 0;
		std::list<Fiber *> _waiting;
		
	public:
		Semaphore(std::size_t count = 1) : _count(count) {}
		~Semaphore();
		
		struct Acquire
		{
			Semaphore & semaphore;
			
			Acquire(Semaphore & semaphore) : semaphore(semaphore) {semaphore.acquire();}
			~Acquire() {semaphore.release();}
		};
		
		std::size_t count() const noexcept {return _count;}
		
		void acquire();
		void release();
		
		void wait();
		
		void broadcast();
		void signal(std::size_t count = 1);
	};
}
