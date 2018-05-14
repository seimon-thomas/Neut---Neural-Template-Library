/*
 *  NodeOutput.h
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 23/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */
#ifndef NodeOutput_H
#define NodeOutput_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "ReceiveFromFrontAndBack.h"
#include "ReceiveFromFront.h"



namespace Neural 
{
	template<class T, class TOutputProvider> class NodeOutput: public virtual ReceiveFromFront<T>
	{
	public:
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;		
        typedef FastConnection< T > TConnection;
		
		NodeOutput( boost::shared_ptr< TOutputProvider > pOutputProvider )
		:m_pOutputProvider( pOutputProvider )
		{
		};
		
		virtual ~NodeOutput()
		{
		};
		
		virtual void activateFromFront()
		{
			//
			// Put the input to this node out to the output set output set to the output provider 
			// 
			m_pOutputProvider->putNextOutputSet( *ReceiveFromFront<T>::m_pFrontInputValues ); 
			//
			// Send each set entry to the corresponding output node
			//
			typename std::vector< TConnection* >::iterator itNextConnection = Node<T>::m_ConnectedNodes[OUTPUTS].begin();
			typename std::vector< T >::iterator itNextValue = ReceiveFromFront<T>::m_pFrontInputValues->begin();
            
            typename std::vector< TConnection* >::iterator itEndConnection = Node<T>::m_ConnectedNodes[OUTPUTS].end();
			typename std::vector< T >::iterator itEndValue = ReceiveFromFront<T>::m_pFrontInputValues->end();
            
			while(   ( itNextConnection != itEndConnection )
				  && ( itNextValue != itEndValue ) )
			{
				(*itNextConnection)->send( *itNextValue );
				
				++itNextConnection;
				++itNextValue;
			}
		}
		
	protected:
		
		boost::shared_ptr< TOutputProvider > m_pOutputProvider;		
	};
}

#endif
