////////////////////////////////////////////////////////////////////////////////////////////////////
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
//  Row.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 26/11/2013.
//
//

#ifndef NeuralNetworkApplication_TrainingRow_h
#define NeuralNetworkApplication_TrainingRow_h

#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>

namespace Neural
{
    template<class T> class TrainingRow
    {
    private:
        std::string m_Heading;
        boost::shared_ptr< std::vector< T > > m_pValues;
        
    public:
        TrainingRow()
        :m_pValues( new std::vector< T >() )
        {
            
        }

        TrainingRow( const std::string& heading )
        :m_Heading( heading ),
         m_pValues( new std::vector< T >() )
        {
            
        }
        
        // Copy constructor
        TrainingRow( const TrainingRow& other )
        :m_Heading( other.m_Heading ),
         m_pValues( new std::vector< T >( *(other.m_pValues) ) )
        {
        }
        
        // Assignment operator
        TrainingRow& operator=( const TrainingRow& rhs )
        {
            m_Heading = rhs.m_Heading;
            m_pValues = boost::shared_ptr< std::vector< T > >( new std::vector< T >( *(rhs.m_pValues) ) );
            return *this;
        }
        
        virtual ~TrainingRow(){};
        
        // Copy Values
        virtual void copyValuesTo( TrainingRow& targetRow )
        {
            targetRow.m_pValues = boost::shared_ptr< std::vector< T > >( new std::vector< T >( *m_pValues ) );
        }

        virtual const std::string& getHeading() const
        {
            return this->m_Heading;
        }
        
        virtual void setHeading(const std::string& newHeading)
        {
            this->m_Heading = newHeading;
        }
        
        virtual unsigned long size() const
        {
            return m_pValues->size();
        }
        
        virtual std::istream& populateFromStream( std::istream& is )
        {
            std::string line;
            
            if(std::getline(is,line))
            {
                parseFromString( line );
            }
            
            return is;
        }
        
        void parseFromString(const std::string& line)
        {
            // Build a tokenizer
            boost::tokenizer< boost::escaped_list_separator<char> > tok(line);
            
			boost::tokenizer< boost::escaped_list_separator<char> >::const_iterator itNextItem = tok.begin();
            
            if(itNextItem != tok.end())
            {
                // First entry is the row heading
                m_Heading = *itNextItem;
                ++itNextItem;
                
                // Next populate the values
                while( itNextItem != tok.end() )
                {
                    m_pValues->push_back( boost::lexical_cast< T >( *itNextItem ) );
                    ++itNextItem;
                }
            }
        }
        
        
        virtual std::ostream& outputToStream( std::ostream& os ) const
        {
            os.precision(std::numeric_limits<double>::digits10 + 2);
            
            os << this->m_Heading;
            
            for(typename std::vector<T>::const_iterator itNextItem = this->m_pValues->begin();
                itNextItem != this->m_pValues->end();
                itNextItem++ )
            {
                os << "," << *itNextItem;
            }
            
            return os;
        }
        
        template< class TBinOp > void transform( const TrainingRow<T>& inputTrainingRow,
                                                 TrainingRow<T>& outputTrainingRow,
                                                 TBinOp binaryOperator )
        {
            // First ensure that the output has enough space fo the result
            if(m_pValues->size() > outputTrainingRow.m_pValues->size() )
            {
                outputTrainingRow.m_pValues->resize(m_pValues->size());
            }
            // Perform the transformation
            std::transform( this->m_pValues->begin(),
                            this->m_pValues->end(),
                            inputTrainingRow.m_pValues->begin(),
                            outputTrainingRow.m_pValues->begin(),
                            binaryOperator );
        }
        
        template< class TUnOp > void transform( TrainingRow<T>& outputTrainingRow,
                                                TUnOp unitaryOperator )
        {
            // First ensure that the output has enough space fo the result
            if(m_pValues->size() > outputTrainingRow.m_pValues->size() )
            {
                outputTrainingRow.m_pValues->resize(m_pValues->size());
            }
            // Perform the transformation
            std::transform( this->m_pValues->begin(),
                           this->m_pValues->end(),
                           outputTrainingRow.m_pValues->begin(),
                           unitaryOperator );
        }
        
    };
    
}

template<typename T> std::ostream & operator<<(std::ostream &os, const Neural::TrainingRow< T >& row)
{
    return row.outputToStream( os );
}

template<typename T> std::istream &operator>>(std::istream &is, Neural::TrainingRow< T >& row)
{
    return row.populateFromStream( is );
}

#endif
