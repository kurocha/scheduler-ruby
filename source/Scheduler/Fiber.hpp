//
//  Fiber.hpp
//  This file is part of the "Scheduler" project and released under the .
//
//  Created by Samuel Williams on 19/4/2023.
//  Copyright, 2023, by Samuel Williams. All rights reserved.
//

#pragma once

#include <functional>
#include <ruby.h>

namespace Scheduler
{
	class Fiber final
	{
		VALUE _handle;
		std::function<void()> _function;
		
	public:
		Fiber(std::function<void()> function);
		~Fiber();
		
		void resume();
		void yield();
		void transfer();
		
		~Fiber()
		{
		}
	};
}
