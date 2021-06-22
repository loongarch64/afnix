// ---------------------------------------------------------------------------
// - cthr.cxx                                                                -
// - standard platform library - c thread function implementation            -
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

#include "cthr.hpp"
#include "cerr.hpp"
#include "csys.hpp"
#include "cmem.hpp"
#include "cthr.hxx"

namespace afnix {

  // -------------------------------------------------------------------------
  // - task private section                                                  -
  // -------------------------------------------------------------------------

  // the kill signal handler
  static void cthr_ksig_handler (int);
  // the kill signal structure
  struct s_ksig {
    // the control mutex
    void* p_kmtx;
    // the condition variable
    void* p_ktcv;
    // the kill condition
    bool d_ksig;
    // create a killing structure
    s_ksig (void) {
      // preset members
      p_kmtx = c_mtxcreate ();
      p_ktcv = c_tcvcreate ();
      d_ksig = false;
      // install signal
      struct sigaction sa;
      c_memset (&sa, sizeof (sa), nilc);
      sa.sa_handler = cthr_ksig_handler;
      sigemptyset (&sa.sa_mask);
      sigaction (SIGTERM, &sa, NULL);
      sigaction (SIGQUIT, &sa, NULL);
    }
    // destroy this control structure
    ~s_ksig (void) {
      c_tcvdestroy (p_ktcv);
      c_mtxdestroy (p_kmtx);
    }
    // wait for a condition
    void wait (void) {
      // take the lock
      c_mtxlock (p_kmtx);
      // wait for the condition
      while (d_ksig == false) c_tcvwait (p_ktcv, p_kmtx);
      // here we have the lock back
      c_mtxunlock (p_kmtx);
    }
    // mark a condition
    void mark (void) {
      // take the lock
      c_mtxlock (p_kmtx);
      // notify if needed
      if (d_ksig == false) {
	d_ksig = true;
	c_tcvbdcast (p_ktcv);
      }
      // unlock
      c_mtxunlock (p_kmtx);
    }
  };
  // static kill signal creator
  static s_ksig* cthr_ksig_create(void);
  // global kill signal structure
  static s_ksig* cthr_ksig = cthr_ksig_create (); 
  // destroy a kill signal structure
  static void cthr_ksig_destroy (void) {
    delete cthr_ksig;
  }
  // create a kill signal structure
  static s_ksig* cthr_ksig_create (void) {
    s_ksig* result = new s_ksig;
    c_atexit (cthr_ksig_destroy);
    return result;
  }
  // the kill signal handler
  static void cthr_ksig_handler (int) {
    cthr_ksig->mark ();
  }
  
  // -------------------------------------------------------------------------
  // - thread private section                                                -
  // -------------------------------------------------------------------------

  // the maximum number of threads with 32 bits platform (hint)
  static const long THR_MAX_32 = 128;
  // the maximum number of threads with 64 bits platform (hint)
  static const long THR_MAX_64 = 32768;

  // the the thread type
  enum t_cthr
    {
     CTHR_THRO, // thread object
     CTHR_TSKO  // task object
    };

  // the thread information structure
  struct s_thri {
    t_cthr    d_cthr; // the thread type
    pthread_t d_tid;  // the thread id
    bool      d_done; // the done flag
    void*     p_thrr; // the thread result
  };
  
  // the thread object structure
  struct s_thro : s_thri {
    long      d_tgid; // the thread group id
    t_thrf    p_func; // the start function
    void*     p_args; // the start function argument
    t_thrd    p_dtor; // the object destructor
    t_thrn    p_thrn; // the notifier function
    void*     p_thrs; // the thread set object
    long      d_rcnt; // the reference count for this structure
    s_thro*   p_next; // next thread in list
    s_thro*   p_prev; // previous thread in list
    // default constructor
    s_thro (const s_targ& targ) {
      d_cthr = CTHR_THRO;
      d_done = false;
      p_thrr = nullptr;
      d_tgid = (targ.d_tgid < 0) ? 0 : targ.d_tgid;
      p_func = targ.p_func;
      p_args = targ.p_args;
      p_dtor = targ.p_dtor;
      p_thrn = targ.p_thrn;
      p_thrs = targ.p_thrs;
      d_rcnt = 0L;
      p_next = nullptr;
      p_prev = nullptr;
    }
    ~s_thro (void) {
      if (p_dtor != nullptr) p_dtor (p_thrr, false);
      if (p_dtor != nullptr) p_dtor (p_args, true);
      if (p_dtor != nullptr) p_dtor (p_thrs, false);
    }
  };

