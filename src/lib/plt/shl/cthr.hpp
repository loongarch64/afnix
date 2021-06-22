// ---------------------------------------------------------------------------
// - cthr.hpp                                                                -
// - standard platform library - c thread function definitions               -
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

#ifndef  AFNIX_CTHR_HPP
#define  AFNIX_CTHR_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the definition used to model the platform independant
  /// thread systems. The model is largely built from the Posix pthread model.
  /// The interface is also designed to handle the mutexes and the condition
  /// variables that permit to synchronize the various threads.
  /// @author amaury darsch

  // -------------------------------------------------------------------------
  // - task section                                                          -
  // -------------------------------------------------------------------------

  /// The task is a low level thread api which provides a simple interface for
  /// runtime joignable thread creation. Theere is no thread tracking with a
  /// task, and as such should be used carefully.

  /// the task function to execute
  using t_tskf = void* (*) (void*);

  /// create a new task by function
  /// @param func the function to execute
  /// @param the task argument
  void* c_tsknew (t_tskf func, void* args);

  /// delete a task
  /// @param ptsk the task pointer
  void c_tskdel (void* ptsk);

  /// @return true if the task has ended
  bool c_tskend (void* ptsk);
  
  /// wait for a task to complete
  /// @param ptsk the task pointer
  void c_tskwait (void* ptsk);
  
  // -------------------------------------------------------------------------
  // - thread section                                                        -
  // -------------------------------------------------------------------------

  /// the thread list structure - this list returns the internal thread id
  /// in the form of a list - the internal reference count is already
  /// increment when the list is created - the destruction of the list must
  /// not destroy the thread id since this operation is done by the thread
  /// object destructor which calls automatically 'c_thrdestroy'.
  struct s_thrl {
    /// the thread object
    void*   p_thro;
    /// the next record
    s_thrl* p_next;
    /// create default list
    s_thrl (void) {
      p_thro = nullptr;
      p_next = nullptr;
    }
    /// destroy the list
    ~s_thrl (void) {
      delete p_next;
    }
  };

  /// the thread function to execute
  using t_thrf = void* (*) (void*);
  /// the object destructor callback
  using t_thrd = void  (*) (void*, const bool);
  /// the thread set notifier function
  using t_thrn = void  (*) (void*);

  /// the thread argument structure - this is the structure which is
  /// used during the thread creation call
  struct s_targ {
    /// the thread group id
    long   d_tgid;
    /// the function to start
    t_thrf p_func;
    /// the function arguments
    void*  p_args;
    /// the object destructor
    t_thrd p_dtor;
    /// the notifier function
    t_thrn p_thrn;
    /// the thread set object
    void*  p_thrs;
    /// create a default argument
    s_targ (void) {
      d_tgid = 0L;
      p_func = nullptr;
      p_args = nullptr;
      p_dtor = nullptr;
      p_thrn = nullptr;
      p_thrs = nullptr;
    }
  };

  /// the maximum number of threads as a hint
  long c_thrmax (void);

  /// @return true if the thread list is nil
  bool c_thrnullptr (void);

  /// wait for the thread list to be nil
  void c_thrwnil (void);

  /// get a list of threads by running flag
  /// @param rflg the running flag
  /// @return a list of running thread
  s_thrl* c_thrgetl (const bool rflg);

  /// get a list of threads by group id
  /// @param tgid the thread group id
  /// @param rflg the running flag
  /// @return a list of threads
  s_thrl* c_thrgetl (const long tgid, const bool rflg);

  /// create a new thread of control
  /// @param targ the thread argument structure
  void* c_thrnew (const s_targ& targ);

  /// destroy eventually a thread record
  /// @param thr the thread to destroy
  void c_thrdel (void* thr);
  
  /// @return true if the thread system is initialized
  bool c_thralive (void);

  /// @return the unique thread id
  void* c_thrself (void);

  /// @return true if the thread id equal the self one
  bool c_threqual (void* thr);

  /// @return true if the thread is the master one
  bool c_thrmaster (void);

  /// @return the thread groupd id
  long c_thrgetgid (void* thr);

  /// @return true if the thread has ended
  bool c_thrend (void* thr);

  /// @return the thread result
  void* c_thrgetres (void* thr);

  /// wait for a thread to terminate
  /// @param thr the thread handle
  void c_thrwait (void* thr);

  /// exit a thread
  void c_threxit (void);

  // -------------------------------------------------------------------------
  // - mutex section                                                         -
  // -------------------------------------------------------------------------

  /// create a new mutex in a unlock state
  void* c_mtxcreate (void);

  /// destroy the mutex argument
  void c_mtxdestroy (void* mtx);

  /// lock a mutex and return true on success
  /// @param mtx the mutex to lock
  bool c_mtxlock (void* mtx);

  /// unlock a mutex and return true on success
  /// @param mtx the mutex to unlock
  bool c_mtxunlock (void* mtx);

  /// @return true if a mutex is locked
  bool c_mtxtry (void* mtx);

  // -------------------------------------------------------------------------
  // - condition variable section                                            -
  // -------------------------------------------------------------------------
  
  /// create a new condition variable
  void* c_tcvcreate (void);

  /// destroy a condition variable
  /// @param tcv the condition variable
  void c_tcvdestroy (void* tcv);
  
  /// wait on a condition variable
  /// @param tcv the condition variable
  /// @param mtx the control mutex
  void c_tcvwait (void* tcv, void* mtx);

  /// signal on a condition variable
  /// @param tcv the condition variable
  void c_tcvsignal (void* tcv);

  /// broadcast on a condition variable
  /// @param tcv the condition variable
  void c_tcvbdcast (void* tcv);

  // -------------------------------------------------------------------------
  // - signal section                                                        -
  // -------------------------------------------------------------------------

  /// wait for kill signal
  void c_sigwait (void);
}

#endif
