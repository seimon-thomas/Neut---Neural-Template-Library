/*
 *  MessagingHub.h
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 23/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */
#ifndef MessagingHub_H
#define MessagingHub_H

#include <vector>
#include <map>
#include <list>
#include <boost/property_tree/ptree.hpp>
#include "IMessageReceiver.h"

namespace Neural 
{
	class MessagingHub
	{
	public:
		virtual void registerMessageReceiver( boost::shared_ptr< IMessageReceiver > pReceiver, size_t messageTypeOfInterest )
		{
			m_receiversAgainstMsgTypes[ MESSAGE_TYPE_ANY ]
		};
		virtual void sendMessage( boost::shared_ptr< boost::property_tree::ptree > pMessage, size_t messageType )
		{
			
		};
		
		~MessagingHub(){};
		
	protected:
		MessagingHub(){};
		
		std::map< size_t, std::list<boost::shared_ptr< IMessageReceiver > > m_receiversAgainstMsgTypes;
	}
}
#endif
