///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Activate.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef Activate_H
#define Activate_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "IActivate.h"
#include "IActivatable.h"

namespace Neural 
{
	class Activate : public virtual IActivate
	{
	public:		
		Activate( IActivatablePtr pIActivatable )
			:IActivate(),
			 m_pIActivatable( pIActivatable )
		{};
		
		virtual ~Activate(){};
		
		virtual void activate()
		{
			m_pIActivatable->onActivate();
		}
		
	protected:
		IActivatablePtr m_pIActivatable;
		
	};
		
}

#endif

