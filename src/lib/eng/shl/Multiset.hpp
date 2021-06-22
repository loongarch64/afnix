// ---------------------------------------------------------------------------
// - Multiset.hpp                                                            -
// - afnix engine - multiset class definition                                -
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

#ifndef  AFNIX_MULTISET_HPP
#define  AFNIX_MULTISET_HPP

#ifndef  AFNIX_NAMESET_HPP
#include "Nameset.hpp"
#endif

#ifndef  AFNIX_NAMETABLE_HPP
#include "NameTable.hpp"
#endif

namespace afnix {

  /// The Multiset class is a nameset class designed to handle the complex
  /// hierarchy of a multiset objects. From this point of view, the multiset
  /// is designed to hold an extra nameset, the primary and secondary tables.
  /// The primary table operates like a multiset. A secondary table is 
  /// designed for shadow objects. This secondary table is associated with
  /// a locking flag that prevents symbols to be added afterward.
  /// The evaluation process operates with the primary table, the secondary
  /// table and the closed variable nameset. The linkset method links in one
  /// call the parent and extra namesets and set the locking flag to true.
  /// @author amaury darsch

  class Multiset : public Nameset {
  private:
    /// the primary name table
    NameTable* p_ptbl;
    /// the secondary name table
    NameTable* p_stbl;
    /// the extra nameset
    Nameset* p_xset;
    /// the secondary locking
    bool d_slok;

  public:
    /// create a new multiset
    Multiset (void);
    
    /// create a multiset with a secondary table
    /// @param sflg the secondary table flag
    Multiset (const bool sflg);

    /// create a multiset with a parent nameset
    /// @param nset the parent nameset
    Multiset (Nameset* nset);

    /// create a multiset with a parent and extra namesets
    /// @param nset the parent nameset
    /// @param xset the extra  nameset
    Multiset (Nameset* nset, Nameset* xset);

    /// destroy this multiset
    ~Multiset (void);

    /// @return the class name
    String repr (void) const;

    /// reset this multiset
    void reset (void);

    /// @return a child multiset
    Nameset* dup (void);

    /// bind a new object by quark
    /// @param quark the object quark
    /// @param object the object to bind
    void bind (const long quark, Object* object);

    /// @return true if the quark exists in this multiset
    bool exists (const long quark) const;

    /// @return an object by quark locally
    Object* get (const long quark) const;

    /// remove an object by quark in this multiset
    /// @param quark the binding to remove
    void remove (const long quark);
    
    /// link the associated namesets
    /// @param pset the parent nameset
    /// @param xset the extra nameset
    virtual void linkset (Nameset* pset, Nameset* xset);

    /// set the extra nameset
    /// @param xset the closed nameset
    virtual void setxset (Nameset* xset);
 
    /// set the secondary locking flag
    /// @param slok the secondary lock flag
    virtual void setslok (const bool slok);

    /// @return the secondary locking flag
    virtual bool getslok (void) const;

  private:
    // make the copy constructor private
    Multiset (const Multiset&);
    // make the assignment operator private
    Multiset& operator = (const Multiset&);
  };
}

#endif
