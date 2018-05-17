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
