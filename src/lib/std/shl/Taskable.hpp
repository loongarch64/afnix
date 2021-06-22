// ---------------------------------------------------------------------------
// - Taskable.hpp                                                            -
// - standard object library - taskable abstract class definition            -
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

#ifndef  AFNIX_TASKABLE_HPP
#define  AFNIX_TASKABLE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Taskable class is the virtual class used as an interface to the 
  /// task object. The class defines a single method which is called when
  /// the thread is created.
  /// @author amaury darsch

  class Taskable : public Object {
  public:    
    /// run the taskable object
    virtual void run (void) noexcept =0;

    /// @return the taskable result
    virtual Object* toresult (void) const =0;
  };
}

#endif
