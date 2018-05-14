//
//  PrepareTrainingSet.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 19/11/2013.
//
//

#ifndef NeuralNetworkApplication_PrepareTrainingSet_h
#define NeuralNetworkApplication_PrepareTrainingSet_h


#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "Exception.h"
#include "TrainingMatrix.h"

namespace Neural
{
    template<class T> class PrepareTrainingSet
    {
    private:
        const std::string m_inputFileName;
        const std::string m_outputFileName;
        const std::string m_trainingInputsFileName;
        const std::string m_trainingTargetsFileName;
		
        std::ifstream* m_pifStream;
        std::ofstream* m_pofStream;
        std::ofstream* m_pofStreamInputs;
        std::ofstream* m_pofStreamTargets;

        unsigned long m_numInputRows;
        unsigned long m_numTargetRows;
        T m_scale;
        T m_offset;

        
    public:
        PrepareTrainingSet( const std::string& inputFileName,
                            const std::string& outputFileName,
                            const std::string& trainingInputsFileName,
                            const std::string& trainingTargetsFileName,
                            unsigned long numInputRows,
                            unsigned long numTargetRows,
                            T scale,
                            T offset)
            :m_inputFileName( inputFileName ),
             m_outputFileName( outputFileName ),
             m_trainingInputsFileName( trainingInputsFileName ),
             m_trainingTargetsFileName( trainingTargetsFileName ),
             m_numInputRows( numInputRows ),
             m_numTargetRows( numTargetRows ),
             m_scale( scale ),
             m_offset( offset )
        {
            
        }
        
        virtual ~PrepareTrainingSet(){}
        
        void run()
        {
            open();
            
            Neural::TrainingMatrix<double> trainingMatrix;
            
            *m_pifStream >> trainingMatrix;
            
            //trainingMatrix.process(0.7,0.15);
            trainingMatrix.process(m_scale,m_offset);
            
            *m_pofStream << trainingMatrix;
            
            trainingMatrix.trainingInputsToStream( *m_pofStreamInputs, m_numInputRows, m_numTargetRows );
            trainingMatrix.trainingTargetsToStream( *m_pofStreamTargets, m_numInputRows, m_numTargetRows );
            
            close();
        }
    protected:
        
        void open()
        {
            m_pifStream = new std::ifstream( m_inputFileName.c_str() );
			if (!m_pifStream->is_open())
			{
				THROW_EXCEPTION( "Failed to open ifstream");
			}
			
            m_pofStream = new std::ofstream( m_outputFileName.c_str() );
			if (!m_pofStream->is_open())
			{
				THROW_EXCEPTION( "Failed to open ofstream");
			}

            m_pofStreamInputs = new std::ofstream( m_trainingInputsFileName.c_str() );
			if (!m_pofStreamInputs->is_open())
			{
				THROW_EXCEPTION( "Failed to open ofstream for writing network inputs");
			}

            m_pofStreamTargets = new std::ofstream( m_trainingTargetsFileName.c_str() );
			if (!m_pofStreamTargets->is_open())
			{
				THROW_EXCEPTION( "Failed to open ofstream for writing network targets");
			}
        }
        void close()
        {
			if( m_pifStream->is_open() )
			{
				m_pifStream->close();
			}
            delete m_pifStream;

			if( m_pofStream->is_open() )
			{
				m_pofStream->close();
			}
            delete m_pofStream;

			if( m_pofStreamInputs->is_open() )
			{
				m_pofStreamInputs->close();
			}
            delete m_pofStreamInputs;
			
            if( m_pofStreamTargets->is_open() )
			{
				m_pofStreamTargets->close();
			}
            delete m_pofStreamTargets;
        }
        
    };
}


#endif
