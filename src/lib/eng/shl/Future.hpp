// ---------------------------------------------------------------------------
// - Future.hpp                                                              -
// - afnix engine - future class definition                                  -
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

#ifndef  AFNIX_FUTURE_HPP
#define  AFNIX_FUTURE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Future class is high-level representation of an asynchronous
  /// evaluation. The sole purpose of creating a future is to abstract the
  /// asynchronous execution and the evaluation of the future result. For the
  /// sake of completness, a future can operate on any object and it is the
  /// resolve operation which will trigger the execution of if any.
  /// @author amaury darsch

  class Future : public Object {
  private:
    /// the future form
    Object* p_form;
    /// the future context
    struct s_fctx* p_fctx;

  public:
    /// create a nil future
    Future (void);

    /// create a new future with a form
    /// @param form the future form
    Future (Object* form);

    /// destroy this future
    ~Future (void);

    /// @return the class name
    String repr (void) const override;

    /// wait for this future if needed
    virtual void wait (void);
    
    /// force the evaluation of this future
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    virtual Object* force (Evaluable* zobj, Nameset* nset);
    
  private:
    // make the copy constructor private
    Future (const Future&) =delete;
    // make the assignment operator private
    Future& operator = (const Future&) =delete;

  public:
    /// evaluate this future according to the delay flag
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    Object* eval (Evaluable* zobj, Nameset* nset) override;
  };
}

#endif
