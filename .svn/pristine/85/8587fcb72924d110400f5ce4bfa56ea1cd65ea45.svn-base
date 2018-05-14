///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeReceiver.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeReceiver_H
#define NodeReceiver_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "IReceiver.h"
#include "IReceiveFromFrontAndBack.h"

namespace Neural 
{	
	template<class T> class NodeReceiver: public virtual IReceiver<T>
	{
	public:		
		NodeReceiver(IReceiveFromFrontAndBack<T>& node, ConnectionHandle hConnection)
			:IReceiver<T>(),
			 m_Node( node ),
			 m_hConnection( hConnection )
			{};
			
		virtual ~NodeReceiver(){};
		
		virtual void onActivate()=0;
		virtual void onReceive( T inputValue )=0;
	protected:
		IReceiveFromFrontAndBack<T>&	m_Node;
		ConnectionHandle	m_hConnection;
		
	};
	
	template<class T> class NodeReceiverFront: public virtual NodeReceiver<T>
	{
	public:		
		NodeReceiverFront(IReceiveFromFrontAndBack<T>& node, ConnectionHandle hConnection)
		:NodeReceiver<T>(node, hConnection)
		{};
		
		virtual ~NodeReceiverFront(){};
		
		virtual void onActivate()
		{
			NodeReceiver<T>::m_Node.activateFromFront();
		}
		virtual void onReceive( T inputValue )
		{
			NodeReceiver<T>::m_Node.receiveFromFront( inputValue, NodeReceiver<T>::m_hConnection );
		}
		
	};

	template<class T> class NodeReceiverBack: public virtual NodeReceiver<T>
	{
	public:		
		NodeReceiverBack(IReceiveFromFrontAndBack<T>& node, ConnectionHandle hConnection)
		:NodeReceiver<T>(node, hConnection)
		{};
		
		virtual ~NodeReceiverBack(){};
		
		virtual void onActivate()
		{
			NodeReceiver<T>::m_Node.activateFromBack();
		}
		virtual void onReceive( T inputValue )
		{
			NodeReceiver<T>::m_Node.receiveFromBack( inputValue, NodeReceiver<T>::m_hConnection );
		}
		
	};
}

#endif

