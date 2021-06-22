// ---------------------------------------------------------------------------
// - Globalset.hpp                                                           -
// - afnix engine - global set class definition                              -
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

#ifndef  AFNIX_GLOBALSET_HPP
#define  AFNIX_GLOBALSET_HPP

#ifndef  AFNIX_NAMESET_HPP
#include "Nameset.hpp"
#endif

#ifndef  AFNIX_QUARKTABLE_HPP
#include "QuarkTable.hpp"
#endif

namespace afnix {

  /// The Globalset class is a nameset designed to handle a large set of
  /// objects. Each object is associated with a symbol. Creating an object
  /// by quark automatically creates the symbol object. The evaluation
  /// process automatically retreives the symbol by quark and return the
  /// evaluated object. Such evaluation is done recursively.
  /// @author amaury darsch

  class Globalset : public Nameset {
  private:
    QuarkTable* p_ntbl;

  public:
    /// create a new globalset
    Globalset (void);
    
    /// create a new globalset with a parent
    /// @param nset the parent nameset
    Globalset (Nameset* nset);

    /// destroy this globalset
    ~Globalset (void);

    /// @return the class name
    String repr (void) const;

    /// reset this global set
    void reset (void);

    /// @return a child globalset
    Nameset* dup (void);

    /// add a new object by quark
    /// @param quark the object quark
    /// @param object the object to bind
    void bind (const long quark, Object* object);

    /// @return true if the quark exists in this globalset
    bool exists (const long quark) const;

    /// @return an object by quark locally
    Object* get (const long quark) const;

    /// remove an object by quark in this globalset
    /// @param quark the binding to remove
    void remove (const long quark);

  private:
    // make the copy constructor private
    Globalset (const Globalset&);
    // make the assignment operator private
    Globalset& operator = (const Globalset&);
  };
}

#endif
