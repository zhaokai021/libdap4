
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
//         Dan Holloway <dan@hollywood.gso.uri.edu>
//         Reza Nekovei <reza@intcomm.net>
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
 
// (c) COPYRIGHT URI/MIT 1994-1999,2001,2002
// Please first read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//	jhrg,jimg	James Gallagher <jgallagher@gso.uri.edu>
//	dan		Dan Holloway <dholloway@gso.uri.edu>
//	reza		Reza Nekovei <rnekovei@ieee.org>

// Connect objects are used as containers for information pertaining to a
// connection that a user program makes to a dataset. The dataset may be
// either local (i.e., a file on the user's own computer) or a remote
// dataset. In the later case a DAP2 URL will be used to reference the
// dataset. 
//
// Connect contains methods which can be used to read the DOS DAS and DDS
// objects from the remote dataset as well as reading reading data. The class
// understands in a rudimentary way how DAP2 constraint expressions are
// formed and how to manage the CEs generated by a API to request specific
// variables with the URL initially presented to the class when the object
// was instantiated.
//
// Connect also provides additional services such as error processing.
//
// Connect is not intended for use on the server-side.
//
// jhrg 9/29/94

#ifndef _connect_h
#define _connect_h


#include <stdio.h>

#include <string>

#ifndef _das_h
#include "DAS.h"
#endif

#ifndef _dds_h
#include "DDS.h"
#endif

#ifndef _error_h
#include "Error.h"
#endif

#ifndef _util_h
#include "util.h"
#endif

#ifndef _datadds_h
#include "DataDDS.h"
#endif

#ifndef _httpconnect_h
#include "HTTPConnect.h"
#endif

#ifndef response_h
#include "Response.h"
#endif

using std::string;

/** Connect objects are used as containers for information pertaining
    to the connection a user program makes to a dataset. The
    dataset may be either local (for example, a file on the user's own
    computer) or a remote dataset. In the latter case a DAP2 URL will
    be used to reference the dataset, instead of a filename.

    Connect contains methods which can be used to read the DAP2 DAS and
    DDS objects from the remote dataset as well as reading 
    data. The class understands in a rudimentary way how DAP2
    constraint expressions are formed and how to manage them.

    Connect also provides additional services such as automatic
    decompression of compressed data, transmission progress reports
    and error processing.  Refer to the Gui and Error classes for more
    information about these features. See the DODSFilter class for
    information on servers that compress data.

    @note Update: I removed the DEFAULT_BASETYPE_FACTORY switch because it
    caused more confusion than it avoided. See Trac #130.

    @note The compile-time symbol DEFAULT_BASETYPE_FACTORY controls whether
    the old (3.4 and earlier) DDS and DataDDS constructors are supported.
    These constructors now use a default factory class (BaseTypeFactory,
    implemented by this library) to instantiate Byte, ..., Grid variables. To
    use the default ctor in your code you must also define this symbol. If
    you \e do choose to define this and fail to provide a specialization of
    BaseTypeFactory when your software needs one, you code may not link or
    may fail at run time. In addition to the older ctors for DDS and DataDDS,
    defining the symbol also makes some of the older methods in Connect
    available (because those methods require the older DDS and DataDDS ctors.

    @brief Holds information about the link from a DAP2 client to a
    dataset.
    @see DDS
    @see DAS
    @see DODSFilter
    @see Error
    @author jhrg */

class Connect {
private:
    bool _local;		// Is this a local connection?

    HTTPConnect *d_http;
#if 0
    // #ifdef DEFAULT_BASETYPE_FACTORY
    // *** These are used by deprecated methods only!
    DAS _das;			// Dataset attribute structure
    DDS _dds;			// Dataset descriptor structure
    // #endif

    Error _error;		// Error object
#endif
    string _URL;		// URL to remote dataset (minus CE)
    string _proj;		// Projection part of initial CE.
    string _sel;		// Selection of initial CE

    string d_version;           // Server implementation information
    string d_protocol;          // DAP protocol from the server

    void process_data(DataDDS &data, Response *rs) 
	throw(Error, InternalErr);
    
