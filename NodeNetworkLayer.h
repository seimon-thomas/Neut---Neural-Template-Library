///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeNetworkLayer.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeNetworkLayer_H
#define NodeNetworkLayer_H

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
#include <boost/property_tree/ptree.hpp>

#include "NodeContainer.h"
//

namespace Neural 
{
	template<class T> class GetLeafChildrenFunctor 
	{ 
		typename INode<T>::INodePtrList&	m_LeafNodes;
		ConnectionSite						m_ConnectionSite;
		
	public: 
		GetLeafChildrenFunctor( typename INode<T>::INodePtrList&	leafNodes, 
								ConnectionSite						connectionSite )
								:m_LeafNodes( leafNodes ),
								 m_ConnectionSite( connectionSite ){}
		
		void operator()( const typename INode<T>::INodePtr& pNode) const 
		{
			pNode->getLeafChildren( m_LeafNodes, m_ConnectionSite );
		}
	};
	
	

	template<class T> class NodeNetworkLayer: public virtual NodeContainer<T>
	{
	public:
		typedef boost::shared_ptr< std::vector< typename INode<T>::INodePtr> >	VectorINodePtr;

		NodeNetworkLayer():NodeContainer<T>(0) // 0 Num threads means check
		{
		};

		NodeNetworkLayer(const boost::property_tree::ptree& nodeConfig):NodeContainer<T>(2)
		{
		 // No intenal attributes to set
		};
						 
		
		virtual ~NodeNetworkLayer()
		{
		};
				
		virtual void getLeafChildren( typename INode<T>::INodePtrList& leafNodes, ConnectionSite siteOnThis)
		{
			//
			// First check if we have any children
			//
			if( NodeContainer<T>::m_pItems->size() )
			{
				//
				// We have children, and (as this is a layer)
				// we get leaves from all of them
				// Time for 'for each' with some cunning
				// boost::bind
				//
				std::for_each(NodeContainer<T>::m_pItems->begin(), 
							  NodeContainer<T>::m_pItems->end(), 
							  GetLeafChildrenFunctor<T>( leafNodes, siteOnThis ));
			}
		}

		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// No intenal attributes to serialize
		};
	};
}


#endif