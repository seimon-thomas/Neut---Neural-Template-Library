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
//  MultiThreadedActivator.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 09/11/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_MultiThreadedActivator_h
#define NeuralNetworkApplication_MultiThreadedActivator_h

#include <list>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

#include "IActivate.h"

namespace Neural 
{    
    class ActivationThreadFunctor
    {
    public:
        ActivationThreadFunctor( boost::shared_ptr< boost::barrier > pStartBarrier,
                                 boost::shared_ptr< boost::barrier > pStopBarrier,
                                 boost::shared_ptr< std::list< IActivatePtr > >  pActivationList
                                 )
        :m_pStartBarrier( pStartBarrier ),
         m_pStopBarrier( pStopBarrier ),
         m_pActivationList( pActivationList )
        {
            
        };
        ~ActivationThreadFunctor()
        {
            
        };
        void operator()()
        {
            try
            {
                while(true)
                {
                    //
                    // start barrier here
                    //
                    m_pStartBarrier->wait();
                    //
                    // Activate the activation list
                    //
                    std::for_each( m_pActivationList->begin(),
                                   m_pActivationList->end(),
                                   boost::bind(&IActivate::activate,_1));
                    //
                    // Stop barrier here
                    //
                    m_pStopBarrier->wait();
                }
            }
            catch(boost::thread_interrupted& interupedEx )
            {
                // Benign
            }
        };
        ActivationThreadFunctor( const ActivationThreadFunctor& that )
            :m_pStartBarrier( that.m_pStartBarrier ),
            m_pStopBarrier( that.m_pStopBarrier ),
            m_pActivationList( that.m_pActivationList )
        {
        };
        ActivationThreadFunctor operator=(ActivationThreadFunctor that)
        {
            m_pStartBarrier = that.m_pStartBarrier;
            m_pStopBarrier = that.m_pStopBarrier;
            m_pActivationList = that.m_pActivationList;
            return *this;
        };        

    protected:
        boost::shared_ptr< std::list< IActivatePtr > >  m_pActivationList;
        boost::shared_ptr< boost::barrier > m_pStartBarrier;
        boost::shared_ptr< boost::barrier > m_pStopBarrier;
    };

    template< typename T > class MultiThreadedActivator
    {
    public:
        MultiThreadedActivator( size_t numThreads )
            :m_numThreads( numThreads ),
             m_nextRoundRobin( 0 ),
             m_pActivationLists(new std::vector< boost::shared_ptr< std::list< IActivatePtr > > >()),
             m_pThreads(new std::list< boost::shared_ptr< boost::thread > >() )
        {
            // Create the activation list for the primary/main thread (this one)
            m_pActivationLists->push_back( boost::shared_ptr< std::list< IActivatePtr > >( new std::list< IActivatePtr >() ) );
            // Only construct the rest of this scaffholding if we're running multthreaded
            if( numThreads > 1 )
            {
                // These barriers control the synchronised starting & stopping of the thread functors
                m_pStartBarrier = boost::shared_ptr< boost::barrier >( new boost::barrier( numThreads ) );
                m_pStopBarrier = boost::shared_ptr< boost::barrier >( new boost::barrier( numThreads ) );
                
                //
                // Subtract 1, as (this) main thread counts towards the total
                //
                for(size_t i=0; i<numThreads-1; ++i)
                {
                    // Create a new activation list for the next thread
                    boost::shared_ptr< std::list< IActivatePtr > >  pNextActivationList( new std::list< IActivatePtr >() );
                    // Save it in its own slot of the activation lists
                    m_pActivationLists->push_back( pNextActivationList );
                    // Create a functor with this list
                    ActivationThreadFunctor nextFunctor( m_pStartBarrier, m_pStopBarrier, pNextActivationList );
                    // Finally create a thread with the functor
                    m_pThreads->push_back( boost::shared_ptr< boost::thread >( new boost::thread( nextFunctor ) ) );
                }
            }
        };
        
        ~MultiThreadedActivator()
        {
            // Earlier versions of this class interrupted & joined all of the threads here
            // but that caused a BAD_ACCESS in the latest version of boost, so they have been removed.
            // Boost seems to clean up all of the threads itself on destruction of the thread objects.
        };
        
        void pushBackActivation( IActivatePtr pActivate )
        {
            //
            // Round robin to the next functor
            //
            size_t nextFunctorIdx = m_nextRoundRobin % m_numThreads;
            //
            // Add the activate to it (if not us)
            //
            m_pActivationLists->at( nextFunctorIdx )->push_back( pActivate );
            //
            // Move round robin on 
            //
            ++m_nextRoundRobin;
        };

        void pushFrontActivation( IActivatePtr pActivate )
        {
            //
            // Round robin to the next functor
            //
            size_t nextFunctorIdx = m_nextRoundRobin % m_numThreads;
            //
            // Add the activate to it (if not us)
            //
            m_pActivationLists->at( nextFunctorIdx )->push_front( pActivate );
            //
            // Move round robin on 
            //
            ++m_nextRoundRobin;
        };
        
        void activate()
        {
            //
            // start barrier here (if multi-threaded)
            //
            if( m_numThreads > 1 ) m_pStartBarrier->wait();
            //
            // Activate the activation list for this primary thread
            //
            unlockedActivate();
            //
            // Stop barrier here (if multithreaded)
            //
            if( m_numThreads > 1 ) m_pStopBarrier->wait();
        };
        
        void unlockedActivate()
        {
            //
            // Activate the activation list
            //
            std::for_each( m_pActivationLists->at(0)->begin(),
                           m_pActivationLists->at(0)->end(),
                           boost::bind(&IActivate::activate,_1));
        };
    protected:
        size_t m_numThreads;
        size_t m_nextRoundRobin;
        
        boost::shared_ptr< std::list< boost::shared_ptr< boost::thread > > > m_pThreads;

        boost::shared_ptr< boost::barrier > m_pStartBarrier;
        boost::shared_ptr< boost::barrier > m_pStopBarrier;
       
        boost::shared_ptr< std::vector< boost::shared_ptr< std::list< IActivatePtr > > > >  m_pActivationLists;
    };
}
#endif
