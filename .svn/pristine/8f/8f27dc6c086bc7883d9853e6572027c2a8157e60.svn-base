/*
 *  MessagingHub.h
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 22/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */
#ifndef IMessagingHub_H
#define IMessagingHub_H

#include <vector>
#include <map>
#include <list>
#include <boost/property_tree/ptree.hpp>
#include "IMessageReceiver.h"

namespace Neural 
{
	class IMessagingHub
	{
		public:
			static const size_t MESSAGE_TYPE_ANY = 0;
			
			virtual void registerMessageReceiver( boost::shared_ptr< IMessageReceiver > pReceiver, size_t messageTypeOfInterest ) = 0;
			virtual void sendMessage( boost::shared_ptr< boost::property_tree::ptree > pMessage, size_t messageType ) = 0;

			~IMessagingHub(){};

		protected:
			IMessagingHub(){};
	}
}
#endif