    // Use when you cannot use libwww/libcurl. Reads HTTP response. 
    void parse_mime(Response *rs);

protected:
    /** @name Suppress the C++ defaults for these. */
    //@{
    Connect() { }
    Connect(const Connect &) { }
    Connect &operator=(const Connect &) {
	throw InternalErr(__FILE__, __LINE__, "Unimplemented assignment");
    }
    //@}

public:
    // This ctor is deprecated. See Connect.cc 
#if 0
    Connect(const string &name, bool www_verbose_errors,
	    bool accept_deflate, string uname = "",
	    string password = "") throw (Error, InternalErr); 
#endif
    Connect(const string &name, string uname = "", string password = "") 
	throw (Error, InternalErr); 

    virtual ~Connect();

    bool is_local();

    // *** Add get_* versions of accessors. 02/27/03 jhrg
    virtual string URL(bool CE = true);
    virtual string CE();
    
    void set_credentials(string u, string p);
    void set_accept_deflate(bool deflate);

    void set_cache_enabled(bool enabled);
    bool is_cache_enabled();

    /** Return the protocol/implementation version of the most recent
	response. This is a poorly designed method, but it returns
	information that is useful when used correctly. Before a response is
	made, this contains the string "unknown." This should ultimately hold
	the \e protocol version; it currently holds the \e implementation
	version. 
        
        @see get_protocol()
        @deprecated */
    string get_version() { return d_version; }

    /** Return the DAP protocol version of the most recent
        response. Before a response is made, this contains the string "2.0."
        */
    string get_protocol() { return d_protocol; }

    
    virtual string request_version() throw(Error, InternalErr);
    virtual string request_protocol() throw(Error, InternalErr);

    virtual void request_das(DAS &das) throw(Error, InternalErr);
    virtual void request_das_url(DAS &das) throw(Error, InternalErr);

    virtual void request_dds(DDS &dds, string expr = "") 
	throw(Error, InternalErr);
    virtual void request_dds_url(DDS &dds) 
	throw(Error, InternalErr);

    virtual void request_data(DataDDS &data, string expr = "") 
	throw(Error, InternalErr);
    virtual void request_data_url(DataDDS &data) 
	throw(Error, InternalErr);

    // *** The Response breaks this, replace with a FileConnect
#if 0
    virtual void read_data(DataDDS &data, FILE *data_source) 
	throw(Error, InternalErr);
#endif
    virtual void read_data(DataDDS &data, Response *rs)
        throw(Error, InternalErr);

#if 0
    /** @deprecated The GUI is no longer supported. This always returns null.
	*/
    void *gui() {
	return 0;
    }

    // #ifdef DEFAULT_BASETYPE_FACTORY
    bool request_dds(bool, const string &) throw(Error, InternalErr) {
	request_dds(_dds, "");
	return true;
    }

    DDS *request_data(string expr, bool, bool, const string &) 
        throw(Error, InternalErr) {
	DataDDS *new_dds = new DataDDS("received_data");
	request_data(*new_dds, expr);
	return new_dds;
    }

    bool request_das(bool,  const string &) throw(Error, InternalErr) {
	request_das(_das);
	return true;
    }

    DDS *read_data(FILE *data_source, bool, bool) throw(Error, InternalErr) {
	DataDDS *data = new DataDDS;
	read_data(*data, data_source);
	return data;
    }

    // These are deprecated.
    DAS &das();
    DDS &dds();
    Error &error();
    // #endif
#endif
};

