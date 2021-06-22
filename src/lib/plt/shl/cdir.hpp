// ---------------------------------------------------------------------------
// - cdir.hpp                                                                -
// - standard platform library - c directory function definition             -
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

#ifndef  AFNIX_CDIR_HPP
#define  AFNIX_CDIR_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the functions that handle the file system directory
  /// structure. Normally, these functions are independant of the file system
  /// specifics and provide access, with even named rooted (aka dos) file
  /// systems and special file system separators.
  /// @author amaury darsch

  /// @return the root directory name
  const char* c_getrdir (void);

 /// @return the local directory name
  const char* c_getldir (void);

 /// @return the upward directory name
  const char* c_getudir (void);

  /// @return the temporary directory name
  const char* c_gettdir (void);

  /// @return the native directory separator
  const char c_getdsep (void);

  /// @return the user data directory
  char* c_getddir (const char* name);
  
  /// open a directory by name
  void* c_opendir (const char* name);

  /// get the next file name in the directory handle
  /// @param handle the directory handle
  char* c_readdir (void* handle);

  /// rewind a directory entry to its begining
  /// @param handle the directory handle
  void c_rewinddir (void* handle);

  /// close a directory handle
  /// @param handle the directory handle
  void c_closedir (void* handle);

  /// return the current directory name
  char* c_getcwd (void);

  /// check if the directory name is a dot directory
  /// @param name the name to check
  bool c_isdot (const char* name);

  /// check if the directory name is a local directory
  /// @param name the name to check
  bool c_isldir (const char* name);

  /// check if the directory name is an upward directory
  /// @param name the name to check
  bool c_isudir (const char* name);

  /// create a directory. If the directory exists, the function succeds.
  /// @param name the directory name
  /// @return on success or false in case of error
  bool c_mkdir (const char* name);

  /// remove an empty directory
  bool c_rmdir (const char* name);

  /// extract the directory name from a path
  /// @param path the path to process
  char* c_xdir (const char* path);  

  /// extract the directory name from a path
  /// @param path the path to process
  /// @param cbrk the path separator
  char* c_xdir (const char* path, const char cbrk);  

  /// extract the base name from a path
  /// @param path the path to process
  char* c_xbase (const char* path);  

  /// extract the base name from a path
  /// @param path the path to process
  /// @param cbrk the path separator
  char* c_xbase (const char* path, const char cbrk);  

  /// extract the file name from a path
  /// @param path the path to process
  char* c_xname (const char* path);  

  /// extract the file name from a path
  /// @param path the path to process
  /// @param cbrk the path separator
  char* c_xname (const char* path, const char cbrk);  

  /// remove the file extension from a file name
  /// @param name the original file name
  /// @return the file name without the extension
  char* c_rmext (const char* name);

  /// extract a file name extension
  /// @param name the original file name
  char* c_xext (const char* name);
}

#endif
