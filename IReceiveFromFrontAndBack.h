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
// IReceiveFromFrontAndBack.h
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IReceiveFromFrontAndBack_H
#define IReceiveFromFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "INode.h"
#include "IActivationFrontAndBack.h"

namespace Neural 
{
	typedef size_t ConnectionHandle;
	
	
	template<class T> class IReceiveFromFrontAndBack : public virtual INode<T>, public virtual IActivationFrontAndBack
	{
	public:		
        typedef std::vector< T > ContainerType;

		virtual ~IReceiveFromFrontAndBack(){};
		
		virtual void receiveFromFront( T inputValue, ConnectionHandle hConnection ) = 0;
		virtual void receiveFromBack( T inputValue, ConnectionHandle hConnection ) = 0;

	protected:
		IReceiveFromFrontAndBack(){};
		
	};

	
}

#endif

