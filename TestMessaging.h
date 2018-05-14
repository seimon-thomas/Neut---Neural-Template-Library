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
