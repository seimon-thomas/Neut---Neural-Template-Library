///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2010-2018 Seimon Thomas
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IActivatable.h
//
// Base interface for all activatables
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IActivatable_H
#define IActivatable_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

namespace Neural 
{	
	class IActivatable
	{
	public:		
		virtual ~IActivatable(){};
		
		virtual void onActivate() = 0;
		
	protected:
		IActivatable(){};
		
	};
	
	typedef boost::shared_ptr< IActivatable > IActivatablePtr;
}

#endif

