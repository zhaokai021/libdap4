
// Implementation for Url.
//
// jhrg 9/7/94

// $Log: Url.cc,v $
// Revision 1.6  1995/02/10 02:22:52  jimg
// Added DBMALLOC includes and switch to code which uses malloc/free.
// Private and protected symbols now start with `_'.
// Added new accessors for name and type fields of BaseType; the old ones
// will be removed in a future release.
// Added the store_val() mfunc. It stores the given value in the object's
// internal buffer.
// Made both List and Str handle their values via pointers to memory.
// Fixed read_val().
// Made serialize/deserialize handle all malloc/free calls (even in those
// cases where xdr initiates the allocation).
// Fixed print_val().
//
// Revision 1.5  1995/01/19  20:05:20  jimg
// ptr_duplicate() mfunc is now abstract virtual.
// Array, ... Grid duplicate mfuncs were modified to take pointers, not
// referenves.
//
// Revision 1.4  1995/01/11  15:54:37  jimg
// Added modifications necessary for BaseType's static XDR pointers. This
// was mostly a name change from xdrin/out to _xdrin/out.
// Removed the two FILE pointers from ctors, since those are now set with
// functions which are friends of BaseType.
//
// Revision 1.3  1994/11/29  20:16:35  jimg
// Added mfunc for data transmission.
// Uses special xdr function for serialization and xdr_coder.
// Removed `type' parameter from ctor.
// Added FILE *in and *out to ctor parameter list.
//
// Revision 1.2  1994/09/23  14:36:15  jimg
// Fixed errors in comments.
//
// Revision 1.1  1994/09/15  21:08:50  jimg
// Added many classes to the BaseType hierarchy - the complete set of types
// described in the DODS API design documet is now represented.
// The parser can parse DDS files.
// Fixed many small problems with BaseType.
// Added CtorType.
//

#ifdef __GNUG__
#pragma implementation
#endif

#include <assert.h>
#include <string.h>

#include "Url.h"
#include "util.h"

Url::Url(const String &n) : BaseType(n, "Url", (xdrproc_t)xdr_str)
{
    _buf = 0;
}

unsigned int
Url::size()
{
    return sizeof(char *);
}

unsigned int
Url::len()
{
    return _buf ? strlen(_buf): 0;
}

bool
Url::serialize(bool flush)
{
    bool stat = (bool)xdr_str(_xdrout, &_buf);
    if (stat && flush)
	stat = expunge();

    return stat;
}

unsigned int
Url::deserialize(bool reuse)
{
    if (_buf && !reuse) {
	free(_buf);
	_buf = 0;
    }

    unsigned int num = xdr_str(_xdrin, &_buf);

    return num;
}

unsigned int
Url::store_val(void *val, bool reuse)
{
    assert(val);

    if (_buf && !reuse) {
	free(_buf);
	_buf = 0;
    }

    if (!_buf) {
	_buf = strdup((char *)val); // allocate new memory
	return strlen(_buf);
    }
    else {
	unsigned int len = strlen(_buf); // _buf might be bigger...
	strncpy(_buf, (char *)val, len);
	*(_buf + len) = '\0';	// strlen won't supply a \0 above
	return len;
    }
}

unsigned int 
Url::read_val(void **val)
{
    assert(_buf && val);

    if (!*val)
	*val = new char[strlen((char *)_buf) + 1];

    strcpy(*(char **)val, _buf); // I'm not sure about this...

    return size();
}

void
Url::print_val(ostream &os, String space, bool print_decl_p)
{
    if (print_decl_p) {
	print_decl(os, space, false);
	os << " = " << _buf << ";" << endl;
    }
    else 
	os << _buf;
}

