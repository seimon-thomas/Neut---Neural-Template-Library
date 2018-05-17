#include <iostream>
#include <list>
#include <exception>

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/progress.hpp>

#include "NeuralNetworkApp.h"

namespace po = boost::program_options;

using namespace Neural;
//int main (int argc, char * const argv[]) 
int main (int argc, char * argv[]) 
{
	int retVal = 1;
	try
	{
		po::options_description desc("Allowed options");
        desc.add_options()
		("help,H", "produce help message")
		("unittest,U", "run unit tests")
		("profile,P", "run for profiler")
		("create", "create a network from template (use with template & network options)")
		("train", "train a network")
		("recall", "recall a network")
		("gnuplot,G", "produce gnuplot formatted stdout")
		("template,T", po::value<std::string>() , "template filename")
		("network,N", po::value<std::string>() , "network filename")
		("input,I", po::value<std::string>() , "input filename")
		("target,R", po::value<std::string>() , "target filename")
 		("error,E", po::value<double>() , "stop training at error")
  		("saveevery,S", po::value<unsigned long>() , "iterations between saves")
		("output,O", po::value<std::string>() , "output filename")
  		("epochsize,E", po::value<unsigned long>() , "size of the epoch - number of exemplars in the input set")
        ("bulk,B", "loaded network will accept a bulk input")
  		("seed", po::value<unsigned int>() , "seed for random number")
      ;

        po::variables_map vm;        
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    
		
		if(vm.count("help")) 
		{
			std::cout << "Help asked for..." << std::endl; 
			std::cout << desc << std::endl; 
			retVal = 0;
		}
		else if(vm.count("unittest"))
		{
			retVal = NeuralNetworkApp().runUnitTests();
		}
		else if(vm.count("profile"))
		{
            boost::progress_timer progTimer;
			retVal = NeuralNetworkApp().runForProfiler();
		}
		else if(vm.count("create"))
		{
			if(!vm.count("template"))
			{
				std::cout << "No template argument provided for network creation." << std::endl; 
			}
			else if(!vm.count("network"))
			{
				std::cout << "No network argument provided for network creation." << std::endl; 
			}
			else
			{
                unsigned int randomSeed = 0;
				if(vm.count("seed"))
				{
					randomSeed = vm["seed"].as<unsigned int>();
				}
				
				const std::string templateFilename( vm["template"].as<std::string>() );
				const std::string networkFilename( vm["network"].as<std::string>() );
				
                // hello
				boost::progress_timer progTimer;
				retVal = NeuralNetworkApp().createNetwork( templateFilename, networkFilename, randomSeed );
			}
		}
		else if(vm.count("train"))
		{
			if(!vm.count("input"))
			{
				std::cout << "No input argument provided for training." << std::endl; 
			}
			else if(!vm.count("network"))
			{
				std::cout << "No network argument provided for training." << std::endl; 
			}
			else if(!vm.count("target"))
			{
				std::cout << "No target argument provided for training." << std::endl; 
			}
			else if(!vm.count("error"))
			{
				std::cout << "No error argument provided for training." << std::endl; 
			}
			else if(!vm.count("saveevery"))
			{
				std::cout << "No saveevery argument provided for training." << std::endl; 
			}
			else if(!vm.count("epochsize"))
			{
				std::cout << "No epochsize argument provided for training." << std::endl; 
			}
			else
			{
				bool gnuplot = false;
				if(vm.count("gnuplot"))
				{
					gnuplot = true;
				}
                bool bulkInput = false;
				if(vm.count("bulk"))
				{
					bulkInput = true;
				}
                
				const std::string inputFilename( vm["input"].as<std::string>() );
				const std::string networkFilename( vm["network"].as<std::string>() );
				const std::string targetFilename( vm["target"].as<std::string>() );
				const double errorThreshold = vm["error"].as<double>();
				const unsigned long long saveIteration = vm["saveevery"].as<unsigned long>();
                const unsigned long long epochSize = vm["epochsize"].as<unsigned long>();
                
				boost::progress_timer progTimer;
				retVal = NeuralNetworkApp().trainNetwork( inputFilename, 
														 networkFilename,
														 targetFilename,
														 errorThreshold,
														 saveIteration,
                                                         epochSize,
														 gnuplot,
                                                         bulkInput);
			}
			retVal = 0;
		}
		else if(vm.count("recall"))
		{
			if(!vm.count("input"))
			{
				std::cout << "No input argument provided for recall." << std::endl; 
			}
			else if(!vm.count("network"))
			{
				std::cout << "No network argument provided for recall." << std::endl; 
			}
			else if(!vm.count("output"))
			{
				std::cout << "No output argument provided for recall." << std::endl; 
			}
			else if(!vm.count("epochsize"))
			{
				std::cout << "No epochsize argument provided for recall." << std::endl; 
			}
			else
			{
                bool bulkInput = false;
				if(vm.count("bulk"))
				{
					bulkInput = true;
				}
                std::string targetScaleFilename;
				if(vm.count("target"))
				{
                    targetScaleFilename = vm["target"].as<std::string>();
				}

				const std::string inputFilename( vm["input"].as<std::string>() );
				const std::string networkFilename( vm["network"].as<std::string>() );
				const std::string outputFilename( vm["output"].as<std::string>() );
                const unsigned long long epochSize = vm["epochsize"].as<unsigned long>();

				boost::progress_timer progTimer;
				retVal = NeuralNetworkApp().recallNetwork( inputFilename, 
														  networkFilename,
														  outputFilename,
                                                          epochSize,
                                                          bulkInput,
                                                          targetScaleFilename );
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
}
