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