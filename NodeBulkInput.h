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
//  NodeBulkInput.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 22/11/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_NodeBulkInput_h
#define NeuralNetworkApplication_NodeBulkInput_h

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <boost/serialization/shared_ptr.hpp>

#include "IInputs.h"
#include "ReceiveFromFrontAndBack.h"
#include "NodeInput.h"

namespace Neural 
{
	template<class T, class TInputProvider> class NodeBulkInput: public virtual NodeInput<T, TInputProvider >
	{
	public:
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;	
        typedef NodeInput<T, TInputProvider > Parent;
		
		NodeBulkInput( boost::shared_ptr< TInputProvider > pInputProvider )
		:NodeInput<T, TInputProvider >( pInputProvider ),
        m_pOuputSet( new typename IInputs<T>::ContainerType ),
        m_bulkSize( 0 )
		{
		};
        
		virtual ~NodeBulkInput()
		{
		};
		
		virtual void connect( typename Node<T>::INodePtr pNodeToConnect, ConnectionSite siteOnThis )
		{
            m_pOuputSet = dynamic_cast<Node<T>* >(pNodeToConnect.get())->offerShareBulkInputs( m_pOuputSet, !siteOnThis, m_bulkSize );
		}		

		virtual void activateFromFront()
		{
            if( NULL != m_pOuputSet.get() )
            {
                //
                // Get the inputs
                //
                boost::shared_ptr< typename IInputs<T>::ContainerType > pNextInputSet = Parent::m_pInputProvider->getNextInputSet( m_bulkSize );
                //
                // And copy to the shared output
                //
                std::copy( pNextInputSet->begin(), pNextInputSet->end(), m_pOuputSet->begin() );
            }
        }
        
	protected:
       
        boost::shared_ptr< typename IInputs<T>::ContainerType > m_pOuputSet;
        size_t m_bulkSize;
        
        
	};
}


#endif