  // the main thread id
  static pthread_t       cthr_top;
  // the thread system is initialized
  static bool            cthr_sif = false;
  // the thread structure key
  static pthread_key_t   cthr_kid;
  // the control for one initialization
  static pthread_once_t  cthr_koc = AFNIX_PTHREAD_ONCE_INIT;
  // the threads linked list 
  static s_thro*         cthr_lst = nullptr;
  // the global lock for the thread list
  static pthread_mutex_t cthr_mtx = AFNIX_PTHREAD_MUTEX_INITIALIZER;
  // the condition variable during start
  static pthread_cond_t  cthr_cvs = AFNIX_PTHREAD_COND_INITIALIZER;
  // the condition variable during wait
  static pthread_cond_t  cthr_cvw = AFNIX_PTHREAD_COND_INITIALIZER;
  // the condition variable during nil test
  static pthread_cond_t  cthr_cvn = AFNIX_PTHREAD_COND_INITIALIZER;

  // this procedure marks the thread as finished
  static void cthr_mark_finished (s_thro* thro) {
    if ((thro == nullptr) || (thro->d_done == true)) return;
    // lock the thread list
    pthread_mutex_lock (&cthr_mtx);
    // mark as finished
    thro->d_done = true;
    // destroy the arguments - for circular reference when the argument
    // is an object which holds the thread object
    if (thro->p_dtor != nullptr) {
      thro->p_dtor (thro->p_args, true); 
      thro->p_args = nullptr;
    }
    // call the notifier if any
    if (thro->p_thrn != nullptr) thro->p_thrn (thro->p_thrs);
    // signal we have changed some status
    pthread_cond_broadcast (&cthr_cvw);
    // unlock the thread list
    pthread_mutex_unlock (&cthr_mtx);
  }

  // this procedure add a new thread in the thread list
  static void cthr_insert_list (s_thro* thro) {
    if (thro == nullptr) return;
    // get the thread list lock
    pthread_mutex_lock (&cthr_mtx);
    // increase the reference count and mark as started
    thro->d_rcnt = 2L;
    // insert in the list
    thro->p_next = cthr_lst;
    if (cthr_lst != nullptr) cthr_lst->p_prev = thro;
    cthr_lst = thro;
    // signal we are ready and unlock
    pthread_cond_signal  (&cthr_cvs);
    pthread_mutex_unlock (&cthr_mtx);
  }

  // this procedure removes a thread from the thread list
  static void cthr_remove_list (s_thro* thro) {
    if (thro == nullptr) return;
    // get the thread list lock
    pthread_mutex_lock (&cthr_mtx);
    if (thro->d_rcnt > 1) {
      thro->d_rcnt--;
      pthread_mutex_unlock (&cthr_mtx);
      return;
    }
    // remove from the list
    s_thro* prev = thro->p_prev;
    s_thro* next = thro->p_next;
    if ((thro == cthr_lst) || (prev == nullptr)) {
      cthr_lst = next;
      if (next != nullptr) next->p_prev = nullptr;
    } else {
      if (prev != nullptr) prev->p_next = next;
      if (next != nullptr) next->p_prev = prev;
    }
    thro->p_next = nullptr;
    thro->p_prev = nullptr;
    // decrement the reference count and eventually remove
    if (--thro->d_rcnt == 0) delete thro;
    // signal we have removed a thread
    pthread_cond_signal  (&cthr_cvn);
    // release the lock
    pthread_mutex_unlock (&cthr_mtx);
  }

  // this procedure initialize the key when the first thread is started.
  // this procedure is called by pthread_once
  static void cthr_key_once (void) {
    // create the initial key
    pthread_key_create (&cthr_kid, nullptr);
    cthr_top = pthread_self ();
    cthr_sif = true;
    // set the unexpected handler
    c_errsetexpt (nullptr);
  }

  // this procedure is the start routine for the thread - it takes
  // the thread structure and register the key, insert the thread descriptor
  // in the thread list and finally start the function
  static void* cthr_start (void* pthr) {
    // finish to fill the structure
    auto thro = reinterpret_cast<s_thro*> (pthr);
    // map the key with the structure
    pthread_setspecific (cthr_kid, (void*) thro);
    // install the thread in the list
    cthr_insert_list (thro);
    // run the procedure
    try {
      // call the thread function
      thro->p_thrr = thro->p_func (thro->p_args);
      // mark as finished
      cthr_mark_finished (thro);
      // remove from the list
      cthr_remove_list (thro);
      // exit the thread
      return nullptr;
    } catch (...) {
      // mark as finished
      cthr_mark_finished (thro);
      // remove from the list
      cthr_remove_list   (thro);
      // exit the thread
      throw;
    }
  }

