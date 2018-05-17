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
