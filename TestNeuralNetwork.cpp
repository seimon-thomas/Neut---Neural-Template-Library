/*
 *  TestNeuralNetwork.cpp
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 25/05/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */

#include <iostream>
#include <list>
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "TestNeuralNetwork.h"
#include "INode.h"
#include "NodeResult.h"
#include "NodeInput.h"
#include "Inputs.h"
#include "InputsFromCSV.h"
#include "NodeOutput.h"
#include "OutputsToCSV.h"
#include "NodeNetworkPillar.h"
#include "NodeSerializer.h"
#include "NodeContainerTwoForOneActivation.h"
#include "NodeContainerActivationRepeater.h"
#include "NodeContainerPeriodicSerializer.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestNeuralNetwork);

void TestNeuralNetwork :: setUp (void)
{
 
	
}

void TestNeuralNetwork :: tearDown (void) 
{
}

void TestNeuralNetwork :: simpleEndToEndTest (void)
{	
/*
	// Setup the training simple pass thru
	std::vector< std::vector <double> > inputSet(2);
	inputSet[ 0 ].push_back( 0.1 );
	inputSet[ 1 ].push_back( 0.9 );
	IInputsDoublePtr pInputVals( new Inputs<double>( inputSet ) );
	
	std::vector< std::vector <double> > targetSet(2);
	targetSet[ 0 ].push_back( 0.1 );	
	targetSet[ 1 ].push_back( 0.9 );	
	
	IInputsDoublePtr pTargetVals( new Inputs<double>( targetSet ) );
	
	
	// Setup the layers
	
	boost::shared_ptr< PerceptronTrainingParms<double> > pParms(new PerceptronTrainingParms<double>(0.8,0.2));
	
	std::vector< INodeDoublePtr > layers;
	
	layers.push_back(INodeDoublePtr( new NodeNetworkLayer<double>() ) ); 
	layers[0]->add( INodeDoublePtr( new NodePerceptronTraining<double>( pParms ) ) );
	
	layers.push_back(INodeDoublePtr( new NodeNetworkLayer<double>() ) ); 
	layers[1]->add( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	
	// Connect the layers together
	layers[0]->connect( layers[1] );
	
	// Get input connection
	IConnectionDoublePtr pInputConnection = layers[0]->items()->at(0)->getConnection( INPUTS );
	
	
	// Get Activation connections to the layers
	std::list< IActivatePtr > recallActivations;
	recallActivations.push_back( layers[ 0 ]->getActivationConnection( INPUTS ) );
	recallActivations.push_back( layers[ 1 ]->getActivationConnection( INPUTS ) );
	
	std::list< IActivatePtr > trainActivations;
	trainActivations.push_back( layers[ 1 ]->getActivationConnection( OUTPUTS ) );
	trainActivations.push_back( layers[ 0 ]->getActivationConnection( OUTPUTS ) );
	
	// Train for this many iterations
	for(size_t runIdx = 0; runIdx < 100; ++runIdx )
	{
#ifdef NN_DUMP_OUTPUT
		std::cout << "Iteration = " << runIdx << std::endl;
#endif		
		// Input the next set
		const std::vector<double>& nextInputSet = pInputVals->getNextInputSet();
		pInputConnection->send( nextInputSet[ 0 ] );
		
		// Recall
		BOOST_FOREACH( IActivatePtr pNextRecall, recallActivations )
		{
			pNextRecall->activate();
		}
		
		// Train
		BOOST_FOREACH( IActivatePtr pNextTrain, trainActivations )
		{
			pNextTrain->activate();
		}		
	}
	
    CPPUNIT_ASSERT_EQUAL (*a + *b, Fraction (7, 6));
    CPPUNIT_ASSERT_EQUAL (*b + *c, Fraction (1));
    CPPUNIT_ASSERT_EQUAL (*d + *e, Fraction (-5));
    CPPUNIT_ASSERT_EQUAL (*e + *f, Fraction (0));
    CPPUNIT_ASSERT_EQUAL (*h + *c, Fraction (2, 6));
    CPPUNIT_ASSERT_EQUAL (*a + *b + *c + *d + *e + *f + *g + *h, Fraction (3, 2));
*/
}