/* 
 * $Log: Connect.h,v $
 * Revision 1.69  2005/03/30 21:24:40  jimg
 * Added DEFAULT_BASETYPE_FACTORY define; use this to control whether
 * the DDS objects suppy the BaseTypeFactory by default.
 *
 * Revision 1.68  2005/01/28 17:25:12  jimg
 * Resolved conflicts from merge with release-3-4-9
 *
 * Revision 1.65.2.6  2005/01/18 23:02:05  jimg
 * FIxed documentation.
 *
 * Revision 1.67  2004/07/07 21:08:47  jimg
 * Merged with release-3-4-8FCS
 *
 * Revision 1.65.2.5  2004/07/02 20:41:51  jimg
 * Removed (commented) the pragma interface/implementation lines. See
 * the ChangeLog for more details. This fixes a build problem on HP/UX.
 *
 * Revision 1.66  2003/12/08 18:02:29  edavis
 * Merge release-3-4 into trunk
 *
 * Revision 1.65.2.4  2003/11/19 18:47:12  jimg
 * Added request_version() which is a way to get _just_ the version information
 * from a server.
 *
 * Revision 1.65.2.3  2003/09/06 22:37:50  jimg
 * Updated the documentation.
 *
 * Revision 1.65.2.2  2003/06/23 11:49:18  rmorris
 * The // #pragma interface directive to GCC makes the dynamic typing functionality
 * go completely haywire under OS X on the PowerPC.  We can't use that directive
 * on that platform and it was ifdef'd out for that case.
 *
 * Revision 1.65.2.1  2003/05/06 22:06:42  jimg
 * I added a new constructor to Connect and deprecated the old ctor. The
 * old ctor was forcing compression to be on, ignoring the value of DEFLATE
 * in the .dodsrc file. This was left over behavior from before we started
 * using that file. However, now HTTPConnect (and later other protocol
 * modules?) look at the value in the .dodsrc (using RCReader) and decide
 * what action to take.
 *
 * Revision 1.65  2003/04/22 19:40:27  jimg
 * Merged with 3.3.1.
 *
 * Revision 1.64  2003/03/04 21:44:03  jimg
 * Removed code in #if 0 ... #endif. Added d_version and a get_version() method.
 * This is useful for some clients, like geturl.
 *
 * Revision 1.63  2003/03/04 17:34:48  jimg
 * Modified to use Response objects. Removed many old methods which no longer
 * have any meaning (i.e., they were hold overs from several years ago).
 *
 * Revision 1.62  2003/02/27 23:24:52  jimg
 * Moved the empty constructor, et c., from private to protected so that
 * children can define their own empty ctors to suppress the C++ default
 * versions. Removed some old code and reorganized the declarations. There are a
 * lot of methods here that should be removed...
 *
 * Revision 1.61  2003/02/21 00:14:24  jimg
 * Repaired copyright.
 *
 * Revision 1.60.2.1  2003/02/21 00:10:07  jimg
 * Repaired copyright.
 *
 * Revision 1.60  2003/01/23 00:22:23  jimg
 * Updated the copyright notice; this implementation of the DAP is
 * copyrighted by OPeNDAP, Inc.
 *
 * Revision 1.59  2003/01/10 19:46:40  jimg
 * Merged with code tagged release-3-2-10 on the release-3-2 branch. In many
 * cases files were added on that branch (so they appear on the trunk for
 * the first time).
 *
 * Revision 1.49.4.19  2002/12/24 00:18:06  jimg
 * Made output(), close_output(), source() and close_source() private. These
 * four methods made up a deprecated interface to data. All data accessed
 * using this class is now read directly from a returned DataDDS instance.
 * The request_data() and request_dds() methods now have a constraint
 * expression parameter that defaults to the empty string.
 *
 * Revision 1.49.4.18  2002/12/05 20:36:19  pwest
 * Corrected problems with IteratorAdapter code, making methods non-inline,
 * creating source files and template instantiation file. Cleaned up file
 * descriptors and memory management problems. Corrected problem in Connect
 * where the xdr source was not being cleaned up or a new one created when a
 * new file was opened for reading.
 *
 * Revision 1.49.4.17  2002/09/13 16:12:21  jimg
 * Added guards for our header includes.
 *
 * Revision 1.49.4.16  2002/08/08 06:54:56  jimg
 * Changes for thread-safety. In many cases I found ugly places at the
 * tops of files while looking for globals, et c., and I fixed them up
 * (hopefully making them easier to read, ...). Only the files RCReader.cc
 * and usage.cc actually use pthreads synchronization functions. In other
 * cases I removed static objects where they were used for supposed
 * improvements in efficiency which had never actually been verifiied (and
 * which looked dubious).
 *
 * Revision 1.49.4.15  2002/07/06 19:36:32  jimg
 * Added throw(...) specification to Connect's ctor.
 *
 * Revision 1.49.4.14  2002/06/21 22:23:07  jimg
 * I revised the basic interface to this class. The old interface is implemented
 * using the new one. Methods to get objects from DAP2 objects now take
 * references to the containers for those objects as formal parameters. This
 * enables a user of Connect to control how/when/where those containers are
 * created and managed.
 *
 * Revision 1.49.4.13  2002/06/20 23:59:37  jimg
 * I added a new method to access the DDS. This represents the first
 * substantive change to Connect's interface in a long time. The new method
 * is request_dds(DDS &dds, string expr, const string &ext = "dds"). Use this
 * method to have Connect read a DDS for you into an instance of DDS your
 * code has allocated/created. This makes Connect much more flexible since
 * your program (e.g., a subclass of Connect) can have many DDS objects, not
 * just the one that Connect holds internally, without duplicating the code
 * that reads URLs (which would have become a major problem when we start
 * supporting access by other protocols). This is far from complete, but just
 * this one method makes it possible to rewrite nce-dods/NCConnect so that
 * it'll work with the new Connect that uses libcurl (through an instance of
 * HTTPConnect).
 *
 * Revision 1.49.4.12  2002/06/20 06:22:13  jimg
 * Put the local FILE * member back in the class. Local access should now work
 * as before.
 *
 * Revision 1.49.4.11  2002/06/20 03:18:48  jimg
 * Fixes and modifications to the Connect and HTTPConnect classes. Neither
 * of these two classes is complete, but they should compile and their
 * basic functions should work.
 *
 * Revision 1.49.4.10  2002/06/18 23:04:33  jimg
 * Removed the old libwww includes.
 *
 * Revision 1.49.4.9  2002/06/18 21:55:03  jimg
 * Partially reworked Connect interface. I've removed all the explicit
 * references to HTTP.
 *
 * Revision 1.58  2002/06/18 15:36:24  tom
 * Moved comments and edited to accommodate doxygen documentation-generator.
 *
 * Revision 1.57  2002/06/03 22:21:15  jimg
 * Merged with release-3-2-9
 *
 * Revision 1.49.4.8  2002/05/27 00:49:33  jimg
 * Removed gui Progress indicator stuff. The methods that took a bool to
 * swtich on/off the PI now ignore that bool (request_das, request_dds,
 * request_data and read_data). In addition, I made the methods that took
 * a bool to switch on/off asynchornous I/O explicitly ignore that param
 * as well since it's never actually used by the code.
 *
 * Revision 1.49.4.7  2002/01/17 00:42:03  jimg
 * I added a new method to disable use of the cache. This provides a way
 * for a client to suppress use of the cache even if the user wants it
 * (or doesn't say they don't want it).
 *
 * Revision 1.56  2001/10/14 01:28:38  jimg
 * Merged with release-3-2-8.
 *
 * Revision 1.49.4.6  2001/10/08 17:15:30  jimg
 * Added throw declarations to methods that read data from servers.
 *
 * Revision 1.55  2001/08/27 16:38:34  jimg
 * Merged with release-3-2-6
 *
 * Revision 1.49.4.5  2001/07/28 01:10:41  jimg
 * Some of the numeric type classes did not have copy ctors or operator=.
 * I added those where they were needed.
 * In every place where delete (or delete []) was called, I set the pointer
 * just deleted to zero. Thus if for some reason delete is called again
 * before new memory is allocated there won't be a mysterious crash. This is
 * just good form when using delete.
 * I added calls to www2id and id2www where appropriate. The DAP now handles
 * making sure that names are escaped and unescaped as needed. Connect is
 * set to handle CEs that contain names as they are in the dataset (see the
 * comments/Log there). Servers should not handle escaping or unescaping
 * characters on their own.
 *
 * Revision 1.54  2001/06/15 23:49:01  jimg
 * Merged with release-3-2-4.
 *
 * Revision 1.49.4.4  2001/05/03 19:00:15  jimg
 * Added the _always_validate field.
 *
 * Revision 1.49.4.3  2001/04/16 17:10:00  jimg
 * Added a companion field void *_gui for the field Gui *_gui. The former is
 * used when `GUI' is not defined. This makes sure (assuming that the size of
 * the two pointers is identical) that code which uses libdap++-gui.a gets the
 * correct definition of Connect if it does not define GUI itself. Note that the
 * whole `defining GUI' thing is a hack which should go away during some future
 * refactoring of Connect so I'd rather patch around the problem like this than
 * say every client must define GUI or create two include files.
 *
 * Revision 1.49.4.2  2001/02/14 00:10:05  jimg
 * Merged code from the trunk's HEAD revision for this/these files onto
 * the release-3-2 branch. This moves the authentication software onto the
 * release-3-2 branch so that it will be easier to get it in the 3.2 release.
 *
 * Revision 1.53  2001/02/12 23:16:32  jimg
 * Fixed an error with the using statements. `using std::vector<string>;' makes
 * g++ barf, but it like using std::string; and using std::vector;.
 *
 * Revision 1.52  2001/02/09 22:25:14  jimg
 * Added extract_auth_info() method to parse username/password information from a
 * URL and record it in this object.
 * Moved set_credentials() method from the header, where it was inline, to the
 * .cc file. Save inlines for performance optimizations.
 * Removed set_username_password() function since dods_username_password() was
 * modified to do its job.
 * Added process_www_errors() to streamline error reporting for both the popup
 * and non-popup version of this class. This friend function is called directly
 * from Connect's methods, not via a libwww callback. This was necessary because
 * it throws Error objects and throws don't (seem to, anyway) work from within
 * callbacks.
 *
 * Revision 1.51  2001/02/05 18:59:31  jgarcia
 * Added support so a Connect object can be created with credentials to be
 * able to resolve challenges issued by web servers (Basic Authentication).
 * Added exception to notify "No Authorization".
 *
 * Revision 1.50  2001/01/26 19:48:09  jimg
 * Merged with release-3-2-3.
 *
 * Revision 1.49.4.1  2000/11/22 05:35:09  brent
 * allow username/password in URL for secure data sets
 *
 * Revision 1.49  2000/09/22 02:17:19  jimg
 * Rearranged source files so that the CVS logs appear at the end rather than
 * the start. Also made the ifdef guard symbols use the same naming scheme and
 * wrapped headers included in other headers in those guard symbols (to cut
 * down on extraneous file processing - See Lakos).
 *
 * Revision 1.48  2000/08/02 22:46:48  jimg
 * Merged 3.1.8
 *
 * Revision 1.38.4.4  2000/08/02 21:10:07  jimg
 * Removed the header config.h. If this file uses the dods typedefs for
 * cardinal datatypes, then it gets those definitions from the header
 * dods-datatypes.h.
 *
 * Revision 1.47  2000/07/26 12:24:01  rmorris
 * Modified intermediate (dod*) file removal under win32 to take into account
 * a 1-to-n correspondence between connect objects and intermediate files.
 * Implemented solution through vector of strings containing the intermediate
 * filenames that are removed when the connect obj's destructor is invoked.
 * Might consider using the same code for unix in the future.  Previous
 * win32 solution incorrectly assumed the correspondence was 1-to-1.
 *
 * Revision 1.46  2000/07/13 07:07:13  rmorris
 * Mod to keep the intermediate file name in the Connect object in the
 * case of win32 (unlink() works differently on win32,  needed another approach).
 *
 * Revision 1.45  2000/07/09 21:57:09  rmorris
 * Mods's to increase portability, minimuze ifdef's in win32 and account
 * for differences between the Standard C++ Library - most notably, the
 * iostream's.
 *
 * Revision 1.44  2000/06/07 18:06:58  jimg
 * Merged the pc port branch
 *
 * Revision 1.43.4.1  2000/06/02 18:16:47  rmorris
 * Mod's for port to Win32.
 *
 * Revision 1.43  2000/04/07 00:19:04  jimg
 * Merged Brent's changes for the progress gui - he added a cancel button.
 * Also repaired the last of the #ifdef Gui bugs so that we can build Gui
 * and non-gui versions of the library that use one set of header files.
 *
 * Revision 1.42  1999/12/15 01:14:37  jimg
 * Added static members to help control the cache.
 *
 * Revision 1.41  1999/12/01 21:37:44  jimg
 * Added members to support caching.
 * Added accessors for cache control. libwww 5.2.9 does not supoort these,
 * however.
 *
 * Revision 1.40  1999/09/03 22:07:44  jimg
 * Merged changes from release-3-1-1
 *
 * Revision 1.39  1999/08/23 18:57:44  jimg
 * Merged changes from release 3.1.0
 *
 * Revision 1.38.4.3  1999/08/28 06:43:03  jimg
 * Fixed the implementation/interface pragmas and misc comments
 *
 * Revision 1.38.4.2  1999/08/10 00:40:04  jimg
 * Changes for the new source code organization
 *
 * Revision 1.38.4.1  1999/08/09 22:57:49  jimg
 * Removed GUI code; reactivate by defining GUI
 *
 * Revision 1.38  1999/05/21 20:41:45  dan
 * Changed default 'gui' flag to 'false' in request_data, and read_data
 * methods.
 *
 * Revision 1.37  1999/04/29 03:04:51  jimg
 * Merged ferret changes
 *
 * Revision 1.36  1999/04/29 02:29:28  jimg
 * Merge of no-gnu branch
 *
 * Revision 1.35.8.1  1999/04/14 22:32:46  jimg
 * Made inclusion of timeval _tv depend on the definition of LIBWWW_5_0. See
 * comments in Connect.cc.
 *
 * Revision 1.35  1999/02/18 19:22:38  jimg
 * Added the field _accept_types and two accessor functions. See Connect.cc and
 * the documentation comments for more information.
 *
 * Revision 1.34  1999/01/21 20:42:01  tom
 * Fixed comment formatting problems for doc++
 *
 * Revision 1.33.4.1  1999/02/02 21:56:56  jimg
 * String to string version
 *
 * Revision 1.33  1998/06/04 06:31:33  jimg
 * Added two new member functions to get/set the www_errors_to_stderr property.
 * Also added a new member _www_errors_to_stderr to hold that property. When
 * true, www errors are printed to stderr in addition to being recorded in the
 * object's Error object. The property is false by default.
 *
 * Revision 1.32  1998/03/19 23:49:28  jimg
 * Removed code associated with the (bogus) caching scheme.
 * Removed _connects.
 *
 * Revision 1.31  1998/02/11 21:28:04  jimg
 * Changed x_gzip/x-gzip to deflate since libwww 5.1 offers internal support
 * for deflate (which uses the same LZW algorithm as gzip without the file
 * processing support of gzip).
 * Added to the arguments accepted by the ctor and www_lib_init so that they
 * can now be called with a flag used to control compression.
 * Fixed up the comments to reflect these changes.
 *
 * Revision 1.30  1998/02/04 14:55:31  tom
 * Another draft of documentation.
 *
 * Revision 1.29  1997/12/18 15:06:10  tom
 * First draft of class documentation, entered in doc++ format,
 * in the comments
 *
 * Revision 1.28  1997/09/22 23:05:45  jimg
 * Added comment info.
 *
 * Revision 1.27  1997/06/06 00:45:15  jimg
 * Added read_data(), parse_mime() and process_data() mfuncs.
 *
 * Revision 1.26  1997/03/23 19:40:20  jimg
 * Added field _comp_childpid. See the note in Connect.cc Re: this field.
 *
 * Revision 1.25  1997/03/05 08:25:28  jimg
 * Removed the static global _logfile. See Connect.cc.
 *
 * Revision 1.24  1997/02/13 17:35:39  jimg
 * Added a field to store information from the MIME header `Server:'. Added
 * a member function to access the value of the field and a MIME header
 * `handler' function to store the value there.
 *
 * Revision 1.23  1997/02/12 21:39:10  jimg
 * Added optional parameter to the ctor for this class; it enables
 * informational error messages from the WWW library layer.
 * * Revision 1.22 1997/02/10 02:31:27 jimg 
 * Changed the return type of request_data() and related functions from DDS &
 * to * DDS *.
 *
 * Revision 1.21  1996/11/25 03:39:25  jimg
 * Added web-error to set of object types.
 * Added two MIME parsers to set of friend functions.
 * Removed unused friend functions.
 *
 * Revision 1.20  1996/11/13 18:57:15  jimg
 * Now uses version 5.0a of the WWW library.
 *
 * Revision 1.19  1996/10/08 17:02:10  jimg
 * Added fields for the projection and selection parts of a CE supplied with
 * the URL passed to the Connect ctor.
 *
 * Revision 1.18  1996/07/10 21:25:34  jimg
 * *** empty log message ***
 *
 * Revision 1.17  1996/06/21 23:14:15  jimg
 * Removed GUI code to a new class - Gui.
 *
 * Revision 1.16  1996/06/18 23:41:01  jimg
 * Added support for a GUI. The GUI is actually contained in a separate program
 * that is run in a subprocess. The core `talks' to the GUI using a pty and a
 * simple command language.
 *
 * Revision 1.15  1996/06/08 00:07:57  jimg
 * Added support for compression. The Content-Encoding header is used to
 * determine if the incoming document is compressed (values: x-plain; no
 * compression, x-gzip; gzip compression). The gzip program is used to
 * decompress the document. The new software uses UNIX IPC and a separate
 * subprocess to perform the decompression.
 *
 * Revision 1.14  1996/06/04 21:33:17  jimg
 * Multiple connections are now possible. It is now possible to open several
 * URLs at the same time and read from them in a round-robin fashion. To do
 * this I added data source and sink parameters to the serialize and
 * deserialize mfuncs. Connect was also modified so that it manages the data
 * source `object' (which is just an XDR pointer).
 *
 * Revision 1.13  1996/05/29 21:51:51  jimg
 * Added the enum ObjectType. This is used when a Content-Description header is
 * found by the WWW library to record the type of the object without first
 * parsing it.
 * Added ctors for the struct constraint.
 * Removed the member _request.
 *
 * Revision 1.12  1996/05/21 23:46:33  jimg
 * Added support for URLs directly to the class. This uses version 4.0D of
 * the WWW library from W3C.
 *
 * Revision 1.11  1996/04/05 01:25:40  jimg
 * Merged changes from version 1.1.1.
 *
 * Revision 1.10  1996/02/01 21:45:33  jimg
 * Added list of DDSs and constraint expressions that produced them.
 * Added mfuncs to manage DDS/CE list.
 *
 * Revision 1.9.2.1  1996/02/23 21:38:37  jimg
 * Updated for new configure.in.
 *
 * Revision 1.9  1995/06/27  19:33:49  jimg
 * The mfuncs request_{das,dds,dods} accept a parameter which is appended to
 * the URL and used by the data server CGI to select which filter program is
 * run to handle a particular request. I changed the parameter name from cgi
 * to ext to better represent what was going on (after getting confused
 * several times myself).
 *
 * Revision 1.8  1995/05/30  18:42:47  jimg
 * Modified the request_data member function so that it accepts the variable
 * in addition to the existing arguments.
 *
 * Revision 1.7  1995/05/22  20:43:12  jimg
 * Removed a paramter from the REQUEST_DATA member function: POST is not
 * needed since we no longer use the post mechanism.
 *
 * Revision 1.6  1995/04/17  03:20:52  jimg
 * Removed the `api' field.
 *
 * Revision 1.5  1995/03/09  20:36:09  jimg
 * Modified so that URLs built by this library no longer supply the
 * base name of the CGI. Instead the base name is stripped off the front
 * of the pathname component of the URL supplied by the user. This class
 * append the suffix _das, _dds or _serv when a Connect object is used to
 * get the DAS, DDS or Data (resp).
 *
 * Revision 1.4  1995/02/10  21:54:52  jimg
 * Modified definition of request_data() so that it takes an additional
 * parameter specifying sync or async behavior.
 *
 * Revision 1.3  1995/02/10  04:43:17  reza
 * Fixed the request_data to pass arguments. The arguments string is added to
 * the file name before being posted by NetConnect. Default arg. is null.
 *
 * Revision 1.2  1995/01/31  20:46:56  jimg
 * Added declaration of request_data() mfunc in Connect.
 *
 * Revision 1.1  1995/01/10  16:23:04  jimg
 * Created new `common code' library for the net I/O stuff.
 *
 * Revision 1.2  1994/10/05  20:23:28  jimg
 * Fixed errors in *.h files comments - CVS bites again.
 * Changed request_{das,dds} so that they use the field `_api_name'
 * instead of requiring callers to pass the api name.
 *
 * Revision 1.1  1994/10/05  18:02:08  jimg
 * First version of the connection management classes.
 * This commit also includes early versions of the test code. */

#endif // _connect_h
