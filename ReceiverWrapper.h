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
// ReceiverWrapper.h
//
// Base interface for all receivers
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ReceiverWrapper_H
#define ReceiverWrapper_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "NullReceiver.h"

namespace Neural 
{	

	template<class T> class ReceiverWrapper: public virtual IReceiver<T>
	{
	public:	
		typedef boost::shared_ptr< IReceiver<T> > IReceiverPtr;		
				
		ReceiverWrapper()
			:IReceiver<T>(),m_pReceiver(new NullReceiver<T>())
		{};
		virtual ~ReceiverWrapper(){};
				
		void replaceReceiver( IReceiverPtr pReceiver )
		{
			m_pReceiver = pReceiver;
		}
		
		virtual void onReceive( T inputValue )
		{		
			m_pReceiver->onReceive( inputValue );
		}
		
		virtual void onActivate()
		{
			m_pReceiver->onActivate();
		}				
	protected:
		IReceiverPtr m_pReceiver;
		
	};
		
}

#endif

