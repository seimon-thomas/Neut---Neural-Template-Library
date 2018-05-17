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
// NodePerceptronTarget.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodePerceptronTarget_H
#define NodePerceptronTarget_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <math.h>

#include <boost/foreach.hpp>
#include "Node.h"
#include "IInputs.h"
#include "Exception.h"



namespace Neural 
{
	template<class T> class NodePerceptronTarget: public virtual ReceiveFromFront<T>
	{
	public:
		typedef boost::shared_ptr< INode<T> >                       INodePtr;
		typedef boost::shared_ptr< IConnection<T> >                 IConnectionPtr;	
		typedef typename IInputs<T>::ContainerType                  InputsContainerType;
        typedef typename IReceiveFromFrontAndBack<T>::ContainerType FrontValuesContainerType;
        
		NodePerceptronTarget(boost::shared_ptr< IInputs<T> > pTargets)
		:m_pTargets( pTargets )
		{
		};
		
		virtual ~NodePerceptronTarget()
		{
		};
		
		virtual void activateFromFront()
		{
			//
			// Get the target for this run
			//
			boost::shared_ptr< InputsContainerType  > pNextTargetSet = m_pTargets->getNextInputSet();
            //
            // Check that the size is appropriate
            //
            size_t targetSetSize = pNextTargetSet->size();
            size_t inputValueSize = ReceiveFromFront<T>::m_pFrontInputValues->size();
            if(  inputValueSize != targetSetSize )
            {
                THROW_EXCEPTION( "Target set size (" << targetSetSize << ") not equal to input values size (" << inputValueSize << ")" )
            }
			// 
			// Get the error for each target slot & calculate RMS error
			//
            typename InputsContainerType::iterator itInputsEnd = pNextTargetSet->end();
			
            typename InputsContainerType::iterator itNextInput = pNextTargetSet->begin();
            typename FrontValuesContainerType::iterator itNextFrontValue = ReceiveFromFront<T>::m_pFrontInputValues->begin();
            
			m_Output = T(0);
            register T nextValue;
            for(;
                itNextInput != itInputsEnd;
                ++itNextInput, ++itNextFrontValue)
			{
                nextValue = (*itNextInput) - (*itNextFrontValue);
                (*itNextFrontValue) = nextValue;
                m_Output += nextValue * nextValue;
			}

			m_Output = sqrt( m_Output/((T)inputValueSize) );
			//
			// Finally, pass this RMS error on on by sending to the nodes at the back
			//
			Node<T>::sendToAll( m_Output, OUTPUTS );
		}
        
		//
		// TODO: Refactor this for commonality with the Training variant of the neuron
		//
		virtual void activateFromBack()
		{
            size_t inputValsSize = ReceiveFromFront<T>::m_pFrontInputValues->size();
            size_t conNodesSize = Node<T>::m_ConnectedNodes[INPUTS].size();
			//
			// Send the appropriate delta to the layer above
			//
			for( size_t i = 0; (i < inputValsSize) && ( i < conNodesSize ); ++i )
			{
				//
				// We've recycled m_FrontInputValues to hold our deltas, so send each delta to each equvalent node
				//
				Node<T>::m_ConnectedNodes[INPUTS][i]->send( (*ReceiveFromFront<T>::m_pFrontInputValues)[ i ] );				
			}
		}
				
	protected:
		// Output for Target node is rms error
		T								m_Output;
		boost::shared_ptr< IInputs<T> > m_pTargets;
	};
}

#endif
