
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

#ifndef ddx_exceptions_h
#define ddx_exceptions_h

#ifndef _error_h
#include "Error.h"
#endif

/** Thrown when the DDX response cannot be parsed.. */
class DDXParseFailed :public Error {
public:
    DDXParseFailed() : Error("The DDX response document parse failed.") {}
    DDXParseFailed(const string &msg) : 
	Error(string("The DDX response document parse failed: ") + msg) {}
};

// $Log: DDXExceptions.h,v $
// Revision 1.1  2003/05/28 05:43:57  jimg
// Added.
//

#endif // ddx_exceptions_h