void TestNeuralNetwork :: xorEndToEndTest(void)
{	
	//
	// Setup the training set (XOR in this case)
	//
	// The input CSV
	//
	IInputsDoublePtr pInputVals( new InputsFromCSVFile<double>( "../Data/Inputs/XORInput.csv" ) );
	//
	// The target CSV
	//	
	IInputsDoublePtr pTargetVals( new InputsFromCSVFile<double>( "../Data/Targets/XORTargets.csv" ) );
	//
	// Training Parameters
	//
	double gain = 0.3;
	double momentum = 0.4;
	//
	// Add the layers to a network
	//
	INodeDoublePtr pNetwork( new NodeNetworkPillar<double>() );
	//
	// Input layer (from CSV)
	//
	INodeDoublePtr pLayer( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeInput<double, IInputs<double> >( pInputVals ) ) );
	pNetwork->addChild( pLayer );
	//
	// Pass throught to connect input to first hidden layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	for(size_t i=0; i<2; ++i) pLayer->addChild( INodeDoublePtr( new NodePassThrough<double>() ) );
	pLayer->addChild( INodeDoublePtr( new NodePerceptronBias<double>() ) );
	pNetwork->addChild( pLayer );
	//
	// Hidden neuron layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	for(size_t j=0; j<5; ++j) pLayer->addChild( INodeDoublePtr( new NodePerceptronTraining<double>( gain, momentum ) ) );
	pLayer->addChild( INodeDoublePtr( new NodePerceptronBias<double>() ) );
	pNetwork->addChild( pLayer );
	//
	// Output neuron layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTraining<double>( gain, momentum ) ) );	
	pNetwork->addChild( pLayer );
	//
	// Output target & output layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	IOutputsDoublePtr pOutputVals( new OutputsToCSVFile<double, std::vector< double > >( "../Data/Outputs/XOROutputs.csv" ) );
	pLayer->addChild(INodeDoublePtr( new NodeOutput<double, IOutputs<double, std::vector< double > > >( pOutputVals ) ) );
	pNetwork->addChild( pLayer );
	//
	// Results capture layer
	//
	std::vector< double > rmsError;
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeResult< double, std::vector< double > >( rmsError ) ) );	
	pNetwork->addChild( pLayer );
	
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	IActivatePtr pTrainActivation( pNetwork->getActivationConnection( OUTPUTS ) );

	//
	// The outer networks for training
	//
	INodeDoublePtr pOuterRunningNetwork( new NodeContainerActivationRepeater<double, NodeNetworkPillar<double> >(6000) );
	INodeDoublePtr pTrainingNetwork( new NodeContainerTwoForOneActivation<double, NodeNetworkPillar<double> >() );
	pTrainingNetwork->addChild( pNetwork );
	pOuterRunningNetwork->addChild(	pTrainingNetwork );
	//
	// Get connection and train the network for 6000 iterations
	//
	IActivatePtr pFullTrainingActivation( pOuterRunningNetwork->getActivationConnection( OUTPUTS ) );
	pFullTrainingActivation->activate();

	//
	// Recall test
	//
	std::vector< double > rmsErrorsForAll;
	std::cout << std::endl;
	std::cout.precision(std::numeric_limits<double>::digits10 + 2);
	for(size_t recallIdx = 0; recallIdx < 4; ++recallIdx )
	{
		//
		// Recall
		//
		pRecallActivation->activate();
		//
		// Train
		//
		pTrainActivation->activate();
		//
		// Store & display error
		//		
		rmsErrorsForAll.push_back( rmsError[0] );
		std::cout << "rmsError[ " << recallIdx << " ] = " <<  rmsError[0] << std::endl;
	}
	//
	// Check for assertion
	//
//	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 0",0.04, rmsErrorsForAll[0], 0.01);
//	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 1",0.04, rmsErrorsForAll[1], 0.01);
//	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 2",0.04, rmsErrorsForAll[2], 0.01);
//	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 3",0.04, rmsErrorsForAll[3], 0.01);

	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 0",0.0649785, rmsErrorsForAll[0], 0.00001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 1",0.0524784, rmsErrorsForAll[1], 0.00001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 2",0.056017, rmsErrorsForAll[2], 0.00001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Failed XOR error test on epock 3",0.0349544, rmsErrorsForAll[3], 0.00001);
	
}

