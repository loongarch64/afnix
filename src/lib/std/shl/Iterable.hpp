// ---------------------------------------------------------------------------
// - Iterable.hpp                                                            -
// - standard object library - iterable abstract class definition            -
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

#ifndef  AFNIX_ITERABLE_HPP
#define  AFNIX_ITERABLE_HPP

#ifndef  AFNIX_ITERATOR_HPP
#include "Iterator.hpp"
#endif

namespace afnix {

  /// The Iterable class is an abstract class which define the virtual method
  /// "makeit" for a class which is iterable. If an object is iterable, 
  /// an iterator can be constructed for that object. Once the
  /// iterator is constructed, it is possible to iterate in this object.
  /// When an iterator is constructed, the container is marked locked and
  /// and a read lock is taken by the iterator. This means that nothing can
  /// be written in the container.
  /// @author amaury darsch

  class Iterable : public virtual Object {
  public:
    /// create a default iterable
    Iterable (void) =default;
    
    /// copy move this iterable
    /// @param that the iterable to move
    Iterable (Iterable&& that) noexcept;

    /// move an iterable into this one
    /// @param that the iterable to move
    Iterable& operator = (Iterable&& that) noexcept;
    
    /// @return a new iterator for this object
    virtual Iterator* makeit (void) =0;

  public:
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
