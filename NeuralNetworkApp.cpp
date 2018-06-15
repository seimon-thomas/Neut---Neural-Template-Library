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

#include <iostream>
#include <fstream>
#include <list>
#include <exception>
#include <vector>
#include <stdio.h>
#include <signal.h>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <boost/date_time.hpp>
#include <boost/timer/timer.hpp>

#include "NeuralNetworkApp.h"
#include "NodeSerializer.h"
#include "NodeContainerPeriodicSerializer.h"
#include "NodeContainerPeriodicSerializerLoadOnly.h"

#include "InputsFromCSV.h"
#include "NodeInput.h"
#include "NodeBulkInput.h"
#include "NodePerceptronTarget.h"
#include "NodeResult.h"
#include "NodeOutput.h"
#include "IOutputs.h"
#include "OutputsToCSV.h"
#include "InputsCached.h"
#include "OutputsToCSVScaled.h"

using namespace Neural;

bool NeuralNetworkApp::m_Finish = false;

NeuralNetworkApp::NeuralNetworkApp():m_PlotBuffer( PLOT_BUF_SIZE )
{
	
}

void NeuralNetworkApp::ctrlCHandler( int param )
{
    std::cout << "Ctrl-C received, finishing up..." << std::endl;
    m_Finish = true;
}

void NeuralNetworkApp::registerCtrlCHandler()
{
    signal( SIGINT, NeuralNetworkApp::ctrlCHandler );
}

int  NeuralNetworkApp::runUnitTests()
{
	// informs test-listener about testresults
	CPPUNIT_NS :: TestResult testresult;
	
	// register listener for collecting the test-results
	CPPUNIT_NS :: TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);
	
	// register listener for per-test progress output
	CPPUNIT_NS :: BriefTestProgressListener progress;
	testresult.addListener (&progress);
	
	// insert test-suite at test-runner by registry
	CPPUNIT_NS :: TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
	testrunner.run (testresult);
	
	// output results in compiler-format
	CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
	compileroutputter.write ();
	
	return collectedresults.wasSuccessful () ? 0 : 1;
	
}
int NeuralNetworkApp::runForProfiler()
{
    //
    // Register Ctrl_C handler
    //
    registerCtrlCHandler();
	//
	// The inputs & targets
	//	
    std::vector< boost::shared_ptr< std::vector <double> > > inputSet(4);
     inputSet[ 0 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97, 0.2) );
     inputSet[ 1 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97, 0.4) );
     inputSet[ 2 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97, 0.6) );
     inputSet[ 3 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97, 0.8) );
    
    IInputsDoublePtr pInputValsOriginal( new Inputs<double>( inputSet ) );
    IInputsDoublePtr pInputVals( new InputsCached<double, IInputs<double> >( pInputValsOriginal ) );
    IInputsDoublePtr pTargetVals( new InputsCached<double, IInputs<double> >( pInputValsOriginal ) );

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
	for(size_t i=0; i<97; ++i) pLayer->addChild( INodeDoublePtr( new NodePassThrough<double>() ) );
	pLayer->addChild( INodeDoublePtr( new NodePerceptronBias<double>() ) );
	pNetwork->addChild( pLayer );
	//
	// Hidden neuron layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	for(size_t j=0; j<1000; ++j) pLayer->addChild( INodeDoublePtr( new NodePerceptronTraining<double>( gain, momentum ) ) );
	pLayer->addChild( INodeDoublePtr( new NodePerceptronBias<double>() ) );
	pNetwork->addChild( pLayer );
	//
	// Output neuron layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	for(size_t i=0; i<97; ++i) pLayer->addChild( INodeDoublePtr( new NodePerceptronTraining<double>( gain, momentum ) ) );	
	pNetwork->addChild( pLayer );
	//
	// Output target & output layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	pNetwork->addChild( pLayer );
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	IActivatePtr pTrainActivation( pNetwork->getActivationConnection( OUTPUTS ) );
  
    for(size_t i = 0; (i < 800) && !NeuralNetworkApp::m_Finish; ++i )
    {
        //
        // Recall
        //
        pRecallActivation->activate();
        //
        // Train
        //
        pTrainActivation->activate();
    }
    
    return 0;
}

