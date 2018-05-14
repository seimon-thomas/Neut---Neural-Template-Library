///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeResult.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeResult_H
#define NodeResult_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "ReceiveFromFrontAndBack.h"
#include "utils.h"



namespace Neural 
{
	template<class T, class V> class NodeResult: public virtual ReceiveFromFrontAndBack<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IReceiver<T> > IReceiverPtr;		
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;		
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType ValueContainerType;
        typedef FastConnection< T > TConnection;
		
		NodeResult( V& frontInputValues )
		:m_FrontInputValues( frontInputValues )
		{
		};
		
		virtual ~NodeResult()
		{
		};
		
		// TODO: Refactor 
        virtual ValueContainerType* onGrowAndGetReceiverValuesFront()
        { 
            m_FrontInputValues.push_back( T() );
            return &m_FrontInputValues;
        };

	protected:
		V&	m_FrontInputValues;
		
	};
}

#endif