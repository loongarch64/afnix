// ---------------------------------------------------------------------------
// - System.cpp                                                              -
// - standard object library - system function class implementation          -
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

#include "Regex.hpp"
#include "System.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Pathname.hpp"
#include "Exception.hpp"
#include "cloc.hpp"
#include "csio.hpp"
#include "csys.hpp"
#include "cdir.hpp"
#include "cclk.hpp"
#include "cstr.hpp"
#include "ccnv.hpp"
#include "cgen.hpp"
#include "cmem.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // afnix engine name
  static const String AFNIX_SYS_ENM = "afnix";
  // afnix uri
  static const String AFNIX_SYS_URI = "http://www.afnix.org";
  // system 32 bits machine
  static const String AFNIX_SYS_M32 = "32";
  // system 64 bits machine
  static const String AFNIX_SYS_M64 = "64";

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // preset the afnix system

  void System::preset (const String& pgmn, const bool sflg) {
    // check the initial flag
    static bool iflg = false;
    if (iflg == true) return;
    // get the program name
    char* name = pgmn.tochar ();
    // main system initialization
    if (iflg == false) {
      // set localization
      c_iloc ();
      // preset the system
      c_preset (name);
      // seed the random engine
      if (sflg == true) c_initrnd ();
      // mark initialized
      iflg = true;
    }
    // clean the name string
    delete [] name;
  }

  // register the program name

  void System::setpgm (const String& name) {
    char* data = name.tochar ();
    c_setpgm (data);
    delete [] data;
  }

  // get the program name

  String System::getpgm (void) {
    char* name = c_getpgm ();
    String result = name;
    delete [] name;
    return result;
  }

  // return the host name
  
  String System::hostfqdn (void) {
    char* name = c_hostname ();
    String result = name;
    delete [] name;
    return result;
  }

  // return the domain name

  String System::domainname (void) {
    String fqdn = System::hostfqdn ();
    Regex re ("(<$a_->+).(<$N_-.>+)");
    if (re != fqdn) return "";
    return re.getstring (1);
  }

  // return the hostname

  String System::hostname (void) {
    String fqdn = System::hostfqdn ();
    Regex re ("(<$a_->+).(<$N_-.>+)");
    if (re != fqdn) return fqdn;
    return re.getstring (0);
  }
  
  // return the user name
  
  String System::username (void) {
    char* name = c_username ();
    String result = name;
    delete [] name;
    return result;
  }

  // return the user home directory
  
  String System::userhome (void) {
    char* home = c_userhome ();
    String result = home;
    delete [] home;
    return result;
  }

  // return the default user data directory

  String System::userdata (void) {
    return System::userdata (System::getenm ());
  }

  // return the user data directory

  String System::userdata (const String& name) {
    // get the user home directory
    String home = System::userhome ();
    // get the user data directory
    char* cnam = name.tochar ();
    char*  ddir = c_getddir (cnam);
    String dnam = ddir;
    delete [] cnam;
    delete [] ddir;
    return System::join (home, dnam);
  }
  
  // return the afnix engine name
  
  String System::getenm (void) {
    return AFNIX_SYS_ENM;
  }

  // return the afnix uri
  
  String System::geturi (void) {
    return AFNIX_SYS_URI;
  }

  // return the afnix major version
  
  String System::major (void) {
    Integer val (AFNIX_VERSION_MAJOR);
    return val.tostring ();
  }

  // return the afnix minor version
  
  String System::minor (void) {
    Integer val (AFNIX_VERSION_MINOR);
    return val.tostring ();
  }

  // return the afnix patch version
  
  String System::patch (void) {
    Integer val (AFNIX_VERSION_PATCH);
    return val.tostring ();
  }

  // return the os name
  
  String System::osname (void) {
    return c_osname ();
  }

  // return the os type

  String System::ostype (void) {
    return c_ostype ();
  }

  // return the version of the afnix system
  
  String System::version (void) {
    return AFNIX_VERSION_TOTAL;
  }

  // return true if the system is 32 bit

  bool System::is32 (void) {
    return (sizeof (long) == 4);
  }

  // return true if the system is 64 bit

  bool System::is64 (void) {
    return (sizeof (long) == 8);
  }

  // check for a big endian platform

  bool System::isbe (void) {
    return c_isbe ();
  }
  
  // return the default system block size

  long System::blocksz (void) {
    return c_pagesize ();
  }

  // return the machine size

  String System::machs (void) {
    if (sizeof (long) == 4) return AFNIX_SYS_M32;
    if (sizeof (long) == 8) return AFNIX_SYS_M64;
    throw Exception ("internal-error", "unknown machine size");
  }

  // return the option separator

  t_quad System::optsep (void) {
    return Unicode::toquad (c_optsep ());
  }

  // return a formated option

  String System::getopt (const char opte) {
    return System::getopt (Unicode::toquad (opte));
  }

  // return a formated options

  String System::getopt (const t_quad opte) {
    String result = System::optsep ();
    result += opte;
    return result;
  }

  // return an environment variable value

  String System::getenv (const String& name) {
    const char* buf = name.tochar ();
    String result = c_getenv (buf);
    delete [] buf;
    return result;
  }

  // return a system uniq id

  t_long System::uniqid (void) {
    return c_uniqid ();
  }

  // daemonize the executing process

  bool System::daemon (void) {
    return c_daemon ();
  }

  // wait for a kill signal

  void System::sigwait (void) {
    c_sigwait ();
  }
  
  // exit unconditionally with a status code
  
  void System::exit (int status) {
    c_exit (status);
  }

  // abort unconditionally with a status code
  
  void System::abort (void) {
    c_exit (-1);
  }

  // return the process id

  long System::getpid (void) {
    return c_getpid ();
  }

  // pause for a certain time

  void System::sleep (const long time) {
    c_sleep (time);
  }

  // swap a word between host and network order

  t_word System::wswap (const t_word value) {
    return c_wswap (value);
  }

  // swap a quad between host and network order

  t_quad System::qswap (const t_quad value) {
    return c_qswap (value);
  }

  // swap a word between host and network order

  t_octa System::oswap (const t_octa value) {
    return c_oswap (value);
  }
  
  // return the system root directory

  String System::getrdir (void) {
    String result = c_getrdir ();
    return result;
  }

  // return the system local directory

  String System::getldir (void) {
    String result = c_getldir ();
    return result;
  }

  // return the system upward directory

  String System::getudir (void) {
    String result = c_getudir ();
    return result;
  }

  // return the system temporary directory

  String System::gettdir (void) {
    String result = c_gettdir ();
    return result;
  }

  // return the directory separator

  String System::getdsep (void) {
    String result = c_getdsep ();
    return result;
  }

  // return the current working directory

  String System::getcwd (void) {
    String result = c_getcwd ();
    return result;
  }

  // return the system temporary name

  String System::tempnam (void) {
    String result = c_tempnam ("afnix");
    return result;
  }

  // return the system temporary name

  String System::tempnam (const String& prefix) {
    char* pf = prefix.tochar ();
    String result = c_tempnam (pf);
    delete [] pf;
    return result;
  }

  // return a system temporary path

  String System::temppth (void) {
    String dir = System::gettdir ();
    String nam = System::tempnam ();
    String result = System::join (dir, nam);
    return result;
  }

  // return a system temporary path

  String System::temppth (const String& prefix) {
    String dir = System::gettdir ();
    String nam = System::tempnam (prefix);
    String result = System::join (dir, nam);
    return result;
  }

  // remove the file name extension
  
  String System::rmext (const String& name) {
    char* fname = name.tochar ();
    char* data  = c_rmext (fname);
    String result = data;
    delete [] fname;
    delete [] data;
    return result;
  }
    
  // join one name to form an absolute path
  
  String System::join (const String& path) {
    return System::join (System::getrdir (), path);
  }

  // join two names to form a new file name
  
  String System::join (const String& dir, const String& name) {
    String result = dir;
    t_quad dirsep =  Unicode::toquad (c_getdsep ());
    if (result.last () != dirsep) result += dirsep;
    result += name;
    return result;
  }

  // extract a directory name from a path

  String System::xdir (const String& path) {
    char* data = path.tochar ();
    char* name = c_xdir (data);
    String result = name;
    delete [] data;
    delete [] name;
    return result;
  }

  // extract a directory name from a path

  String System::xdir (const String& path, const char cbrk) {
    char* data = path.tochar ();
    char* name = c_xdir (data, cbrk);
    String result = name;
    delete [] data;
    delete [] name;
    return result;
  }

  // extract a base name from a path

  String System::xbase (const String& path) {
    char* data = path.tochar ();
    char* name = c_xbase (data);
    String result = name;
    delete [] data;
    delete [] name;
    return result;
  }

  // extract a base name from a path

  String System::xbase (const String& path, const char cbrk) {
    char* data = path.tochar ();
    char* name = c_xbase (data, cbrk);
    String result = name;
    delete [] data;
    delete [] name;
    return result;
  }

  // extract a file name from a path

  String System::xname (const String& path) {
    char* data = path.tochar ();
    char* name = c_xname (data);
    String result = name;
    delete [] data;
    delete [] name;
    return result;
  }

  // extract a file name from a path

  String System::xname (const String& path, const char cbrk) {
    char* data = path.tochar ();
    char* name = c_xname (data, cbrk);
    String result = name;
    delete [] data;
    delete [] name;
    return result;
  }

  // extract a file name extension

  String System::xext (const String& name) {
    char* data = name.tochar ();
    char* fext = c_xext (data);
    String result = fext;
    delete [] data;
    delete [] fext;
    return result;
  }

  // return true if the name corresponds to a regular file name
  
  bool System::isfile (const String& name) {
    char* fname = name.tochar ();
    bool status = c_isfile (fname);
    delete [] fname;
    return status;
  }
  
  // return true if the name is a regular file in the directory

  bool System::isfile (const String& dir, const String& name) {
    String path = System::join (dir, name);
    return isfile (path);
  }

  // return true if the name corresponds to a directory name
  
  bool System::isdir (const String& name) {
    char* dname = name.tochar ();
    bool status = c_isdir (dname);
    delete [] dname;
    return status;
  }

  // return true if the name is a sub-directory name

  bool System::isdir (const String& dir, const String& name) {
    String path = System::join (dir, name);
    return isdir (path);
  }

  // move a file by name

  bool System::mvfile (const String& name, const String& path) {
    char* fname = name.tochar ();
    char* fpath = path.tochar ();
    bool status = c_mv (fname, fpath);
    delete [] fname;
    delete [] fpath;
    return status;
  }

  // remove a file by name

  bool System::rmfile (const String& name) {
    char* fname = name.tochar ();
    bool status = c_rm (fname);
    delete [] fname;
    return status;
  }

  // create a directory by name

  bool System::mkdir (const String& name) {
    char* dname = name.tochar ();
    bool status = c_mkdir (dname);
    delete [] dname;
    return status;
  }

  // create a directory hierarchicaly by name

  bool System::mhdir (const String& name) {
    // create a path name by name
    Pathname path;
    path.setdnam (name);
    // get the path length
    long len = path.length ();
    if (len == 0) return false;
    // create the first component
    String pnam = path.getpath (0);
    if (System::mkdir (pnam) == false) return false;
    if (len == 1) return true;
    // loop in the other components
    for (long i = 1; i < len; i++) {
      pnam = System::join (pnam, path.getpath (i));
      if (System::mkdir (pnam) == false) return false;
    }
    // all done
    return true;
  }

  // remove a directory by name

  bool System::rmdir (const String& name) {
    char* dname = name.tochar ();
    bool status = c_rmdir (dname);
    delete [] dname;
    return status;
  }
}
