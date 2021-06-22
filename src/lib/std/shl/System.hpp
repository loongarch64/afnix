// ---------------------------------------------------------------------------
// - System.hpp                                                              -
// - standard object library - system function class definition              -
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

#ifndef  AFNIX_SYSTEM_HPP
#define  AFNIX_SYSTEM_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The System class is a collection of static convenient functions. There
  /// purposes is to provide a uniform interface for os dependant features.
  /// The class also provides  the base encodig mode as well as the access
  /// functions.
  /// @author amaury darsch

  class System {
  public:
    /// preset the afnix engine
    /// @param pgmn the program name
    /// @param sflg the seed flag
    static void preset (const String& pgmn, const bool sflg);

    /// register the progran name
    /// @param name the name to register
    static void setpgm (const String& name);

    /// @return the program name
    static String getpgm (void);

    /// @return the host fqdn
    static String hostfqdn (void);

    /// @return the host domain name
    static String domainname (void);

    /// @return the host name
    static String hostname (void);

    /// @return the user name
    static String username (void);

    /// @return the user home directory
    static String userhome (void);

    /// @return the default user data directory
    static String userdata (void);

    /// @return the user data directory
    static String userdata (const String& name);
    
    /// @return the afnix engine name
    static String getenm (void);

    /// @return the afnix uri
    static String geturi (void);

    /// @return the os name
    static String osname (void);

    /// @return the os type
    static String ostype (void);
    
    /// @return the major version of the afnix system
    static String major (void);    
 
   /// @return the minor version of the afnix system
    static String minor (void);    

    /// @return the patch version of the afnix system
    static String patch (void);    

    /// @return the full version of the afnix system
    static String version (void);

    /// @return true if the system is 32 bit
    static bool is32 (void);

    /// @return true if the system is 64 bit
    static bool is64 (void);

    /// @return true if the system is big endian
    static bool isbe (void);

    /// @return the default system block size
    static long blocksz (void);

    /// @return the machine size
    static String machs (void);

    /// @return the option separator
    static t_quad optsep (void);

    /// @return a formatted options
    static String getopt (const char opte);

    /// @return a formatted options
    static String getopt (const t_quad opte);

    /// @return an environment variable value
    static String getenv (const String& name);

    /// @return a system unique id
    static t_long uniqid (void);

    /// daemonize the executing program
    static bool daemon (void);

    /// wait for a kill signal
    static void sigwait (void);
    
    /// exit unconditionnaly with a status code
    /// @param status the exit status code
    static void exit (int status);

    /// abort unconditionnaly with a status code
    static void abort (void);

    /// @return the process id
    static long getpid (void);

    /// pause for a certain time 
    /// @param time the time to pause in milliseconds
    static void sleep (const long time);

    /// swap a word betwen host and network order
    /// @param value the value to swap
    static t_word wswap (const t_word value);

    /// swap a quad betwen host and network order
    /// @param value the value to swap
    static t_quad qswap (const t_quad value);

    /// swap an octa betwen host and network order
    /// @param value the value to swap
    static t_octa oswap (const t_octa value);

    /// @return the system root directory
    static String getrdir (void);

    /// @return the system local directory
    static String getldir (void);

    /// @return the system upward directory
    static String getudir (void);

    /// @return the system temporary directory
    static String gettdir (void);

    /// @return the directory separator
    static String getdsep (void);

    /// @return the current working directory
    static String getcwd (void);

    /// @return a system temporary file name
    static String tempnam (void);

    /// @return a system temporary file name
    static String tempnam (const String& prefix);

    /// @return a system temporary path
    static String temppth (void);

    /// @return a system temporary path
    static String temppth (const String& prefix);

    /// remove a file name extension
    /// @param name the file name
    /// @return a file name without its extension
    static String rmext (const String& name);

    /// build an absolute path from a name
    /// @param path the original path name
    /// @return the new path name
    static String join (const String& dir);

    /// concatenate two names to form a new os path name
    /// @param dir the directory name
    /// @param name the name to concatenate
    /// @return the new path name
    static String join (const String& dir, const String& name);

    /// extract the directory name from a path
    /// @param path the path to process
    static String xdir (const String& path);

    /// extract the directory name from a path
    /// @param path the path to process
    /// @param cbrk the path separator
    static String xdir (const String& path, const char cbrk);

    /// extract the base name from a path
    /// @param path the path to process
    static String xbase (const String& path);
 
    /// extract the base name from a path
    /// @param path the path to process
    /// @param cbrk the path separator
    static String xbase (const String& path, const char cbrk);
 
    /// extract the file name from a path
    /// @param path the path to process
    static String xname (const String& path);
 
    /// extract the file name from a path
    /// @param path the path to process
    /// @param cbrk the path separator
    static String xname (const String& path, const char cbrk);
 
    /// extract the file name extension
    /// @param name the name to process
    static String xext (const String& name);

    /// @return true if the name map to a regular file name
    static bool isfile (const String& name);

    /// @return true if the name maps to a regular file name in a directory
    static bool isfile (const String& dir, const String& name);

    /// @return true if the name corresponds to a directory name
    static bool isdir (const String& name);

    /// @return true if the name corresponds to a sub directory name
    static bool isdir (const String& dir, const String& name);

    /// move a file by name
    /// @param name the file name to move
    /// @param path the target path
    static bool mvfile (const String& name, const String& path);

    /// remove a file by name
    /// @param name the file name to remove
    static bool rmfile (const String& name);

    /// create a directory by name
    /// @param name the directory name to create
    static bool mkdir (const String& name);

    /// create a directory hierarchicaly by name
    /// @param name the directory name to create
    static bool mhdir (const String& name);

    /// @remove a directory by name
    static bool rmdir (const String& name);
  };
}

#endif
