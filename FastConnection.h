//
//  FastConnection.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 13/11/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_FastConnection_h
#define NeuralNetworkApplication_FastConnection_h

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

namespace Neural 
{
	template<class T> class FastConnection
	{
	public:		
		FastConnection( std::vector< T >* pTargetValues,
                        size_t targetIdx )
        :m_pTargetValues( pTargetValues ),
         m_targetIdx( targetIdx )
        {};
		
        ~FastConnection(){};
		
		inline void send( T inputValue )
        {
            if( m_pTargetValues ) 
                (*m_pTargetValues)[ m_targetIdx ] = inputValue;
        }
        
        inline size_t getIndex()
        {
            return m_targetIdx;
        }
        
        inline size_t size()
        {
            if( m_pTargetValues ) 
                return m_pTargetValues->size();
            else
                return 0;
            
        }
	protected:
		std::vector< T >* m_pTargetValues;
        const size_t      m_targetIdx;
	};
    
}


#endif
