///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2010-2018 Seimon Thomas
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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

