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
// ReceiveFromFrontAndBack.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ReceiveFromFrontAndBack_H
#define ReceiveFromFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "Node.h"
#include "IReceiveFromFrontAndBack.h"



namespace Neural 
{
	typedef size_t ConnectionHandle;
	
	template<class T> class ReceiveFromFrontAndBack : public virtual Node<T>, public virtual IReceiveFromFrontAndBack<T>
	{
	public:		
		ReceiveFromFrontAndBack(){};
		virtual ~ReceiveFromFrontAndBack(){};
		
		//
		// This is the default behaviour of this base class = benign
		//
		virtual void receiveFromFront( T inputValue, ConnectionHandle hConnection )
		{
		}
		virtual void receiveFromBack( T inputValue, ConnectionHandle hConnection )
		{
		}

		using Node<T>::getLeafChildren;
        using Node<T>::connect;
        using Node<T>::SetReceiverValuesFront;
        using Node<T>::SetReceiverValuesBack;
        
	};
}

#endif

