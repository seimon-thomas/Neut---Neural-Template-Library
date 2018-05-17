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
// Inputs.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef Inputs_H
#define Inputs_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/shared_ptr.hpp>
#include "IInputs.h"
#include "Exception.h"

namespace Neural 
{
	template<class T> class Inputs: public virtual IInputs<T>
	{
	public:
		Inputs( std::vector< boost::shared_ptr< std::vector<T> > > &inputSet )
			:m_InputSet( inputSet ),
			 m_NextInputSetIdx( 0 ){};
			 
		virtual ~Inputs() {};
		
		virtual boost::shared_ptr< std::vector<T> > getNextInputSet()
		{ 
            size_t nextInputSetIdx = m_NextInputSetIdx;
            size_t nextInputSetSize = m_InputSet.size();
            size_t wrappedIdx = nextInputSetIdx % nextInputSetSize;
            ++m_NextInputSetIdx;
			return m_InputSet[ wrappedIdx ]; 
		}
        
        virtual boost::shared_ptr< std::vector<T> > getNextInputSet( size_t requiredSize )
        {
            //
            // TODO: This is common behavious - refactor
            //
            boost::shared_ptr< std::vector<T> > pRetVal = getNextInputSet();
            if( pRetVal->size() != requiredSize )
            {
                THROW_EXCEPTION( "Required size (" << requiredSize << ") does not match next set size(" << pRetVal->size() << ")" )
            }
            return pRetVal;
        }
		
		virtual unsigned long getEpochNumber()
		{
			return m_NextInputSetIdx/m_InputSet.size();
		}

		
	protected:
		Inputs()
		:m_NextInputSetIdx( 0 ) 
        {
        };
		
		size_t m_NextInputSetIdx;
		std::vector< boost::shared_ptr< std::vector<T> > > m_InputSet;
	};
	
}

#endif
