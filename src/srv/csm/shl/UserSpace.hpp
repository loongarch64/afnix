// ---------------------------------------------------------------------------
// - UserSpace.hpp                                                           -
// - afnix:csm module - user space class definition                          -
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

#ifndef  AFNIX_USERSPACE_HPP
#define  AFNIX_USERSPACE_HPP

#ifndef  AFNIX_LOCALSPACE_HPP
#include "LocalSpace.hpp"
#endif

namespace afnix {

  /// The UserSpace class is a local space those root directory is a
  /// user data directory. It is typically used to store user data
  /// for local configuration with a zone based organization.
  /// @author amaury darsch

  class UserSpace : public LocalSpace {
  public:
    /// create a default local space
    UserSpace (void);

    /// create a local space by root directory
    /// @param root the root directory
    UserSpace (const String& root);

    /// create a local space by root directory and name
    /// @param root the root directory
    /// @param name the workspace name
    UserSpace (const String& root, const String& name);

    /// create a local space by root directory, name and info
    /// @param root the root directory
    /// @param name the workspace name
    /// @param info the workspace info
    UserSpace (const String& root, const String& name, const String& info);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    UserSpace (const UserSpace&) =delete;
    // make the assignment operator private
    UserSpace& operator = (const UserSpace&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