int  NeuralNetworkApp::createNetwork( const std::string& templateFilename, 
                                      const std::string& networkFilename,
                                      unsigned int randomSeed)
{
    std::cout << "Starting..." << std::endl;
	std::cout << "Creating new network \"" << networkFilename << "\" with template \"" << templateFilename << "\"" << std::endl;

    if( 0 != randomSeed )
    {
        //
        // Seed the random number generator
        //
        ::srand( randomSeed );
    }
    
	NodeSerializer<double> mySerialiser;
	
	INodeDoublePtr pInnerNetwork = mySerialiser.deserialze( templateFilename );
	
	{
		INodeDoublePtr pPeriodicSavingNetwork( new NodeContainerPeriodicSerializer<double, NodeNetworkPillar<double> >(networkFilename, 1000 ) );
		pPeriodicSavingNetwork->addChild( pInnerNetwork );
	}
    
    std::cout << "Finished." << std::endl;
	return 0;
}
int  NeuralNetworkApp::trainNetwork( const std::string& inputFilename, 
									 const std::string& networkFilename, 
									 const std::string& targetFilename, 
									 double errorThreshold, 
									 unsigned long long saveInterval,
                                     unsigned long long epochSize,
                                     bool gnuplot,
                                     bool bulkInput )
{
    //
    // Register Ctrl_C handler
    //
    registerCtrlCHandler();
    if(!gnuplot)
	{
		std::cout << "Starting...\n";
		std::cout << "Constructing inputs nodes...\n";
    }
	//
	// The target CSV
	//	
	IInputsDoublePtr pTargetValsCSV( new InputsFromCSVFile<double>( targetFilename ) );
    IInputsDoublePtr pTargetVals( new InputsCached<double, IInputs<double> >( pTargetValsCSV ) );
    
    // TODO: Remove - following for debug only
/*	std::vector< boost::shared_ptr< std::vector <double> > > inputSet(4);
    inputSet[ 0 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97) );
    inputSet[ 1 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97) );
    inputSet[ 2 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97) );
    inputSet[ 3 ] = boost::shared_ptr< std::vector <double> >( new std::vector<double>(97) );
    IInputsDoublePtr pTargetVals( new Inputs<double>( inputSet ) );
*/    
	//
	// The network
	//
	INodeDoublePtr pNetwork( new NodeNetworkPillar<double>() );
	//
	// Input layer (from CSV)
	//
	INodeDoublePtr pLayer( new NodeNetworkLayer<double>() ); 
    
	IInputsDoublePtr pInputValsCSV( new InputsFromCSVFile<double>( inputFilename ) );
    IInputsDoublePtr pInputVals( new InputsCached<double, IInputs<double> >( pInputValsCSV ) );
    
    // TODO: Remove - following for debug only
    // IInputsDoublePtr pInputVals( new Inputs<double>( inputSet ) );
    
    if( bulkInput )
    {
        pLayer->addChild( INodeDoublePtr( new NodeBulkInput<double, IInputs<double> >( pInputVals ) ) );
    }
    else
    {
        pLayer->addChild( INodeDoublePtr( new NodeInput<double, IInputs<double> >( pInputVals ) ) );
    }
	pNetwork->addChild( pLayer );
	
	// ********* Inner Network Start **************
	
    if(!gnuplot)
	{
		std::cout << "Constructing Inner Network...\n";
    }

	INodeDoublePtr pPeriodicSavingNetwork( new NodeContainerPeriodicSerializer<double, NodeNetworkPillar<double> >( networkFilename, saveInterval ) );

    if(!gnuplot)
	{
		std::cout << "Inner Network Constucted, adding to parent...\n";
    }

	pNetwork->addChild( pPeriodicSavingNetwork );

    if(!gnuplot)
	{
		std::cout << "Inner Network added to parent...\n";
 		std::cout << "Constructing output network...\n";
    }
    
	
	// ********* Inner Network End **************
	
	//
	// Output target layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodePerceptronTarget<double>( pTargetVals ) ) );	
	pNetwork->addChild( pLayer );
	//
	// Results capture layer
	//
	std::vector< double > errorVec;
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	pLayer->addChild( INodeDoublePtr( new NodeResult< double, std::vector< double > >( errorVec ) ) );	
	pNetwork->addChild( pLayer );
	
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	IActivatePtr pTrainActivation( pNetwork->getActivationConnection( OUTPUTS ) );
	
	double rmsError = 0.0; //Make big
	//
	// Gnuplot stuff
	//
	if(gnuplot)
	{
		std::cout << "set xtics\n";
		std::cout << "set ytics\n";
		std::cout << "set yrange[0:1.0]\n";
//		std::cout << "set style data linespoints\n";
		std::cout << "set style data points\n";
		std::cout << "set grid\n";
	}
    else
	{
		std::cout << "Beginning to train...\n";
    }

	//
	// Errors
	//
	std::vector<double> allErrors( epochSize );
	//
	// Train the network
	//
	unsigned long iterations = 0;
	do
	{
        {
            
         boost::timer::cpu_timer epochTimer;

		//
		// Reinit error
		//
		rmsError = 0.0;
		//
		// Loop the epock
		//
		for(unsigned long long epocIndex = 0; (epocIndex < epochSize) && !NeuralNetworkApp::m_Finish; ++epocIndex )
		{
/*            if(!gnuplot)
            {
                std::cout << "Epoch iteration: (" << iterations << ") Input iteration: (" << epocIndex << ")             \r";
                std::cout.flush();
            }
*/			//
			// Recall
			//
			pRecallActivation->activate();
			//
			// Train
			//
			pTrainActivation->activate();
			//
			// Next error
			//
			double nextError = errorVec[0];
			//
			// Put it in the collection of errors
			//
			allErrors[ epocIndex ] = nextError;
			//
			// Accumulate the error
			//
			rmsError+= nextError * nextError;
		}
		if(gnuplot)	gnuPlotRMSError( allErrors, iterations );
		++iterations;
        if(!gnuplot)
        {
            std::string sEpochTime = epochTimer.format();
            
            std::cout.precision(std::numeric_limits<double>::digits10 + 2);
            // TODO: Refactor
            //
            bool saveImminent = (( (iterations*epochSize)%saveInterval ) == 0 );
            
            if( saveImminent ) std::cout << boost::posix_time::second_clock::local_time() << " ";

            std::cout << "RMS Error: " << sqrt(rmsError/((double)epochSize));
            
            std::cout.precision( 6 );
            std::cout << "    Time for epoch: " << sEpochTime << "   \r";
            
            if( saveImminent ) std::cout << "\n\r";
            
            std::cout.flush();
            
        }
        }
	}
	while( (sqrt(rmsError/((double)epochSize)) > errorThreshold) && !NeuralNetworkApp::m_Finish );	
	if(gnuplot)
	{
		std::cout << "exit\n";
	}
    else
    {
        std::cout << "\nFinished.\n";
    }
	return 0;
}

