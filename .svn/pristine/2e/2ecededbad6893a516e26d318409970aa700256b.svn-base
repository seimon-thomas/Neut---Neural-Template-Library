///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IConnectionAttachable.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IConnectionAttachable_H
#define IConnectionAttachable_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "IConnection.h"
#include "IReceiver.h"

namespace Neural 
{
	template<class T> class IConnectionAttachable: public virtual IConnection<T>
	{
	public:
		typedef boost::shared_ptr< IReceiver<T> > IReceiverPtr;		
		
		virtual ~IConnectionAttachable(){};
		
		virtual void replaceReceiver( IReceiverPtr pReceiver ) = 0;
		
	protected:
		IConnectionAttachable(){};
		
	};
	typedef IConnectionAttachable<double> IConnectionAttachableDouble;
	typedef boost::shared_ptr< IConnectionAttachable<double> > IConnectionAttachableDoublePtr;
	
}

#endif

