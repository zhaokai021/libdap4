// -*- C++ -*-

// (c) COPYRIGHT URI/MIT 1999
// Please read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//      jhrg,jimg       James Gallagher (jgallagher@gso.uri.edu)

// Interface for the UInt32 (unsigned int 32) type. 
//
// jhrg 9/7/94

#ifndef _uint32_h
#define _uint32_h 1

#ifdef __GNUG__
#pragma interface
#endif

#if 0

#ifdef WIN32
#include <rpc.h>
#include <winsock.h>
#include <xdr.h>
#endif

#include <rpc/types.h>
#include <netinet/in.h>
#include <rpc/xdr.h>
#endif

#ifndef _dods_datatypes_h
#include "dods-datatypes.h"
#endif

#ifndef _basetype_h
#include "BaseType.h"
#endif

/** @brief Holds a 32-bit unsigned integer. 

    @see BaseType */

class UInt32: public BaseType {
    /** This class allows Byte, ..., Float64 access to <tt>_buf</tt> to 
	simplify and speed up the relational operators.

	NB: According to Stroustrup it does not matter where (public, private
	or protected) friend classes are declared. */
    friend class Byte;
    friend class Int16;
    friend class UInt16;
    friend class Int32;
    friend class Float32;
    friend class Float64;

protected:
    dods_uint32 _buf;

public:
    UInt32(const string &n = "");
    virtual ~UInt32() {}

    UInt32(const UInt32 &copy_from);

    UInt32 &operator=(const UInt32 &rhs);

    virtual BaseType *ptr_duplicate() ;
    
    virtual unsigned int width();

    virtual bool serialize(const string &dataset, DDS &dds, XDR *sink,
			   bool ce_eval = true);
    virtual bool deserialize(XDR *source, DDS *dds, bool reuse = false);

    virtual unsigned int val2buf(void *buf, bool reuse = false);
    virtual unsigned int buf2val(void **val);

    virtual void print_val(ostream &os, string space = "",
			   bool print_decl_p = true);

    virtual void print_val(FILE *out, string space = "",
			   bool print_decl_p = true);

    virtual bool ops(BaseType *b, int op, const string &dataset);
};

// $Log: UInt32.h,v $
// Revision 1.15  2003/01/10 19:46:40  jimg
// Merged with code tagged release-3-2-10 on the release-3-2 branch. In many
// cases files were added on that branch (so they appear on the trunk for
// the first time).
//
// Revision 1.12.4.3  2002/12/17 22:35:03  pwest
// Added and updated methods using stdio. Deprecated methods using iostream.
//
// Revision 1.14  2002/06/18 15:36:24  tom
// Moved comments and edited to accommodate doxygen documentation-generator.
//
// Revision 1.12.4.2  2002/05/22 16:57:51  jimg
// I modified the `data type classes' so that they do not need to be
// subclassed for clients. It might be the case that, for a complex client,
// subclassing is still the best way to go, but you're not required to do
// it anymore.
//
// Revision 1.13  2001/08/24 17:46:22  jimg
// Resolved conflicts from the merge of release 3.2.6
//
// Revision 1.12.4.1  2001/07/28 01:10:42  jimg
// Some of the numeric type classes did not have copy ctors or operator=.
// I added those where they were needed.
// In every place where delete (or delete []) was called, I set the pointer
// just deleted to zero. Thus if for some reason delete is called again
// before new memory is allocated there won't be a mysterious crash. This is
// just good form when using delete.
// I added calls to www2id and id2www where appropriate. The DAP now handles
// making sure that names are escaped and unescaped as needed. Connect is
// set to handle CEs that contain names as they are in the dataset (see the
// comments/Log there). Servers should not handle escaping or unescaping
// characters on their own.
//
// Revision 1.12  2000/09/22 02:17:21  jimg
// Rearranged source files so that the CVS logs appear at the end rather than
// the start. Also made the ifdef guard symbols use the same naming scheme and
// wrapped headers included in other headers in those guard symbols (to cut
// down on extraneous file processing - See Lakos).
//
// Revision 1.11  2000/09/21 16:22:09  jimg
// Merged changes from Jose Garcia that add exceptions to the software.
// Many methods that returned error codes now throw exectptions. There are
// two classes which are thrown by the software, Error and InternalErr.
// InternalErr is used to report errors within the library or errors using
// the library. Error is used to reprot all other errors. Since InternalErr
// is a subclass of Error, programs need only to catch Error.
//
// Revision 1.10  2000/08/02 22:46:49  jimg
// Merged 3.1.8
//
// Revision 1.7.6.2  2000/08/02 21:10:08  jimg
// Removed the header config_dap.h. If this file uses the dods typedefs for
// cardinal datatypes, then it gets those definitions from the header
// dods-datatypes.h.
//
// Revision 1.9  2000/07/09 21:57:10  rmorris
// Mods's to increase portability, minimuze ifdef's in win32 and account
// for differences between the Standard C++ Library - most notably, the
// iostream's.
//
// Revision 1.8  2000/06/07 18:06:59  jimg
// Merged the pc port branch
//
// Revision 1.7.6.1  1999/09/11 04:51:38  tom
// corrected and added (minute) documentation
//
// Revision 1.7.20.1  2000/06/02 18:29:32  rmorris
// Mod's for port to Win32.
//
// Revision 1.7.14.1  2000/01/28 22:14:06  jgarcia
// Added exception handling and modify add_var to get a copy of the object
//
// Revision 1.7  1999/04/29 02:29:34  jimg
// Merge of no-gnu branch
//
// Revision 1.6  1999/03/24 23:35:33  jimg
// Added support for the new Int16, UInt16 and Float32 types.
//
// Revision 1.5.6.1  1999/02/02 21:57:04  jimg
// String to string version
//
// Revision 1.5  1997/12/18 15:06:14  tom
// First draft of class documentation, entered in doc++ format,
// in the comments
//
// Revision 1.4  1997/09/22 22:39:54  jimg
// Added DDS * to deserialize parameters.
//
// Revision 1.3  1996/12/02 23:10:32  jimg
// Added dataset as a parameter to the ops member function.
//
// Revision 1.2  1996/10/18 16:53:36  jimg
// Fixed definition of dtor (was Int32 instead of UInt32).
//
// Revision 1.1  1996/08/26 19:40:40  jimg
// Added.
//

#endif // _uint32_h

