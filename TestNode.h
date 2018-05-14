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