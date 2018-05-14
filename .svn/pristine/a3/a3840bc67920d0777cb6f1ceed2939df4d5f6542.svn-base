///////////////////////////////////////////////////////////////////////////////////////////////////
//
// OutputsToCSVScaled.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OutputsToCSVScaled_H
#define OutputsToCSVScaled_H

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
#include "InputsFromCSV.h"
#include "OutputsToCSV.h"


namespace Neural 
{
	template< typename T, typename ContainerT > class OutputsToCSVScaled: public virtual OutputsToCSV<T,ContainerT>, public virtual InputsFromCSV<T>
	{
	public:
		OutputsToCSVScaled( boost::shared_ptr< std::ostream > poStream, 
                            boost::shared_ptr< std::istream > piStream ) // For reading scale/offset from
		:OutputsToCSV<T,ContainerT>( poStream ),
         InputsFromCSV<T>( piStream )
		{
            //
            // Prime the pump from inputs (to get the scale & offset)
            //
            InputsFromCSV<T>::getNextInputSet();
		};
		
		virtual ~OutputsToCSVScaled() 
		{
		};
		
		//
		// TODO: make this more generic
		//
		virtual void putNextOutputSet(const ContainerT& setToWrite)
		{ 
            //
            // Allocate storage for the scaled set
            //
            ContainerT scaledSetToWrite( setToWrite );
            //
            // Inverse scale & offset it
            //
            InputsFromCSV<T>::applyInverseScaleAndOffset( scaledSetToWrite.begin(), scaledSetToWrite.end() );
            //
            // Finally write it
            //
            OutputsToCSV<T,ContainerT>::putNextOutputSet( scaledSetToWrite );
		};
		
	protected:
		OutputsToCSVScaled(){};

	};
	
	template<class T, typename ContainerT> class OutputsToCSVScaledFile: public virtual OutputsToCSVFile<T,ContainerT>,
                                                                         public virtual InputsFromCSVFile<T>,
                                                                         public virtual OutputsToCSVScaled<T,ContainerT>
	{
	public:
		OutputsToCSVScaledFile( const std::string& csvOutFilename,
                                const std::string& csvInFilename )
            : OutputsToCSVFile<T,ContainerT>( csvOutFilename ),
              InputsFromCSVFile<T>( csvInFilename ),
              OutputsToCSVScaled<T,ContainerT>()
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
		}
		
		virtual ~OutputsToCSVScaledFile() 
		{

		};
		
	protected:
        
	};
	
	
}

#endif