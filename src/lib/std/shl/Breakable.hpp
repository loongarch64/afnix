// ---------------------------------------------------------------------------
// - Breakable.hpp                                                            -
// - standard object library - breakable abstract class definition            -
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

#ifndef  AFNIX_BREAKABLE_HPP
#define  AFNIX_BREAKABLE_HPP

#ifndef  AFNIX_RUNNABLE_HPP
#include "Runnable.hpp"
#endif

namespace afnix {

  /// The Breakable class is the virtual class used as an interface to the 
  /// debugger infrastructure. The class provides the methods to manage
  /// breakpoints.
  /// @author amaury darsch

  class Breakable : public virtual Runnable  {
  public:
    /// create a default breakable
    Breakable (void) =default;
    
    /// break the breakable in a nameset with an object
    /// @param nset   the nameset to loop
    /// @param object the object to break on
    virtual bool bpoint (Nameset* nset, Object* object) =0;

    /// @return the next flag for this breakable
    virtual bool getnext (void) const =0;

    /// set the next flag
    /// @param flag the flag to set
    virtual void setnext (const bool flag) =0;

  private:
    /// definitively no copy move
    Breakable (Breakable&& that) noexcept =delete;
    /// definitively no move operator
    Breakable& operator = (Breakable&& that) noexcept =delete;
  };
}

#endif
