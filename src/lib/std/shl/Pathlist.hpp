// ---------------------------------------------------------------------------
// - Pathlist.hpp                                                            -
// - standard object library - path list class definition                    -
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

#ifndef  AFNIX_PATHLIST_HPP
#define  AFNIX_PATHLIST_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

namespace afnix {

  /// The Pathlist class is a base class designed to ease the manipulation
  /// of a file search path. The class acts like a list of search paths
  /// and various facilities are provided to find a valid path for a given
  /// name. The path list can be manipulated like any other list.
  /// @author amaury darsch

  class Pathlist : public virtual Object {
  private:
    /// the local search flag
    bool   d_lsch;
    /// the path list
    Strvec d_plst;

  public:
    /// create a default path list
    Pathlist (void);

    /// create a path list with a local search flag
    /// @param lsch the local search flag
    Pathlist (const bool lsch);

    /// create a path list with an initial path
    /// @param path the initial path
    Pathlist (const String& path);

    /// copy construct this path name
    /// @param that the path name to copy
    Pathlist (const Pathlist& that);

    /// @return the class name
    String repr (void) const;

    /// assign a pathlist to this one
    /// @param that the path name to assign
    Pathlist& operator = (const Pathlist& that);

    /// reset this path list
    void reset (void);

    /// @return the local search path flag
    bool getlsch (void) const;

    /// set the local search path flag
    /// @param lsch the local search flag
    void setlsch (const bool lsch);

    /// @return the path list length
    long length (void) const;

    /// @return a path component by index
    String getpath (const long index) const;

    /// add path component by name
    /// @param path the path to add
    void addpath (const String& path);

    /// @return true if a file name can be resolved
    bool isfile (const String& name) const;

    /// resolve a name in the path list
    String resolve (const String& name) const;

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
