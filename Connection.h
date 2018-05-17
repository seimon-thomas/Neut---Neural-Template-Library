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
// Connection.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef Connection_H
#define Connection_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include "IConnection.h"
#include "IReceiver.h"
#include "ReceiverWrapper.h"
#include "IConnectionAttachable.h"

namespace Neural 
{
	template<class T> class Connection: virtual public IConnectionAttachable<T>
	{
	public:		
		Connection( boost::shared_ptr< IReceiver<T> > pFarReceiver )
		:IConnectionAttachable<T>(),
		 m_pFarReceiver( pFarReceiver )
		 {}

		virtual ~Connection(){};

		virtual void send(T inputValue)
		{
			m_pFarReceiver->onReceive( inputValue );
		}
		virtual void activate()
		{
			m_pFarReceiver->onActivate();
		}
		virtual void replaceReceiver( boost::shared_ptr< IReceiver<T> > pReceiver )
		{
			// Should never happen
			throw std::runtime_error( "Cannot replaceReceiver() for the tied receiver");
		}

	protected:
		boost::shared_ptr< IReceiver<T> >	m_pFarReceiver;
	};

	template<class T> class ConnectionUnattachable: virtual public Connection<T>
	{
	public:		
		ConnectionUnattachable( boost::shared_ptr< IReceiver<T> > pFarReceiver, boost::shared_ptr< IReceiver<T> > pLocalReceiver )
		:Connection<T>( pFarReceiver ),
		m_pLocalReceiver( pLocalReceiver )
		{}
		
		virtual ~ConnectionUnattachable(){};
		
		virtual void replaceReceiver( boost::shared_ptr< IReceiver<T> > pReceiver )
		{
			//
			// TODO: Replace with first class Exception processing
			//
			throw std::runtime_error( "Cannot replaceReceiver() for the tied receiver");
		}
	protected:
		boost::shared_ptr< IReceiver<T> >	m_pLocalReceiver;		
	};
	
	template<class T> class ConnectionAttachable: virtual public Connection<T>
	{
	public:		
		ConnectionAttachable( boost::shared_ptr< IReceiver<T> > pFarReceiver, boost::shared_ptr< ReceiverWrapper<T> > pLocalReceiver )
		:Connection<T>( pFarReceiver ),
		m_pLocalReceiver( pLocalReceiver )
		{}
		
		virtual ~ConnectionAttachable(){};
		
		virtual void replaceReceiver( boost::shared_ptr< IReceiver<T> > pReceiver )
		{
			m_pLocalReceiver->replaceReceiver( pReceiver );
		}
	protected:
		boost::shared_ptr< ReceiverWrapper<T> >	m_pLocalReceiver;		
	};
	
}

#endif
