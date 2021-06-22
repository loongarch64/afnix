// ---------------------------------------------------------------------------
// - Resume.hpp                                                              -
// - afnix cross debugger - resume exception class definition                -
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

#ifndef  AFNIX_RESUME_HPP
#define  AFNIX_RESUME_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Resume class is the object used to tell the debugger to resume
  /// the execution after a breakpoint. It is an error to throw this object
  /// from the top level debugger loop.
  /// @author amaury darsch

  class Resume : public Object {
  public:
    /// create a default resume
    Resume (void);
    
    /// copy move this resume
    /// @param that the resume to move
    Resume (Resume&& that) noexcept;

    /// move an resume into this one
    /// @param that the resume to move
    Resume& operator = (Resume&& that) noexcept;

    /// @resume the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    Resume (const Resume&) = delete;
    // make the assignment operator private
    Resume& operator = (const Resume&) = delete;
  };
}

#endif
