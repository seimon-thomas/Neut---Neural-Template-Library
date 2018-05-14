///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodePerceptronRecall.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NODEPERCEPTRONRECALL_H
#define NODEPERCEPTRONRECALL_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <boost/lexical_cast.hpp>
#include "Node.h"
#include "ReceiveFromFront.h"
#include "utils.h"



namespace Neural 
{
	template<class T> class NodePerceptronRecall: public virtual ReceiveFromFront<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;	
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType ValueContainerType;
        typedef FastConnection< T > TConnection;

		NodePerceptronRecall()
			: m_pWeights( new std::vector< T >() ),
			  m_Output( T() )
		{
		};

		NodePerceptronRecall(const boost::property_tree::ptree& nodeConfig)
			: m_pWeights( new std::vector< T >() ),
			m_Output( T() )
		{
			
			boost::optional< const boost::property_tree::ptree& > optWeights = nodeConfig.get_child_optional("Weights");			
			
			if( optWeights )
			{
				//
				for( boost::property_tree::ptree::const_iterator it = optWeights->begin();
					it != optWeights->end();
					++it )
				{
					const std::string sKey = it->first;
					
					if(sKey == "Weight")
					{
						utils::push_back( *m_pWeights, boost::lexical_cast<double>( (it->second).get_value<std::string>() ) );
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
			else
			{
				// No weights defined, so do nothing, as they'll be randomly initialised on connection
			}

			
		};
		
		virtual ~NodePerceptronRecall()
		{
		};
		
		virtual void activateFromFront()
		{
			// 
			// This is Recall, so...
			// Perform the MAC
			//
			register T output = T(0); 
            register size_t numWeights = m_pWeights->size();
            register T* pFrontInputValues = &((*ReceiveFromFront<T>::m_pFrontInputValues)[0]);
            register T* pWeights = &((*m_pWeights)[0]);
			for (register size_t i = 0; i < numWeights; ++i) 
			{
				output += (*pFrontInputValues++) * (*pWeights++);
			}
			//
			// Next, apply the squashing function, a logistic sigmoid
			//
			m_Output = 1.0/(1.0+exp(output*(-1.0)));
			//
			// Finally, pass this on by sending to the nodes at the back
			//
			Node<T>::sendToAll( m_Output, OUTPUTS );
		}

		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			boost::property_tree::ptree treeWeights;
			
			size_t numWeights = m_pWeights->size();
			
			treeWeights.put( "Count", numWeights );
			
			for (size_t i=0; i < numWeights; ++i) 
			{
				treeWeights.add( "Weight", boost::lexical_cast<std::string>((*m_pWeights)[i]) );
				
			}
			
			nodeConfig.push_back(boost::property_tree::ptree::value_type("Weights", treeWeights));
			
			
		};
		
	
	protected:
        
        virtual void onFrontInputValueAddedAt( size_t addedIndex )
        { 
			//
			// Do we already have a weight at this index?
			//
			if( m_pWeights->size() <= addedIndex )
			{
				// 
				// Nope, so add one
				//
				// TODO: Add a Boost Gausian randon number call in here
				T randomWeight = (T)(rand()); 
                randomWeight+= (T)(rand()); 
                randomWeight+= (T)(rand()); 
                
                randomWeight*= (T)2;
                randomWeight/= (T)(RAND_MAX) + (T)(RAND_MAX) + (T)(RAND_MAX);
                randomWeight-= (T)1;
				
				utils::push_back( *m_pWeights, randomWeight );
            }
        };
        		
		boost::shared_ptr< std::vector< T > > m_pWeights;	

		T					m_Output;
	};
}

#endif