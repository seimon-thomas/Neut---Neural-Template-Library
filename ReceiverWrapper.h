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

