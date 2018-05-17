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
//  OutputsScaled.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 18/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_OutputsScaled_h
#define NeuralNetworkApplication_OutputsScaled_h

#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/shared_ptr.hpp>

#include "IOutputs.h"
#include "Outputs.h"

namespace Neural 
{
    template<typename T> class OutputsScaled: public virtual Outputs<typename T, typename ContainerT>,
                                              public virtual InputsFromCSVFile<T>
    {
    public:
        OutputsScaled( std::vector< boost::shared_ptr< std::vector<T> > > &outputStorage,
                       const std::string& csvInFilename ) 
        :Outputs<T,ContainerT>( outputStorage ),
         InputsFromCSVFile<T>( csvInFilename )
        {
            //TODO: this really needs to be done in a better way
            //
            // Prime the pump from inputs (to get the scale & offset)
            //
            InputsFromCSV<T>::getNextInputSet();
            //
            // Close the file
            //
            InputsFromCSVFile<T>::close();
       };
        
        virtual void putNextOutputSet(const ContainerT& setToWrite)
        {
            //
            // Take a copy of the set to write
            //
            ContainerT scaledSetToWrite = setToWrite;
            //
            // Scale it
            // 
            InputsFromCSV<T>::applyInverseScaleAndOffset( scaledSetToWrite.begin(), scaledSetToWrite.end() );
            //
            // Call base class implimentation to write it
            //
            Outputs<T,ContainerT>::putNextOutputSet( setToWrite );
        };
        
        virtual ~OutputsScaled() 
        {
        };
        
        
    };
}



#endif
