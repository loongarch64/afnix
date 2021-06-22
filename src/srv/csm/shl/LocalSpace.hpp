// ---------------------------------------------------------------------------
// - LocalSpace.hpp                                                          -
// - afnix:csm module - local space class definition                         -
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

#ifndef  AFNIX_LOCALSPACE_HPP
#define  AFNIX_LOCALSPACE_HPP

#ifndef  AFNIX_LOCKF_HPP
#include "Lockf.hpp"
#endif

#ifndef  AFNIX_WORKSPACE_HPP
#include "WorkSpace.hpp"
#endif

namespace afnix {

  /// The LocalSpace class is a local implementation of the abstract
  /// workspace class. A local space is constructed with a root directory
  /// which serves as a root place for the whole workspace. Each zone can be
  /// seen as a local directory and with entity as file, the local space is
  /// persistent. There is no protection mechanism with respect to the
  /// underlying file system hosting the local space.
  /// @author amaury darsch

  class LocalSpace : public WorkSpace {
  private:
    /// the root directory
    String d_root;
    /// the lock file
    Lockf* p_lock;
    
  public:
    /// create a default local space
    LocalSpace (void);

    /// create a local space by root directory
    /// @param root the root directory
    LocalSpace (const String& root);

    /// create a local space by root directory and name
    /// @param root the root directory
    /// @param name the workspace name
    LocalSpace (const String& root, const String& name);

    /// create a local space by root directory, name and info
    /// @param root the root directory
    /// @param name the workspace name
    /// @param info the workspace info
    LocalSpace (const String& root, const String& name, const String& info);

    /// destroy this local zone
    ~LocalSpace (void);
    
    /// @return the class name
    String repr (void) const;

    /// check if a zone exists by name
    /// @param zone the zone to check
    bool iszone (const String& zone) const;

    /// remove a zone by name
    /// @param zone the zone to remove
    bool rmzone (const String& zone);
    
    /// add a new working zone by name
    /// @param zone the zone name
    WorkZone* addzone (const String& zone);

    /// get a zone by name
    /// @param zone the zone name
    WorkZone* getzone (const String& zone) const;

    /// map an entity name to a normalized uri if possible
    /// @param zone the working zone
    /// @param name the entity name to map
    String tonuri (const String& zone, const String& name) const;
    
    /// check if an entity exists by zone and uri
    /// @param zone the working zone
    /// @param uri  the uri to check
    bool exists (const String& zone, const Uri& uri) const;

    /// get an input stream by zone and uri
    /// @param zone the working zone
    /// @param uri  the uri to open
    InputStream* getis (const String& zone, const Uri& uri) const;
    
    /// get an output stream by zone and uri
    /// @param zone the working zone
    /// @param uri  the uri to open
    OutputStream* getos (const String& zone, const Uri& uri) const;
    
    /// get a workspace zone entity list
    /// @param zone the working zone
    Strvec* getelst (const String& zone) const;

    /// get a workspace zone entity print table
    /// get a workspace zone file table
    /// @param zone the working zone
    PrintTable* toeptbl (const String& zone) const;
    
    /// @return the local root directory
    virtual String getroot (void) const;

  private:
    // make the copy constructor private
    LocalSpace (const LocalSpace&) =delete;
    // make the assignment operator private
    LocalSpace& operator = (const LocalSpace&) =delete;

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
