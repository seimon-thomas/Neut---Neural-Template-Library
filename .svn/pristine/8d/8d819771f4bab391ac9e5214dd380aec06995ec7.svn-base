///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeContainerTwoForOneActivation.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeContainerTwoForOneActivation_H
#define NodeContainerTwoForOneActivation_H

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

#include "NodeNetworkPillar.h"

#include "NodeInput.h"



namespace Neural 
{
	template<typename T, typename TBase> class NodeContainerTwoForOneActivation: public virtual TBase
	{
	public:
		typedef boost::shared_ptr< INode<T> >					INodePtr;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;
		
		NodeContainerTwoForOneActivation()
		{
		};
		
		NodeContainerTwoForOneActivation(const boost::property_tree::ptree& nodeConfig)
		{
			// No attributes to set
		};
		
		virtual ~NodeContainerTwoForOneActivation()
		{
		};
		
		virtual void activateFromFront()
		{
			TBase::activateFromFront();
		}
		
		virtual void activateFromBack()
		{
			//
			// Here's the 2 for 1 :-)
			//
			TBase::activateFromFront();
			TBase::activateFromBack();
		}
		
		
		
		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// No attributes to serialise
		};
		
	};
	
};


#endif