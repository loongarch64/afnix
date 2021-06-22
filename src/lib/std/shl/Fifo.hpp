// ---------------------------------------------------------------------------
// - Fifo.hpp                                                                -
// - standard object library - fifo class definition                         -
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

#ifndef  AFNIX_FIFO_HPP
#define  AFNIX_FIFO_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Fifo class is yet another container that provides the implementation
  /// for a fixed size fifo. Unlink the vector object, the fifo is not self
  /// resizable although the object can be resized manually. The objects are
  /// added into the first position after shifting the internal structure.
  /// If the fifo is full, adding element into the fifo will result in loosing
  /// the oldest object although the fifo can be resized prior this operation.
  /// The default size for the fifo is 64 elements.
  /// @author amaury darsch

  class Fifo : public virtual Object {
  private:
    /// the fifo size
    long     d_size;
    /// the fifo array
    Object** p_fifo;

  public:
    /// create a default fifo
    Fifo (void);
  
    /// create a fifo by size
    /// @param size the fifo size
    Fifo (const long size);

    /// copy construct this fifo
    /// @param that the object to copy
    Fifo (const Fifo& that);

    /// destroy this fifo
    ~Fifo (void);

    /// return the class name
    String repr (void) const override;

    /// assign a fifo to this one
    /// @param that the object to assign
    Fifo& operator = (const Fifo& that);

    /// reset this fifo
    virtual void reset (void);

    /// @return the fifo size
    virtual long getsize (void) const;

    /// @return the fifo length
    virtual long length (void) const;

    /// @return true if the fifo is empty
    virtual bool empty (void) const;

    /// @return true if the fifo is full
    virtual bool full (void) const;

    /// push an object into the fifo
    /// @param obj the object to add
    virtual void push (Object* obj);

    /// pop an object from the fifo
    virtual Object* pop (void);

    /// get an object by position index
    /// @param index the object index
    virtual Object* get (const long index) const;

    /// resize this fifo
    /// @param size the new fifo size
    virtual void resize (const long size);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
