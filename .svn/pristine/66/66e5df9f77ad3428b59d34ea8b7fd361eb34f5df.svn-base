///////////////////////////////////////////////////////////////////////////////////////////////////
//
// INode.h
//
// Base interface for all nodes
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INODE_H
#define INODE_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <list>
#include <boost/property_tree/ptree.hpp>

#include "ConnectionSite.h"
#include "IConnection.h"
#include "IActivateProvider.h"

namespace Neural 
{
	
	template<class T> class INode : public virtual boost::enable_shared_from_this< INode<T> >, 
									public virtual IActivateProvider
	{
	public:
		typedef boost::shared_ptr< INode<T> > INodePtr;
		typedef boost::shared_ptr< IConnection<T> > IConnectionPtr;		
		typedef std::list< INodePtr > INodePtrList;
		typedef boost::shared_ptr< std::vector< INodePtr> >	VectorINodePtr;

		virtual ~INode(){};
				
		virtual void connect( INodePtr pNodeToConnect, ConnectionSite siteOnThis = OUTPUTS )=0;

		// TODO: Replace with a vector based connection
//		virtual IConnectionPtr getConnection( ConnectionSite siteOnThis = INPUTS )=0;
		
		virtual void addChild( INodePtr pChildNode ) = 0;
		
		virtual void getLeafChildren( INodePtrList& leafNodes, ConnectionSite siteOnThis) = 0;
		
		virtual VectorINodePtr getChildren() = 0;
		
		virtual void serialize( boost::property_tree::ptree& nodeConfig )
		{
		};
	protected:
		INode(){};

	private:
		// Vestigial serialize for boost serialisation
		friend class boost::serialization::access;
		template<class Archive>	void serialize(Archive & ar, const unsigned int version){};
		
	};
	
	typedef INode<double> INodeDouble;
	typedef boost::shared_ptr< INode<double> > INodeDoublePtr;

}

#endif