void NeuralNetworkApp::gnuPlotRMSError( const std::vector< double >& rmsErrors, unsigned long iterations )
{
	if( (iterations%10) == 0 )
	{
		if( iterations < (PLOT_BUF_SIZE*10) )
		{
			std::cout << "set xrange[0:" << (PLOT_BUF_SIZE*10)-1 << "]\n";
			m_PlotBuffer[iterations/10] = std::pair< unsigned long, std::vector< double > >( iterations, rmsErrors );
			std::cout << "plot \"-\" notitle\n";
			for(unsigned long i=0; i <= (iterations/10); ++i )
			{
				for(size_t j=0; j< m_PlotBuffer[i].second.size(); ++j)
				{
					std::cout << m_PlotBuffer[i].first << " " << m_PlotBuffer[i].second[j]<< "\n";
				}
			}
		}
		else
		{
			unsigned long startRange = iterations - (PLOT_BUF_SIZE*10);
			std::cout << "set xrange[" << startRange << ":" << (startRange+(PLOT_BUF_SIZE*10))-1 << "]\n";
			m_PlotBuffer[(iterations/10)%PLOT_BUF_SIZE] = std::pair< unsigned long, std::vector< double > >( iterations, rmsErrors );
			std::cout << "plot \"-\" notitle\n";
			unsigned long nextBufPoint;
			for(unsigned long i=1; i <= PLOT_BUF_SIZE; ++i )
			{
				nextBufPoint = ((iterations/10)+i)%PLOT_BUF_SIZE;
				for(size_t j=0; j< m_PlotBuffer[nextBufPoint].second.size(); ++j)
				{
					std::cout << m_PlotBuffer[nextBufPoint].first << " " << m_PlotBuffer[nextBufPoint].second[j]<< "\n";
				}
			}
		}
		std::cout << "e\n";
		std::cout.flush();
	}
}

