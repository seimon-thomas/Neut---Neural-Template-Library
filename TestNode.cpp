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
 *  TestNode.cpp
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 25/05/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include "TestNode.h"
#include "NodeInput.h"
#include "NodeOutput.h"
#include "Inputs.h"
#include "InputsFromCSV.h"
#include "OutputsToCSV.h"
#include "InputsCached.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestNode);

void TestNode :: setUp (void)
{
	
	
}

void TestNode :: tearDown (void) 
{
}

void TestNode :: inEqualsOutTest (void)
{
/*
	std::vector< double > inputs;
	inputs.push_back( 0.1 );
	inputs.push_back( 0.2 );
	inputs.push_back( 0.3 );
	inputs.push_back( 1.0 );
	inputs.push_back( 2.0 );
	inputs.push_back( 3.0 );
	std::vector< double > actualOutputs;
	
	// Create a result node
	INodeDoublePtr pResultNode( new NodeResult< double, std::vector< double > >( actualOutputs ) );
	
	// Connect to it
	std::vector< IConnectionDoublePtr > inputConnections;
	for(size_t i=0; i<inputs.size(); ++i)
	{
		inputConnections.push_back( pResultNode->getConnection( INPUTS ) );
	}
	
	// Send the inputs to it
	size_t nextIp = 0;
	BOOST_FOREACH( IConnectionDoublePtr pNextConnection, inputConnections )
	{
	   pNextConnection->send( inputs[ nextIp++ ] );
	}

	// Test that they go there correctly
	for( size_t j=0; j<inputs.size(); ++j )
	{
		CPPUNIT_ASSERT_EQUAL( inputs[j], actualOutputs[j] );
	}
	
     CPPUNIT_ASSERT_EQUAL (*a + *b, Fraction (7, 6));
	 CPPUNIT_ASSERT_EQUAL (*b + *c, Fraction (1));
	 CPPUNIT_ASSERT_EQUAL (*d + *e, Fraction (-5));
	 CPPUNIT_ASSERT_EQUAL (*e + *f, Fraction (0));
	 CPPUNIT_ASSERT_EQUAL (*h + *c, Fraction (2, 6));
	 CPPUNIT_ASSERT_EQUAL (*a + *b + *c + *d + *e + *f + *g + *h, Fraction (3, 2));
	 */
}

