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
        bool    m_finished;
        
        ActivationThreadFunctor( boost::shared_ptr< boost::barrier > pStartBarrier,
                                 boost::shared_ptr< boost::barrier > pStopBarrier )
        :m_pStartBarrier( pStartBarrier ),
         m_pStopBarrier( pStopBarrier ),
        m_finished( false )
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
                    std::for_each( m_activationList.begin(), 
                                   m_activationList.end(), 
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
        void pushBackActivation( IActivatePtr pActivate )
        {
            //
            // Add it to my activates
            //
            m_activationList.push_back( pActivate );
        }
        void pushFrontActivation( IActivatePtr pActivate )
        {
            //
            // Add it to my activates
            //
            m_activationList.push_front( pActivate );
        }
    protected:
        ActivationThreadFunctor( const ActivationThreadFunctor& that )
        {
        };
        ActivationThreadFunctor operator=(ActivationThreadFunctor that)
        {
            return *this;
        };        

        std::list< IActivatePtr >   m_activationList;
        boost::shared_ptr< boost::barrier > m_pStartBarrier;
        boost::shared_ptr< boost::barrier > m_pStopBarrier;
    };

    template< typename T > class MultiThreadedActivator
    {
    public:
        MultiThreadedActivator( size_t numThreads )
            :m_numThreads( numThreads ),
             m_nextRoundRobin( 0 )
        {
            if( numThreads > 1 )
            {
                m_pStartBarrier = boost::shared_ptr< boost::barrier >( new boost::barrier( numThreads ) );
                m_pStopBarrier = boost::shared_ptr< boost::barrier >( new boost::barrier( numThreads ) );
                
                //
                // Subtract 1, as the main thread counts
                //
                m_activationFunctors.reserve( numThreads-1 );
                for(size_t i=0; i<numThreads-1; ++i)
                {
                    ActivationThreadFunctor* pActivationThreadFunctor( new ActivationThreadFunctor( m_pStartBarrier, m_pStopBarrier ) );
                    m_activationFunctors.push_back( boost::shared_ptr< ActivationThreadFunctor >( pActivationThreadFunctor ) );
                    m_threadGroup.create_thread( boost::ref( *pActivationThreadFunctor ) );
                }
            }
        };
        
        ~MultiThreadedActivator()
        {
            //
            // Destroy the threads
            //
            m_threadGroup.interrupt_all();
            m_threadGroup.join_all();
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
            if( nextFunctorIdx )
            {
                m_activationFunctors.at( nextFunctorIdx-1 )->pushBackActivation( pActivate );
            }
            else
            {
                m_activationList.push_back( pActivate );
            }
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
            if( nextFunctorIdx )
            {
                m_activationFunctors.at( nextFunctorIdx-1 )->pushFrontActivation( pActivate );
            }
            else
            {
                m_activationList.push_front( pActivate );
            }
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
            // Activate the activation list
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
            std::for_each( m_activationList.begin(), 
                          m_activationList.end(), 
                          boost::bind(&IActivate::activate,_1));
        };
    protected:
        size_t m_numThreads;
        size_t m_nextRoundRobin;
        
        boost::thread_group m_threadGroup;
        
        std::vector< boost::shared_ptr< ActivationThreadFunctor > >    m_activationFunctors;
        boost::shared_ptr< boost::barrier > m_pStartBarrier;
        boost::shared_ptr< boost::barrier > m_pStopBarrier;
       
        std::list< IActivatePtr >   m_activationList;
    };
}
#endif
