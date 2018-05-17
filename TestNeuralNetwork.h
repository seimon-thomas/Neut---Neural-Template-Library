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
#ifndef TestNeuralNetwork_H
#define TestNeuralNetwork_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Connection.h"
#include "NodePerceptronTraining.h"
#include "NodePerceptronBias.h"
#include "NodePassThrough.h"
#include "NodeNetworkLayer.h"
#include "NodePerceptronTarget.h"

using namespace::Neural;

class TestNeuralNetwork : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (TestNeuralNetwork);
    CPPUNIT_TEST (simpleEndToEndTest);
    CPPUNIT_TEST (xorEndToEndTest);
    CPPUNIT_TEST (serializationTest);
    CPPUNIT_TEST (deSerializationTest);
    CPPUNIT_TEST (deSerializationContinuationTest);
    CPPUNIT_TEST_SUITE_END ();
	
public:
	void setUp (void);
	void tearDown (void);
	
protected:
	void simpleEndToEndTest (void);
	void xorEndToEndTest (void);
	void serializationTest (void);
	void deSerializationTest( void );
	void deSerializationContinuationTest( void );
private:

};

#endif
