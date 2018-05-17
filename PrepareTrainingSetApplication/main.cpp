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
//
//  main.cpp
//  PrepareTrainingSetApplication
//
//  Created by Seimon Thomas on 19/11/2013.
//
//

#include <iostream>
#include <sstream>

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "PrepareTrainingSet.h"
#include "UnDeltaResultsSet.h"
#include "ForecastGrowthResultsSet.h"
#include "TrainingRow.h"
#include "Delta.h"
#include "TrainingMatrix.h"

namespace po = boost::program_options;

int main(int argc, const char * argv[])
{
	int retVal = 1;
	try
	{
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help,H", "produce help message")
        ("scale,S", "scale the input file")
        ("unscale,U", "unscale the input file")
        ("growth,G", "calculate forecast growth from the input file")
        ("input,I", po::value<std::string>() , "input filename")
        ("output,O", po::value<std::string>() , "output filename")
        ("training,R", po::value<std::string>() , "training input filename")
        ("targets,T", po::value<std::string>() , "targets output filename")
        ("inputrows,N", po::value<unsigned long>(), "number of input rows")
        ("offset,X",po::value<double>(), "offset value")
        ("range,A",po::value<double>(), "offset value")
        ;
        
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        
        if(vm.count("help"))
        {
            std::cout << "Help asked for..." << std::endl;
            std::cout << desc << std::endl;
        }
        else if(vm.count("scale"))
        {
            if(!vm.count("input"))
            {
                std::cout << "No input filename provided for scaling." << std::endl;
            }
            else if(!vm.count("output"))
            {
                std::cout << "No output filename provided for scaling." << std::endl;
            }
            else if(!vm.count("training"))
            {
                std::cout << "No training inputs filename provided for scaling." << std::endl;
            }
            else if(!vm.count("targets"))
            {
                std::cout << "No targets filename provided for scaling." << std::endl;
            }
            else if(!vm.count("inputrows"))
            {
                std::cout << "No inputrows provided for scaling." << std::endl;
            }
            else if(!vm.count("offset"))
            {
                std::cout << "No offset value provided for scaling." << std::endl;
            }
            else if(!vm.count("range"))
            {
                std::cout << "No range value provided for scaling." << std::endl;
            }
            else
            {
                const std::string inputFilename( vm["input"].as<std::string>() );
                const std::string outputFilename( vm["output"].as<std::string>() );
                const std::string trainingInputsFilename( vm["training"].as<std::string>() );
                const std::string trainingTargetsFilename( vm["targets"].as<std::string>() );
                const unsigned long inputRows( vm["inputrows"].as<unsigned long>() );
                const double offset( vm["offset"].as<double>() );
                const double range( vm["range"].as<double>() );
                
                Neural::PrepareTrainingSet<double> scaler(inputFilename,
                                                          outputFilename,
                                                          trainingInputsFilename,
                                                          trainingTargetsFilename,
                                                          inputRows, // Was 30
                                                          1,
                                                          range, // Was 0.75
                                                          offset); // Was 0.15
                
                std::cout << "\tScaling: inputrows = " << inputRows << ", offset = " << offset << ", range = " << range << std::endl;
                std::cout << "\tFrom   : " << inputFilename << std::endl;
                std::cout << "\tTo     : " 	<< outputFilename << std::endl;
                
                scaler.run();

                
                retVal = 0;
            }
        }
        else if(vm.count("unscale"))
        {
            if(!vm.count("input"))
            {
                std::cout << "No input filename provided for scaling." << std::endl;
            }
            else if(!vm.count("output"))
            {
                std::cout << "No output filename provided for scaling." << std::endl;
            }
            else
            {
                const std::string inputFilename( vm["input"].as<std::string>() );
                const std::string outputFilename( vm["output"].as<std::string>() );
                
                Neural::UnDeltaResultsSet<double> unScaler(inputFilename,
                                                          outputFilename);

                std::cout << "\tUnScaling" << std::endl;
                std::cout << "\tFrom   : " << inputFilename << std::endl;
                std::cout << "\tTo     : " 	<< outputFilename << std::endl;

                unScaler.run();
                
                retVal = 0;
                
            }
        }
        else if(vm.count("growth"))
        {
            if(!vm.count("input"))
            {
                std::cout << "No input filename provided for growth." << std::endl;
            }
            else if(!vm.count("output"))
            {
                std::cout << "No output filename provided for growth." << std::endl;
            }
            else
            {
                const std::string inputFilename( vm["input"].as<std::string>() );
                const std::string outputFilename( vm["output"].as<std::string>() );
                
                Neural::ForecastGrowthResultsSet<double> growthCalculator( inputFilename,
                                                                           outputFilename);
            
                std::cout << "\tGrowth" << std::endl;
                std::cout << "\tFrom : " << inputFilename << std::endl;
                std::cout << "\tTo   : " << outputFilename << std::endl;
                
                growthCalculator.run();
                
                retVal = 0;
                
            }
        }
        else
        {
            std::cout << desc << std::endl;
            retVal = 1;
        }
    }
    catch(const std::exception& eX)
    {
        std::cout << "std::excepion caught: " << eX.what() << std::endl;
        retVal = 1;
	}
	catch(...)
	{
		std::cout << "Unknown exception caught" << std::endl;
        retVal = 1;
	}
	
	
    return retVal;

//    std::cout << "Initialising" << std::endl;
//
//    
//    
//    Neural::PrepareTrainingSet<double>
//    prepared("/Users/seimon/Development/Production/MarketPredictor/Runs/FTSE.2013.11.18/Inputs/4.Joined/FTSE100.csv",
//             "/Users/seimon/Development/Production/MarketPredictor/Runs/FTSE.2013.11.18/Inputs/4.Joined/FTSE100Scaled.csv");
//
//    std::cout << "Processing" << std::endl;
//    prepared.run();
//    std::cout << "Finished" << std::endl;

//    std::stringstream testStream;
//    testStream << "Headings,AA,BB,CC" << std::endl;
//    testStream << "12/02/1967,0.1,0.2,0.3" << std::endl;
//    testStream << "12/03/1967,0.6,0.8,1.0" << std::endl;
//    testStream << "12/04/1967,0.9,1.4,1.7" << std::endl;
//    testStream << "12/04/1967,1.0,1.5,22.8" << std::endl;
//
//    Neural::TrainingMatrix<double> trainingMatrix;
//    
//    testStream >> trainingMatrix;
//    
//    std::cout << trainingMatrix;
//    
//    trainingMatrix.process(0.7,0.15);
//    
//    std::cout << trainingMatrix;
}

