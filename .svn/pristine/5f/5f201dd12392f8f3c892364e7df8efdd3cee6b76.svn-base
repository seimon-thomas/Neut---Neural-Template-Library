///////////////////////////////////////////////////////////////////////////////////////////////////
//
// INodeContainer.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INodeContainer_H
#define INodeContainer_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "INode.h"

namespace Neural 
{
	template<typename T, typename ItemT> class INodeContainer: public virtual INode< boost::shared_ptr< std::vector< T > > >
	{
	public:
		typedef boost::shared_ptr< ItemT >					ItemPtr;
		typedef boost::shared_ptr< std::vector< ItemPtr > >	VectorItemPtr;
		
		virtual ~INodeContainer(){};
		
		virtual void add( ItemPtr pItem ) = 0;
		virtual VectorItemPtr items() const = 0;
		
	protected:
		INodeContainer(){};
		
	};
	
}

#endif

