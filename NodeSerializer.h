///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeSerializer.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeSerializer_H
#define NodeSerializer_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>
#include <map>
#include <typeinfo>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional.hpp>

#include "NodeNetworkLayer.h"
#include "NodeNetworkPillar.h"
#include "NodePerceptronBias.h"
#include "NodePerceptronTraining.h"
#include "NodePassThrough.h"
#include "NodePassThroughBulk.h"
#include "NodePassThroughBulkWithBias.h"

namespace Neural 
{
	template<typename T> class INodeBuilder
	{
	public:
		typedef boost::shared_ptr< INode<T> >	INodePtr;		
	
		virtual INodePtr nodeFromptree( const boost::property_tree::ptree& nodeConfig ) const = 0;
		
		virtual boost::property_tree::ptree ptreeFromNode( INodePtr pNode ) const = 0;
		
		virtual const std::string& getNodeTypeName() const = 0;
		
		virtual ~INodeBuilder()
		{
		}
	protected:	
		INodeBuilder()
		{
		}
	};
	
	template<typename T, typename TNodeType> class NodeBuilder: public virtual INodeBuilder< T > 
	{
	public:
		typedef boost::shared_ptr< INode<T> >	INodePtr;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;

		NodeBuilder( const std::string& nodeTypeName ): m_NodeTypeName( nodeTypeName )
		{
		}
		virtual ~NodeBuilder()
		{
		}
		
		virtual INodePtr nodeFromptree( const boost::property_tree::ptree& nodeConfig ) const
		{
			return INodePtr( new TNodeType( nodeConfig ) );
		}
		
		virtual const std::string& getNodeTypeName() const
		{
			return m_NodeTypeName;
		}
		
		virtual boost::property_tree::ptree ptreeFromNode( INodePtr pNode ) const
		{
			boost::property_tree::ptree treeNode;
			
			treeNode.put("Type", m_NodeTypeName );
			
			pNode->serialize( treeNode );

			return treeNode;
		}


	protected:
		const std::string m_NodeTypeName;
		
	};


