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
// ReceiveFromFront.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ReceiveFromFront_H
#define ReceiveFromFront_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "ReceiveFromFrontAndBack.h"
#include "utils.h"



namespace Neural 
{
	typedef size_t ConnectionHandle;
	
	
	template<class T> class ReceiveFromFront : public virtual ReceiveFromFrontAndBack<T>
	{
	public:		
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType ValueContainerType;
        typedef boost::shared_ptr< ValueContainerType >             ValueContainerTypePtr;

		ReceiveFromFront():m_pFrontInputValues( new ValueContainerType ){};
		virtual ~ReceiveFromFront(){};

        virtual ValueContainerType* onGrowAndGetReceiverValuesFront()
        { 
            m_pFrontInputValues->push_back( T() );
            onFrontInputValueAddedAt( m_pFrontInputValues->size() - 1 );
            return m_pFrontInputValues.get();
        };
 
        virtual void SetReceiverValuesFront( ValueContainerTypePtr pBulkInputs )
        {
            m_pFrontInputValues = pBulkInputs;
            for( size_t i = 0; i < m_pFrontInputValues->size(); ++i )
            {
                onFrontInputValueAddedAt( i );
            }
        };
        
	protected:
        virtual void onFrontInputValueAddedAt( size_t addedIndex ){};
        
		ValueContainerTypePtr	m_pFrontInputValues;
	};
	
	
}

#endif