void TestNeuralNetwork::serializationTest(void)
{
	//
	// Setup the training set (XOR in this case)
	//
	// The input CSV
	//
	IInputsDoublePtr pInputVals( new InputsFromCSVFile<double>( "../Data/Inputs/XORInput.csv" ) );
	//
	// The target CSV
	//	
	IInputsDoublePtr pTargetVals( new InputsFromCSVFile<double>( "../Data/Targets/XORTargets.csv" ) );
	//
	// Training Parameters
	//
	double gain = 0.3;
	double momentum = 0.4;
	//
	// Add the layers to a network
	//
	INodeDoublePtr pNetwork( new NodeNetworkPillar<double>() );
	//
	// Input layer (from CSV)
	//
	INodeDoublePtr pLayer( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeInput<double, IInputs<double> >( pInputVals ) ) );
	pNetwork->addChild( pLayer );
	
	// ********* Inner Network Start **************
	
	INodeDoublePtr pInnerNetwork( new NodeNetworkPillar<double>() );
	//
	// Pass throught to connect input to first hidden layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	for(size_t i=0; i<2; ++i) pLayer->addChild( INodeDoublePtr( new NodePassThrough<double>() ) );
	pLayer->addChild( INodeDoublePtr( new NodePerceptronBias<double>() ) );
	pInnerNetwork->addChild( pLayer );
	//
	// Hidden neuron layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	for(size_t j=0; j<5; ++j) pLayer->addChild( INodeDoublePtr( new NodePerceptronTraining<double>( gain, momentum ) ) );
	pLayer->addChild( INodeDoublePtr( new NodePerceptronBias<double>() ) );
	pInnerNetwork->addChild( pLayer );
	//
	// Output neuron layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTraining<double>( gain, momentum ) ) );	
	pInnerNetwork->addChild( pLayer );

	pNetwork->addChild( pInnerNetwork );
	
	// ********* Inner Network End **************
	
	//
	// Output target & output layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	IOutputsDoublePtr pOutputVals( new OutputsToCSVFile<double, std::vector< double > >( "../Data/Outputs/XOROutputs.csv" ) );
	pLayer->addChild(INodeDoublePtr( new NodeOutput<double, IOutputs<double, std::vector< double > > >( pOutputVals ) ) );
	pNetwork->addChild( pLayer );
	//
	// Results capture layer
	//
	std::vector< double > rmsError;
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeResult< double, std::vector< double > >( rmsError ) ) );	
	pNetwork->addChild( pLayer );
	
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	IActivatePtr pTrainActivation( pNetwork->getActivationConnection( OUTPUTS ) );
	
	//
	// The outer networks for training
	//
	INodeDoublePtr pOuterRunningNetwork( new NodeContainerActivationRepeater<double, NodeNetworkPillar<double> >(6000) );
	INodeDoublePtr pTrainingNetwork( new NodeContainerTwoForOneActivation<double, NodeNetworkPillar<double> >() );

	pTrainingNetwork->addChild( pNetwork );
	pOuterRunningNetwork->addChild(	pTrainingNetwork );
	
	//
	// Get connection and train the network for 6000 iterations
	//
	IActivatePtr pFullTrainingActivation( pOuterRunningNetwork->getActivationConnection( OUTPUTS ) );
	pFullTrainingActivation->activate();
	
	//
	// Recall test
	//
	std::vector< double > rmsErrorsForAll;
	std::cout << std::endl;
	std::cout.precision(std::numeric_limits<double>::digits10 + 2);
	for(size_t recallIdx = 0; recallIdx < 4; ++recallIdx )
	{
		//
		// Recall
		//
		pRecallActivation->activate();
		//
		// Train
		//
//		pTrainActivation->activate();
		//
		// Check for expected output
		//		
		rmsErrorsForAll.push_back( rmsError[0] );
		std::cout << "Serialize rmsError[ " << recallIdx << " ] = " <<  rmsError[0] << std::endl;
	}	

	
	NodeSerializer<double> mySerialiser;
	
	mySerialiser.serialize( pInnerNetwork, "../Data/Networks/Network.xml" );
	
	
}

