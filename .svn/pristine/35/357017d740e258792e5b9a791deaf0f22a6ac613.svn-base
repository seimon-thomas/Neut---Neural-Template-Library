///////////////////////////////////////////////////////////////////////////////////////////////////
//
// INodeNetwork.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INodeNetwork_H
#define INodeNetwork_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "INode.h"
#include "INodeLayer.h"

namespace Neural 
{
	template<class T> class INodeNetwork : public virtual INodeContainer<T, INodeLayer<T> >
	{
	public:
		virtual ~INodeNetwork(){};
		
		virtual size_t getID()=0;
		
	protected:
		INodeNetwork(){};
		
	private:
		friend class boost::serialization::access;
		
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			
		};
		
	};
	
	typedef INodeNetwork<double> INodeNetworkDouble;
	typedef boost::shared_ptr< INodeNetwork<double> > INodeNetworkDoublePtr;
	
}

#endif

