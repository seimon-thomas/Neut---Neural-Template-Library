////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2010-2018 Seimon Thomas
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
////////////////////////////////////////////////////////////////////////////////////////////////////
//  Exception.h
//  NeuralNetworkApplication
//
//  Created by Seimon Thomas on 26/10/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NeuralNetworkApplication_Exception_h
#define NeuralNetworkApplication_Exception_h

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <exception>
#include <exception>

#define MAKE__EXEPTION_CLASS( ExceptionClassName, BaseExceptionClassName )                          \
class ExceptionClassName: public BaseExceptionClassName                                                \
{                                                                                                      \
public: ExceptionClassName( const char *const& what ): BaseExceptionClassName( what ){};        \
};

namespace Neural 
{
MAKE__EXEPTION_CLASS( Exception, std::runtime_error )
MAKE__EXEPTION_CLASS( ExceptionMethodNotImplimented, Exception )
MAKE__EXEPTION_CLASS( UnknownException, Exception )
MAKE__EXEPTION_CLASS( TestException, Exception )
};

#define THROW_EXCEPTION( MessageStream )                                                             \
{                                                                                                     \
std::stringstream messageStrStream;                                                             \
messageStrStream  << "Exception: \"" << MessageStream << "\" raised in file: \"" << __FILE__ << "\" at line  (" << __LINE__ << ")"<< std::endl;     \
throw Neural::Exception( messageStrStream.str().c_str() );                                           \
}

#define THROW_TYPED_EXCEPTION( ExceptionClassName, MessageStream )                                  \
{                                                                                                     \
std::stringstream messageStrStream;                                                             \
messageStrStream  << "\"" << MessageStream << "\" raised in file: \"" << __FILE__ << "\" at line  (" << __LINE__ << ")"<< std::endl; \
throw ExceptionClassName(  messageStrStream.str().c_str() );                                    \
}

#endif