    // the task object structure
  struct s_tsko : s_thri {
    t_tskf p_func; // the start function
    void*  p_args; // the start function argument
    // default constructor
    s_tsko (void) {
      d_cthr = CTHR_TSKO;
      d_done = false;
      p_thrr = nullptr;
      p_func = nullptr;
      p_args = nullptr;
    }
    // construct with thread arguments
    s_tsko (t_tskf func, void* args) {
      d_cthr = CTHR_TSKO;
      d_done = false;
      p_thrr = nullptr;
      p_func = func;
      p_args = args;
    }
  };

  // this procedure is the start routine for the task
  static void* ctsk_start (void* ptsk) {
    // map the information structure
    auto tsko = reinterpret_cast<s_tsko*> (ptsk);
    // map the key with the structure
    pthread_setspecific (cthr_kid, (void*) tsko);
    // run the procedure
    try {
      // call the thread function
      tsko->p_thrr = tsko->p_func (tsko->p_args);
      // mark as finished
      tsko->d_done = true;
      // exit the thread
      return nullptr;
    } catch (...) {
      // mark as finished
      tsko->d_done = true;
      // exit the thread
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - task section                                                          -
  // -------------------------------------------------------------------------

  // create a new task by function

  void* c_tsknew (t_tskf func, void* args) {
    // check for null pointer
    if (func == nullptr) return nullptr;
    // initialize the tid key
    pthread_once (&cthr_koc, cthr_key_once);
    // set the thread type as joignable
    pthread_attr_t attr;
    if (pthread_attr_init (&attr) != 0) return nullptr;
    if (pthread_attr_setdetachstate (&attr,PTHREAD_CREATE_JOINABLE) != 0) {
      pthread_attr_destroy (&attr);
      return nullptr;
    }
    // allocate the task information
    s_tsko* tsko = new s_tsko (func, args);
    // run the thread 
    int status = pthread_create (&(tsko->d_tid), &attr, ctsk_start, tsko); 
    // destroy the attributes
    pthread_attr_destroy (&attr);
    // check for status
    if (status != 0) {
      delete tsko;
      return nullptr;
    }
    return tsko;
  }

  // delete the task

  void c_tskdel (void* ptsk) {
    // check for null
    if (ptsk == nullptr) return;
    // map the task structure
    auto tsko = reinterpret_cast<s_tsko*>(ptsk);
    // check for done
    #ifdef AFNIX_HAVE_CANCEL
    if (tsko->d_done == false) pthread_cancel (tsko->d_tid);
    #else
    if (tsko->d_done == false) pthread_kill (tsko->d_tid, SIGQUIT);
    #endif
    // delete the structure
    delete tsko;
  }
  
  // return true if the task has ended

  bool c_tskend (void* ptsk) {
    // check for null
    if (ptsk == nullptr) return true;
    // map the task structure
    auto tsko = reinterpret_cast<s_tsko*>(ptsk);
    return tsko->d_done;
  }
  
  // wait for the task completion

  void c_tskwait (void* ptsk) {
    // check for null
    if (ptsk == nullptr) return;
    // map the task structure
    auto tsko = reinterpret_cast<s_tsko*>(ptsk);
    // join the thread
    pthread_join (tsko->d_tid, nullptr);
  }

  // -------------------------------------------------------------------------
  // - thread section                                                        -
  // -------------------------------------------------------------------------

  // return the maximum number of threads as a hint

  long c_thrmax (void) {
    return c_is32 () ? THR_MAX_32 : THR_MAX_64;
  }

  // check if the thread list is nil

  bool c_thrnullptr (void) {
    // get the thread list lock
    if (pthread_mutex_lock (&cthr_mtx) != 0) return false;
    // prepare result
    bool result = (cthr_lst == nullptr);
    // release the lock
    pthread_mutex_unlock (&cthr_mtx);
    return result;
  }

  // wait for the thread list to be nil

  void c_thrwnil (void) {
    // get the thread list lock
    if (pthread_mutex_lock (&cthr_mtx) != 0) return;
    // check for nil
    while (cthr_lst != nullptr) pthread_cond_wait (&cthr_cvn, &cthr_mtx);
    // release the lock
    pthread_mutex_unlock (&cthr_mtx);
  }
    
  // get a list of threads by running flag

  s_thrl* c_thrgetl (const bool rflg) {
    // get the thread list lock
    pthread_mutex_lock (&cthr_mtx);
    // initialize result
    s_thrl* result = nullptr;
    // loop in the list
    s_thro* thro = cthr_lst;
    while (thro != nullptr) {
      // check for terminated
      if ((rflg == true) && (thro->d_done == true)) {
	// link next element
	thro = thro->p_next;
	// continue
	continue;
      }
      // increment reference count
      thro->d_rcnt++;
      // prepare list record
      s_thrl* thrl = new s_thrl;
      thrl->p_thro = thro;
      thrl->p_next = result;
      result = thrl;
      // link next element
      thro = thro->p_next;
    }
    // release the lock
    pthread_mutex_unlock (&cthr_mtx);
    return result;
  }

  // get a list of threads by group id - carefull the calling
  // thread might be in the list and waiting with this list might not be
  // a good idea unless you remove the calling thread with the group id

  s_thrl* c_thrgetl (const long tgid, const bool rflg) {
    // get the thread list lock
    pthread_mutex_lock (&cthr_mtx);
    // initialize result
    s_thrl* result = nullptr;
    // loop in the list
    s_thro* thro = cthr_lst;
    while (thro != nullptr) {
      // check for terminated
      if ((rflg == true) && (thro->d_done == true)) {
	// link next element
	thro = thro->p_next;
	// continue
	continue;
      }
      // check the group id
      if (thro->d_tgid != tgid) {
	// link next element
	thro = thro->p_next;
	// continue
	continue;
      }
      // increment reference count
      thro->d_rcnt++;
      // prepare list record
      s_thrl* thrl = new s_thrl;
      thrl->p_thro = thro;
      thrl->p_next = result;
      result = thrl;
      // link next element
      thro = thro->p_next;
    }
    // release the lock
    pthread_mutex_unlock (&cthr_mtx);
    return result;
  }

  // create a new thread of control

  void* c_thrnew (const s_targ& targ) {
    // initialize the tid key
    pthread_once (&cthr_koc, cthr_key_once);
    // set the thread type as detached
    pthread_attr_t attr;
    if (pthread_attr_init (&attr) != 0) return nullptr;
    if (pthread_attr_setdetachstate (&attr,PTHREAD_CREATE_DETACHED) != 0) {
      pthread_attr_destroy (&attr);
      return nullptr;
    }
    // create the thread data structure
    s_thro* thro = new s_thro (targ);
    // take the lock - so we guard the start condition - the condition
    // variable protect us against a race condition if the thr descritptor
    // is destroyed before the thread is started (i.e in the list).
    pthread_mutex_lock (&cthr_mtx);
    // run the thread 
    int status = pthread_create (&(thro->d_tid), &attr, cthr_start, 
				 (void*) thro);
    // destroy the attributes
    pthread_attr_destroy (&attr);
    // check for status
    if (status != 0) {
      pthread_mutex_unlock (&cthr_mtx);
      // if the reference count is zero - the thread did not started
      // correctly and the thread structure is not yet in the list
      if (thro->d_rcnt == 0) {
	delete thro;
      } else {
	thro->d_rcnt--;
	cthr_remove_list (thro);
      }
      return nullptr;
    }
    // wait until the thread is started
    pthread_cond_wait    (&cthr_cvs, &cthr_mtx);
    pthread_mutex_unlock (&cthr_mtx);
    return (void*) thro;
  }

  // destroys a thread record
  
  void c_thrdel (void* thr) {
    if (thr == nullptr) return;
    // do not destroy ourself
    if (c_threqual (thr) == true) return;
    // map the thread structure
    s_thro* thro = (s_thro*) thr;
    // remove from the list
    cthr_remove_list (thro);
  }

  // return true if the thread system is initialized

  bool c_thralive (void) {
    return cthr_sif;
  }

  // return the thread id

  void* c_thrself (void) {
    return cthr_sif ? pthread_getspecific (cthr_kid) : nullptr;
  }

  // return true if two thread are equals

  bool c_threqual (void* thr) {
    if (cthr_sif == false) return true;
    pthread_t tid = (thr == nullptr) ? cthr_top : ((s_thri*) thr)->d_tid;
    pthread_t sid = pthread_self ();
    return (pthread_equal (tid, sid) == 0) ? false : true;
  }

  // return true if the thread is the master

  bool c_thrmaster (void) {
    return c_threqual (nullptr);
  }

  // return the thread group id

  long c_thrgetgid (void* thr) {
    if (thr == nullptr) return 0;
    s_thro* thro = (s_thro*) thr;
    return thro->d_tgid;
  }

  // return true if the thread has ended

  bool c_thrend (void* thr) {
    if (thr == nullptr) return true;
    s_thri* thri = (s_thri*) thr;
    return thri->d_done;
  }

  // return the thread result
  
  void* c_thrgetres (void* thr) {
    if (thr == nullptr) return nullptr;
    s_thri* thri = (s_thri*) thr;
    return thri->d_done ? thri->p_thrr : nullptr;
  }

  // wait for a thread to terminate
  
  void c_thrwait (void* thr) {
    if (thr == nullptr) return;
    // do not wait on ourself
    if (c_threqual (thr) == true) return;
    // map the thread structure
    s_thro* thro = (s_thro*) thr;
    // lock the thread list
    if (pthread_mutex_lock (&cthr_mtx) != 0) return;
    // eventually check if it is finished
    if (thro->d_done == true) {
      pthread_mutex_unlock (&cthr_mtx);
      return;
    }      
    // wait for the thread to be marked as finished
    while (thro->d_done == false) {
      pthread_cond_wait (&cthr_cvw, &cthr_mtx);
      if (thro->d_done == true) break;
    }
    pthread_mutex_unlock (&cthr_mtx);
  }

  // exit a thread - but do not destroy the thr record

  void c_threxit (void) {
    pthread_exit (nullptr);
  }

  // -------------------------------------------------------------------------
  // - mutex section                                                         -
  // -------------------------------------------------------------------------

  // create a new mutex in an unlocked state

  void* c_mtxcreate (void) {
    // mutex default attribute
    pthread_mutexattr_t attr;
    pthread_mutexattr_init (&attr);
    pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_ERRORCHECK);
    // create the new mutex
    pthread_mutex_t* mtx = new pthread_mutex_t;
    if (mtx != 0) pthread_mutex_init (mtx, &attr);
    // destroy mutex attributes
    pthread_mutexattr_destroy (&attr);
    return mtx;
  }

  // destroy the mutex argument 

  void c_mtxdestroy (void* mtx) {
    if (mtx == nullptr) return;
    pthread_mutex_t* mutex = (pthread_mutex_t*) mtx;
    pthread_mutex_destroy (mutex);
    delete mutex;
  }

  // lock the mutex argument

  bool c_mtxlock (void* mtx) {
    if (mtx == nullptr) return false;    
    pthread_mutex_t* mutex = (pthread_mutex_t*) mtx;
    return (pthread_mutex_lock (mutex) == 0) ? true : false;
  }

  // unlock the mutex argument

  bool c_mtxunlock (void* mtx) {
    if (mtx == nullptr) return true;    
    pthread_mutex_t* mutex = (pthread_mutex_t*) mtx;
    return (pthread_mutex_unlock (mutex) == 0) ? true : false;
  }
  // try to lock a mutex

  bool c_mtxtry (void* mtx) {
    if (mtx == nullptr) return false;    
    pthread_mutex_t* mutex = (pthread_mutex_t*) mtx;
    return (pthread_mutex_trylock (mutex) == 0) ? true : false;
  }

  // -------------------------------------------------------------------------
  // - condition variable section                                            -
  // -------------------------------------------------------------------------

  // create a new condition variable

  void* c_tcvcreate (void) {
    // condition variable default attribute
    pthread_condattr_t attr;
    pthread_condattr_init (&attr);
    // create the condition variable
    pthread_cond_t* tcv = new pthread_cond_t;
    if (tcv != nullptr) pthread_cond_init (tcv, &attr);
    // destroy the condition attributes
    pthread_condattr_destroy (&attr);
    return tcv;
  }

  // destroy a condition variable

  void c_tcvdestroy (void* tcv) {
    pthread_cond_t* condv = (pthread_cond_t*) tcv;
    delete condv;
  }

  // wait on a condition variable

  void c_tcvwait (void* tcv, void* mtx) {
    if ((tcv == nullptr) || (mtx == nullptr)) return;
    pthread_cond_t*  condv = (pthread_cond_t*)  tcv;
    pthread_mutex_t* mutex = (pthread_mutex_t*) mtx;
    pthread_cond_wait (condv, mutex);
  }

  // signal with a condition variable

  void c_tcvsignal (void* tcv) {
    if (tcv == nullptr) return;
    pthread_cond_t* condv = (pthread_cond_t*) tcv;
    pthread_cond_signal (condv);
  }

  // broadcast with a condition variable

  void c_tcvbdcast (void* tcv) {
    if (tcv == nullptr) return;
    pthread_cond_t* condv = (pthread_cond_t*) tcv;
    pthread_cond_signal (condv);
  }

  // -------------------------------------------------------------------------
  // - signal section                                                        -
  // -------------------------------------------------------------------------

  // wait for a kill signal

  void c_sigwait (void) {
    cthr_ksig->wait ();
  }
}
