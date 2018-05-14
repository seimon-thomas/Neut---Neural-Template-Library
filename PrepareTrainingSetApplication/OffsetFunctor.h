//
//  OffsetFunctor.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 28/11/2013.
//
//

#ifndef NeuralNetworkApplication_OffsetFunctor_h
#define NeuralNetworkApplication_OffsetFunctor_h

namespace Neural
{
    template <class T> struct OffsetFunctor
    {
        T m_Offset;
        OffsetFunctor(T offset):m_Offset(offset){};
        
        T operator()(const T& a) const
        {
            return m_Offset - a;
        }
    };
}

#endif
