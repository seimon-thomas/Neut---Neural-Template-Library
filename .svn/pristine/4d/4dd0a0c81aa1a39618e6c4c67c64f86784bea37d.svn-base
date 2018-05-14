/*
 *  IMessageReceiver.h
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 22/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */
#ifndef IMessageReceiver_H
#define IMessageReceiver_H

#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>

namespace Neural 
{
	class IMessageReceiver
	{
		public:
			virtual void onMessageReceive( boost::shared_ptr< boost::property_tree::ptree > pMessage, size_t messageType ) = 0;
			~IMessageReceiver(){};
			
		protected:
			IMessageReceiver(){};
	};
};

#endif