// ---------------------------------------------------------------------------
// - Directory.hpp                                                           -
// - afnix:sio module - directory class definition                           -
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

#ifndef  AFNIX_DIRECTORY_HPP
#define  AFNIX_DIRECTORY_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

namespace afnix {

  /// The Directory class provides facilities to access a directory
  /// and retreive its contents. The directory contents is returned as
  /// a vector of names. The default constructor opens the '.' directory.
  /// @author amaury darsch

  class Directory : public Object {
  public:
    /// remove a directory recursively
    /// @patam path the diectory path
    static void rmall (const String& path);

  private:
    /// the directory name
    String d_name;
    /// the directory handle
    void* p_handle;

  public:
    /// open the default directory
    Directory (void);

    /// open a directory by name
    /// @param name the directory to open
    Directory (const String& name);

    /// destroy this directory accessor
    ~Directory (void);

    /// @return the class name
    String repr (void) const;

    /// @return the directory name
    String getname (void) const;

    /// @return the next available name
    String nxtname (void);

    /// @return the next available path
    String nxtpath (void);

    /// @return the next available file name
    String nxtfnam (void);

    /// @return the next available file path
    String nxtfpth (void);

    /// @return the next available directory name
    String nxtdnam (void);

    /// @return the next available directory path
    String nxtdpth (void);

    /// @return a list of this directory names
    Strvec* getlist (void);

    /// @return a list of this directory names with their path
    Strvec* getplist (void);

    /// @return a list of this directory files
    Strvec* getfiles (void);

    /// @return a list of this directory files with their path
    Strvec* getpfiles (void);

    /// @return a list of sub directories
    Strvec* getdirs (void);

    /// @return a list of sub directories with their path
    Strvec* getpdirs (void);

    /// create a new directory by name
    /// @param name the directory name
    void mkdir (const String& name);

    /// remove a directory by name
    /// @param name the directory to remove
    void rmdir (const String& name);

    /// remove a file by name
    /// @param name the file to remove
    void rmfile (const String& name);

  private:
    // make the copy constructor private
    Directory (const Directory&);
    // make the assignment operator private
    Directory& operator = (const Directory&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
