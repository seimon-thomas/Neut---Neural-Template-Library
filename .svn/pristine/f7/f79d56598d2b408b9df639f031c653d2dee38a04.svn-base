///////////////////////////////////////////////////////////////////////////////////////////////////
//
// InputsCached.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef InputsCached_H
#define InputsCached_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>
#include "Inputs.h"
#include "Exception.h"

namespace Neural 
{
	template<class T, class TInputProvider > class InputsCached: public virtual Inputs<T>
	{
	public:
		InputsCached( boost::shared_ptr< TInputProvider > pInputProvider )
		:Inputs<T>(),
		m_pInputProvider( pInputProvider ) {};
		
		virtual ~InputsCached() {};
        
        typedef typename IInputs<T>::ContainerType InputCountainerType;
		
        virtual boost::shared_ptr< std::vector<T> > getNextInputSet()
		{ 
			if( m_pInputProvider->getEpochNumber() == 0 )
			{
				boost::shared_ptr< std::vector<T> > pNextSet = m_pInputProvider->getNextInputSet();
				//
				// Check again, as input provides sometimes don't know an epoch end till they've past it
				//
				if( m_pInputProvider->getEpochNumber() == 0 )
				{
					Inputs<T>::m_InputSet.push_back( pNextSet );
				}
			}
			return Inputs<T>::getNextInputSet();
		};

        virtual boost::shared_ptr< std::vector<T> > getNextInputSet( size_t requiredSize )
		{ 
            
  			if( m_pInputProvider->getEpochNumber() == 0 )
			{
                //
                // Set to cache
                //
                boost::shared_ptr< std::vector<T> > pToCache( new InputCountainerType( requiredSize ) );
                //
                // Always start by getting the next input set & putting it in the back of the cache
                //
                boost::shared_ptr< std::vector<T> > pNextSet = m_pInputProvider->getNextInputSet();
                m_OverlapCache.push_back( pNextSet );
                //
                // Determine the chunk size (important for later)
                //
                size_t chunkSize = pNextSet->size();
                //
                // Get an iterator to the overlap cache
                //
                typename std::list< boost::shared_ptr< std::vector<T> > >::iterator itNextOverlapCacheEntry = m_OverlapCache.begin();
                //
                // Loop filling the pToCache
                //
                for( size_t i=0; i < requiredSize;)
                {
                    //
                    // What's our chunk index
                    //
                    size_t chunkIdx = i/chunkSize;
                    //
                    // Is that cached
                    //
                    if( m_OverlapCache.size() > chunkIdx )
                    {
                        //
                        // Yes, so let's get it
                        //
                        pNextSet = *itNextOverlapCacheEntry;
                        //
                        // Do we have a next, if so move to it
                        //
                        typename std::list< boost::shared_ptr< std::vector<T> > >::iterator itNextNextOverlapCacheEntry = itNextOverlapCacheEntry;
                        ++itNextNextOverlapCacheEntry;
                        if( m_OverlapCache.end() != itNextNextOverlapCacheEntry )
                        {
                            itNextOverlapCacheEntry = itNextNextOverlapCacheEntry;
                        }
                    }
                    else
                    {
                        //
                        // Not in the cache so get it & put it there & advance the iterator onto it
                        //
                        pNextSet = m_pInputProvider->getNextInputSet();
                        m_OverlapCache.push_back( pNextSet );
                        ++itNextOverlapCacheEntry;
                    }
                    //
                    // What's the size of this next set
                    //
                    size_t nextSetSize = pNextSet->size();
                    //
                    // Check that it matched
                    //
                    if( requiredSize % nextSetSize )
                    {
                        //
                        // This isnt allowed
                        //
                        THROW_EXCEPTION( "A. Required size (" << requiredSize << ") is not an exact multiple of next set size(" << nextSetSize << ")" )
                    }
                    //
                    // Fill the next chunk into the pToCacheSet
                    //
                    for( size_t j=0; j < nextSetSize; ++j, ++i )
                    {
                        pToCache->at( i ) = pNextSet->at( j );
                    }

                } 
                //
                // Erase the front, as we append one at a time to the end
                //
                m_OverlapCache.erase( m_OverlapCache.begin() );
                //
                // Check again, as input provides sometimes don't know an epoch end till they've past it
                //
                if( m_pInputProvider->getEpochNumber() == 0 )
                {
                    Inputs<T>::m_InputSet.push_back( pToCache );
                }
            }
            return Inputs<T>::getNextInputSet();
		};
        
	protected:
		boost::shared_ptr< TInputProvider >                 m_pInputProvider;
        std::list< boost::shared_ptr< std::vector<T> > >    m_OverlapCache;
	};
	
}

#endif