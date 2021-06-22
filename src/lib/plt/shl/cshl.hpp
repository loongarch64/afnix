// ---------------------------------------------------------------------------
// - cshl.hpp                                                                -
// - standard platform library - c dynamic library function definition       -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#ifndef  AFNIX_CSHL_HPP
#define  AFNIX_CSHL_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the procedures that load dynamically a library.
  /// Normaly, a library is loaded by name and an entry point symbol is
  /// searched for execution. This library is extremely low-level and should
  /// not be used by a normal user.
  /// @author amaury darsch

  /// @return the main program as a dynamic library
  void* c_dlmain (void);

  /// open a dynamic library by name.
  /// @param name the library name.
  /// @return a library descriptor suitable for query.
  void* c_dlopen (const char* name);
  
  /// open a dynamic library by prefix and name.
  /// @param pfix the library prefix.
  /// @param name the library name.
  /// @return a library descriptor suitable for query.
  void* c_dlopen (const char* pfix, const char* name);
  
  /// query a symbol by name, using a library descriptor.
  /// @param name the symbol name
  /// @param lptr the library descriptor
  /// @return a symbol descriptor
  void* c_dlsym (const char* name, void* lptr);
  
  /// close the dynamic library.
  /// @param lptr the library descriptor  
  void c_dlclose (void* lptr);
}

#endif
