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
// Node.h
//
// Base class for all nodes
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NODE_H
#define NODE_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/foreach.hpp>
#include "IReceiveFromFrontAndBack.h"
#include "Connection.h"
#include "NodeReceiver.h"
#include "ReceiverWrapper.h"
#include "ActivateProviderFrontAndBack.h"
#include "FastConnection.h"

namespace Neural 
{
	template<class T> class Node: public virtual IReceiveFromFrontAndBack<T>, public virtual ActivateProviderFrontAndBack
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IReceiver<T> > IReceiverPtr;		
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;		
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;
        typedef FastConnection< T > TConnection;
        typedef std::vector< TConnection* > ContainerType;
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType ValueContainerType;
        typedef boost::shared_ptr< ValueContainerType >             ValueContainerTypePtr;
        
        
		Node():	INode<T>(), 
				m_ConnectedNodes( NUM_CONNECTION_SITES )
		{			
		};
		
		virtual ~Node()
		{
            // TODO: Refactor
            for( size_t i = 0; i < NUM_CONNECTION_SITES; ++i )
            {
                for( size_t j = 0; j < m_ConnectedNodes[i].size(); ++j )
                {
                    delete m_ConnectedNodes[i][j];
                }
            }
		};
		
		virtual void connect( INodePtr pNodeToConnect, ConnectionSite siteOnThis )
		{
            //
            // Create the inbound connection
            //
            TConnection* pInboundConnection = createIboundConnection( siteOnThis );
            //
            // Exchange with the other node
            //
            TConnection* pOutboundConnection = dynamic_cast<Node<T>* >(pNodeToConnect.get())->exchangeConnectors( pInboundConnection, !siteOnThis );
            //
            // Add the connection
            //
            addConnection( pOutboundConnection, siteOnThis );
		}		

        virtual ValueContainerTypePtr offerShareBulkInputs( ValueContainerTypePtr pBulkInputs, ConnectionSite siteOnThis, size_t& retSharedSize )
        {
  			if( INPUTS == siteOnThis )
			{
				SetReceiverValuesFront( pBulkInputs );
			}
			else 
			{
				SetReceiverValuesBack( pBulkInputs );
			}
            retSharedSize = pBulkInputs->size();
            return pBulkInputs;
        }
        
        virtual TConnection* exchangeConnectors( TConnection* pConnectionToCaller, ConnectionSite siteOnThis )
        {
            //
            // Create the inbound connection
            //
            TConnection* pInboundConnection = createIboundConnection( siteOnThis );
            //
            // Add the connection & get the idx where it went
            //
            addConnection( pConnectionToCaller, siteOnThis );
            //
            // Return the inbound connection
            //
            return pInboundConnection;
        }
		
		virtual void addChild( INodePtr pChildNode )
		{
			// Can't add children to a leaf node, so throw std::runtime_error(
			// TODO: Add 'Proper' exception handling here 
			throw std::runtime_error( "Node does not impliment addChild");
		}
		
		virtual void getLeafChildren( typename INode<T>::INodePtrList& leafNodes, ConnectionSite siteOnThis)
		{
			//
			// "This" is a leaf node, so add "this"
			//
			leafNodes.push_back( boost::enable_shared_from_this< INode<T> >::shared_from_this() );
		}
		
		virtual VectorINodePtr getChildren()
		{
			return VectorINodePtr( new std::vector< INodePtr> );
		}


	protected:
        TConnection* createIboundConnection( ConnectionSite siteOnThis )
        {
            //
            // The ret val (MAY BE NULL)
            //
            TConnection* pInboundConnection = NULL;
            //
            // Get the target values for the inbound
            //
            ValueContainerType* pTargetVals = growAndGetReceiverValues( siteOnThis );
            //
            // Index for inbound connection
            //
            if( NULL != pTargetVals ) 
            {
                size_t addIdx = pTargetVals->size() - 1;
                //
                // Create the inbound connection
                //
                pInboundConnection = new TConnection( pTargetVals, addIdx );
           }
            //
            // Return it
            //
            return pInboundConnection;
        }

        ValueContainerType* growAndGetReceiverValues( ConnectionSite siteOnThis )
        {
            ValueContainerType* pRetContainer;
			if( INPUTS == siteOnThis )
			{
				pRetContainer = onGrowAndGetReceiverValuesFront();
			}
			else 
			{
				pRetContainer = onGrowAndGetReceiverValuesBack();
			}
            return pRetContainer;
        }
        
		void addConnection( TConnection* pConnection, ConnectionSite direction )
		{
            if( NULL != pConnection )
            m_ConnectedNodes[ direction ].push_back( pConnection );
		}

        virtual ValueContainerType* onGrowAndGetReceiverValuesFront(){ return NULL; };
		virtual ValueContainerType* onGrowAndGetReceiverValuesBack(){ return NULL; };

        virtual void SetReceiverValuesFront( ValueContainerTypePtr pBulkInputs ){};
        virtual void SetReceiverValuesBack( ValueContainerTypePtr pBulkInputs ){};

        
		inline void sendToAll( T inputValue, ConnectionSite direction )
        {
            register size_t numToSend = m_ConnectedNodes[direction].size();
            if( numToSend > 0 )
            {
                TConnection** pFirst = &(m_ConnectedNodes[direction][0]);
                for( register size_t sendIdx = 0; sendIdx < numToSend; ++sendIdx )
                {
                    ( *pFirst++ )->send( inputValue );
                }
            }
        }

		std::vector< ContainerType >	m_ConnectedNodes;	
	};
}

#endif
