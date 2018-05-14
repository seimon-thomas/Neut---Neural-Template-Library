/*
 *  utils.h
 *  NeuralNetworkApplication
 *
 *  Created by Seimon Thomas on 17/06/2011.
 *  Copyright 2011 Seimon Thomas. All rights reserved.
 *
 */
#ifndef utils_H
#define utils_H

namespace Neural 
{
	namespace utils
	{
		template< typename T, typename C > void push_back( C& container, T value )
		{
			size_t oldSize = container.size();
			
			container.resize( oldSize + 1 );
			container[ oldSize ] = value;
		}
		
		///
		/// This is a conditional push_back - it only pushes back if there isn't enought
		/// capacity. If there is, it is benign.
		///
		/// @param container The container to push into
		/// @param value The value to (conditionally) push back
		/// @param index The index to push into
		///
		template< typename T, typename C > void conditional_push_back( C& container, T value, size_t index )
		{
			if (container.size() <= index ) 
			{
				container.resize( index + 1 );
				container[ index ] = value;
			}
		}
	};
};

#endif