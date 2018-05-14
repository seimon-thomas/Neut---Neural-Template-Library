///////////////////////////////////////////////////////////////////////////////////////////////////
//
// NeuralNetworkApp.h
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NeuralNetworkApp_H
#define NeuralNetworkApp_H

////////////////////////////////////////////////////////////////////////////////
// INCLUDED HEADERS
////////////////////////////////////////////////////////////////////////////////



namespace Neural 
{
	class NeuralNetworkApp
	{
	public:
		NeuralNetworkApp();
		
		int  runUnitTests();
        
        int  runForProfiler();
		
		int  createNetwork( const std::string& templateFilename, 
                            const std::string& networkFilename,
                            unsigned int randomSeed );
		
		int  trainNetwork(const std::string& inputFilename, 
                          const std::string& networkFilename, 
                          const std::string& targetFilename, 
                          double errorThreshold, 
                          unsigned long long saveInterval,
                          unsigned long long epochSize,
                          bool gnuplot,
                          bool bulkInput);
								 
		int  recallNetwork(const std::string& inputFilename, 
                           const std::string& networkFilename, 
                           const std::string& outputFilename,
                           unsigned long long epochSize,
                           bool bulkInput,
                           const std::string& targetScaleFilename );
        
        static void ctrlCHandler( int param );
		
        enum 
		{
			PLOT_BUF_SIZE=100
		};
		
		virtual ~NeuralNetworkApp(){};
		
	private:
        static bool m_Finish;
        void registerCtrlCHandler();

		unsigned long long getLinesInFile( const std::string& filename );
		void gnuPlotRMSError( const std::vector< double >& rmsErrors, unsigned long iterations );
		
		
		std::vector< std::pair< unsigned long, std::vector< double > > > m_PlotBuffer;
	};
};

#endif