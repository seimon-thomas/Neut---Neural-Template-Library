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
