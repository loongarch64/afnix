// ---------------------------------------------------------------------------
// - Localset.hpp                                                            -
// - afnix engine - localset class definition                                -
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

#ifndef  AFNIX_LOCALSET_HPP
#define  AFNIX_LOCALSET_HPP

#ifndef  AFNIX_NAMESET_HPP
#include "Nameset.hpp"
#endif

#ifndef  AFNIX_NAMETABLE_HPP
#include "NameTable.hpp"
#endif

namespace afnix {

  /// The Localset class is a nameset designed to handle a small set of
  /// objects. Creating an object by quark automatically creates the symbol
  /// object. The evaluation process automatically retreives the symbol by
  /// quark and return the evaluated object. Such evaluation is done 
  /// recursively.
  /// @author amaury darsch

  class Localset : public Nameset {
  private:
    /// the name table
    NameTable* p_ntbl;

  public:
    /// create a new localset
    Localset (void);
    
    /// create a localset with a parent
    /// @param nset the parent nameset
    Localset (Nameset* nset);

    /// destroy this localset
    ~Localset (void);

    /// @return the class name
    String repr (void) const;

    /// reset this localset
    void reset (void);

    /// @return a child localset
    Nameset* dup (void);

    /// bind a new object by quark
    /// @param quark the object quark
    /// @param object the object to bind
    void bind (const long quark, Object* object);

    /// @return true if the quark exists in this localset
    bool exists (const long quark) const;

    /// @return an object by quark locally
    Object* get (const long quark) const;

    /// remove an object by quark in this localset
    /// @param quark the binding to remove
    void remove (const long quark);

  private:
    // make the copy constructor private
    Localset (const Localset&);
    // make the assignment operator private
    Localset& operator = (const Localset&);
  };
}

#endif
