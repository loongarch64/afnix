// ---------------------------------------------------------------------------
// - Queue.hpp                                                               -
// - standard object library - dynamic queue class definition                -
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

#ifndef  AFNIX_QUEUE_HPP
#define  AFNIX_QUEUE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Queue class is a container class which can be used to queue and
  /// dequeue objects. When an object is queued, it is placed at the end of
  /// the queue. The dequeue method returns the first available object in the
  /// queue or nil. The empty method should be called to check if the queue
  /// is empty. When the queue is full, the resize method is automatically
  /// called.
  /// @author amaury darsch

  class Queue : public virtual Object {
  private:
    /// the array size
    long d_size;
    /// the allocated array
    Object** p_queue;
    /// the next queing index
    long d_qidx;
    /// the next dequeing index
    long d_didx;

  public:
    /// create an empty queue.
    Queue (void);

    /// create a queue with a predefined allocated size
    /// @param size the requested size
    Queue (const long size);

    /// create a queue with a vector of objects
    /// @param vobj the vector object
    Queue (const Vector& vobj);

    /// destroy this queue
    ~Queue (void);

    /// @return the class name
    String repr (void) const;

    /// queue an object
    /// @param object the object to queue
    void enqueue (Object* object);

    /// @return the next avaliable queued object or nil
    Object* dequeue (void);

    /// @return the number of objects in this queue
    long length (void) const;
    
    /// @return true if this queue is empty
    bool empty (void) const;

    /// @return an object in the queue by index
    Object* get (const long index) const;

    /// flush this queue
    void flush (void);

    /// resize this queue
    void resize (void);

  private:
    // make the copy constructor private
    Queue (const Queue&);
    // make the assignment operator private
    Queue& operator = (const Queue&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
