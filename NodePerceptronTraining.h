///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodePerceptronTraining.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NODEPERCEPTRONTRAINING_H
#define NODEPERCEPTRONTRAINING_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

#include "NodePerceptronRecall.h"
#include "ReceiveFromBack.h"
#include "utils.h"



namespace Neural 
{

	template<class T> class NodePerceptronTraining: public virtual NodePerceptronRecall<T>, public virtual ReceiveFromBack<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;	
		typedef NodePerceptronRecall<T> Parent;
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType ValueContainerType;
        typedef FastConnection< T > TConnection;

		NodePerceptronTraining( T gain, T momentum)
		: NodePerceptronRecall<T>(),
		  m_Gain( gain ),
		  m_Momentum( momentum ),
		  m_pPreviousWeights( new std::vector< T >() )
		{
		};
		
		NodePerceptronTraining(const boost::property_tree::ptree& nodeConfig)
			: NodePerceptronRecall<T>( nodeConfig ),
			m_pPreviousWeights( new std::vector< T >() )
		{
			
			m_Gain = boost::lexical_cast<double>( nodeConfig.get<std::string>("Gain") );
			m_Momentum = boost::lexical_cast<double>( nodeConfig.get<std::string>("Momentum") );
			
			boost::optional< const boost::property_tree::ptree& > optWeights = nodeConfig.get_child_optional("PreviousWeights");			
			
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
						utils::push_back( *m_pPreviousWeights, boost::lexical_cast<double>( (it->second).get_value<std::string>() ) );
						utils::push_back( m_FrontOutputDeltas, T() );
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

		virtual ~NodePerceptronTraining()
		{
		};
		
		virtual void activateFromBack()
		{
			//
			// First, calculate delta
			//
			T delta = calculateDelta();
			//
			// Before we update our weights, we need to calculate deltas to backprop (these
			// will be prepared here then sent back after weights have been updated
			//
			prepareBackpropDeltas( delta );
			//
			// Call our common method to update weights
			//
			updateWeights( delta );			
			//
			// Send the appropriate delta to the layer above
			//
			size_t numWeights = m_FrontOutputDeltas.size();
            size_t numCons = Node<T>::m_ConnectedNodes[INPUTS].size();
            
            register size_t numSends = numCons < numWeights ? numCons : numWeights;
            
            if( numSends > 0 )
            {
                T* pNextOutput = &m_FrontOutputDeltas[0];
                TConnection** pNextConnection = &(Node<T>::m_ConnectedNodes[INPUTS][0]);
                
                for(register size_t i = 0; i < numSends; ++i )
                {
                    //
                    // We've recycled m_FrontOutputDeltas to hold our deltas, so send each delta to each equvalent node
                    //
                    ( *pNextConnection++ )->send( *pNextOutput++ );
                }
            }
		}

		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			// First call base serialze
			Parent::serialize( nodeConfig );
		
			nodeConfig.put("Gain", boost::lexical_cast<std::string>(m_Gain) );
			nodeConfig.put("Momentum", boost::lexical_cast<std::string>(m_Momentum) );
			
			boost::property_tree::ptree treePrevWeights;
			
			size_t numWeights = Parent::m_pWeights->size();
			
			treePrevWeights.put( "Count", numWeights );
			
			for (size_t i=0; i < numWeights; ++i) 
			{
				
				treePrevWeights.add( "Weight", boost::lexical_cast<std::string>((*m_pPreviousWeights)[i]) );
			}
			
			nodeConfig.push_back(boost::property_tree::ptree::value_type("PreviousWeights", treePrevWeights));
			
			
		};
        
        using ReceiveFromBack<T>::onGrowAndGetReceiverValuesBack;
		
protected:
        virtual void onFrontInputValueAddedAt( size_t addedIndex )
        {
            //
            // Base class first
            //
            NodePerceptronRecall<T>::onFrontInputValueAddedAt( addedIndex );
			//
			// Next, this class needs to add some previous weights
			// and some space for deltas tobackpropagate
			//
			utils::conditional_push_back( *m_pPreviousWeights, T(), addedIndex );
			utils::conditional_push_back( m_FrontOutputDeltas, T(), addedIndex );
        }
        
		inline T calculateDelta()
		{
			//
			// First, calculate the partial delta with a summation of the
			// deltas back propped to here (for an output neuron ReceiveFromBack<T>::m_BackInputValues
			// will only have a size of 1
			//
			register T delta = T();
			register size_t numDeltas = ReceiveFromBack<T>::m_pBackInputValues->size();
            register T* pBackInputVals = &((*ReceiveFromBack<T>::m_pBackInputValues)[0]);
            
			for(register size_t i=0; i<numDeltas; ++i)
			{
				delta += *pBackInputVals++;
			}
			//
			// Next calculate the full delta
			//
			delta = Parent::m_Output * ((T)1 - Parent::m_Output) * delta;	
			//
			// Finally return it
			//
			return delta;
		}
			
		inline void prepareBackpropDeltas( T delta )
		{
            //
            // Only bother, if there are outward connections to backprop to
            //
            if( Node<T>::m_ConnectedNodes[INPUTS].size() > 0 )
            {
                //
                // Cycle through each of the connections to neurons at input,
                // mutiplying that weight by delta in prep for sending it back
                //
                register size_t numWeights = m_FrontOutputDeltas.size();
                register T* pFrontOutputDeltas = &m_FrontOutputDeltas[0];
                register T* pWeights = &( (*Parent::m_pWeights)[0] );
                for(register size_t i=0; i<numWeights; ++i) 
                {
                    *pFrontOutputDeltas++ = (*pWeights++) * delta;
                }
            }
		}
		

		inline void updateWeights(T delta)
		{
			register size_t numWeights = Parent::m_pWeights->size();
            register T* pPreviousWeights = &( (*m_pPreviousWeights)[ 0 ] );
            register T* pWeights = &( (*Parent::m_pWeights)[0] );
            register T* pFrontInputValues = &((*ReceiveFromFront<T>::m_pFrontInputValues)[0]);
			for(register size_t i=0; i < numWeights; ++i) 
			{
				//
				// Recycle the previous weights slot (we'll swap back later)
				//
				*pPreviousWeights = 
                    //
                    // Currrent weights
                    //
                    (*pWeights) 
                    //
                    // Next add the gain expression which is just gain times delta times the input value
                    // TODO: Should the following expression *really* use m_FrontInputValues
                    //
                    + ( m_Gain * delta * (*pFrontInputValues) )
                    //
                    // Next add the momentum expression which is just current weight minus previous weight 
                    // times momentum
                    //
                    + ( m_Momentum * ( (*pWeights) - (*pPreviousWeights) ) );
                ++pWeights;
                ++pPreviousWeights;
                ++pFrontInputValues;
			}
			//
			// We've put the brand new wights in m_pPreviousWeights (recycled), so we now need
			// to make these the 'current' weights & move the old weights to m_pPreviousWeights
			//
			boost::shared_ptr< std::vector< T > > pCurrentWeights = m_pPreviousWeights;
			m_pPreviousWeights = Parent::m_pWeights;
			Parent::m_pWeights = pCurrentWeights;
			
		};
			
		T m_Gain;
		T m_Momentum;
		
		boost::shared_ptr< std::vector< T > > m_pPreviousWeights;	
		
		std::vector< T >						m_FrontOutputDeltas;


	};
}

#endif