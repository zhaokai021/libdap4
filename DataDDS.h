// -*- C++ -*-

// (c) COPYRIGHT URI/MIT 1997-1999
// Please read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//      jhrg,jimg       James Gallagher (jgallagher@gso.uri.edu)

// Specialize DDS for returned data. This currently means adding version
// information about the source of the data. Was it from a version 1, 2 or
// later server?
// 
// jhrg 9/19/97

#ifndef _datadds_h
#define _datadds_h 1

#ifdef __GNUG__
#pragma interface
#endif

#include <iostream>
#include <string>

#ifndef _dds_h
#include "DDS.h"
#endif

/** This class adds some useful state information to the DDS
    structure.  It is for use on the client side of the DODS
    connection. 
    
    @brief Holds a DODS DDS.
    @see Connect
    */

class DataDDS : public DDS {
private:
    string _server_version;
    int _server_version_major;
    int _server_version_minor;

    void _version_string_to_numbers();

#if 0
    // The last level read from a sequence. This is used to read nested
    // sequences. 
    int _sequence_level;
#endif
public:
    DataDDS(const string &n = "", const string &v = "");
    virtual ~DataDDS();

    void set_version(const string &v);
    int get_version_major();
    int get_version_minor();

    int sequence_level();
    
    void set_sequence_level(int level);
};

// Revision 1.8.4.1  2002/09/05 22:52:54  pwest
// Replaced the GNU data structures SLList and DLList with the STL container
// class vector<>. To maintain use of Pix, changed the Pix.h header file to
// redefine Pix to be an IteratorAdapter. Usage remains the same and all code
// outside of the DAP should compile and link with no problems. Added methods
// to the different classes where Pix is used to include methods to use STL
// iterators. Replaced the use of Pix within the DAP to use iterators instead.
// Updated comments for documentation, updated the test suites, and added some
// unit tests. Updated the Makefile to remove GNU/SLList and GNU/DLList.
//
// $Log: DataDDS.h,v $
// Revision 1.11  2003/01/10 19:46:40  jimg
// Merged with code tagged release-3-2-10 on the release-3-2 branch. In many
// cases files were added on that branch (so they appear on the trunk for
// the first time).
//
// Revision 1.10  2002/06/18 15:36:24  tom
// Moved comments and edited to accommodate doxygen documentation-generator.
//
// Revision 1.9  2002/06/03 22:21:15  jimg
// Merged with release-3-2-9
//
// Revision 1.8  2000/09/22 02:17:19  jimg
// Rearranged source files so that the CVS logs appear at the end rather than
// the start. Also made the ifdef guard symbols use the same naming scheme and
// wrapped headers included in other headers in those guard symbols (to cut
// down on extraneous file processing - See Lakos).
//
// Revision 1.7  2000/08/02 22:46:49  jimg
// Merged 3.1.8
//
// Revision 1.4.6.1  2000/08/02 21:10:07  jimg
// Removed the header config_dap.h. If this file uses the dods typedefs for
// cardinal datatypes, then it gets those definitions from the header
// dods-datatypes.h.
//
// Revision 1.6  2000/07/09 21:57:09  rmorris
// Mods's to increase portability, minimuze ifdef's in win32 and account
// for differences between the Standard C++ Library - most notably, the
// iostream's.
//
// Revision 1.5  2000/06/07 18:06:58  jimg
// Merged the pc port branch
//
// Revision 1.4.20.1  2000/06/02 18:21:26  rmorris
// Mod's for port to Win32.
//
// Revision 1.4  1999/04/29 02:29:29  jimg
// Merge of no-gnu branch
//
// Revision 1.3.6.1  1999/02/02 21:56:58  jimg
// String to string version
//
// Revision 1.3  1998/02/05 20:13:52  jimg
// DODS now compiles with gcc 2.8.x
//
// Revision 1.2  1998/01/12 14:27:57  tom
// Second pass at class documentation.
//
// Revision 1.1  1997/09/22 22:19:27  jimg
// Created this subclass of DDS to hold version information in the data DDS
//

#endif // _datadds_h
