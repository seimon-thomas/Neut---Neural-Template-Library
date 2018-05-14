///////////////////////////////////////////////////////////////////////////////////////////////////
//
// OutputsToCSV.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OutputsToCSV_H
#define OutputsToCSV_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "IOutputs.h"



namespace Neural 
{
	template< typename T, typename ContainerT > class OutputsToCSV: public virtual IOutputs<T,ContainerT>
	{
	public:
		OutputsToCSV( boost::shared_ptr< std::ostream > poStream )
		:m_poStream( poStream ) 
		{
			m_poStream->precision(std::numeric_limits<double>::digits10 + 2);
		};
		
		virtual ~OutputsToCSV() 
		{
		};
		
		//
		// TODO: make this more generic
		//
		virtual void putNextOutputSet(const ContainerT& setToWrite)
		{ 
			typename ContainerT::const_iterator itNextItem = setToWrite.begin();
			while( itNextItem != setToWrite.end() )
			{
				*m_poStream << *itNextItem;
				++itNextItem;
				if( itNextItem != setToWrite.end() )
				{
					*m_poStream << ",";
				}				
			}
			*m_poStream << std::endl;
		};
		
	protected:
		OutputsToCSV(){};
		
		boost::shared_ptr< std::ostream > m_poStream;
	private:
		// For Boost Serialization
		friend class boost::serialization::access;
		template<class Archive>	void serialize(Archive & ar, const unsigned int version)
		{
			boost::serialization::base_object<IOutputs<T,ContainerT> >(*this);
			
			ar & BOOST_SERIALIZATION_NVP( m_poStream );
		};
	};
	
	template<class T, typename ContainerT> class OutputsToCSVFile: public virtual OutputsToCSV<T,ContainerT>
	{
	public:
		OutputsToCSVFile( const std::string& csvFilename )
		{
			m_pofStream = new std::ofstream( csvFilename.c_str() );
			if (!m_pofStream->is_open())
			{
				// TODO: Better Exception handling here
				throw std::runtime_error( "Failed to open ofstream in OutputsToCSV");
			}
			//
			// Set precision
			//
			m_pofStream->precision(std::numeric_limits<double>::digits10 + 2);
			//
			// Give to parent
			//
			OutputsToCSV<T,ContainerT>::m_poStream = boost::shared_ptr< std::ostream >( m_pofStream );
		}
		
		virtual ~OutputsToCSVFile() 
		{
			if( m_pofStream->is_open() )
			{
				m_pofStream->close();
			}
		};
		
	protected:
		std::ofstream* m_pofStream;

	};
	
	
}

#endif