// ---------------------------------------------------------------------------
// - LocalZone.hpp                                                           -
// - afnix:csm module - local zone class definition                          -
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

#ifndef  AFNIX_LOCALZONE_HPP
#define  AFNIX_LOCALZONE_HPP

#ifndef  AFNIX_LOCKF_HPP
#include "Lockf.hpp"
#endif

#ifndef  AFNIX_WORKZONE_HPP
#include "WorkZone.hpp"
#endif

namespace afnix {

  /// The LocalZone class is a local implementation of the abstract
  /// workzone class. A local zone is constructed with a root directory
  /// which serves as a root place for the whole zone. The local zone
  /// is persistent and there is no protection mechanism with respect to
  /// the underlying file system hosting the local zone. Note that the
  /// implementation is protected with a lock file, and thus can be used
  /// in a multi process environment.
  /// @author amaury darsch

  class LocalZone : public WorkZone {
  private:
    /// the root directory
    String d_root;
    /// the lock file
    Lockf* p_lock;
    
  public:
    /// create a default local zone
    LocalZone (void);

    /// create a local zone by root directory
    /// @param root the root directory
    LocalZone (const String& root);

    /// create a local zone by root and name
    /// @param root the root directory
    /// @param name the zone name
    LocalZone (const String& root, const String& name);

    /// create a local zone by root directory name and info
    /// @param root the root directory
    /// @param name the zone name
    /// @param info the zone info
    LocalZone (const String& root, const String& name, const String& info);

    /// destroy this local zone
    ~LocalZone (void);

    /// @return the class name
    String repr (void) const;

    /// map an entity name to a normalized uri string
    /// @param name the file name to map
    String tonuri (const String& name) const;

    /// check if an entity exists by uria
    /// @param uri the uri to check
    bool exists (const Uri& uri) const;

    /// rename an entity by uri
    /// @param nuri the name uri
    /// @param turi the target uri
    bool rename (const Uri& nuri, const Uri& turi);

    /// remove an entity by uria
    /// @param uri the uri to check
    bool remove (const Uri& uri);

    /// clean the zone
    bool clean (void);
    
    /// get an input stream by and uri
    /// @param uri the uri to open
    InputStream* getis (const Uri& uri) const;
    
    /// get an output stream by uri
    /// @param uri the uri to open
    OutputStream* getos (const Uri& uri) const;

    /// @return a local zone entity list
    Strvec* getelst (void) const;

    /// @return a local zone entity list by identity
    Strvec* getelst (const String& idty) const;

    /// @return a local zone entity print table
    PrintTable* toeptbl (void) const;

    /// @return the local root directory
    virtual String getroot (void) const;

  private:
    // make the copy constructor private
    LocalZone (const LocalZone&) =delete;
    // make the assignment operator private
    LocalZone& operator = (const LocalZone&) =delete;

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
