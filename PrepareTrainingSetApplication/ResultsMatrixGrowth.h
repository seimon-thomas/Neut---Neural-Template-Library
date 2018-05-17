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
//  ResultsMatrixGrowth.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 10/04/2015.
//
//

#ifndef NeuralNetworkApplication_ResultsMatrixGrowth_h
#define NeuralNetworkApplication_ResultsMatrixGrowth_h
#include <string>
#include <algorithm>
#include <functional>

#include <boost/foreach.hpp>

#include "TrainingRow.h"
#include "MaxFunctor.h"
#include "MinFunctor.h"
#include "ScaleFunctor.h"
#include "OffsetFunctor.h"
#include "GrowthFunctor.h"

namespace Neural
{
    template < class T >   class ResultsMatrixGrowth
    {
    private:
        TrainingRow<std::string>    m_HeadingsRow;
        
        boost::shared_ptr< TrainingRow< T > >                   m_pBaseDateRow;
        std::vector< boost::shared_ptr< TrainingRow< T > > >    m_valueRows;
        
    public:
        ResultsMatrixGrowth()
        {
            
        }
        
        virtual ~ResultsMatrixGrowth(){};
        
        virtual std::istream& populateFromStream( std::istream& is )
        {
            // Initialise forecast counter
            int forecastNo = 0;
            
            // First read the heading rows
            is >>  m_HeadingsRow;
            
            // Got headings ok, so populate value rows (N.B. First will be the base row)
            boost::shared_ptr< TrainingRow< T > > pValueRow( new TrainingRow<T>() );
            while( is >> *pValueRow )
            {
                // Don't store meta rows
                if( !isMetaRow( *pValueRow ) )
                {
                    // Is it a forecast row
                    if( isForecastRow( *pValueRow ) )
                    {
                        // Set the heading to the forecast number
                        pValueRow->setHeading( std::to_string( ++forecastNo ) );
                        
                        // Cool got a forecast row, so add it
                        m_valueRows.push_back( pValueRow );
                    }
                    else
                    {
                        // Not a forecast row, so store it in m_pBaseDateRow
                        // Which will eventually become valid
                        m_pBaseDateRow = pValueRow;
                    }
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
            os << *m_pBaseDateRow << std::endl;
            
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
            TrainingRow< T >& baseDateRow = *m_pBaseDateRow;
            
            typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_iterator itNextRow = m_valueRows.begin();
            
            for(; itNextRow != m_valueRows.end(); itNextRow++)
            {
                TrainingRow< T >& nextRow = **(itNextRow);
                
                (*itNextRow)->transform( baseDateRow, nextRow, GrowthFunctor< T >() );
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
        
        bool isForecastRow( const TrainingRow< T >& trainingRow )
        {
            return trainingRow.getHeading().compare("forecast") == 0;
        }
        
    };
    
}

template<typename T> std::ostream & operator<<(std::ostream &os, const Neural::ResultsMatrixGrowth< T >& matrix)
{
    return matrix.outputToStream(os);
}

template<typename T> std::istream &operator>>(std::istream &is, Neural::ResultsMatrixGrowth< T >& matrix)
{
    return matrix.populateFromStream( is );
}



#endif
