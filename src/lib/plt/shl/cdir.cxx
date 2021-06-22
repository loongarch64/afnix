// ---------------------------------------------------------------------------
// - cdir.cxx                                                                -
// - standard platform library - c directory function implementation         -
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

#include "cdir.hpp"
#include "cstr.hpp"
#include "cdir.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the maximum directory size
  static const long CDIR_MAX_SIZE = 1024;

  // this procedure creates a user data directory name
  static char* cdir_user_ddir (const char* name) {
    // check for null
    if (name == nullptr) return nullptr;
    // create the target directory name
    long slen = c_strlen (name);
    if (slen == 0L) return nullptr;
    // create target result
    char* result = new char[slen+2];
    result[0] = AFNIX_PLATFORM_HSEP;
    result[1] = nilc;
    // append target name
    c_strcat(result, name);
    // here it is;
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return the root directory name

  const char* c_getrdir (void) {
    return AFNIX_PLATFORM_RDIR;
  }

  // return the local directory name

  const char* c_getldir (void) {
    return AFNIX_PLATFORM_LDIR;
  }

  // return the upward directory name

  const char* c_getudir (void) {
    return AFNIX_PLATFORM_UDIR;
  }

  // return the temporary directory name

  const char* c_gettdir (void) {
    return AFNIX_PLATFORM_TDIR;
  }

  // return the user data directory
  char* c_getddir (const char* name) {
    return cdir_user_ddir (name);
  }
  
  // return the os dependant directory separator
  
  const char c_getdsep (void) {
    return AFNIX_PLATFORM_DSEP;
  }

  // open a directory by name and return a handle

  void* c_opendir (const char* name) {
    // check for name first
    if (c_strlen (name) == 0) return 0;
    return opendir (name);
  }

  // get the next entry in the directory

  char* c_readdir (void* handle) {
    // get the handle
    if (!handle) return 0;
    DIR* dirp = (DIR*) handle;
    // get the next name
    struct dirent* ent = readdir (dirp);
    if (!ent) return 0;
    return c_strdup (ent->d_name);
  }

  // rewind the directory

  void c_rewinddir (void* handle) {
    // get the handle
    if (!handle) return;
    DIR* dirp = (DIR*) handle;
    rewinddir (dirp);
  }

  // close the directory structure

  void c_closedir (void* handle) {
    // get the handle
    if (!handle) return;
    DIR* dirp = (DIR*) handle;
    closedir (dirp);
  }

  // check if the name if a dot directory

  bool c_isdot (const char* name) {
    // check for nil
    if (name == nullptr) return false;
    // loop in the dot directories
    const char* ddot[] = AFNIX_PLATFORM_DDOT;
    const char** ptr   = &ddot[0];
    while (*ptr != nullptr) {
      if (c_strcmp (*ptr, name) == true) return true;
      ptr++;
    }
    return false;
  }

  // check if the name if a local directory

  bool c_isldir (const char* name) {
    return c_strcmp (name, AFNIX_PLATFORM_LDIR);
  }

  // check if the name if a upward directory

  bool c_isudir (const char* name) {
    return c_strcmp (name, AFNIX_PLATFORM_UDIR);
  }

  // return the current directory name

  char* c_getcwd (void) {
    char* buffer = new char[CDIR_MAX_SIZE];
    char* name   = getcwd (buffer, CDIR_MAX_SIZE);
    char* result = c_strdup (name);
    delete [] buffer;
    return result;
  }

  // create a directory. If the directory exists, the function return
  // true. In case of error, return false

  bool c_mkdir (const char* name) {
    int         status;
    struct stat buffer;

    // check the name
    if (c_strlen (name) == 0) return false;
  
    // check if directory exists
    status = stat (name, &buffer);
    if ((status == 0) && S_ISDIR(buffer.st_mode)) return true;
  
    // try to create the directory
    status = mkdir (name,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
    if (status  == 0) return true;
    return false;
  }

  // remove a directory - the function returns true on success

  bool c_rmdir (const char* name) {
    if (c_strlen (name) == 0) return true;
    if (rmdir (name)    != 0) return false;
    return true;
  }

  // rename a file with a new name

  bool c_rename (const char* opath, const char* npath) {
    if (c_strlen (opath) == 0) return false;
    if (c_strlen (npath) == 0) return false;
    if (rename (opath, npath) != 0) return false;
    return true;
  }

  // reverse trim a string by size
  static long cdir_rtrim (const char* name, const long size, const char sbrk) {
    // get length and check
    long rpos = size;
    if (rpos == 0L) return -1L;
    // strip if not null
    while ((rpos > 0) && (name[rpos - 1L] == sbrk)) rpos--;
    return rpos;
  }
  
  // get the name size after strip
  static long cdir_rsize (const char* name, const char sbrk) {
    // get length and check
    long size = c_strlen (name);
    // trim if not null
    return cdir_rtrim (name, size, sbrk);
  }
  
  // find a character extension position in reverse mode
  static long cdir_rfind (const char* name, const long rsiz, const char cbrk) {
    // find position in reverse
    for (long i = rsiz-1L; i >= 0L; i--) {
      if (name[i] == cbrk)  return i;
      if (name[i] == AFNIX_PLATFORM_DSEP) break;
    }
    return -1;
  }

  // extract the directory name from a path

  char* c_xdir (const char* path) {
    return c_xdir (path, AFNIX_PLATFORM_DSEP);
  }
  
  // extract the directory name from a path

  char* c_xdir (const char* path, const char cbrk) {
    // get extension size and position
    long size = cdir_rsize (path, cbrk);
    long rpos = cdir_rfind (path, size, cbrk);
    long rsiz = cdir_rtrim (path, rpos, cbrk);
    if (rsiz == -1L) return nullptr;
    // prepare result
    char* result = new char[rsiz+1L];
    for (long i = 0L; i < rsiz; i++) result[i] = path[i];
    result[rsiz] = nilc;
    return result;
  }
  
  // extract the base name from a path

  char* c_xbase (const char* path) {
    return c_xbase (path, AFNIX_PLATFORM_DSEP);
  }

  // extract the base name from a path

  char* c_xbase (const char* path, const char cbrk) {
    // get extension size and position
    long size = cdir_rsize (path, cbrk);
    long rpos = cdir_rfind (path, size, cbrk);
    if (rpos == -1L) return c_strdup (path);
    // compute result size
    long rlen = size - rpos;
    long blen = rlen - 1L;
    if (blen < 0) return c_strdup (path);
    // create result and copie
    char* result = new char[rlen];
    for (long i = 0L; i < blen; i++) result[i] = path[rpos+i+1L];
    result[blen] = nilc;
    return result;
  }
  
  // extract the file name from a path

  char* c_xname (const char* path) {
    return c_xname (path, AFNIX_PLATFORM_DSEP);
  }

  // extract the file name from a path

  char* c_xname (const char* path, const char cbrk) {
    // get extension size and position
    long size = cdir_rsize (path, nilc);
    long rpos = cdir_rfind (path, size, cbrk);
    if (rpos == -1L) return c_strdup (path);
    // compute result size
    long rlen = size - rpos;
    long blen = rlen - 1L;
    if (blen < 0L) return c_strdup (path);
    // create result and copie
    char* result = new char[rlen];
    for (long i = 0L; i < blen; i++) result[i] = path[rpos+i+1L];
    result[blen] = nilc;
    return result;
  }

  // remove the file extension from a file name

  char* c_rmext (const char* name) {
    // get extension size and position
    long size = cdir_rsize (name, nilc);
    long rpos = cdir_rfind (name, size, AFNIX_PLATFORM_ESEP);
    if (rpos == -1L) return c_strdup (name);
    // prepare result
    char* result = new char[rpos+1L];
    for (long i = 0L; i < rpos; i++) result[i] = name[i];
    result[rpos] = nilc;
    return result;
  }

  // extract the file name extension

  char* c_xext (const char* name) {
    // get extension size and position
    long size = cdir_rsize (name, nilc);
    long rpos = cdir_rfind (name, size, AFNIX_PLATFORM_ESEP);
    if (rpos == -1) return nullptr;
    // compute result size
    long rlen = size - rpos;
    long blen = rlen - 1L;
    if (blen < 0) return nullptr;
    // create result and copie
    char* result = new char[rlen];
    for (long i = 0L; i < blen; i++) result[i] = name[rpos+i+1L];
    result[blen] = nilc;
    return result;
  }
}
