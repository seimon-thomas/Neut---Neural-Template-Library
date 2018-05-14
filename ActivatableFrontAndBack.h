///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ActivatableFrontAndBack.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ActivatableFrontAndBack_H
#define ActivatableFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include "IActivatable.h"
#include "IActivationFrontAndBack.h"

namespace Neural 
{	
	class ActivatableFrontAndBack : public virtual IActivatable
	{
	public:		
		ActivatableFrontAndBack( IActivationFrontAndBack* pIActivationFrontAndBack )
		:IActivatable(),
		 m_pIActivationFrontAndBack( pIActivationFrontAndBack )
		{};

		virtual ~ActivatableFrontAndBack(){};
		
		virtual void onActivate() = 0;
		
	protected:
		IActivationFrontAndBack* m_pIActivationFrontAndBack;
	};

	class ActivatableFront : public virtual ActivatableFrontAndBack
	{
	public:		
		ActivatableFront( IActivationFrontAndBack* pIActivationFrontAndBack )
		:ActivatableFrontAndBack( pIActivationFrontAndBack )
		{};
		
		virtual ~ActivatableFront(){};
		
		virtual void onActivate()
		{
			ActivatableFrontAndBack::m_pIActivationFrontAndBack->activateFromFront();
		}
	};	


	class ActivatableBack : public virtual ActivatableFrontAndBack
	{
	public:		
		ActivatableBack( IActivationFrontAndBack* pIActivationFrontAndBack )
		:ActivatableFrontAndBack( pIActivationFrontAndBack )
		{};
		
		virtual ~ActivatableBack(){};
		
		virtual void onActivate()
		{
			ActivatableFrontAndBack::m_pIActivationFrontAndBack->activateFromBack();
		}
	};	
}

#endif

