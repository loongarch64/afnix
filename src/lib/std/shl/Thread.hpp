// ---------------------------------------------------------------------------
// - Thread.hpp                                                              -
// - standard object library - thread class definition                       -
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

#ifndef  AFNIX_THREAD_HPP
#define  AFNIX_THREAD_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Thread class is the thread management class. When a thread is 
  /// created, an instance of a Evaluable object is ready to be run. Calling 
  /// the constructor will automatically invoke the run method.
  /// @author amaury darsch

  class Thread : public virtual Object  {
  public:
    /// @return true if the master thread
    static bool ismaster (void);

    /// @return true if the thread list is nil
    static bool isnil (void);

    /// wait for the thread list to be nil
    static void wnil (void);

    /// wait for all thread to terminate 
    static void wall (void);

    /// wait for all thread to terminate by group name
    /// @param tgn the thread group name
    static void wall (const String& tgn);

    /// make the current thread terminate itself
    static void exit (void);

  private:
    /// the thread id
    void*   p_tid;
    /// the thread group name
    String  d_tgn;
    /// the thread notifier
    void*   p_thrn;
    /// the thread set object
    Object* p_thrs;

  public:
    /// create a default thread
    Thread (void);

    /// create a thread by group name
    /// @param tgn the thread ground name
    Thread (const String& tgn);

    /// destroy this thread
    ~Thread (void);

    /// @return the class name
    String repr (void) const override;

    /// start a thread if possible
    /// @param zobj the evaluable object
    virtual void start (Evaluable* zobj);

    /// wait for this thread to terminate
    virtual void wait (void);

    /// @return true if the thread has ended
    virtual bool isend (void) const;

    /// @return the thread group name
    virtual String gettgn (void) const;

    /// @return the thread result object
    virtual Object* toresult (void) const;

  private:
    // make the copy constructor private
    Thread (const Thread&);
    // make the assignment operator private
    Thread& operator = (const Thread&);
    // create a thread by id
    Thread (void* tid);
    // create a thread by notifier, set and group name
    Thread (void* thrn, Object* thrs, const String& tgn);
    // make the thread set a friend
    friend class Thrset;

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
