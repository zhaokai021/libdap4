
// Implementation for Array.
//
// jhrg 9/13/94

// $Log: Array.cc,v $
// Revision 1.15  1995/02/10 02:22:54  jimg
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
// Revision 1.14  1995/01/19  20:05:21  jimg
// ptr_duplicate() mfunc is now abstract virtual.
// Array, ... Grid duplicate mfuncs were modified to take pointers, not
// referenves.
//
// Revision 1.13  1995/01/11  15:54:39  jimg
// Added modifications necessary for BaseType's static XDR pointers. This
// was mostly a name change from xdrin/out to _xdrin/out.
// Removed the two FILE pointers from ctors, since those are now set with
// functions which are friends of BaseType.
//
// Revision 1.12  1994/12/19  20:52:45  jimg
// Minor modifications to the print_val mfunc.
//
// Revision 1.11  1994/12/16  20:13:31  dan
// Fixed serialize() and deserialize() for arrays of strings.
//
// Revision 1.10  1994/12/14  20:35:36  dan
// Added dimensions() member function to return number of dimensions
// contained in the array.
// Removed alloc_buf() and free_buf() member functions and placed them
// in util.cc.
//
// Revision 1.9  1994/12/14  17:50:34  dan
// Modified serialize() and deserialize() member functions to special
// case BaseTypes 'Str' and 'Url'.  These special cases do not call
// xdr_array, but iterate through the arrays using calls to XDR_STR.
// Modified print_val() member function to handle arrays of different
// BaseTypes.
// Modified append_dim() member function for initializing its dimension
// components.
// Removed dim() member function.
//
// Revision 1.7  1994/12/09  21:36:33  jimg
// Added support for named array dimensions.
//
// Revision 1.6  1994/12/08  15:51:41  dan
// Modified size() member to return cumulative size of all dimensions
// given the variable basetype.
// Modified serialize() and deserialize() member functions for data
// transmission using XDR.
//
// Revision 1.5  1994/11/22  20:47:45  dan
// Modified size() to return total number of elements.
// Fixed errors in deserialize (multiple returns).
//
// Revision 1.4  1994/11/22  14:05:19  jimg
// Added code for data transmission to parts of the type hierarchy. Not
// complete yet.
// Fixed erros in type hierarchy headers (typos, incorrect comments, ...).
//
// Revision 1.3  1994/10/17  23:34:42  jimg
// Added code to print_decl so that variable declarations are pretty
// printed.
// Added private mfunc duplicate().
// Added ptr_duplicate().
// Added Copy ctor, dtor and operator=.
//
// Revision 1.2  1994/09/23  14:31:36  jimg
// Added check_semantics mfunc.
// Added sanity checking for access to shape list (is it empty?).
// Added cvs log listing to Array.cc.
//

#ifdef __GNUG__
#pragma implementation
#endif

#ifdef DBMALLOC
#include <stdlib.h>
#include <dbmalloc.h>
#endif
#include <assert.h>

#include "Array.h"
#include "util.h"
#include "errmsg.h"
#include "debug.h"

void
Array::_duplicate(const Array *a)
{
    set_name(a->name());
    set_type(a->type());

    _shape = a->_shape;
    _var = a->_var->ptr_duplicate();
}

// Construct an instance of Array. The (BaseType *) is assumed to be
// allocated using new -- The dtor for Array will delete this object.

Array::Array(const String &n, BaseType *v)
     : BaseType( n, "Array", xdr_array), _var(v)
{
    set_name(n);
}

Array::Array(const Array &rhs)
{
    _duplicate(&rhs);
}

Array::~Array()
{
    delete _var;
}

const Array &
Array::operator=(const Array &rhs)
{
    if (this == &rhs)
	return *this;
    
    _duplicate(&rhs);

    return *this;
}

// NAME defaults to NULL. It is present since the definition of this mfunc is
// inherited from CtorType, which declares it like this since some ctor types
// have several member variables.

BaseType *
Array::var(const String &name)
{
    return _var;
}

// Add the BaseType pointer to this ctor type instance. Propagate the name of
// the BaseType instance to this instance This ensures that variables at any
// given level of the DDS table have unique names (i.e., that Arrays do not
// have there default name "").
// NB: Part p defaults to nil for this class

void
Array::add_var(BaseType *v, Part p)
{
    if (_var)
	err_quit("Array::add_var:Attempt to overwrite base type of an array");

    _var = v;
    set_name(v->name());

    DBG(cerr << "Array::add_var: Added variable " << v << " (" \
	 << v->get_var_name() << " " << v->get_var_type() << ")" << endl);
}

unsigned int
Array::size()
{
    return sizeof(void *);
}

// When you want to allocate a buffer big enough to hold the entire array,
// in the local representation, allocate (length() * var()->size()) bytes.

unsigned int
Array::length()
{
    assert(_var);

    unsigned int sz = 1;
    for (Pix p = first_dim(); p; next_dim(p)) 
	sz *= dimension_size(p); 

    return sz;
}

// Serialize an array. This uses the BaseType member XDR_CODER to encode each
// element of the array. See Sun's XDR manual. 
//
// NB: The array must already be in BUF (in the local machine's
// representation) *before* this call is made.
//
// NB: For arrays of strings or urls, serialize will interate through the
// array and call xdr_str for each array element member instead of calling
// xdr_array with xdr_str function element.  

