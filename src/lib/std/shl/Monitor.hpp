// ---------------------------------------------------------------------------
// - Monitor.hpp                                                             -
// - standard object library - monitor class definition                      -
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

#ifndef  AFNIX_MONITOR_HPP
#define  AFNIX_MONITOR_HPP

namespace afnix {

  /// The Monitor class implements the behavior of a re-entrant mutex. A
  /// monitor comprises a mutex and a reference count. Each time a thread
  /// try to enter the monitor, the mutex is acquired. If the the mutex is
  /// already in use, two situations can arise. If the "enter" method is
  /// called from the same thread who owns the monitor, the method succeds.
  /// If the thread is different, the method is blocked until the owning
  /// thread has released the monitor.
  /// @author amaury darsch

  class Monitor {
  private:
    /// monitor reference count
    long  d_cnt;
    /// the monitor thread id
    void* p_tid;
    /// monitor mutex
    void* p_mtx;
    /// monitor condition variable
    void* p_mcv;

  public:
    /// create a new monitor
    Monitor (void);

    /// destroy this monitor
    ~Monitor (void);

    /// enter the monitor
    void enter (void);

    /// leave this monitor
    void leave (void);

  private:
    // make the copy constructor private
    Monitor (const Monitor&);
    // make the assignment operator private
    Monitor& operator = (const Monitor&);
  };
}

#endif
