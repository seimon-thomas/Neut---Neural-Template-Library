//
//  NodePassThroughBulkWithBias.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 02/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_NodePassThroughBulkWithBias_h
#define NeuralNetworkApplication_NodePassThroughBulkWithBias_h

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include "ReceiveFromFront.h"
#include "ReceiveFromBack.h"

namespace Neural 
{
    //
    // TODO: Change the name of this class to reflect it's one-way pass through.
    //
	template<class T> class NodePassThroughBulkWithBias: public virtual ReceiveFromFront<T> //, public virtual ReceiveFromBack<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;	
        typedef typename Node<T>::ValueContainerTypePtr ValueContainerTypePtr;
		
		NodePassThroughBulkWithBias( size_t bulkSize ) : m_bulkSize( bulkSize ), m_connected( false )
		{
		};
		
		NodePassThroughBulkWithBias(const boost::property_tree::ptree& nodeConfig) : m_connected( false )
		{
			m_bulkSize = boost::lexical_cast< size_t >( nodeConfig.get<std::string>("BulkSize") );
		};
		
		virtual ~NodePassThroughBulkWithBias()
		{
		};
        
		virtual void connect( typename Node<T>::INodePtr pNodeToConnect, ConnectionSite siteOnThis )
		{
            //
            // Only for rearward connection
            //
            if( OUTPUTS == siteOnThis )
            {
                //
                // First make sure that m_pFrontInputValues are the correct size (also do a little check here)
                //
                size_t currenInputSize = ReceiveFromFront<T>::m_pFrontInputValues->size();
                if( 0 == currenInputSize )
                {
                    //
                    // This is allowed, as some bulk inputs want the client end to set the size
                    // TODO: Refactor
                    // Add 1 for bias
                    //
                    ReceiveFromFront<T>::m_pFrontInputValues->resize( m_bulkSize + 1 );
                    //
                    // Set the bias
                    //
                    ReceiveFromFront<T>::m_pFrontInputValues->at( m_bulkSize ) = (T)1.0;
                }
                else if( (m_bulkSize+1) != currenInputSize )
                {
                    //
                    // This isnt allowed
                    //
                    throw std::runtime_error( "NodePassThroughBulkWithBias input size does not match the bulk size.");
                }
                //
                // Now force the node to connect to use this bulk
                //
                size_t retSharedSize;
                dynamic_cast<Node<T>* >(pNodeToConnect.get())->offerShareBulkInputs( ReceiveFromFront<T>::m_pFrontInputValues, !siteOnThis, retSharedSize );
                //
                // Set that we're connected now
                //
                m_connected = true;
            }
            else
            {
                throw std::runtime_error( "Cannot connect() to the front of a NodePassThroughBulkWithBias");
            }
		}		
        
        
        virtual ValueContainerTypePtr offerShareBulkInputs( ValueContainerTypePtr pBulkInputs, ConnectionSite siteOnThis, size_t& retSharedSize )
        {
            //
            // The return value
            //
            ValueContainerTypePtr pRetVal;
            //
            // Only for rearward connection
            //
            if( INPUTS == siteOnThis )
            {
                //
                // Are we already connected?
                //
                if( !m_connected )
                {
                    //
                    // Ok, we're not connected, so we can accept the share, as long it's size accords
                    // or it has no size
                    //
                    size_t offeredSize = pBulkInputs->size();
                    if( 0 == offeredSize )
                    {
                        //
                        // This is ok, so we just resize
                        //
                        pBulkInputs->resize( m_bulkSize + 1 );
                        pBulkInputs->at( m_bulkSize ) = (T)1.0;
                    }
                    else if( offeredSize != ReceiveFromFront<T>::m_pFrontInputValues->size() )
                    {
                        //
                        // Otherwise hey have to be the same
                        //
                        throw std::runtime_error( "offerShareBulkInputs() must offer either an empty container, or one the same size as current inputs");
                    }
                    //
                    // Set our values to be the bulk input
                    //
                    ReceiveFromFront<T>::m_pFrontInputValues = pBulkInputs;
                }
                //
                // Set the retVal
                //
                pRetVal = ReceiveFromFront<T>::m_pFrontInputValues;
                retSharedSize = ReceiveFromFront<T>::m_pFrontInputValues->size() - 1; // Snip bias
                //
                // Set that we're connected now
                //
                m_connected = true;
            }
            else
            {
                throw std::runtime_error( "Cannot offerShareBulkInputs() to the back of a NodePassThroughBulkWithBias");
            }
            //
            // Finally return the ret val
            //
            return pRetVal;
        }
        //
        // Use benign base class activation as the purpose of this node is just
        // to connect a bulk input from one side to the other
        //
        
		
        virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// If base did anything we'd call it here
            
			nodeConfig.put("BulkSize", boost::lexical_cast<std::string>(m_bulkSize) );
		};
        
    protected:
        size_t m_bulkSize;
        bool   m_connected;
		
	};
}

#endif
