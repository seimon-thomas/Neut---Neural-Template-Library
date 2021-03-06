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
//  UnDeltaResultsSet.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 16/03/2015.
//
//

#ifndef NeuralNetworkApplication_UnDeltaResultsSet_h
#define NeuralNetworkApplication_UnDeltaResultsSet_h
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "Exception.h"
#include "ResultsMatrix.h"

namespace Neural
{
    template<class T> class UnDeltaResultsSet
    {
    private:
        const std::string m_inputFileName;
        const std::string m_outputFileName;
        
        std::ifstream* m_pifStream;
        std::ofstream* m_pofStream;
        
        
        
    public:
        UnDeltaResultsSet( const std::string& inputFileName,
                           const std::string& outputFileName)
        :m_inputFileName( inputFileName ),
        m_outputFileName( outputFileName )
        {
            
        }
        
        virtual ~UnDeltaResultsSet(){}
        
        void run()
        {
            open();
            
            Neural::ResultsMatrix<double> resultsMatrix;
            
            *m_pifStream >> resultsMatrix;
            
            resultsMatrix.process();
            
            *m_pofStream << resultsMatrix;
            
            close();
        }
    protected:
        
        void open()
        {
            m_pifStream = new std::ifstream( m_inputFileName.c_str() );
            if (!m_pifStream->is_open())
            {
                THROW_EXCEPTION( "Failed to open ifstream");
            }
            
            m_pofStream = new std::ofstream( m_outputFileName.c_str() );
            if (!m_pofStream->is_open())
            {
                THROW_EXCEPTION( "Failed to open ofstream");
            }
            
        }
        void close()
        {
            if( m_pifStream->is_open() )
            {
                m_pifStream->close();
            }
            delete m_pifStream;
            
            if( m_pofStream->is_open() )
            {
                m_pofStream->close();
            }
            delete m_pofStream;
            
        }
        
    };
}


#endif
