///////////////////////////////////////////////////////////////////////////////////////////////////
//
// INodeLayer.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INodeLayer_H
#define INodeLayer_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "INode.h"

namespace Neural 
{
	template<class T> class INodeLayer : public virtual INodeContainer<T, INode<T> >
	{
	public:
		virtual ~INodeLayer(){};

	protected:
		INodeLayer(){};

	private:
		// Vestigial serialize for boost serialisation
		friend class boost::serialization::access;
		template<class Archive>	void serialize(Archive & ar, const unsigned int version){};
	};
	
	typedef INodeLayer<double> INodeLayerDouble;
	typedef boost::shared_ptr< INodeLayer<double> > INodeLayerDoublePtr;
	
}

#endif

