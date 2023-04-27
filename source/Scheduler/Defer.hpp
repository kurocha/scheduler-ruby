//
//  Deferred.hpp
//  This file is part of the "Scheduler" project and released under the .
//
//  Created by Samuel Williams on 25/4/2023.
//  Copyright, 2023, by Samuel Williams. All rights reserved.
//

#pragma once

namespace Scheduler
{
	template <typename Callback>
	class Defer {
		bool _cancelled = false;
		Callback _callback;
		
	public:
		Defer(Callback callback) : _callback(callback) {}
		~Defer()
		{
			if (!_cancelled) _callback();
		}
		
		void cancel() {_cancelled = true;}
	};
	
	template <typename Callback>
	Defer<Callback> defer(Callback callback) {
		return Defer<Callback>(callback);
	}
}
