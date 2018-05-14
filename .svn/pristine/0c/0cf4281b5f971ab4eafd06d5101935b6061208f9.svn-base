///////////////////////////////////////////////////////////////////////////////////////////////////
//
// IInputs.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IInputs_H
#define IInputs_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/shared_ptr.hpp>

namespace Neural 
{
	template<class T> class IInputs
	{
	public:
        typedef std::vector<T> ContainerType;
        
		virtual boost::shared_ptr< ContainerType > getNextInputSet()=0;
        virtual boost::shared_ptr< std::vector<T> > getNextInputSet( size_t requiredSize )=0;
		
		virtual unsigned long getEpochNumber()=0;

		virtual ~IInputs() {};
			
	protected:
		IInputs() {};
	
	};

	typedef IInputs<double> IInputsDouble;
	typedef boost::shared_ptr< IInputsDouble > IInputsDoublePtr;
	
}

#endif