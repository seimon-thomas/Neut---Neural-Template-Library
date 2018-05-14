///////////////////////////////////////////////////////////////////////////////////////////////////
//
// InputsFromCSV.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef InputsFromCSV_H
#define InputsFromCSV_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

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

#include "IInputs.h"
#include "Exception.h"



namespace Neural 
{
	template<class T> class InputsFromCSV: public virtual IInputs<T>
	{
	public:
		InputsFromCSV( boost::shared_ptr< std::istream > piStream )
					 :m_piStream( piStream ),
					  m_epochNumber( 0 ),
                      m_skip1stCol( false )
		{
		};
		
		virtual ~InputsFromCSV() 
		{
		};

		virtual unsigned long getEpochNumber()
		{
			return m_epochNumber;
		}
		
        virtual boost::shared_ptr< std::vector<T> > getNextInputSet( size_t requiredSize )
        {
            //
            // TODO: This is common behavious - refactor
            //
            boost::shared_ptr< std::vector<T> > pRetVal = getNextInputSet();
            if( pRetVal->size() != requiredSize )
            {
                THROW_EXCEPTION( "Required size (" << requiredSize << ") does not match next set size(" << pRetVal->size() << ")" )
            }
            return pRetVal;
        }

		virtual boost::shared_ptr< std::vector<T> > getNextInputSet()
		{ 
			
            bool dataRetrieved = false;
            boost::shared_ptr< std::vector<T> > pRetVec;
            
            while( !dataRetrieved )
            {
                boost::shared_ptr< std::vector< std::string > > pVec = getNextLine();

                if( 0 == boost::to_upper_copy<std::string>(pVec->at(0)).compare("DATE") )
                {
                    m_skip1stCol = true;
                }
                else if( 0 == boost::to_upper_copy<std::string>(pVec->at(0)).compare("HEADINGS") )
                {
                    m_skip1stCol = true;
                }
                else if( 0 == boost::to_upper_copy<std::string>(pVec->at(0)).compare("SCALE") )
                {
                    m_skip1stCol = true;
                    m_scalingVec.resize( pVec->size() - 1);
                    for( size_t i=1; i< pVec->size(); ++i )
                    {
                        try
                        {
                            m_scalingVec[ i-1 ] = boost::lexical_cast<double>( pVec->at(i) );
                        }
                        catch( const std::exception& sEx )
                        {
                             THROW_EXCEPTION( "A. lexical_cast failed with exception message: \"" << sEx.what() << "\" trying to convert: \"" << pVec->at(i) << "\" at index: " << i )
                        }
                    }
                }
                else if( 0 == boost::to_upper_copy<std::string>(pVec->at(0)).compare("OFFSET") )
                {
                    m_skip1stCol = true;
                    m_offsetVec.resize( pVec->size() - 1);
                    for( size_t i=1; i< pVec->size(); ++i )
                    {
                        try
                        {
                            m_offsetVec[ i-1 ] = boost::lexical_cast<double>( pVec->at(i) );
                        }
                        catch( const std::exception& sEx )
                        {
                            THROW_EXCEPTION( "B. lexical_cast failed with exception message: \"" << sEx.what() << "\" trying to convert: \"" << pVec->at(i) << "\" at index: " << i )
                        }
                    }
                }
                else if( 0 == boost::to_upper_copy<std::string>(pVec->at(0)).substr(0,4).compare("BASE") )
                {
                    // Just hop over
                    m_skip1stCol = true;
                }
                else
                {
                    dataRetrieved = true;
                    
                    pRetVec = boost::shared_ptr< std::vector<T> >( new std::vector<T>( pVec->size() - (m_skip1stCol ? 1:0) ) );
                                                                       
                    for( size_t i=0; i< pRetVec->size(); ++i )
                    {
                        try
                        {
                            pRetVec->at( i ) = boost::lexical_cast<double>( pVec->at( i + (m_skip1stCol ? 1:0) ) );
                        }
                        catch( const std::exception& sEx )
                        {
                            THROW_EXCEPTION( "C. lexical_cast failed with exception message: \"" << sEx.what() << "\" trying to convert: \"" <<pVec->at( i + (m_skip1stCol ? 1:0) ) << "\" at index: " << ( i + (m_skip1stCol ? 1:0) ) )
                        }
                        
                    }
                    
                    applyScaleAndOffset( pRetVec->begin(), pRetVec->end() );
                }
            }
               
			return pRetVec;
		};
		
	protected:
        template< class InputIterator > void applyScaleAndOffset( InputIterator first, InputIterator last ) const
        {
            // TODO: Refactor these to a for each or something
            if( !m_scalingVec.empty() )
            {
                std::vector< double >::const_iterator scaleIt = m_scalingVec.begin();
                for( InputIterator it = first; it != last; ++it )
                {
                    *it = *it * (*scaleIt++);
                }
            }
            if( !m_offsetVec.empty() )
            {
                std::vector< double >::const_iterator offsetIt = m_offsetVec.begin();
                for( InputIterator it = first; it != last; ++it )
                {
                    *it = *it + (*offsetIt++);
                }
            }
        }
        
        template< class InputIterator > void applyInverseScaleAndOffset( InputIterator first, InputIterator last ) const
        {
            // TODO: Refactor these to a for each or something
            if( !m_offsetVec.empty() )
            {
                std::vector< double >::const_iterator offsetIt = m_offsetVec.begin();
                for( InputIterator it = first; it != last; ++it )
                {
                    *it = *it - (*offsetIt++);
                }
            }
            if( !m_scalingVec.empty() )
            {
                std::vector< double >::const_iterator scaleIt = m_scalingVec.begin();
                for( InputIterator it = first; it != last; ++it )
                {
                    *it = *it / (*scaleIt++);
                }
            }
        }
        boost::shared_ptr< std::vector< std::string > > getNextLine()
        {
			// TODO: maybe not keep these on the stack?
            boost::shared_ptr< std::vector< std::string > > pVec( new std::vector< std::string >() );
			std::string line;

//			if(!std::getline(*m_piStream,line,'\r'))
			if(!std::getline(*m_piStream,line))
			{
				// Indicate new epoch
				newEpoch( ++m_epochNumber );
				// EOS, so need to wind back to the beginning
				m_piStream->clear();
				m_piStream->seekg( 0, std::ios::beg);
				getline(*m_piStream,line);
			}
//            std::cout << line << std::endl;

			boost::tokenizer< boost::escaped_list_separator<char> > tok(line);
			pVec->assign(tok.begin(),tok.end());
            
            boost::to_upper(pVec->at(0));
            
            return pVec;
        }
        
		virtual void newEpoch(unsigned long epochNumber )
		{
			// Overriden
		}
	
		InputsFromCSV():m_epochNumber(0),m_skip1stCol(false){};
		
		boost::shared_ptr< std::istream > m_piStream;
		unsigned long					  m_epochNumber;
        bool                              m_skip1stCol;
		std::vector< double >			  m_scalingVec;
		std::vector< double >			  m_offsetVec;
	};

	template<class T> class InputsFromCSVFile: public virtual InputsFromCSV<T>
	{
	public:
		InputsFromCSVFile( const std::string& csvFilename )
		:InputsFromCSV<T>(),
        m_closed( false )
		{
			m_pifStream = new std::ifstream( csvFilename.c_str() );
			if (!m_pifStream->is_open())
			{
				// TODO: Better Exception handling here
				throw std::runtime_error( "Failed to open ifstream in InputsFromCSV");
			}
			//
			// Give to parent
			//
			InputsFromCSV<T>::m_piStream = boost::shared_ptr< std::istream >( m_pifStream );
		}
		
		virtual ~InputsFromCSVFile() 
		{
            close();
		};
		
	protected:
        void close()
        {
			if( (!m_closed) && m_pifStream->is_open() )
			{
				m_pifStream->close();
                
                m_closed = true;
			}
        }
		std::ifstream* m_pifStream;
        bool m_closed;
	};
	
	
}

#endif