// ---------------------------------------------------------------------------
// - Iterator.hpp                                                            -
// - standard object library - iterator abstract class definition            -
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

#ifndef  AFNIX_ITERATOR_HPP
#define  AFNIX_ITERATOR_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Iterator class is an abstract class which is used to build an
  /// iterator for an iterable object. Once the iterator has been constructed,
  /// it it possible to traverse the object. The "begin" method reset the 
  /// iterator to the begining. The "end" method reset the iterator to the end.
  /// The "next" method returns the next available object. The "previous" 
  /// method returns the previous objet.
  /// @author amaury darsch

  class Iterator : public virtual Object {
  public:
    /// create a default iterator
    Iterator (void) =default;
    
    /// copy move this iterator object
    /// @param that the iterator to move
    Iterator (Iterator&& that) noexcept;

    /// move an iterator object into this one
    /// @param that the iterator to move
    Iterator& operator = (Iterator&& that) noexcept;
    
    /// reset the iterator to the begining
    virtual void begin (void) =0;

    /// reset the iterator to the end
    virtual void end (void) =0;

    /// move the iterator to the next position
    virtual void next (void) =0;

    /// move the iterator to the previous
    virtual void prev (void) =0;

    /// @return the object at the iterator position
    virtual Object* getobj (void) const =0;

    /// @return true if the iterator is at the end
    virtual bool isend (void) const =0;

    /// @return true if the iterator is not at the end
    virtual bool valid (void) const;

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