void TestNeuralNetwork::deSerializationTest(void)
{
	//
	// Setup the training set (XOR in this case)
	//
	// The input CSV
	//
	IInputsDoublePtr pInputVals( new InputsFromCSVFile<double>( "../Data/Inputs/XORInput.csv" ) );
	//
	// The target CSV
	//	
	IInputsDoublePtr pTargetVals( new InputsFromCSVFile<double>( "../Data/Targets/XORTargets.csv" ) );
	//
	// Add the layers to a network
	//
	INodeDoublePtr pNetwork( new NodeNetworkPillar<double>() );
	//
	// Input layer (from CSV)
	//
	INodeDoublePtr pLayer( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeInput<double, IInputs<double> >( pInputVals ) ) );
	pNetwork->addChild( pLayer );
	
	// ********* Inner Network Deserialised Start **************
	
	NodeSerializer<double> mySerialiser;
	
	INodeDoublePtr pInnerNetwork = mySerialiser.deserialze("../Data/Networks/Network.xml");
	
	pNetwork->addChild( pInnerNetwork );
	
	// ********* Inner Network Deserialised End **************
	
	//
	// Output target & output layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	IOutputsDoublePtr pOutputVals( new OutputsToCSVFile<double, std::vector< double > >( "../Data/Outputs/XOROutputs.csv" ) );
	pLayer->addChild(INodeDoublePtr( new NodeOutput<double, IOutputs<double, std::vector< double > > >( pOutputVals ) ) );
	pNetwork->addChild( pLayer );
	//
	// Results capture layer
	//
	std::vector< double > rmsError;
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeResult< double, std::vector< double > >( rmsError ) ) );	
	pNetwork->addChild( pLayer );
	
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	IActivatePtr pTrainActivation( pNetwork->getActivationConnection( OUTPUTS ) );
	
	//
	// Just Recall To Dest The DeSerializer
	//
	std::vector< double > rmsErrorsForAll;
	std::cout << std::endl;
	std::cout.precision(std::numeric_limits<double>::digits10 + 2);
	for(size_t recallIdx = 0; recallIdx < 4; ++recallIdx )
	{
		//
		// Recall
		//
		pRecallActivation->activate();
		//
		// Train
		//
//		pTrainActivation->activate();
		//
		// Check for expected output
		//		
		rmsErrorsForAll.push_back( rmsError[0] );
		std::cout << "Deserialize rmsError[ " << recallIdx << " ] = " <<  rmsError[0] << std::endl;
	}	
	
}

void TestNeuralNetwork::deSerializationContinuationTest()
{
	//
	// Setup the training set (XOR in this case)
	//
	// The input CSV
	//
	IInputsDoublePtr pInputVals( new InputsFromCSVFile<double>( "../Data/Inputs/XORInput.csv" ) );
	//
	// The target CSV
	//	
	IInputsDoublePtr pTargetVals( new InputsFromCSVFile<double>( "../Data/Targets/XORTargets.csv" ) );
	//
	// Add the layers to a network
	//
	INodeDoublePtr pNetwork( new NodeNetworkPillar<double>() );
	//
	// Input layer (from CSV)
	//
	INodeDoublePtr pLayer( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeInput<double, IInputs<double> >( pInputVals ) ) );
	pNetwork->addChild( pLayer );
	
	// ********* Inner Network Start **************
	
	INodeDoublePtr pPeriodicSavingNetwork( new NodeContainerPeriodicSerializer<double, NodeNetworkPillar<double> >("../Data/Networks/XorNetwork/XorNetwork.xml", 1000) );
	
	pNetwork->addChild( pPeriodicSavingNetwork );
	
	// ********* Inner Network End **************
	
	//
	// Output target & output layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	IOutputsDoublePtr pOutputVals( new OutputsToCSVFile<double, std::vector< double > >( "../Data/Outputs/XOROutputs.csv" ) );
	pLayer->addChild(INodeDoublePtr( new NodeOutput<double, IOutputs<double, std::vector< double > > >( pOutputVals ) ) );
	pNetwork->addChild( pLayer );
	//
	// Results capture layer
	//
	std::vector< double > rmsError;
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeResult< double, std::vector< double > >( rmsError ) ) );	
	pNetwork->addChild( pLayer );
	
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	IActivatePtr pTrainActivation( pNetwork->getActivationConnection( OUTPUTS ) );
	
	//
	// The outer networks for training
	//
	INodeDoublePtr pOuterRunningNetwork( new NodeContainerActivationRepeater<double, NodeNetworkPillar<double> >(6000) );
	INodeDoublePtr pTrainingNetwork( new NodeContainerTwoForOneActivation<double, NodeNetworkPillar<double> >() );
	
	pTrainingNetwork->addChild( pNetwork );
	pOuterRunningNetwork->addChild(	pTrainingNetwork );
	
	//
	// Get connection and train the network for 6000 iterations
	//
	IActivatePtr pFullTrainingActivation( pOuterRunningNetwork->getActivationConnection( OUTPUTS ) );
	pFullTrainingActivation->activate();
	
	//
	// Recall test
	//
	std::vector< double > rmsErrorsForAll;
	std::cout << std::endl;
	std::cout.precision(std::numeric_limits<double>::digits10 + 2);
	for(size_t recallIdx = 0; recallIdx < 4; ++recallIdx )
	{
		//
		// Recall
		//
		pRecallActivation->activate();
		//
		// Train
		//
		//		pTrainActivation->activate();
		//
		// Check for expected output
		//		
		rmsErrorsForAll.push_back( rmsError[0] );
		std::cout << "Deserialize continuation rmsError[ " << recallIdx << " ] = " <<  rmsError[0] << std::endl;
	}	
	
	
}
