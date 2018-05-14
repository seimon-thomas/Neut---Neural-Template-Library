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