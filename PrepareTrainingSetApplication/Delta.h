//
//  Delta.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 27/11/2013.
//
//

#ifndef NeuralNetworkApplication_Delta_h
#define NeuralNetworkApplication_Delta_h

namespace Neural
{
    template <class T> struct delta
    {
        T operator()(const T& x, const T& y) const
        {
            return (x - y)/y;
        }
    };

}

#endif
