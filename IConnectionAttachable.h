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
// IConnectionAttachable.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IConnectionAttachable_H
#define IConnectionAttachable_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "IConnection.h"
#include "IReceiver.h"

namespace Neural 
{
	template<class T> class IConnectionAttachable: public virtual IConnection<T>
	{
	public:
		typedef boost::shared_ptr< IReceiver<T> > IReceiverPtr;		
		
		virtual ~IConnectionAttachable(){};
		
		virtual void replaceReceiver( IReceiverPtr pReceiver ) = 0;
		
	protected:
		IConnectionAttachable(){};
		
	};
	typedef IConnectionAttachable<double> IConnectionAttachableDouble;
	typedef boost::shared_ptr< IConnectionAttachable<double> > IConnectionAttachableDoublePtr;
	
}

#endif

