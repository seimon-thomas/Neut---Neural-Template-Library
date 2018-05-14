//
//  GrowthFunctor.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 10/04/2015.
//
//

#ifndef NeuralNetworkApplication_GrowthFunctor_h
#define NeuralNetworkApplication_GrowthFunctor_h

namespace Neural
{
    template <class T> struct GrowthFunctor
    {
        T operator()(const T& x, const T& y) const
        {
            return ( x - y )/y;
        }
    };
    
}
#endif
