// ---------------------------------------------------------------------------
// - SessionSet.hpp                                                          -
// - afnix:csm service - session set class definition                        -
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

#ifndef  AFNIX_SESSIONSET_HPP
#define  AFNIX_SESSIONSET_HPP

#ifndef  AFNIX_SESSION_HPP
#include "Session.hpp"
#endif

#ifndef  AFNIX_HASHTABLE_HPP
#include "HashTable.hpp"
#endif

namespace afnix {

  /// The SessionSet class is a collection of session object organized
  /// with a hash table, those key is the session hash id. Normally, the
  /// session object is used to track the user activity. In particular,
  /// the session time must be carefully monitored in order to close a
  /// session which has timed-out. Futhermore, for security purpose, the
  /// session hash id must be carefully generated from the session itself
  /// but in a way which cannot be guessed from the client side.
  /// @author amaury darsch

  class SessionSet : public Object {
  private:
    /// the hash table
    HashTable d_hash;

  public:
    /// create an empty set
    SessionSet (void);

    /// @return the class name
    String repr (void) const;

    /// reset this session set
    virtual void reset (void);

    /// @return the number of session
    virtual long length (void) const;

    /// @return true if the session set is empty
    virtual bool empty (void) const;

    /// check if a session exists by hid
    /// @param hid the session hash id to validate
    virtual bool exists (const String& hid) const;

    /// add a session object
    /// @param sobj the session object
    virtual void add (Session* sobj);

    /// get a session by index
    /// @param idx the session index
    virtual Session* get (const long idx) const;

    /// locate a session by hid
    /// @param hid the session hash id to find
    virtual Session* lookup (const String& hid) const;

    /// remove a session by hid
    /// @param hid the session hash id to remove
    virtual void remove (const String& hid);

  private:
    // make the copy constructor private
    SessionSet (const SessionSet&);
    // make the assignment operator private
    SessionSet& operator = (const SessionSet&);

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
