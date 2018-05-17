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
// IConnection.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IConnection_H
#define IConnection_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "IActivate.h"

namespace Neural 
{
	template<class T> class IConnection: public virtual IActivate
	{
	public:		
		virtual ~IConnection(){};
		
		virtual void send(T inputValue) = 0;
							
	protected:
		IConnection(){};
		
	};
	typedef IConnection<double> IConnectionDouble;
	typedef boost::shared_ptr< IConnection<double> > IConnectionDoublePtr;

}

#endif

