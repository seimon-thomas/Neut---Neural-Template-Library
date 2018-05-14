///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IReceiveFromFrontAndBack.h
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IReceiveFromFrontAndBack_H
#define IReceiveFromFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "INode.h"
#include "IActivationFrontAndBack.h"

namespace Neural 
{
	typedef size_t ConnectionHandle;
	
	
	template<class T> class IReceiveFromFrontAndBack : public virtual INode<T>, public virtual IActivationFrontAndBack
	{
	public:		
        typedef std::vector< T > ContainerType;

		virtual ~IReceiveFromFrontAndBack(){};
		
		virtual void receiveFromFront( T inputValue, ConnectionHandle hConnection ) = 0;
		virtual void receiveFromBack( T inputValue, ConnectionHandle hConnection ) = 0;

	protected:
		IReceiveFromFrontAndBack(){};
		
	};

	
}

#endif

