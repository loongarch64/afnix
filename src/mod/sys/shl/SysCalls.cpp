// ---------------------------------------------------------------------------
// - SysCalls.cpp                                                            -
// - afnix:sys module - system call implementation                           -
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

#include "Cons.hpp"
#include "Real.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "SysCalls.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // exit this process with an exit code

  Object* sys_exit (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error", 
		       "invalid number of arguments with exit");
    }
    try {
      long val = argv->getlong (0);
      delete argv; argv = nullptr;
      System::exit (val);
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a formated options

  Object* sys_getopt (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error",
		       "invalid number of arguments with getopt");
    }
    try {
      t_quad opte = argv->getchar (0);
      delete argv; argv = nullptr;
      return new String (System::getopt (opte));
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return the process id

  Object* sys_getpid (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    long argc = (args == nullptr) ? 0 : args->length ();
    if (argc != 0) {
      throw Exception ("argument-error", 
		       "invalid number of arguments with getpid");
    }
    return new Integer (System::getpid ());
  }

  // pause for a certain time

  Object* sys_sleep (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv; argv = nullptr;
      throw Exception ("argument-error", 
		       "invalid number of arguments with sleep");
    }
    try {
      long val = argv->getlong (0);
      delete argv; argv = nullptr;
      System::sleep (val);
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return an environment variable value

  Object* sys_getenv (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error", 
		       "invalid number of arguments with getenv");
    }
    try {
      String val = argv->getstring (0);
      delete argv; argv = nullptr;
      return new String (System::getenv (val));
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get a unique id from the system

  Object* sys_uniqid (Evaluable* zobj, Nameset* nset, Cons* args) {
    return new Integer (System::uniqid ());
  }

  // return the host fqdn

  Object* sys_hostfqdn (Evaluable* zobj, Nameset* nset, Cons* args) {
    return new String (System::hostfqdn ());
  }

  // return the host domain name

  Object* sys_domainname (Evaluable* zobj, Nameset* nset, Cons* args) {
    return new String (System::domainname ());
  }
  // return the host name

  Object* sys_hostname (Evaluable* zobj, Nameset* nset, Cons* args) {
    return new String (System::hostname ());
  }

  // return the user name

  Object* sys_username (Evaluable* zobj, Nameset* nset, Cons* args) {
    return new String (System::username ());
  }

  // return the user home directiry

  Object* sys_userhome (Evaluable* zobj, Nameset* nset, Cons* args) {
    return new String (System::userhome ());
  }
}
