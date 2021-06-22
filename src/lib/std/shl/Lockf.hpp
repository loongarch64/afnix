// ---------------------------------------------------------------------------
// - Lockf.hpp                                                               -
// - standard object library - file lock class definition                    -
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

#ifndef  AFNIX_LOCKF_HPP
#define  AFNIX_LOCKF_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Lockf class implements the behavior of a read-write file lock.
  /// the file lock is created by path and once created can locked in read
  /// or write mode. Although file locking cab achieved by several threads,
  /// the mechanism
  /// @author amaury darsch

  class Lockf : public Object {
  private:
    /// the file path
    String d_path;
    /// the remove on close
    bool d_rmoc;
    /// the file descriptor
    int d_sid;
    
  public:
    /// create a new file lock by path
    /// @param path the lock file path
    Lockf (const String& path);

    /// destroy this file lock
    ~Lockf (void);

    /// @return the class name
    String repr (void) const override;

    /// lock this file lock for reading
    void rdlock (void) const override;

    /// lock this file lock for writing
    void wrlock (void) const override;

    /// unlock this file lock
    void unlock (void) const override;

    /// @return the lock file path
    String getpath (void) const;

    /// set the remove on close
    /// @param rmoc the remove on close flag
    void setrmoc (const bool rmoc);
    
  private:
    // make the copy constructor private
    Lockf (const Lockf&) =delete;
    // make the assignment operator private
    Lockf& operator = (const Lockf&) =delete;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
