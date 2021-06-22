// ---------------------------------------------------------------------------
// - Task.cpp                                                                -
// - standard object library - task class implementation                     -
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

#include "Task.hpp"
#include "Exception.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function starts the task
  static void* tsk_start (void* args) {
    auto task = reinterpret_cast <Taskable*> (args);
    task->run ();
    return nullptr;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default task

  Task::Task (void) {
    p_task = nullptr;
    p_ptsk = nullptr;
  }

  // create and start a task

  Task::Task (Taskable* task) {
    p_task = nullptr;
    p_ptsk = nullptr;
    start (task);
  }
  
  // destroy this task
  
  Task::~Task (void) {
    c_tskdel (p_ptsk);
    Object::dref (p_task);
  }

  // return the class name

  String Task::repr (void) const {
    return "Task";
  }

  // start a task 

  void Task::start (Taskable* task) {
    wrlock ();
    try {
      // update the taskable object
      Object::iref (task); Object::dref (p_task); p_task = task;
      // check for a valid taskable
      if (p_task == nullptr) {
	throw Exception ("task-error", "invalid nil taskable object");
      }
      // create a task argument structure
      p_ptsk = c_tsknew (tsk_start, p_task);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // wait for this task to terminate

  void Task::wait (void) {
    wrlock ();
    try {
      c_tskwait (p_ptsk);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the task has ended

  bool Task::isend (void) const {
    rdlock ();
    try {
      bool result = (p_ptsk == nullptr) ? false : c_tskend (p_ptsk);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the task result

  Object* Task::toresult (void) const {
    rdlock ();
    try {
      Object* result = (p_task == nullptr) ? nullptr : p_task->toresult ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