void TestNode::nodeInputTest(void)
{
	// Setup the training set (XOR in this case)
	std::vector< boost::shared_ptr< std::vector <double> > > inputSet(4);
    inputSet[ 0 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    inputSet[ 1 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    inputSet[ 2 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    inputSet[ 3 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );

	inputSet[ 0 ]->push_back( 0.1 );	inputSet[ 0 ]->push_back( 1.1 );
 	inputSet[ 1 ]->push_back( 0.2 );	inputSet[ 1 ]->push_back( 1.2 );
	inputSet[ 2 ]->push_back( 0.3 );	inputSet[ 2 ]->push_back( 1.3 );
	inputSet[ 3 ]->push_back( 0.4 );	inputSet[ 3 ]->push_back( 1.4 );
	
	IInputsDoublePtr pInputVals( new Inputs<double>( inputSet ) );
	
	// Create the NodeInput and an activation connection
	INodeDoublePtr pNodeInput( new NodeInput<double, IInputs<double> >( pInputVals ) );
	IActivatePtr   pActivate = pNodeInput->getActivationConnection();

	
	std::vector< double > actualOutputs;	
	// Create a result node
	INodeDoublePtr pResultNode( new NodeResult< double, std::vector< double > >( actualOutputs ) );
	
	//
	// Create 2 connections between input & results
	//
	pNodeInput->connect( pResultNode );
	pNodeInput->connect( pResultNode );
	
	//
	// Activate 4 times & get result
	//
	for(size_t i=0; i<inputSet.size(); ++i)
	{
		pActivate->activate();
		//
		// Check the result & assert if needed
		//
		for (size_t j=0; j<actualOutputs.size(); ++j) 
		{
			CPPUNIT_ASSERT_EQUAL( inputSet[i]->at(j), actualOutputs[j] );
		}
	}
	
}

void TestNode::nodeInputCSVTest(void)
{
	// Setup the training set (XOR in this case)
	std::vector< std::vector <double> > correctInputSet(4);
	correctInputSet[ 0 ].push_back( 0.9 );	correctInputSet[ 0 ].push_back( 0.9 );
	correctInputSet[ 1 ].push_back( 0.9 );	correctInputSet[ 1 ].push_back( 0.1 );
	correctInputSet[ 2 ].push_back( 0.1 );	correctInputSet[ 2 ].push_back( 0.9 );
	correctInputSet[ 3 ].push_back( 0.1 );	correctInputSet[ 3 ].push_back( 0.1 );
	
	IInputsDoublePtr pInputVals( new InputsFromCSVFile<double>( "../Data/Inputs/XORInput.csv" ) );
	
	// Create the NodeInput and an activation connection
	INodeDoublePtr pNodeInput( new NodeInput<double, IInputs<double> >( pInputVals ) );
	IActivatePtr   pActivate = pNodeInput->getActivationConnection();
	
	
	std::vector< double > actualOutputs;	
	// Create a result node
	INodeDoublePtr pResultNode( new NodeResult< double, std::vector< double > >( actualOutputs ) );
	
	//
	// Create 2 connections between input & results
	//
	pNodeInput->connect( pResultNode );
	pNodeInput->connect( pResultNode );
	
	//
	// Activate 4 times & get result
	//
	for(size_t i=0; i<correctInputSet.size(); ++i)
	{
		pActivate->activate();
		//
		// Check the result & assert if needed
		//
		for (size_t j=0; j<actualOutputs.size(); ++j) 
		{
			CPPUNIT_ASSERT_EQUAL( correctInputSet[i][j], actualOutputs[j] );
		}
	}
	
}

void TestNode::nodeInputCachedTest(void)
{
	// Setup the training set (XOR in this case)
/*	std::vector< std::vector <double> > correctInputSet(4);
	correctInputSet[ 0 ].push_back( 0.9 );	correctInputSet[ 0 ].push_back( 0.9 );
	correctInputSet[ 1 ].push_back( 0.9 );	correctInputSet[ 1 ].push_back( 0.1 );
	correctInputSet[ 2 ].push_back( 0.1 );	correctInputSet[ 2 ].push_back( 0.9 );
	correctInputSet[ 3 ].push_back( 0.1 );	correctInputSet[ 3 ].push_back( 0.1 );
*/	
    nodeInputCachedTestHelper(3);
    nodeInputCachedTestHelper(6);
    nodeInputCachedTestHelper(9);
}

void TestNode::nodeInputCachedTestHelper(size_t setSize)
{
    std::cout << std::endl;
    std::cout << " TestNode::nodeInputCachedTest with set size (" << setSize << ")" << std::endl;
    // Create the NodeInput and an activation connection
    IInputsDoublePtr pInputValsCSV( new InputsFromCSVFile<double>( "../Data/Inputs/OverlappedInput.csv" ) );
    IInputsDoublePtr pInputVals( new InputsCached<double, IInputs<double> >( pInputValsCSV ) );
    
    INodeDoublePtr pNodeInput( new NodeInput<double, IInputs<double> >( pInputVals ) );
    IActivatePtr   pActivate = pNodeInput->getActivationConnection();
    
    
    std::vector< double > actualOutputs;	
    // Create a result node
    INodeDoublePtr pResultNode( new NodeResult< double, std::vector< double > >( actualOutputs ) );
    
    //
    // Create 2 connections between input & results
    //
    for( size_t setSizeIdx = 0; setSizeIdx < setSize; ++setSizeIdx )
    {
        pNodeInput->connect( pResultNode );
    }
    //
    // Activate 4 times & get result
    //
    for(size_t i=0; i<16; ++i)
    {
        pActivate->activate();
        //
        // Check the result & assert if needed
        //
        for (size_t j=0; j<actualOutputs.size(); ++j) 
        {
            //			CPPUNIT_ASSERT_EQUAL( correctInputSet[i][j], actualOutputs[j] );
            std::cout <<  actualOutputs[j] << " ,";
        }
        std::cout << std::endl;
    }
}


void TestNode::nodeOutputCSVTest(void)
{
	std::vector< boost::shared_ptr< std::vector <double> > > inputSet(4);
    inputSet[ 0 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    inputSet[ 1 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    inputSet[ 2 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    inputSet[ 3 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>() );
    
	inputSet[ 0 ]->push_back( 0.1 );	inputSet[ 0 ]->push_back( 1.1 );
 	inputSet[ 1 ]->push_back( 0.2 );	inputSet[ 1 ]->push_back( 1.2 );
	inputSet[ 2 ]->push_back( 0.3 );	inputSet[ 2 ]->push_back( 1.3 );
	inputSet[ 3 ]->push_back( 0.4 );	inputSet[ 3 ]->push_back( 1.4 );
	
	IInputsDoublePtr pInputVals( new Inputs<double>( inputSet ) );
	
	// Create the NodeInput and an activation connection
	INodeDoublePtr pNodeInput( new NodeInput<double, IInputs<double> >( pInputVals ) );
	IActivatePtr   pIpActivate = pNodeInput->getActivationConnection();
	
	IOutputsDoublePtr pOutputVals( new OutputsToCSVFile<double, std::vector< double > >( "../Data/Outputs/nodeOutputCSVTest.csv" ) );
	
	// Create the NodeOutput and an activation connection
	INodeDoublePtr pNodeOutput( new NodeOutput<double, IOutputs<double, std::vector< double > > >( pOutputVals ) );
	IActivatePtr   pOpActivate = pNodeOutput->getActivationConnection();
	
	//
	// Create 2 connections between input & results
	//
	pNodeInput->connect( pNodeOutput );
	pNodeInput->connect( pNodeOutput );
	
	//
	// Activate 4 times & get result
	//
	for(size_t i=0; i<inputSet.size(); ++i)
	{
		pIpActivate->activate();
		pOpActivate->activate();
	}
}


