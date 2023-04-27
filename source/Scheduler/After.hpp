//
//  After.hpp
//  File file is part of the "Scheduler" project and released under the MIT License.
//
//  Created by Samuel Williams on 29/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Reactor.hpp"

namespace Scheduler
{
	class After final
	{
	public:
		After(Interval duration) : _duration(duration) {}
		~After() {}
		
		After(const After &) = delete;
		After & operator=(const After &) = delete;
		
		void wait();
		
	private:
		Interval _duration;
	};
}
