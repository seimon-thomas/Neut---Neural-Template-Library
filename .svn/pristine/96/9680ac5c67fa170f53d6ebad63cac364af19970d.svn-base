//
//  ScaleFunctor.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 28/11/2013.
//
//

#ifndef NeuralNetworkApplication_ScaleFunctor_h
#define NeuralNetworkApplication_ScaleFunctor_h

namespace Neural
{
    template <class T> struct ScaleFunctor
    {
        T m_Scale;
        ScaleFunctor(T scale):m_Scale(scale){};
        
        T operator()(const T& a) const
        {
            return m_Scale/a;
        }
    };
    
}

#endif
