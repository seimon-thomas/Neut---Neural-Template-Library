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
//  Outputs.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 17/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_Outputs_h
#define NeuralNetworkApplication_Outputs_h

#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/shared_ptr.hpp>

#include "IOutputs.h"

namespace Neural 
{
    template<typename T, typename ContainerT> class Outputs: public virtual IOutputs<typename T, typename ContainerT> 
    {
    public:
        Outputs( std::vector< boost::shared_ptr< std::vector<T> > > &outputStorage ) 
        :m_OutputStorage( outputStorage )
        {
        };

        virtual void putNextOutputSet(const ContainerT& setToWrite)
        {
            //
            // Create a new slot copying what's been passed.
            //
            boost::shared_ptr< std::vector<T> > pNextSetToWrite( new std::vector<T>(setToWrite.begin(),setToWrite.end()-1) );
            //
            // Push it back
            //
            m_outputStorage.push_back( pNextSetToWrite );
        };
        
        virtual ~Outputs() 
        {
        };
    
    protected:
        std::vector< boost::shared_ptr< std::vector<T> > >& m_outputStorage;
  
    };
}

#endif
