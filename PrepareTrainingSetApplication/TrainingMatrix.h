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
//  TrainingMatrix.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 27/11/2013.
//
//

#ifndef NeuralNetworkApplication_TrainingMatrix_h
#define NeuralNetworkApplication_TrainingMatrix_h

#include <string>
#include <algorithm>
#include <functional>

#include <boost/foreach.hpp>

#include "TrainingRow.h"
#include "MaxFunctor.h"
#include "MinFunctor.h"
#include "ScaleFunctor.h"
#include "OffsetFunctor.h"
#include "Delta.h"

namespace Neural
{
    template < class T >   class TrainingMatrix
    {
    private:
        TrainingRow<std::string>    m_HeadingsRow;
        TrainingRow< T >            m_ScaleRow;
        TrainingRow< T >            m_OffsetRow;
        TrainingRow< T >            maxRow;
        TrainingRow< T >            minRow;
        TrainingRow< T >            rangeRow;
        boost::shared_ptr< TrainingRow< T > >   m_BaseRow;
        
        std::vector< boost::shared_ptr< TrainingRow< T > > >    m_valueRows;
        
    public:
        TrainingMatrix()
        :m_ScaleRow("Scale"),
         m_OffsetRow("Offset"),
         maxRow("Max"),
         minRow("Min"),
         rangeRow("Range")
        
        {
            
        }
        
        virtual ~TrainingMatrix(){};
        
        virtual std::istream& populateFromStream( std::istream& is )
        {
            // First read the heading row
            if(is >> m_HeadingsRow)
            {
                // Got heading ok, so populate value rows
                boost::shared_ptr< TrainingRow< T > > pValueRow( new TrainingRow<T>() );
                while( is >> *pValueRow )
                {
                    // Cool got a value row, so add it
                    m_valueRows.push_back( pValueRow );
                    
                    // Prep a new value row for the next row in the stream
                    pValueRow = boost::shared_ptr< TrainingRow< T > >( new TrainingRow<T>() );
                }
            }
            return is;
        }
        
        virtual std::ostream& outputToStream( std::ostream& os ) const
        {
            os.precision(std::numeric_limits<double>::digits10 + 2);
            
            os <<  m_HeadingsRow << std::endl;
            os <<  maxRow << std::endl;
            os <<  minRow << std::endl;
            os <<  rangeRow << std::endl;
            os <<  m_ScaleRow << std::endl;
            os <<  m_OffsetRow << std::endl;
            os <<  *m_BaseRow << std::endl;

            for(typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_iterator itNextItem = m_valueRows.begin();
                itNextItem != m_valueRows.end();
                itNextItem++ )
            {
                os << **itNextItem << std::endl;
            }
            
            return os;
        }
        
        virtual unsigned long trainingInputsToStream( std::ostream& os, unsigned long numInputRows, unsigned long numOutputRows ) const
        {
            os.precision(std::numeric_limits<double>::digits10 + 2);
            
            os <<  m_HeadingsRow << std::endl;
            os <<  m_ScaleRow << std::endl;
            os <<  m_OffsetRow << std::endl;
            os <<  *m_BaseRow << std::endl;
            
            unsigned long rowsToOutput = m_valueRows.size() - numOutputRows;
            for(unsigned long i=0; i < rowsToOutput; ++i)
            {
                os << *( m_valueRows[i] ) << std::endl;
            }
            
            return m_valueRows[0]->size() * numInputRows;
        }
        
        virtual unsigned long trainingTargetsToStream( std::ostream& os, unsigned long numInputRows, unsigned long numOutputRows ) const
        {
            os.precision(std::numeric_limits<double>::digits10 + 2);
            
            os <<  m_HeadingsRow << std::endl;
            os <<  m_ScaleRow << std::endl;
            os <<  m_OffsetRow << std::endl;
            os <<  *m_BaseRow << std::endl;
            
            for(unsigned long i=numInputRows; i < m_valueRows.size(); ++i)
            {
                os << *( m_valueRows[i] ) << std::endl;
            }
            
            return m_valueRows[0]->size() * numOutputRows;
        }
        
        virtual void process(T scale, T offset)
        {

            generateDeltas();
            generate( maxRow, MaxFunctor<T>() );
            generate( minRow, MinFunctor<T>() );
            
            // Get the range (max-min)
            maxRow.transform( minRow, rangeRow, std::minus<T>());
            
            // Generate the scale
            rangeRow.transform( m_ScaleRow, ScaleFunctor<T>(scale));
            
            // Generate offset - first multiply min * scale, then offset
            TrainingRow< T > minXscaleRow;
            minRow.transform( m_ScaleRow, minXscaleRow, std::multiplies<T>());
            minXscaleRow.transform( m_OffsetRow, OffsetFunctor<T>(offset));
        }
        
    protected:
        virtual void generateDeltas()
        {
            // Start at the last row
            for(typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_reverse_iterator itPrevRow = m_valueRows.rbegin()+1;
                itPrevRow != m_valueRows.rend();
                itPrevRow++ )
            {
                TrainingRow< T >& previousRow = **(itPrevRow);
                TrainingRow< T >& nextRow = **(itPrevRow-1);
                
                nextRow.transform(previousRow, nextRow, delta< T >());
            }
            // Lift the first row out & make it base
            m_BaseRow = *(m_valueRows.begin());
            m_BaseRow->setHeading( "Base:" + m_BaseRow->getHeading());
            m_valueRows.erase(m_valueRows.begin());
        }
        
        template< class TBinOp > void generate( TrainingRow< T >& resultRow, TBinOp binaryOperator )
        {
            // Seed the result with the first row
            typename std::vector< boost::shared_ptr< TrainingRow< T > > >::const_iterator itNextRow = m_valueRows.begin();
            (*itNextRow)->copyValuesTo( resultRow );
            itNextRow++;
            
            for(; itNextRow != m_valueRows.end(); itNextRow++)
            {
                (*itNextRow)->transform( resultRow, resultRow, binaryOperator );
            }
        }
    };
}

template<typename T> std::ostream & operator<<(std::ostream &os, const Neural::TrainingMatrix< T >& matrix)
{
    return matrix.outputToStream(os);
}

template<typename T> std::istream &operator>>(std::istream &is, Neural::TrainingMatrix< T >& matrix)
{
    return matrix.populateFromStream( is );
}


#endif
