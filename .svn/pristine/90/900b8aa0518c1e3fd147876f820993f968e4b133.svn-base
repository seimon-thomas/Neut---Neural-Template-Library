///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeInput.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeInput_H
#define NodeInput_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <boost/serialization/shared_ptr.hpp>

#include "IInputs.h"
#include "ReceiveFromFrontAndBack.h"



namespace Neural 
{
	template<class T, class TInputProvider> class NodeInput: public virtual ReceiveFromFrontAndBack<T>
	{
	public:
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;		
		
		NodeInput( boost::shared_ptr< TInputProvider > pInputProvider )
		:m_pInputProvider( pInputProvider )
		{
		};
	
		virtual ~NodeInput()
		{
		};
		
		virtual void activateFromFront()
		{
            //
            // How many inputs required
            //
            size_t requiredInputs = Node<T>::m_ConnectedNodes[OUTPUTS].size();
            if( requiredInputs > 0 )
            {
                //
                // Get the inputs
                //
                boost::shared_ptr< typename IInputs<T>::ContainerType > pNextInputSet = m_pInputProvider->getNextInputSet( requiredInputs );
                //
                // Next, iterate sending these inputs to the outputs
                // First, get the ends
                //
                typename Node<T>::ContainerType::iterator itNodesEnd = Node<T>::m_ConnectedNodes[OUTPUTS].end();
                typename IInputs<T>::ContainerType::iterator itInputsEnd = pNextInputSet->end();
                //
                // Next, get the beginnings
                //
                typename Node<T>::ContainerType::iterator itNextNode = Node<T>::m_ConnectedNodes[OUTPUTS].begin();
                typename IInputs<T>::ContainerType::iterator itNexInput = pNextInputSet->begin();
                //
                // Next iterate sending
                //
                while( (itNexInput != itInputsEnd) && (itNextNode != itNodesEnd) )
                {
                    //
                    // Send the next one
                    //
                    (*itNextNode)->send( *itNexInput );
                    //
                    // Move on
                    //
                    ++itNexInput;
                    ++itNextNode;
                }
            }
        }

        using Node<T>::connect;

	
	protected:
		boost::shared_ptr< TInputProvider > m_pInputProvider;		

	};
}

#endif