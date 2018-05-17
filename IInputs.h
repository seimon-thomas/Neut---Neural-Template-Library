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
