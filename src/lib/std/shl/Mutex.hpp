// ---------------------------------------------------------------------------
// - Mutex.hpp                                                               -
// - standard object library - mutex class definition                        -
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

#ifndef  AFNIX_MUTEX_HPP
#define  AFNIX_MUTEX_HPP

namespace afnix {

  /// The Mutex class is the basic synchronization device for multi-threaded
  /// application. When a mutex is created, the mutex is in the unlocked 
  /// state. The "lock" method locks the thread or suspend the calling thread
  /// until the lock is released. The "unlock" method unlock the mutex. The
  /// "trylock" method try to lock the mutex and return true on success.
  /// @author amaury darsch

  class Mutex {
  private:
    /// the mutex object
    void* p_mutex;

  public:
    /// create a new unlocked mutex
    Mutex (void);

    /// destroy this mutex
    ~Mutex (void);

    /// lock the mutex
    void lock (void) const;

    /// try to unlock the mutex
    void unlock (void) const;

    /// @return true if the mutex has been locked
    bool trylock (void) const;

  private:
    // make the copy constructor private
    Mutex (const Mutex&) =delete;
    // make the assignment operator private
    Mutex& operator = (const Mutex&) =delete;
  };
}

#endif
