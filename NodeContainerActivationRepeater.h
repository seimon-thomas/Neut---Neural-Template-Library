///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeContainerActivationRepeater.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NodeContainerActivationRepeater_H
#define NodeContainerActivationRepeater_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "NodeNetworkPillar.h"

#include "NodeInput.h"



namespace Neural 
{
	template<typename T, typename TBase> class NodeContainerActivationRepeater: public virtual TBase
	{
	public:
		typedef boost::shared_ptr< INode<T> >					INodePtr;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;
		
		NodeContainerActivationRepeater( unsigned long long totalNumActivationIterations )
			:m_TotalNumActivationIterations( totalNumActivationIterations ),
			 m_RemainingNumActivationIterationsFront( totalNumActivationIterations ),
			 m_RemainingNumActivationIterationsBack( totalNumActivationIterations )
		{
		};
		
		NodeContainerActivationRepeater(const boost::property_tree::ptree& nodeConfig)
			:m_TotalNumActivationIterations( boost::lexical_cast<unsigned long long>( nodeConfig.get<std::string>("RepeatActivationCount") ) ),
			 m_RemainingNumActivationIterationsFront( m_TotalNumActivationIterations ),
			 m_RemainingNumActivationIterationsBack( m_TotalNumActivationIterations )
		{
		};
		
		virtual ~NodeContainerActivationRepeater()
		{
		};

		virtual void activateFromFront()
		{
			while (m_RemainingNumActivationIterationsFront--)
			{
				TBase::activateFromFront();
			} 
		}
		
		virtual void activateFromBack()
		{
			while (m_RemainingNumActivationIterationsBack--)
			{
				TBase::activateFromBack();
			}
		}
		
		
		
		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
			nodeConfig.put("RepeatActivationCount", boost::lexical_cast<std::string>(m_TotalNumActivationIterations) );
		};
		
	protected:
		const unsigned long long m_TotalNumActivationIterations;
		unsigned long long m_RemainingNumActivationIterationsFront;
		unsigned long long m_RemainingNumActivationIterationsBack;
	};

};


#endif