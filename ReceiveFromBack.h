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

