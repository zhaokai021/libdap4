
// Implementation of the attribute table (AttrTable) class.
//
// jhrg 7/29/94

// $Log: AttrTable.cc,v $
// Revision 1.2  1994/08/02 19:17:37  jimg
// Fixed `$Log$' comments and rcsid[] variables (syntax errors due to //
// comments caused compilation failures.
// das.tab.c and .h are commited now as well.
//
// Revision 1.1  1994/08/02  18:32:04  jimg
// The implementation of AttrTable. This file defined ostream &operator<< and
// a static class variable String empty (it is initialized to "").
//

static char rcsid[]="$Id: AttrTable.cc,v 1.2 1994/08/02 19:17:37 jimg Exp $";

#ifdef __GNUG__
#pragma implementation
#endif
#include "AttrTable.h"

#include <iostream.h>
#include <Pix.h>

String AttrTable::empty = "";	// this must only be defined once.

ostream& operator<< (ostream &os, AttrTable &t)
{
    for(Pix p = t.first(); p; t.next(p))
	os << t.key(p) << " : " << t.contents(p) << endl;
    
    return os;
}
