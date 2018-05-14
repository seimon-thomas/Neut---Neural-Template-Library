///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IActivationFrontAndBack.h
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IActivationFrontAndBack_H
#define IActivationFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

namespace Neural 
{		
	class IActivationFrontAndBack
	{
	public:		
		virtual ~IActivationFrontAndBack(){};
		
		virtual void activateFromFront() = 0;
		virtual void activateFromBack() = 0;
		
	protected:
		IActivationFrontAndBack(){};
		
	};
	
	
}

#endif
