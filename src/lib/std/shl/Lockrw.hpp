// ---------------------------------------------------------------------------
// - Lockrw.hpp                                                              -
// - standard object library - read-write lock class definition              -
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

#ifndef  AFNIX_LOCKRW_HPP
#define  AFNIX_LOCKRW_HPP

namespace afnix {

  /// The Lockrw class implements the behavior of a read-write lock. The
  /// general idea is to give a write lock to the caller if there is no
  /// reader which owns the lock. The implementation is done by using a 
  /// counter to indicate the number of readers. In that respect, the
  /// implementation is safe for recursive call to get a read lock. For the
  /// write part, the implementation is also safe for recursive call. This
  /// implementation is a slightly modified version of the pthread_rwlock
  /// as described by W. Richard Stevens.
  /// @author amaury darsch

  class Lockrw {
  private:
    /// the thread handle (write lock)
    void* p_tid;
    /// the mutex for this object
    void* p_mtx;
    /// reader condition variable
    void* p_rcv;
    /// writer condition variable
    void* p_wcv;
    /// the write reference count
    int   d_wcount;
    /// the read reference count
    int   d_rcount;
    /// number of waiting readers
    int   d_waitrd;
    /// the number of waiting writers
    int   d_waitwr;

  public:
    /// create a new read-write lock
    Lockrw (void);

    /// destroy this read-write lock
    ~Lockrw (void);

    /// lock for reading
    void rdlock (void);

    /// lock for writing
    void wrlock (void);

    /// adaptative read locking
    void arlock (void);

    /// unlock this read-write lock
    void unlock (void);

  private:
    // make the copy constructor private
    Lockrw (const Lockrw&) =delete;
    // make the assignment operator private
    Lockrw& operator = (const Lockrw&) =delete;
  };
}

#endif
