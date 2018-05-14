//
//  MaxFunctor.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 27/11/2013.
//
//

#ifndef NeuralNetworkApplication_MaxFunctor_h
#define NeuralNetworkApplication_MaxFunctor_h

namespace Neural
{
    template <class T> struct MaxFunctor
    {
        T operator()(const T& x, const T& y) const
        {
            return (x<y)?y:x;
        }
    };
    
}

#endif
