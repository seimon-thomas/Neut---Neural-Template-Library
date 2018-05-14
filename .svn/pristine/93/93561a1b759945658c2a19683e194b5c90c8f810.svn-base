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
