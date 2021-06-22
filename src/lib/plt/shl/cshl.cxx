// ---------------------------------------------------------------------------
// - cshl.cxx                                                                -
// - standard platform library - c dynamic library implementation            -
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

#include "cshl.hpp"
#include "cstr.hpp"
#include "cshl.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // get the version name of a library
  static char* get_shl_name (const char* name, bool vflag) {
    // standard check
    long nlen = c_strlen (name);
    if (nlen == 0) return nullptr;
    // build name with version
    char buffer[512];
    if (vflag == true) {
      sprintf (buffer, "lib%s.%s.%d.%d", name, AFNIX_LIBRARY_EXT,
	       AFNIX_VERSION_MAJOR, AFNIX_VERSION_MINOR);
    } else {
      sprintf (buffer, "lib%s.%s", name, AFNIX_LIBRARY_EXT);
    }
    return c_strdup (buffer);
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // open the main program as a shared object

  void* c_dlmain (void) {
    return dlopen (nullptr, RTLD_NOW);
  }
  
  // open a dynamic library by name
 
  void* c_dlopen (const char* name) {
    // get the version name first
    char* lname = get_shl_name (name, true);
    if (lname == nullptr) return nullptr;
    void* library = dlopen (lname, RTLD_LAZY|RTLD_GLOBAL);
    delete [] lname;
    if (library != nullptr) return library;
    // fallback with regular name
    lname = get_shl_name (name, false);
    if (lname == nullptr) return nullptr;
    library = dlopen (lname, RTLD_LAZY|RTLD_GLOBAL);
    delete [] lname;
    return library;
  }
  
  // query a symbol by name
  
  void* c_dlsym (const char* name, void* lptr) {
    long len = c_strlen (name);
    // standard check as usual
    if ((len == 0) || (lptr == nullptr)) return nullptr;
    // symbol access
    return dlsym (lptr, name);
  }
  
  // close the library
  
  void c_dlclose (void* lptr) {
    // standard chech again
    if (lptr == nullptr) return;  
    dlclose (lptr);
  }
}

// library symbols
extern "C" {
  // mark the library as registered
  char afnix_plt_resident_library = '\0';
  // test symbol
  char cshl_test_symbol = '\0';
}
