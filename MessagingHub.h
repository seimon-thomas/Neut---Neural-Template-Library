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
