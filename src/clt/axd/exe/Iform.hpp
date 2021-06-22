// ---------------------------------------------------------------------------
// - Iform.hpp                                                               -
// - afnix cross debugger - instrumented form class definitions              -
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

#ifndef  AFNIX_IFORM_HPP
#define  AFNIX_IFORM_HPP

#ifndef  AFNIX_FORM_HPP
#include "Form.hpp"
#endif

namespace afnix {

  /// The Iform class is an instrumented form designed to store a breakpoint
  /// and reacts to it during the evaluation process.
  /// @author amaury darsch

  class Iform : public Form {
  private:
    /// the breakpoint flag
    bool d_brkp;

  public:
    /// create a nil form
    Iform (void);

    /// create a form by form
    /// @param form to bind
    Iform (Form* form);

    /// copy constuct this object
    /// @param that the object to copy
    Iform (const Iform& that);

    /// assign a form to this one
    /// @param that the form to copy
    Iform& operator = (const Iform& that);
    
    /// return the class name
    String repr (void) const;

    /// set the form breakpoint
    /// @param brkp the breakpoint flag
    virtual void setbrkp (const bool brkp);
    
  public:    
    /// evaluate this object with the calling nameset
    /// @param dbg  the current evaluable
    /// @param nset the current nameset 
    Object* eval (Evaluable* zobj, Nameset* nset);
  };
}

#endif
