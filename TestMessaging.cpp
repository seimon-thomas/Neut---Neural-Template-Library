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
 *  TestMessaging.cpp
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 23/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */

#include "TestMessaging.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestMessaging);

void TestMessaging::setUp(void)
{
	
	
}

void TestMessaging::tearDown(void) 
{
}

void TestMessaging::sendTest(void)
{
	
	
	/*    CPPUNIT_ASSERT_EQUAL (*a + *b, Fraction (7, 6));
	 CPPUNIT_ASSERT_EQUAL (*b + *c, Fraction (1));
	 CPPUNIT_ASSERT_EQUAL (*d + *e, Fraction (-5));
	 CPPUNIT_ASSERT_EQUAL (*e + *f, Fraction (0));
	 CPPUNIT_ASSERT_EQUAL (*h + *c, Fraction (2, 6));
	 CPPUNIT_ASSERT_EQUAL (*a + *b + *c + *d + *e + *f + *g + *h, Fraction (3, 2));
	 */
}

void TestMessaging::onMessageReceive( boost::shared_ptr< boost::property_tree::ptree > pMessage, size_t messageType )
{

}


