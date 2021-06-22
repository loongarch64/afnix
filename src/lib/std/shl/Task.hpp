// ---------------------------------------------------------------------------
// - Task.hpp                                                              -
// - standard object library - task class definition                       -
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

#ifndef  AFNIX_TASK_HPP
#define  AFNIX_TASK_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

#ifndef  AFNIX_TASKABLE_HPP
#include "Taskable.hpp"
#endif

namespace afnix {

  /// The Task class is the task management class. When a task is 
  /// created, an instance of a taskable object is ready to be run. Calling 
  /// the constructor will automatically invoke the run method. The task is
  /// a simple thread object which is not intended to be lanched by the
  /// interpreter. On the other hand, the task has a simple management
  /// environment.
  /// @author amaury darsch

  class Task : public virtual Object{
  private:
    /// the taskable object
    Taskable* p_task;
    /// the task pointer
    void* p_ptsk;
    
  public:
    /// create a default task
    Task (void);

    /// create and start a task
    /// @param task the taskable object
    Task (Taskable* task);

    /// destroy this task
    ~Task (void);

    /// @return the class name
    String repr (void) const;

    /// start a task if possible
    /// @param task the taskable object
    virtual void start (Taskable* task);

    /// wait for this task to terminate
    virtual void wait (void);

    /// @return true if the task has ended
    virtual bool isend (void) const;

    /// @return the task result
    virtual Object* toresult (void) const;
    
  private:
    // make the copy constructor private
    Task (const Task&) =delete;
    // make the assignment operator private
    Task& operator = (const Task&) =delete;
  };
}

#endif
