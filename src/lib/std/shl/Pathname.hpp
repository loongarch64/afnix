// ---------------------------------------------------------------------------
// - Pathname.hpp                                                            -
// - standard object library - path name class definition                    -
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

#ifndef  AFNIX_PATHNAME_HPP
#define  AFNIX_PATHNAME_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

namespace afnix {

  /// The Pathname class is a base class designed to manipulate system 
  /// i/o paths. The class operates with a directory name and a file name.
  /// Both names are kept separated to ease the path manipulation. The path
  /// components can be extracted individually. However, it shall be noted
  /// that the first componenent has a special treatment to process the 
  /// root directory name.
  /// @author amaury darsch

  class Pathname : public virtual Object {
  public:
    /// normalize a system path
    /// @param path the path to normalize
    static String normalize (const String& path);

  private:
    /// the file name
    String d_fnam;
    /// the root name
    String d_root;
    /// the directory vector
    Strvec d_dvec;

  public:
    /// create a default path name
    Pathname (void);

    /// create a path name by file name
    /// @param fnam the file path name
    Pathname (const String& fnam);

    /// create a path name by file and directory name
    /// @param fnam the file path name
    Pathname (const String& fnam, const String& dnam);

    /// copy construct this path name
    /// @param that the path name to copy
    Pathname (const Pathname& that);

    /// @return the class name
    String repr (void) const;

    /// assign a pathname to this one
    /// @param that the path name to assign
    Pathname& operator = (const Pathname& that);

    /// reset this path name
    void reset (void);

    /// @return true if the path is a directory
    bool isdir (void) const;

    /// @return true if the path is a file
    bool isfile (void) const;

    /// set the file name
    /// @param name the file name to set
    void setfnam (const String& name);

    /// @return the file name
    String getfnam (void) const;

    /// add a directory name
    /// @param name the directory name to set
    void adddnam (const String& name);

    /// set the directory name
    /// @param name the directory name to set
    void setdnam (const String& name);

    /// @return the directory name
    String getdnam (void) const;

    /// @return the root name
    String getroot (void) const;

    /// @return the full path name
    String getfull (void) const;

    /// @return the system path name
    String getsysp (void) const;

    /// @return the path length
    long length (void) const;

    /// @return a diretory path by index
    String getpath (const long index) const;

    /// add path component by name
    /// @param name the name to add
    void addpath (const String& name);

    /// normalize a path with respect to its type
    void normalize (void);

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
