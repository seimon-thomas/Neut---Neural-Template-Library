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
// ReceiveFromBack.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ReceiveFromBack_H
#define ReceiveFromBack_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "INode.h"
#include "ReceiveFromFrontAndBack.h"
#include "utils.h"



namespace Neural 
{
	typedef size_t ConnectionHandle;
	
	
	template<class T> class ReceiveFromBack : public virtual ReceiveFromFrontAndBack<T>
	{
	public:		
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType ValueContainerType;
        typedef boost::shared_ptr< ValueContainerType >             ValueContainerTypePtr;

		ReceiveFromBack():m_pBackInputValues( new ValueContainerType ){};
		virtual ~ReceiveFromBack(){};
		
        virtual ValueContainerType* onGrowAndGetReceiverValuesBack()
        { 
            m_pBackInputValues->push_back( T() );
            onBackInputValueAddedAt( m_pBackInputValues->size() - 1 );
            return m_pBackInputValues.get(); 
        };

        virtual void SetReceiverValuesBack( ValueContainerTypePtr pBulkInputs )
        {
            m_pBackInputValues = pBulkInputs;
            for( size_t i = 0; i < m_pBackInputValues->size(); ++i )
            {
                onBackInputValueAddedAt( i );
            }
        };

        
	protected:
        virtual void onBackInputValueAddedAt( size_t addedIndex ){};

		ValueContainerTypePtr   m_pBackInputValues;
	};
	
	
}

#endif

