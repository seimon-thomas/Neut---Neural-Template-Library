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
 *  TestMessaging.h
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 23/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */

#ifndef TestMessaging_H
#define TestMessaging_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "IMessageReceiver.h"

using namespace::Neural;

class TestMessaging : public CPPUNIT_NS::TestFixture, public virtual IMessageReceiver
{
    CPPUNIT_TEST_SUITE (TestMessaging);
    CPPUNIT_TEST (sendTest);
    CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	
protected:
	void sendTest (void);

	virtual void onMessageReceive( boost::shared_ptr< boost::property_tree::ptree > pMessage, size_t messageType );

private:
	
};

#endif
