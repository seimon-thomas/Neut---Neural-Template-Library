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
// ActivateProviderFrontAndBack.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ActivateProviderFrontAndBack_H
#define ActivateProviderFrontAndBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/foreach.hpp>
#include <vector>
#include "ConnectionSite.h"
#include "IActivationFrontAndBack.h"
#include "Activate.h"
#include "ActivatableFrontAndBack.h"

namespace Neural 
{
	class ActivateProviderFrontAndBack: public virtual IActivateProvider, public virtual IActivationFrontAndBack
	{
	public:		
		ActivateProviderFrontAndBack()
		:m_ActivationConnections( NUM_CONNECTION_SITES )
		{			
		};
		
		virtual ~ActivateProviderFrontAndBack()
		{
		};
				
		virtual IActivatePtr	   getActivationConnection( ConnectionSite siteOnThis )
		{
			//
			// First check which site they're asking for to see if we have one already instantiated
			//
			IActivatePtr pRetActivate = m_ActivationConnections[ siteOnThis ];
			//
			// Is it instantiated
			//
			if ( NULL == pRetActivate.get() ) 
			{
				if( INPUTS == siteOnThis )
				{
					// TODO: Why are these nests so deep?
					pRetActivate = IActivatePtr( new Activate( IActivatablePtr( new ActivatableFront( this ) ) ) );
				}
				else
				{
					pRetActivate = IActivatePtr( new Activate( IActivatablePtr( new ActivatableBack( this ) ) ) );
				}
				//
				// Pop this in the m_ActivationConnections (it may be asked for again)
				//
				m_ActivationConnections[ siteOnThis ] = pRetActivate;
			}
			//
			// Return the IActivatePtr
			//
			return pRetActivate;
		};

		//
		// Benign activateFromFront() which is the default behaviour
		// Derived classes override
		//
		virtual void activateFromFront()
		{
		}
		
		//
		// Benign activateFromBack() which is the default behaviour
		// Derived classes override
		//
		virtual void activateFromBack()
		{
		}
		
		
	protected:
		
		std::vector< IActivatePtr >		m_ActivationConnections;		
	};
}

#endif
