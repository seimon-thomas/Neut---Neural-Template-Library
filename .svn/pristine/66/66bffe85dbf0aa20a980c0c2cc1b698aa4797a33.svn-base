///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ReceiveFromFrontAndBack.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ReceiveFromFrontAndBack_H
#define ReceiveFromFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "Node.h"
#include "IReceiveFromFrontAndBack.h"



namespace Neural 
{
	typedef size_t ConnectionHandle;
	
	template<class T> class ReceiveFromFrontAndBack : public virtual Node<T>, public virtual IReceiveFromFrontAndBack<T>
	{
	public:		
		ReceiveFromFrontAndBack(){};
		virtual ~ReceiveFromFrontAndBack(){};
		
		//
		// This is the default behaviour of this base class = benign
		//
		virtual void receiveFromFront( T inputValue, ConnectionHandle hConnection )
		{
		}
		virtual void receiveFromBack( T inputValue, ConnectionHandle hConnection )
		{
		}

		using Node<T>::getLeafChildren;
        using Node<T>::connect;
        using Node<T>::SetReceiverValuesFront;
        using Node<T>::SetReceiverValuesBack;
        
	};
}

#endif

