// ---------------------------------------------------------------------------
// - Viewable.hpp                                                            -
// - standard object library - viewable abstract class definition            -
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

#ifndef  AFNIX_VIEWABLE_HPP
#define  AFNIX_VIEWABLE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// The Viewable class is an abstract class which provides an interface
  /// for viewable memory. The interface merely provides an access to the
  /// byte memory implementation of a given object as well as the viewable
  /// size. It is obviously a low level interface which is not exported at
  /// the interpreter level.
  /// @author amaury darsch

  class Viewable : public virtual Object {
  public:
    /// create a default viewable
    Viewable (void) =default;
    
    /// copy move this viewable object
    /// @param that the viewable to move
    Viewable (Viewable&& that) noexcept;

    /// move a viewable into this one
    /// @param that the viewable to move
    Viewable& operator = (Viewable&& that) noexcept;
    
    /// @return the viewable size
    virtual long tosize (void) const =0;

    /// @return the viewable data
    virtual t_byte* tobyte (void) =0;

    /// @return the viewable data
    virtual const t_byte* tobyte (void) const =0;
  };
}

#endif
