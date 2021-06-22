// ---------------------------------------------------------------------------
// - SioCalls.hpp                                                            -
// - afnix:sio module - i/o specific calls definitions                       -
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

#ifndef  AFNIX_SIOCALLS_HPP
#define  AFNIX_SIOCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// identity a serial file
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_ident (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// rename a file by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_mvfile (Evaluable* zobj, Nameset* nset, Cons* args);

  /// remove a list of files by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_rmfile (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create a list of directories by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_mkdir (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create hierarchicaly a list of directories by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_mhdir (Evaluable* zobj, Nameset* nset, Cons* args);

  /// remove a list of directories by name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_rmdir (Evaluable* zobj, Nameset* nset, Cons* args);

  /// extract the base name from a path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_xname (Evaluable* zobj, Nameset* nset, Cons* args);

  /// extract the directory base path from a path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_xdir (Evaluable* zobj, Nameset* nset, Cons* args);

  /// extract the extension from a path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_xext (Evaluable* zobj, Nameset* nset, Cons* args);

  /// remove the extension from a path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_rmext (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create a user data directory name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_usr_ddir (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create a temporary file name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_tmp_name (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create a temporary path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_tmp_path (Evaluable* zobj, Nameset* nset, Cons* args);

  /// build an absolute file path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_abs_path (Evaluable* zobj, Nameset* nset, Cons* args);

  /// build a relative file path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sio_rel_path (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