int  NeuralNetworkApp::recallNetwork( const std::string& inputFilename, 
                                      const std::string& networkFilename, 
                                      const std::string& outputFilename,
                                      unsigned long long epochSize,
                                      bool bulkInput,
                                      const std::string& targetScaleFilename )
{
    std::cout << "Starting..." << std::endl;
	std::cout << "Recalling network \"" << networkFilename << "\" with inputs \"" << inputFilename << "\"" << std::endl;
	//
	// The input CSV
	//
	IInputsDoublePtr pInputValsCSV( new InputsFromCSVFile<double>( inputFilename ) );
    IInputsDoublePtr pInputVals( new InputsCached<double, IInputs<double> >( pInputValsCSV ) );
	//
	// Add the layers to a network
	//
	INodeDoublePtr pNetwork( new NodeNetworkPillar<double>() );
	//
	// Input layer (from CSV)
	//
	INodeDoublePtr pLayer( new NodeNetworkLayer<double>() ); 
    if( bulkInput )
    {
        pLayer->addChild( INodeDoublePtr( new NodeBulkInput<double, IInputs<double> >( pInputVals ) ) );
    }
    else
    {
        pLayer->addChild( INodeDoublePtr( new NodeInput<double, IInputs<double> >( pInputVals ) ) );
    }
	pNetwork->addChild( pLayer );
	
	// ********* Inner Network Start **************
	
	INodeDoublePtr pPeriodicSavingNetwork( new NodeContainerPeriodicSerializerLoadOnly<double, NodeNetworkPillar<double> >(networkFilename, 1000) );
	
	pNetwork->addChild( pPeriodicSavingNetwork );
	
	// ********* Inner Network End **************
	
	//
	// Output target & output layer
	//
	pLayer = INodeDoublePtr( new NodeNetworkLayer<double>() ); 
	IOutputsDoublePtr pOutputVals;
    if( targetScaleFilename.empty() )
    {
        pOutputVals = IOutputsDoublePtr( new OutputsToCSVFile<double, std::vector< double > >( outputFilename ) );
    }
    else
    {
        pOutputVals = IOutputsDoublePtr( new OutputsToCSVScaledFile<double, std::vector< double > >( outputFilename, targetScaleFilename ) );
        std::cout << "Using scale and offset from \"" << targetScaleFilename << "\"" << std::endl;
        
    }
	pLayer->addChild(INodeDoublePtr( new NodeOutput<double, IOutputs<double, std::vector< double > > >( pOutputVals ) ) );
	pNetwork->addChild( pLayer );
	
	//
	// Get Activation connections to the network
	//
	IActivatePtr pRecallActivation( pNetwork->getActivationConnection( INPUTS ) );
	
	//
	// Loop the epock
	//
	for(unsigned long long epocIndex = 0; epocIndex < epochSize; ++epocIndex )
	{
		//
		// Recall
		//
		pRecallActivation->activate();
	}

    std::cout << "Outputs placed in \"" << outputFilename << "\"" << std::endl;
    std::cout << "Finished." << std::endl;
	return 0;
}

#if 0
unsigned long long NeuralNetworkApp::getLinesInFile( const std::string& filename )
{
	unsigned long long	linesInFile = 0;
	
	std::ifstream file( filename.c_str() );
	std::string line;
	
	while( std::getline( file, line ) ) 
    {
        std::vector< std::string > vec;
        boost::tokenizer< boost::escaped_list_separator<char> > tok(line);
        vec.assign(tok.begin(),tok.end());
        boost::to_upper(vec[0]);
        if( 0 == vec[0].compare("HEADINGS") )
        {
        }
        else if( 0 == vec[0].compare("SCALE") )
        {
        }
        else if( 0 == vec[0].compare("OFFSET") )
        {
        }
        else
        {
            ++linesInFile;   
        }
    }
	
	return linesInFile;
}
#endif

