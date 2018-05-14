///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeContainer.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeContainer_H
#define NodeContainer_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>

#include "INode.h"
#include "ActivateProviderFrontAndBack.h"
#include "MultiThreadedActivator.h"

//

namespace Neural 
{
	template<typename T> class NodeContainer: public virtual INode<T>, public virtual ActivateProviderFrontAndBack
	{
	public:
		typedef boost::shared_ptr< INode<T> >					INodePtr;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;
		
		NodeContainer( size_t numThreads = 1 )
		: m_pItems( new std::vector<INodePtr>() ),
		  m_Activators( NUM_CONNECTION_SITES )
		{
            //
            // If numThreads is zero, this indicates that we are to determine the number automatically
            //
            if( 0 == numThreads )
            {
                numThreads = boost::thread::hardware_concurrency();
                //
                // If zero that means unavailable, so fall back to 1
                //
                if( 0 == numThreads ) numThreads = 1;
            }
            m_Activators[ INPUTS ] = boost::shared_ptr< MultiThreadedActivator< T > >( new MultiThreadedActivator< T >( numThreads ) );
            m_Activators[ OUTPUTS ] = boost::shared_ptr< MultiThreadedActivator< T > >( new MultiThreadedActivator< T >( numThreads ) );
		};
		
		virtual ~NodeContainer()
		{
		};
		
		virtual void activateFromFront()
		{
            if( m_pItems->size() > 1 )
            {
                m_Activators[ INPUTS ]->activate();
            }
            else
            {
                m_Activators[ INPUTS ]->unlockedActivate();
            }
		}
		
		virtual void activateFromBack()
		{
            if( m_pItems->size() > 1 )
            {
                m_Activators[ OUTPUTS ]->activate();
            }
            else
            {
                m_Activators[ OUTPUTS ]->unlockedActivate();
            }
		}
		
		virtual void addChild( INodePtr pItem )
		{
			//
			// Add to the items collection
			//
			m_pItems->push_back( pItem );
			//
			// Store an input activation connection to this 
			//
			m_Activators[ INPUTS ]->pushBackActivation( pItem->getActivationConnection( INPUTS ) );
			//
			// Store an output activation connection to this node 
			// (notice push_front as these are activated in reverese order)
			//
			m_Activators[ OUTPUTS ]->pushFrontActivation( pItem->getActivationConnection( OUTPUTS ) );
			
		}
		
		virtual void connect( INodePtr pNodeToConnect, ConnectionSite siteOnThis = OUTPUTS )
		{
			// Not implemented for containers
			throw std::runtime_error( "connect() method is not implimented for containers");
		}

		virtual VectorINodePtr getChildren()
		{
			return m_pItems;
		}
		
		
	protected:
		VectorINodePtr									m_pItems;

       std::vector< boost::shared_ptr< MultiThreadedActivator< T > > > m_Activators;
		
	};
}

#endif