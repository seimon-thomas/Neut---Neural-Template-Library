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
// NodePerceptronBias.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodePerceptronBias_H
#define NodePerceptronBias_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <boost/property_tree/ptree.hpp>

#include "ReceiveFromFrontAndBack.h"



namespace Neural 
{
	template<class T> class NodePerceptronBias: public virtual ReceiveFromFrontAndBack<T>
	{
	public:
		
		NodePerceptronBias()
		{
		};

		NodePerceptronBias(const boost::property_tree::ptree& nodeConfig)
		{
			// No need to do anything here
		};
		
		virtual ~NodePerceptronBias()
		{
		};
		
		virtual void activateFromFront()
		{
			//
			// Just, send the bias vialue to the nodes on the output
			//
			Node<T>::sendToAll( (T)1, OUTPUTS );
		}

		
		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// No internal config to write
		};

		virtual void getLeafChildren( typename INode<T>::INodePtrList& leafNodes, ConnectionSite siteOnThis)
		{
            // Only become known to outputs as we don't want connection to inputs
            if( siteOnThis == OUTPUTS )
            {
                Node<T>::getLeafChildren( leafNodes, siteOnThis );
            }
		}
        
		
	};
}

#endif
