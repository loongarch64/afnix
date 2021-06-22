// ---------------------------------------------------------------------------
// - Thrset.hpp                                                              -
// - standard object library - thread set class definition                   -
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

#ifndef  AFNIX_THRSET_HPP
#define  AFNIX_THRSET_HPP

#ifndef  AFNIX_SET_HPP
#include "Set.hpp"
#endif

#ifndef  AFNIX_THREAD_HPP
#include "Thread.hpp"
#endif

#ifndef  AFNIX_CONDVAR_HPP
#include "Condvar.hpp"
#endif

namespace afnix {

  /// The Thrset class is a thread container class designed to manage a set
  /// of thread in a uniform way. The thread set can be designed to allocate
  /// one or several thread objects and acts as a thread server object. When
  /// a thread is requested, it is retreived from the thread set and can be 
  /// started at the user option. There are two modes of operations for this
  /// thread set. In the first mode called persistent, a thread is kept in the
  /// set until it has been specifically removed by the user. In the automatic
  /// mode, the thread is automatically removed from the thread set as soon as
  /// it has finished. The first mode is mandatory if one needs the thread
  /// result object. The second mode is usefull when it come to distribute
  /// thread that performs a certain task and no usefull result is needed.
  /// The second mode is somehow similar to the concept of thread pool.
  /// @author amaury darsch

  class Thrset : public virtual Object  {
  private:
    /// the set maximum size
    long d_msiz;
    /// the thread set size
    long d_tsiz;
    /// the automatic pool mode
    bool d_pmod;
    /// thread set 
    Set* p_tset;
    /// the pool condition variable
    Condvar* p_cvar;

  public:
    /// create a new thread set
    Thrset (void);

    /// create a new thread set by group name
    /// @param tgn the thread group name
    Thrset (const String& tgn);

    /// create a new thread set by size
    /// @param msiz the maximum set size
    Thrset (const long msiz);

    /// create a new thread set by mode
    /// @param pmod the pool mode
    Thrset (const bool pmod);

    /// create a new thread set by size and mode
    /// @param msiz the maximum set size
    /// @param pmod the pool mode
    Thrset (const long msiz, const bool pmod);

    /// destroy this thread set
    ~Thrset (void);

    /// @return the class name
    String repr (void) const override;

    /// @return true if the thread set if empty
    bool empty (void) const;

    /// @return true if the thread set if full
    bool full (void) const;

    /// @return the maximum set size
    long getmsiz (void) const;

    /// @return the thread set size
    long length (void) const;

    /// request a new thread of control
    Thread* request (void);

    /// request a new thread of control by group name
    /// @param tgn the thread group name
    Thread* request (const String& tgn);

    /// wait for all thread to complete
    void wait (void);

    /// notify this thread set
    /// @return true if the set has been notified
    bool notify (void);

  private:
    // make the copy constructor private
    Thrset (const Thrset&);
    // make the assignment operator private
    Thrset& operator = (const Thrset&);

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