bool
Array::serialize(bool flush)
{
    assert(_buf);

    bool status;
    unsigned int num = length();

    if ( _var->type() == "Str" || _var->type() == "Url" ) {
	for (int i = 0; i < num; ++i) {
	    status = (bool)xdr_str(_xdrout, ((char **)_buf) + i);
	    if (!status) 
		break;
	}
    }
    else {
	status = (bool)xdr_array(_xdrout, (char **)&_buf, &num, 
				 DODS_MAX_ARRAY, _var->size(), 
				 _var->xdr_coder());
    }

    if (status && flush)
	status = expunge();

    return status;
}

// NB: If you do not allocate any memory to BUF *and* ensure that BUF ==
// NULL, then deserialize will allocate the memory for you. However, it will
// do so using malloc so YOU MUST USE FREE, NOT DELETE, TO RELEASE IT. 
// You can avoid all this hassle by using the mfunc alloc_buf and free_buf
// in CtorType. You can use free_buf to free the contents of BUF when they
// were allocated by alloc_buf *or* deserialize (but *not* new).

unsigned int
Array::deserialize(bool reuse)
{
    bool status;
    unsigned int num;

    if (_buf && !reuse) {
	free(_buf);
	_buf = 0;
    }

    if (_var->get_var_type() == "Str" || _var->get_var_type() == "Url") {
	for (int i = 0; i < num; ++i) {
	    status = (bool)xdr_str(_xdrin, (char **)_buf + i);
	    if (!status) 
		break;
	}
    }
    else {
	status = (bool)xdr_array(_xdrin, (char **)&_buf, &num, DODS_MAX_ARRAY, 
				 _var->size(), _var->xdr_coder());
    }

    return status ? num: status;
}

// Assume that VAL points to memory which contains, in row major order,
// enough elements of the correct type to fill the array. They are memcpy'd
// into _buf.

unsigned int
Array::store_val(void *val, bool reuse)
{
    assert(val);

    if (_buf && !reuse) {
	free(_buf);
	_buf = 0;
    }

    unsigned int len = length() * var()->size();
    if (!_buf) {
	_buf = malloc(len);
	memcpy(_buf, val, len);
    }
    else { 
	// here we *assume* the user knows that _buf contains enough
	// memory... Yeah, right.

	memcpy(_buf, val, len);
    }

    return len;
}

unsigned int
Array::read_val(void **val)
{
    assert(_buf && val);

    unsigned int sz = length() * var()->size();
    if (!*val)
	*val = new char[sz];

    (void)memcpy(*val, _buf, sz);

    return sz;
}

// Add the dimension DIM to the list of dimensions for this array. If NAME is
// given, set it to the name of this dimension. NAME defaults to "".
//
// Returns: void

void 
Array::append_dim(int size, String name)
{ 
    dimension d;
    d.size = size;
    d.name = name;
    _shape.append(d); 
}

Pix 
Array::first_dim() 
{ 
    return _shape.first();
}

void 
Array::next_dim(Pix &p) 
{ 
    if (!_shape.empty() && p)
	_shape.next(p); 
}

// Return the number of dimensions contained in the array.

unsigned int
Array::dimensions()
{
  unsigned int dim = 0;
  for( Pix p = first_dim(); p; next_dim(p)) dim ++;

  return dim;
}

// Return the size of the array dimension referred to by P.

int 
Array::dimension_size(Pix p) 
{ 
    if (!_shape.empty() && p)
	return _shape(p).size; 
}

// Return the name of the array dimension referred to by P.

String
Array::dimension_name(Pix p) 
{ 
    if (!_shape.empty() && p)
	return _shape(p).name; 
}

void
Array::print_decl(ostream &os, String space, bool print_semi)
{
    _var->print_decl(os, space, false); // print it, but w/o semicolon

    for (Pix p = _shape.first(); p; _shape.next(p)) {
	os << "[";
	if (_shape(p).name != "")
	    os << _shape(p).name << "=";
	os << _shape(p).size << "]";
    }

    if (print_semi)
	os << ";" << endl;
}

static unsigned int
print_array(ostream &os, BaseType *var, void *array, unsigned int dims, 
	    unsigned int shape[])
{
    if (dims == 1) {
	unsigned int iarray = (unsigned int)array;

	os << "{";
	for (int i = 0; i < shape[0]-1; ++i) {
	    array += var->store_val(array);
	    var->print_val(os, "", false);
	    os << ", ";
	}
	array += var->store_val(array);
	var->print_val(os, "", false);
	os << "}";
	return (unsigned int)array - iarray;
    }
    else {
	os << "{";
	for (int i = 0; i < shape[dims-1]-1; ++i) {
	    array += print_array(os, var, array, dims - 1, shape + 1);
	    os << "},";
	}
	array += print_array(os, var, array, dims - 1, shape + 1);
	os << "}";
    }
}

void 
Array::print_val(ostream &os, String space, bool print_decl_p)
{
    // print the declaration if print decl is true.
    // for each dimension,
    //   for each element, 
    //     call store_val and then call print_val with PRINT_DECL false.
    // Add the `;'
    
    if (print_decl_p) {
	print_decl(os, space, false);
	os << " = ";
    }

    unsigned int dims = dimensions();
    unsigned int shape[dims];
    unsigned int i = 0;
    for (Pix p = first_dim(); p; next_dim(p))
	shape[i++] = dimension_size(p);

    print_array(os, _var, _buf, dims, shape);

    if (print_decl_p) {
	os << ";" << endl;
    }
}

bool
Array::check_semantics(bool all)
{
    bool sem = BaseType::check_semantics() && !_shape.empty();

    if (!sem)
	cerr << "An array variable must have dimensions" << endl;

    return sem;
}
