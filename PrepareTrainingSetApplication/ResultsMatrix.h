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
//  ResultsMatrix.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 16/03/2015.
//
//

#ifndef NeuralNetworkApplication_ResultsMatrix_h
#define NeuralNetworkApplication_ResultsMatrix_h

#include <string>
#include <algorithm>
#include <functional>

#include <boost/foreach.hpp>

#include "TrainingRow.h"
#include "MaxFunctor.h"
#include "MinFunctor.h"
#include "ScaleFunctor.h"
#include "OffsetFunctor.h"
#include "UnDeltaFunctor.h"

namespace Neural
{
    template < class T >   class ResultsMatrix
    {
    private:
        TrainingRow<std::string>    m_HeadingsRow;
 
        std::vector< boost::shared_ptr< TrainingRow< T > > >    m_metaDataRows;
        std::vector< boost::shared_ptr< TrainingRow< T > > >    m_valueRows;
        
    public:
        ResultsMatrix()
        {
            
        }
        
        virtual ~ResultsMatrix(){};
        
        virtual std::istream& populateFromStream( std::istream& is )
        {
            // First read the heading rows
            is >>  m_HeadingsRow;

            // Got headings ok, so populate value rows (N.B. First will be the base row)
            boost::shared_ptr< TrainingRow< T > > pValueRow( new TrainingRow<T>() );
            while( is >> *pValueRow )
            {
                // Is it a true value row or a meta row
                if( isMetaRow( *pValueRow) )
                {
                    m_metaDataRows.push_back( pValueRow );
                }
                else
                {
                    // Cool got a value row, so add it
                    m_valueRows.push_back( pValueRow );
                }
                
                // Prep a new value row for the next row in the stream
                pValueRow = boost::shared_ptr< TrainingRow< T > >( new TrainingRow<T>() );
            }
            return is;
        }
        
        virtual std::ostream& outputToStream( std::ostream& os ) const
        {
            os.precision(std::numeric_limits<double>::digits10 + 2);

            os <<  m_HeadingsRow << std::endl;
            
            for(typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_iterator itNextItem = m_metaDataRows.begin();
                itNextItem != m_metaDataRows.end();
                itNextItem++ )
            {
                os << **itNextItem << std::endl;
            }

            for(typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_iterator itNextItem = m_valueRows.begin();
                itNextItem != m_valueRows.end();
                itNextItem++ )
            {
                os << **itNextItem << std::endl;
            }
            
            return os;
        }
        
        virtual void process()
        {
            typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_iterator itNextRow = m_valueRows.begin()+1;

            for(; itNextRow != m_valueRows.end(); itNextRow++)
            {
                TrainingRow< T >& previousRow = **(itNextRow-1);
                TrainingRow< T >& nextRow = **(itNextRow);

                (*itNextRow)->transform( previousRow, nextRow, UnDeltaFunctor< T >() );
            }
            
        }
        
    private:
        bool isMetaRow( const TrainingRow< T >& trainingRow )
        {
            bool retVal = false;
            if( trainingRow.getHeading().compare("Max") == 0 ) retVal = true;
            if( trainingRow.getHeading().compare("Min") == 0 ) retVal = true;
            if( trainingRow.getHeading().compare("Range") == 0 ) retVal = true;
            if( trainingRow.getHeading().compare("Scale") == 0 ) retVal = true;
            if( trainingRow.getHeading().compare("Offset") == 0 ) retVal = true;
            
            return retVal;
        }
        
    };
    
}

template<typename T> std::ostream & operator<<(std::ostream &os, const Neural::ResultsMatrix< T >& matrix)
{
    return matrix.outputToStream(os);
}

template<typename T> std::istream &operator>>(std::istream &is, Neural::ResultsMatrix< T >& matrix)
{
    return matrix.populateFromStream( is );
}


#endif
