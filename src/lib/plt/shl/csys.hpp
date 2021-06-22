// ---------------------------------------------------------------------------
// - csys.hpp                                                                -
// - standard platform library - c system native function definition         -
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

#ifndef  AFNIX_CSYS_HPP
#define  AFNIX_CSYS_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the procedures that operates with the running system.
  /// The interface is designed to characterize the operating system as well
  /// as its configuration.
  /// @author amaury darsch

  /// @return true if the system is a 32 bit machine
  bool c_is32 (void);

  /// @return true if the system is a 64 bit machine
  bool c_is64 (void);

  /// @return a system unique id counter
  t_long c_uniqid (void);

  /// @return the host name
  char* c_hostname (void);

  /// @return the user name
  char* c_username (void);

  /// @return the user home directory
  char* c_userhome (void);

  /// return a unique temporary name
  /// @param prefix the temporary name prefix
  char* c_tempnam (const char* prefix);

  /// set the program name
  /// @param name the program name to set
  void c_setpgm (const char* name);

  /// @return the program name
  char* c_getpgm (void);

  /// @return the os name
  const char* c_osname (void);

  /// @return the os type
  const char* c_ostype (void);

  /// @return the native option separator
  const char c_optsep (void);

  /// abort the execution without registered code
  void c_abort (void);

  /// exit unconditionnaly with a status code
  void c_exit (int status);

  /// register a function to execute at exit
  /// @param func the function to register
  void c_atexit (void (*func) (void));

  /// @return the process id
  long c_getpid (void);

  /// @return true if a process exists
  bool c_ispid (const long pid);

  /// @return an environment variable value
  const char* c_getenv (const char* name);

  /// @return the backtrace of the calling function
  void* c_backtrace (void);

  /// print the backtrace on the standard error stream
  /// @param bptr the backtrace pointer
  void c_printtrace (void* bptr);

  /// destroy the backtrace information
  /// @param bptr the backtrace pointer
  void c_destroytrace (void* bptr);

  /// preset the system by program name
  /// @param name the program name
  void c_preset (const char* name);

  /// daemonize the current process
  /// @return true in case of success
  bool c_daemon (void);
}

#endif
