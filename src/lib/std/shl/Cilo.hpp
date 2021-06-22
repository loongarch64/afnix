// ---------------------------------------------------------------------------
// - Cilo.hpp                                                                -
// - standard object library - circular cilo class definition                -
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

#ifndef  AFNIX_CILO_HPP
#define  AFNIX_CILO_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Cilo class is a circular object container. By default, a cilo
  /// for 64 objects is created. The constructor also accepts an integer
  /// to preset size. Objects can be inserted with the 'add' method. When the
  /// cilo is full, the first object is overwritten. An index can be used to 
  /// traverse and reterive an object at a given position.
  /// @author amaury darsch

  class Cilo : public virtual Object {
  private:
    /// the cilo size
    long d_size;
    /// the start index
    long d_sidx;
    /// the end index
    long d_eidx;
    /// the traverse index
    long d_tidx;
    /// the full flag
    bool d_full;
    /// the allocated array
    Object** p_cilo;

  public:
    /// create a default cilo
    Cilo (void);

    /// create a cilo with a predefined allocated size
    /// @param size the requested size
    Cilo (const long size);

    /// copy constructor for this cilo
    /// @param that the cilo to copy 
    Cilo (const Cilo& that);

    /// destroy this cilo
    ~Cilo (void);

    /// @return the class name
    String repr (void) const override;

    /// add an object to this cilo
    /// @param push the object to push
    virtual void add (Object* object);

    /// @return true if the traverse object is at the top
    virtual bool istop (void) const;

    /// @return true if the traverse object is at the bottom
    virtual bool isbottom (void) const;

    /// @return the object by traversing upward
    virtual Object* getup (void);

    /// @return the object by traversing downward
    virtual Object* getdown (void);

  private:
    // make the assignment operator private
    Cilo& operator = (const Cilo&) =delete;
  };
}

#endif
