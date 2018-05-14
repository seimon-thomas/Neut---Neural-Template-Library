///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeNetworkPillar.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeNetworkPillar_H
#define NodeNetworkPillar_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "NodeContainer.h"

#include "NodeInput.h"



namespace Neural 
{
	template<class T> class NodeNetworkPillar: public virtual NodeContainer<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> >					INodePtr;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;

		NodeNetworkPillar()
		{
		};
		
		NodeNetworkPillar(const boost::property_tree::ptree& nodeConfig)
		{
			// No attributes to set
		};
		
		virtual ~NodeNetworkPillar()
		{
		};
		
		virtual void addChild( typename INode<T>::INodePtr pItem )
		{
			//
			// First, call the base implementation
			//
			NodeContainer<T>::addChild( pItem );
			//
			// Next connect to the previous node (if there is one)
			// We actually connect leaf nodes
			// TODO: Refactor this for different connection topologies as opposed
			// to this default one of "fully connected"
			//
			size_t numItems = NodeContainer<T>::m_pItems->size();
			if( numItems > 1 )
			{
				//
				// First get the child leaves of the last but one child
				//
				typename INode<T>::INodePtrList prevChildleaves;
				NodeContainer<T>::m_pItems->at(numItems-2)->getLeafChildren( prevChildleaves, OUTPUTS );
				//
				// Next get the child leaves of the last child
				//
				typename INode<T>::INodePtrList nextChildleaves;
				NodeContainer<T>::m_pItems->at(numItems-1)->getLeafChildren( nextChildleaves, INPUTS );
				//
				// Next fully connect the two sets of leaves
				//
				BOOST_FOREACH( typename INode<T>::INodePtr pNextNode, prevChildleaves )
				{
					BOOST_FOREACH( typename INode<T>::INodePtr pNextOtherNode, nextChildleaves )
					{
						pNextNode->connect( pNextOtherNode, OUTPUTS );
					}
				}			
			}
		}
		
		virtual void getLeafChildren( typename INode<T>::INodePtrList& leafNodes, ConnectionSite siteOnThis)
		{
			//
			// First check if we have any children
			//
			if( NodeContainer<T>::m_pItems->size() )
			{
				//
				// As this is a pillar we either call getLeafChildren() on the
				// first ot last child node, depending on siteOnThis
				//
				if( OUTPUTS == siteOnThis )
				{
					NodeContainer<T>::m_pItems->back()->getLeafChildren( leafNodes, siteOnThis);
				}
				else
				{
					NodeContainer<T>::m_pItems->front()->getLeafChildren( leafNodes, siteOnThis);
				}
			}
		}

		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// No attributes to serialise
		};
		
		
	};
	
};


#endif