// ---------------------------------------------------------------------------
// - Runnable.hpp                                                            -
// - standard object library - runnable abstract class definition            -
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

#ifndef  AFNIX_RUNNABLE_HPP
#define  AFNIX_RUNNABLE_HPP

#ifndef  AFNIX_LOOPABLE_HPP
#include "Loopable.hpp"
#endif

namespace afnix {

  /// The Runnable class is the virtual class used as an interface to the 
  /// interpreter thread subsystem. It extends the loopable thus given
  /// interpreter object a complex runnable environment.
  /// @author amaury darsch

  class Runnable : public Loopable  {
  public:
    /// create a default runnable
    Runnable (void) =default;
    
    /// evaluate the evaluable form
    virtual Object* run (void) =0;

    /// evaluate a form in a thread
    /// @param form the form to evaluate
    virtual Object* launch (Object* form) =0;

    /// evaluate a form in a thread
    /// @param tobj the thread to run
    /// @param form the form to evaluate
    virtual Object* launch (Object* tobj, Object* form) =0;

  private:
    // definitively no copy move
    Runnable (Runnable&& that) noexcept =delete;
    // definitively no move operator
    Runnable& operator = (Runnable&& that) noexcept =delete;

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
