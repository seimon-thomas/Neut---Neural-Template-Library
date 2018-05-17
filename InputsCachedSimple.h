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
//  InputsCachedSimple.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 17/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_InputsCachedSimple_h
#define NeuralNetworkApplication_InputsCachedSimple_h

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>
#include "Inputs.h"
#include "Exception.h"

namespace Neural 
{
	template<class T, class TInputProvider > class InputsCachedSimple: public virtual Inputs<T>
	{
	public:
		InputsCachedSimple( std::vector< boost::shared_ptr< std::vector<T> > > &inputSet, 
                            boost::shared_ptr< TInputProvider > pInputProvider )
		:Inputs<T>( inputSet ),
		m_pInputProvider( pInputProvider ) 
        {
            //
            // Fill the inputSet from the provider
			while( m_pInputProvider->getEpochNumber() == 0 )
			{
				boost::shared_ptr< std::vector<T> > pNextSet = m_pInputProvider->getNextInputSet();
				//
				// Check again, as input provides sometimes don't know an epoch end till they've past it
				//
				if( m_pInputProvider->getEpochNumber() == 0 )
				{
					Inputs<T>::m_InputSet.push_back( pNextSet );
				}
			}
			return Inputs<T>::getNextInputSet();
        };
		
		virtual ~InputsCachedSimple() {};
        
	protected:
		boost::shared_ptr< TInputProvider >                 m_pInputProvider;
	};
	
}


#endif
