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
#ifndef TestNode_H
#define TestNode_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "NodeResult.h"

using namespace::Neural;

class TestNode : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestNode);
    CPPUNIT_TEST (inEqualsOutTest);
    CPPUNIT_TEST (nodeInputTest);
    CPPUNIT_TEST (nodeInputCSVTest);
    CPPUNIT_TEST (nodeInputCachedTest);
    CPPUNIT_TEST (nodeOutputCSVTest);
    CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	
protected:
	void inEqualsOutTest (void);
	void nodeInputTest (void);
	void nodeInputCSVTest (void);
    void nodeInputCachedTest(void);
	void nodeOutputCSVTest (void);

private:
	void nodeInputCachedTestHelper(size_t setSize);
};

#endif
