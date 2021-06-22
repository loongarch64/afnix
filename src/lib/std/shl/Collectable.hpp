// ---------------------------------------------------------------------------
// - Collectable.hpp                                                         -
// - standard object library - collectable abstract class definition         -
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

#ifndef  AFNIX_COLLECTABLE_HPP
#define  AFNIX_COLLECTABLE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Collectable class is an abstract class designed to provide the
  /// methods for collectable object. Collectable objects are special objects
  /// that can be controlled by a special memory manager, called a memory 
  /// collector or garbage collector. Unlike a simple object associated with
  /// a reference count, the collectable object is also controled by special
  /// methods. In particular, the 'release' method can be invoked to release
  /// all internal links susceptible to create loops and thus preventing the
  /// object to be release by the standard reference count mechanism.
  /// Releasing a link do not necessarily result in the object destruction.
  /// @author amaury darsch

  class Collectable : public virtual Object  {
  protected:
    /// the collector pointer
    void* p_cptr;

  public:
    /// create a collectable object
    Collectable (void);
    
    /// copy move this collectable  object
    /// @param that the collectable to move
    Collectable (Collectable&& that) noexcept;
    
    /// destroy this object
    ~Collectable (void);

    /// move a collectable to this one
    /// @param that the collectable to move
    Collectable& operator = (Collectable&& that) noexcept;
    
    /// release the object links
    virtual void release (void) = 0;

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