	template<typename T> class NodeSerializer
	{
	public:
		typedef boost::shared_ptr< INode<T> >									INodePtr;
		typedef std::map< std::string, boost::shared_ptr< INodeBuilder<T> > >	TStringToBuilderMap;
		typedef boost::shared_ptr< std::vector< typename INode<T>::INodePtr> >	VectorNodePtr;		
		
		NodeSerializer()
		{
			registerNodeType< NodeNetworkPillar<T> >( "NodeNetworkPillar" );
			registerNodeType< NodeNetworkLayer<T> >( "NodeNetworkLayer" );
			registerNodeType< NodePassThrough<T> >( "NodePassThrough" );
			registerNodeType< NodePerceptronBias<T> >( "NodePerceptronBias" );
			registerNodeType< NodePerceptronTraining<T> >( "NodePerceptronTraining" );
            registerNodeType< NodePassThroughBulk<T> >( "NodePassThroughBulk" );
            registerNodeType< NodePassThroughBulkWithBias<T> >( "NodePassThroughBulkWithBias" );
		};
		
		virtual ~NodeSerializer()
		{
		};
		
		void serialize( INodePtr pNode, const std::string& fileName) const
		{
			boost::property_tree::ptree nodeConfig = serialize( pNode );

// Following removed because it causes a compile error with the new version of XCode
//			boost::property_tree::xml_writer_settings<char> writerSettings( '\t', 1 );

//			write_xml( std::cout, nodeConfig, writerSettings );

            // Following removed because it causes a compile error with the new version of XCode (related to error few lines previous)
//            write_xml( fileName, nodeConfig, std::locale(), writerSettings );
            write_xml( fileName, nodeConfig, std::locale() );
		}
		
		boost::property_tree::ptree serialize( INodePtr pNode ) const
		{
			boost::property_tree::ptree retTree;
			
			serialize( pNode, retTree );
			
			return retTree;
		}

		void serialize( INodePtr pNode, boost::property_tree::ptree& nodeConfig ) const
		{
			std::string actualTypeName = typeid( *(pNode.get()) ).name();
			
			boost::property_tree::ptree treeNodeTypeName;
			
			typename TStringToBuilderMap::const_iterator itFind =  m_NodeBuilderFromActualTypeName.find( actualTypeName );
			
			if( itFind == m_NodeBuilderFromActualTypeName.end() )
			{
				throw std::runtime_error( "Actual Type name is not registered!");
			}
			else 
			{
				treeNodeTypeName = itFind->second->ptreeFromNode( pNode );
			}
			
			VectorNodePtr pChildren = pNode->getChildren();
			
			const size_t numChildren = pChildren->size();
			
			if( numChildren > 0 )
			{
				boost::property_tree::ptree treeChildren;
				
				treeChildren.put( "Count", numChildren );
				
				BOOST_FOREACH( typename INode<T>::INodePtr pNextNode, *pChildren )
				{
					serialize( pNextNode, treeChildren );
				}
				
				treeNodeTypeName.push_back(boost::property_tree::ptree::value_type("Children", treeChildren));

			}
	
			nodeConfig.push_back(boost::property_tree::ptree::value_type("Node", treeNodeTypeName));
		}
		
		
		INodePtr deserialze( const std::string& fileName) const
		{
			boost::property_tree::ptree treeNode;
			
			read_xml( fileName, treeNode, boost::property_tree::xml_parser::trim_whitespace );
			
			return deserialze( treeNode );
		}
		
		INodePtr deserialze( const boost::property_tree::ptree& nodeConfig ) const
		{ 
			const boost::property_tree::ptree& treeNode = nodeConfig.get_child("Node");
			
			return innerDeserialze( treeNode );
		}
		
	protected:
		INodePtr innerDeserialze( const boost::property_tree::ptree& nodeConfig ) const
		{
			INodePtr pRetNode;
		
			const std::string nodeTypeName( nodeConfig.get< std::string >("Type") );
			
			
			typename TStringToBuilderMap::const_iterator itFind =  m_NodeBuilderFromUniqueTypeName.find( nodeTypeName );
			if( itFind == m_NodeBuilderFromUniqueTypeName.end() )
			{
				throw std::runtime_error( "Unique Type name is not registered!");
			}
			else 
			{
				pRetNode = itFind->second->nodeFromptree( nodeConfig );
			}
			
			boost::optional< const boost::property_tree::ptree& > optChildren = nodeConfig.get_child_optional("Children");			
			
			if( optChildren )
			{
				for( boost::property_tree::ptree::const_iterator it = optChildren->begin();
					it != optChildren->end();
					++it )
				{
					const std::string sKey = it->first;
					
					if(sKey == "Node")
					{
						boost::optional< size_t > instances = (it->second).get_optional< size_t >("Instances");
						size_t numInstances = ( instances ) ? ( *instances ) : 1;
						for( size_t i = 0; i< numInstances; ++i )
						{
							pRetNode->addChild( innerDeserialze(it->second) );
						}
					}
					else if( sKey == "Count")
					{
						// Ignore for now
					}
					else
					{
						throw std::runtime_error( "Expected \"Node\" element");
					}
					
				}
			}
			return pRetNode;
			
		}

		TStringToBuilderMap	m_NodeBuilderFromUniqueTypeName;
		TStringToBuilderMap	m_NodeBuilderFromActualTypeName;
		
		template< class TNodeType > void registerNodeType( const std::string& uniqueTypeName )
		{
			if( m_NodeBuilderFromUniqueTypeName.find( uniqueTypeName ) == m_NodeBuilderFromUniqueTypeName.end() )
			{
				m_NodeBuilderFromUniqueTypeName[ uniqueTypeName ] = boost::shared_ptr< INodeBuilder<T> >( new NodeBuilder< T, TNodeType>( uniqueTypeName ) );
			}
			else 
			{
				throw std::runtime_error( "Type name is not unique!");
			}
			
			std::string actualTypeName( typeid( TNodeType ).name() );
			
			if( m_NodeBuilderFromActualTypeName.find( actualTypeName ) == m_NodeBuilderFromActualTypeName.end() )
			{
				m_NodeBuilderFromActualTypeName[ actualTypeName ] = boost::shared_ptr< INodeBuilder<T> >( new NodeBuilder< T, TNodeType>( uniqueTypeName ) );
			}
			else 
			{
				throw std::runtime_error( "Actual Type name is not unique!");
			}
			
		};
	};
};

#endif
