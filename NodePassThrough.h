///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodePassThrough.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodePassThrough_H
#define NodePassThrough_H

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
	template<class T> class NodePassThrough: public virtual ReceiveFromFront<T> //, public virtual ReceiveFromBack<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;	
		
		NodePassThrough()
		{
		};
		
		NodePassThrough(const boost::property_tree::ptree& nodeConfig)
		{
			// No internal attributes to construct
		};
		
		virtual ~NodePassThrough()
		{
		};
		
		virtual void activateFromFront()
		{
			//
			// This is a pass-through node, so just pass it on to the connected output nodes
			//
            typename std::vector< T >::iterator itNextValue = ReceiveFromFront<T>::m_pFrontInputValues->begin();
			typename std::vector< T >::iterator itEndValue = ReceiveFromFront<T>::m_pFrontInputValues->end();

            for(; itNextValue!= itEndValue; ++itNextValue )
            {
                Node<T>::sendToAll( *itNextValue, OUTPUTS );
            }
		}
		
		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// Nothing needed
		};
		
	};
}

#endif