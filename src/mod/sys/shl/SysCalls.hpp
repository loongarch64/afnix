// ---------------------------------------------------------------------------
// - SysCalls.hpp                                                            -
// - afnix:sys module - system call definitions                              -
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

#ifndef  AFNIX_SYSCALLS_HPP
#define  AFNIX_SYSCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// exit the process with an exit code
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_exit (Evaluable* zobj, Nameset* nset, Cons* args);

  /// pause for a certain time
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_sleep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// format an option to the system convention
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_getopt (Evaluable* zobj, Nameset* nset, Cons* args);

  /// return the process id
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_getpid (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get an environemnt variable
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_getenv (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a unique id from the system
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_uniqid (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get the host fqdn
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_hostfqdn (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get the host domain name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_domainname (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get the host name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_hostname (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get the user name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_username (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get the user home directory name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* sys_userhome (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
