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
//  NodeContainerPeriodicSerializerLoadOnly.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 08/03/2015.
//
//

#ifndef NeuralNetworkApplication_NodeContainerPeriodicSerializerLoadOnly_h
#define NeuralNetworkApplication_NodeContainerPeriodicSerializerLoadOnly_h

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "NodeContainerPeriodicSerializer.h"


namespace Neural
{
    template<typename T, typename TBase> class NodeContainerPeriodicSerializerLoadOnly: public virtual NodeContainerPeriodicSerializer<T,TBase>
    {
    public:
        typedef boost::shared_ptr< INode<T> >					INodePtr;
        typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;
        
        NodeContainerPeriodicSerializerLoadOnly( const std::string& filename, unsigned long long activationIterationSaveInterval )
        :NodeContainerPeriodicSerializer<T,TBase>( filename, activationIterationSaveInterval, true )
        {
        };
        
        NodeContainerPeriodicSerializerLoadOnly(const boost::property_tree::ptree& nodeConfig)
        {
            throw std::runtime_error( "NodeContainerPeriodicSerializer cannot be serialized");
        };
        
        virtual ~NodeContainerPeriodicSerializerLoadOnly()
        {
            
        };
        
    protected:
        virtual void saveChild(const std::string& filename )
        {
            //
            // Null method - we don't save in this impl
            //
        }
        
    };
    
};

#endif
