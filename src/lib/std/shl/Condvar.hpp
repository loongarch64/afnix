// ---------------------------------------------------------------------------
// - Condvar.hpp                                                             -
// - standard object library - condition variable class definition           -
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

#ifndef  AFNIX_CONDVAR_HPP
#define  AFNIX_CONDVAR_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Condvar class is the class which implements the behavior of a
  /// condition variable. The condition variable system provides a mechanism
  /// for a thread to wait on a condition. When that condition is obtained
  /// one or several thread can be signaled.
  /// @author amaury darsch

  class Condvar : public Object {
  private:
    /// the boolean condition
    bool  d_cond;
    /// the control mutex
    void* p_mtx;
    /// the condition variable
    void* p_tcv;

  public:
    /// create a new condition variable
    Condvar (void);

    /// destroy this condition variable
    ~Condvar (void);

    /// @return the class name
    String repr (void) const override;

    /// lock the condition variable mutex
    virtual void lock (void) const;

    /// remove the condition variable mutex
    virtual void rmlock (void) const;

    /// wait on this condition variable
    /// @param lflg the lock flag for variable
    virtual void wait (const bool lflg);

    /// mark this condition variable and broadcast
    virtual void mark (void);

    /// reset this condition variable
    virtual void reset (void);

  private:
    // make the copy constructor private
    Condvar (const Condvar&) =delete;
    // make the assignment operator private
    Condvar& operator = (const Condvar&) =delete;

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
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
