
// -*- C++ -*-

// Class for array variables. The dimensions of the array are stored in the
// list SHAPE. 
//
// jhrg 9/6/94

/* $Log: Array.h,v $
/* Revision 1.20  1995/10/23 23:20:46  jimg
/* Added _send_p and _read_p fields (and their accessors) along with the
/* virtual mfuncs set_send_p() and set_read_p().
/*
 * Revision 1.19  1995/08/26  00:31:23  jimg
 * Removed code enclosed in #ifdef NEVER #endif.
 *
 * Revision 1.18  1995/08/22  23:48:12  jimg
 * Removed card() member function.
 * Removed old, deprecated member functions.
 * Changed the names of read_val and store_val to buf2val and val2buf.
 *
 * Revision 1.17  1995/05/10  13:45:03  jimg
 * Changed the name of the configuration header file from `config.h' to
 * `config_dap.h' so that other libraries could have header files which were
 * installed in the DODS include directory without overwriting this one. Each
 * config header should follow the convention config_<name>.h.
 *
 * Revision 1.16  1995/04/28  19:53:45  reza
 * First try at adding constraints capability.
 * Enforce a new size calculated from constraint expression.
 *
 * Revision 1.15  1995/03/16  17:23:00  jimg
 * Added include of config_dap.h before all other includes.
 * Fixed deletes of buffers in read_val().
 * Added initialization of _buf in ctor.
 *
 * Revision 1.14  1995/03/04  14:34:53  jimg
 * Major modifications to the transmission and representation of values:
 * 	Added card() virtual function which is true for classes that
 * 	contain cardinal types (byte, int float, string).
 * 	Changed the representation of Str from the C rep to a C++
 * 	class represenation.
 * 	Chnaged read_val and store_val so that they take and return
 * 	types that are stored by the object (e.g., inthe case of Str
 * 	an URL, read_val returns a C++ String object).
 * 	Modified Array representations so that arrays of card()
 * 	objects are just that - no more storing strings, ... as
 * 	C would store them.
 * 	Arrays of non cardinal types are arrays of the DODS objects (e.g.,
 * 	an array of a structure is represented as an array of Structure
 * 	objects).
 *
 * Revision 1.13  1995/02/10  02:22:53  jimg
 * Added DBMALLOC includes and switch to code which uses malloc/free.
 * Private and protected symbols now start with `_'.
 * Added new accessors for name and type fields of BaseType; the old ones
 * will be removed in a future release.
 * Added the store_val() mfunc. It stores the given value in the object's
 * internal buffer.
 * Made both List and Str handle their values via pointers to memory.
 * Fixed read_val().
 * Made serialize/deserialize handle all malloc/free calls (even in those
 * cases where xdr initiates the allocation).
 * Fixed print_val().
 *
 * Revision 1.12  1995/01/19  21:59:08  jimg
 * Added read_val from dummy_read.cc to the sample set of sub-class
 * implementations.
 * Changed the declaration of readVal in BaseType so that it names the
 * mfunc read_val (to be consistant with the other mfunc names).
 * Removed the unnecessary duplicate declaration of the abstract virtual
 * mfuncs read and (now) read_val from the classes Byte, ... Grid. The
 * declaration in BaseType is sufficient along with the decl and definition
 * in the *.cc,h files which contain the subclasses for Byte, ..., Grid.
 *
 * Revision 1.11  1995/01/18  18:37:54  dan
 * Declared member function 'readVal', defined in dummy_read.cc
 *
 * Revision 1.10  1995/01/11  15:54:40  jimg
 * Added modifications necessary for BaseType's static XDR pointers. This
 * was mostly a name change from xdrin/out to _xdrin/out.
 * Removed the two FILE pointers from ctors, since those are now set with
 * functions which are friends of BaseType.
 *
 * Revision 1.9  1994/12/14  20:36:58  dan
 * Added dimensions() member function definition.
 * Removed alloc_buf() and free_buf() definitions.
 *
 * Revision 1.8  1994/12/14  17:55:23  dan
 * Removed dim() member function.
 *
 * Revision 1.7  1994/12/12  20:33:34  jimg
 * Fixed struct dimensions.
 *
 * Revision 1.6  1994/12/12  19:40:30  dan
 * Modified Array class definition removing inheritance from class CtorType
 * and to inherit directly from class BaseType.  Removed member function
 * dimensions().
 *
 * Revision 1.5  1994/12/09  21:36:34  jimg
 * Added support for named array dimensions.
 *
 * Revision 1.4  1994/11/22  14:05:22  jimg
 * Added code for data transmission to parts of the type hierarchy. Not
 * complete yet.
 * Fixed erros in type hierarchy headers (typos, incorrect comments, ...).
 *
 * Revision 1.3  1994/10/17  23:34:43  jimg
 * Added code to print_decl so that variable declarations are pretty
 * printed.
 * Added private mfunc duplicate().
 * Added ptr_duplicate().
 * Added Copy ctor, dtor and operator=.
 *
 * Revision 1.2  1994/09/23  14:31:37  jimg
 * Added check_semantics mfunc.
 * Added sanity checking for access to shape list (is it empty?).
 * Added cvs log listing to Array.cc.
 */

#ifndef _Array_h
#define _Array_h 1

#ifdef __GNUG__
#pragma interface
#endif

#include <limits.h>
#include <SLList.h>
#include "BaseType.h"
#include "BaseTypeVec.h"

const int DODS_MAX_ARRAY = UINT_MAX;

class Array: public BaseType {
private:
    struct dimension {		// each dimension has a size and a name
	int size;
	String name;
    };

    BaseType *_var;		// var that is an array. I.E., if this is an
				// array of Int32, _var -> an instance of Int32
    SLList<dimension> _shape;	// list of dimensions (i.e., the shape)

    long _const_length;		// Length after constraint 

    void *_buf;			// used for arrays of cardinal types
    BaseTypePtrVec _vec;	// used for arrays of all other types

    void _duplicate(const Array &a);

public:
    Array(const String &n = (char *)0, BaseType *v = 0);
    Array(const Array &rhs);
    virtual ~Array();

    const Array &operator=(const Array &rhs);
    virtual BaseType *ptr_duplicate() = 0; 

    virtual void set_send_p(bool state);
    virtual void set_read_p(bool state);

    virtual unsigned int width(); // bytes in the pointer to the array
    unsigned int length();	// how many elements are there in the array
    void const_length(long size);

    virtual bool serialize(bool flush = false);
    virtual bool deserialize(bool reuse = false);

    virtual bool read(String dataset, String var_name, String constraint) = 0;

    virtual unsigned int store_val(void *val, bool reuse = false); // dep.
    virtual unsigned int val2buf(void *val, bool reuse = false);
    virtual unsigned int read_val(void **val); // deprecated name
    virtual unsigned int buf2val(void **val);

    void set_vec(int i, BaseType *val);
    BaseType *vec(int i);

    virtual BaseType *var(const String &name = (char *)0);
    virtual void add_var(BaseType *v, Part p = nil);

    void append_dim(int size, String name = "");

    Pix first_dim();
    void next_dim(Pix &p);
    int dimension_size(Pix p);
    String dimension_name(Pix p);
    unsigned int dimensions();

    virtual void print_decl(ostream &os, String space = "    ",
			    bool print_semi = true,
			    bool constraint_info = false);
    virtual void print_val(ostream &os, String space = "", 
			   bool print_decl_p = true);

    virtual bool check_semantics(bool all = false);
};

#endif


