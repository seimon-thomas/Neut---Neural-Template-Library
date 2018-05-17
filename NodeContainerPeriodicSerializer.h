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
// NodeContainerPeriodicSerializer.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeContainerPeriodicSerializer_H
#define NodeContainerPeriodicSerializer_H

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

#include "NodeNetworkPillar.h"

#include "NodeInput.h"
#include "NodeSerializer.h"



namespace Neural 
{
	template<typename T, typename TBase> class NodeContainerPeriodicSerializer: public virtual TBase
	{
	public:
		typedef boost::shared_ptr< INode<T> >					INodePtr;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;
		
		NodeContainerPeriodicSerializer( const std::string& filename, unsigned long long activationIterationSaveInterval, bool loadOnly=false )
		:m_BaseFilename( filename ),
		 m_ActivationIterationSaveInterval( activationIterationSaveInterval ),
		 m_TotalIterationsSoFar( 0 ),
		 m_Deserialized( false ),
         m_LoadOnly( loadOnly )
		{
			boost::filesystem::path filePath( filename );
			
			if( boost::filesystem::exists( filePath ) )
			{
				//
				// Extract the iterationless filename and iterations
				//
				extractIterations( filename, m_BaseFilename, m_TotalIterationsSoFar );
				//
				// Load the child now
				//
				loadChild( filename );
			}
			else 
			{
				//
				// It doesn't exist, so is it  continuation or brand-new
				//
				boost::filesystem::path parentPath = filePath.parent_path();
				
				std::vector< boost::filesystem::path >  dirFiles;
				std::copy( boost::filesystem::directory_iterator(parentPath), boost::filesystem::directory_iterator(), std::back_inserter(dirFiles) );
				
				std::string baseStem = filePath.stem().string();
				size_t stemLength = baseStem.length();
				
				std::vector< std::string >  filteredDirFiles;
				
				for( std::vector< boost::filesystem::path >::iterator it = dirFiles.begin();
					 it != dirFiles.end();
					 ++it )
				{
					if( baseStem.compare( 0, stemLength, (*it).stem().string(), 0, stemLength ) == 0 )
					{
						filteredDirFiles.push_back( it->string() );
					}
				}
				
				// Any matches?
				
				if( filteredDirFiles.empty() )
				{
					// 
					// Not found, so not a continuation, but a brand new start
					// Construct the base filename
					//
					std::stringstream ssBaseFilename;
					ssBaseFilename << parentPath << "/" << baseStem << ".Iter-";
					m_BaseFilename = ssBaseFilename.str();
                    
                    // This is here because of a bug (I think) in std::stringstream which places a double quote in the m_BaseFilename
                    m_BaseFilename.erase(std::remove(m_BaseFilename.begin(), m_BaseFilename.end(), '"'), m_BaseFilename.end());
				}
				else
				{
					//
					// Find the latest path
					//
					for( std::vector< std::string >::iterator it = filteredDirFiles.begin();
						it != filteredDirFiles.end();
						++it )
					{
						std::string			outBaseFilename;
						unsigned long long	outIterations;
						
						extractIterations( *it, outBaseFilename, outIterations );

						if( outIterations >= m_TotalIterationsSoFar )
						{
							m_TotalIterationsSoFar = outIterations;
							m_BaseFilename = outBaseFilename;
						}
						
					}
					//
					// Load the latest one
					//
					loadChild( getFilename() );
				}

			}

		};
		
		NodeContainerPeriodicSerializer(const boost::property_tree::ptree& nodeConfig)
		{
			throw std::runtime_error( "NodeContainerPeriodicSerializer cannot be serialized"); 
		};
		
		virtual ~NodeContainerPeriodicSerializer()
		{
			//
			// TODO: Move this to the shutDown method when it arrives !
			//
			//
			// Save the Child
			//
			saveChild( getFilename() );
			
		};
		
		virtual void activateFromFront()
		{
			TBase::activateFromFront();
		}
		
		virtual void activateFromBack()
		{
			//
			// First delegate activation to the back
			//
			TBase::activateFromBack();
			//
			// Check if the requisite number of iterations have passed
			//
			if( 0 == (++m_TotalIterationsSoFar)%(m_ActivationIterationSaveInterval) )
			{
				//
				// Save the Child
				//
				saveChild( getFilename() );
			}
		}
		
		virtual void addChild( INodePtr pItem )
		{
			//
			// First, check if we've been deserialized
			//
			if( m_Deserialized )
			{
				throw std::runtime_error("Cannot add child, as child node has been deserialized from disk!");
			}
			//
			// Delegate upwards
			//
			TBase::addChild( pItem );
		}

		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			throw std::runtime_error( "NodeContainerPeriodicSerializer cannot be serialized"); 
		};
		
	protected:
		virtual void saveChild(const std::string& filename )
		{
            if(!m_LoadOnly)
            {
                //
                // Do we have children to save?
                //
                VectorINodePtr pchildren = this->getChildren();
                if( pchildren->size() )
                {
                    //
                    // Next save the first child
                    //
                    m_Serializer.serialize( *(pchildren->begin()), filename );
                }
                else 
                {
                    //
                    // Soemthing's gone wrong - we have no children, so throw std::runtime_error(
                    //
                    throw std::runtime_error( "NodeContainerPeriodicSerializer must have children to serialize");
                }
            }
		}
	
	
		virtual void loadChild(const std::string& filename )
		{
			//
			// Deserialise the child
			//
			INodePtr pChild = m_Serializer.deserialze( filename );
			//
			// And add the child
			//
			addChild( pChild );
			//
			// Set our deserialized flag to let us know we've done this
			//
			m_Deserialized = true;
			
		}
		
		std::string getFilename() const
		{
			std::stringstream strFilename;
			strFilename << m_BaseFilename << boost::lexical_cast<std::string>( m_TotalIterationsSoFar ) << ".xml";
			return strFilename.str();
		}

		void extractIterations( const std::string& inFilename, std::string& outBaseFilename, unsigned long long &outIterations ) const
		{ 
			//
			// Extract the iterationless filename and iterations
			//
			size_t lastDotPos = inFilename.rfind(".");
			
			std::string withoutExtension( inFilename.substr(0,lastDotPos) );
			
            std::string iterKey(".Iter-");
            
			size_t secondLastDotPos = withoutExtension.rfind(iterKey) + iterKey.length();
			
			outBaseFilename = withoutExtension.substr(0, secondLastDotPos);
			
			std::string sIterationsSoFar = withoutExtension.substr(secondLastDotPos, lastDotPos);
			
			outIterations = boost::lexical_cast<unsigned long long>( sIterationsSoFar );
			
		}
		
		const unsigned long long m_ActivationIterationSaveInterval;
		unsigned long long m_TotalIterationsSoFar;
		std::string m_BaseFilename;
		NodeSerializer< T > m_Serializer;
		bool m_Deserialized;
        bool m_LoadOnly;
	};
	
};


#endif
