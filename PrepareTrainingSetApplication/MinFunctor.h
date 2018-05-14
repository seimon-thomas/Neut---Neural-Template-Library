//
//  MinFunctor.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 28/11/2013.
//
//

#ifndef NeuralNetworkApplication_MinFunctor_h
#define NeuralNetworkApplication_MinFunctor_h

namespace Neural
{
    template <class T> struct MinFunctor
    {
        T operator()(const T& x, const T& y) const
        {
            return !(x<y)?y:x;
        }
    };
    
}


#endif
