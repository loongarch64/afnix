// ---------------------------------------------------------------------------
// - Thrmap.hpp                                                              -
// - standard object library - object thread mapper class definition         -
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

#ifndef  AFNIX_THRMAP_HPP
#define  AFNIX_THRMAP_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Thrmap class is a thread specific container class. The class 
  /// stores an object on a thread basis. That is the get method will
  /// return an object which has been set in a specific thread.
  /// @author amaury darsch

  class Thrmap : public virtual Object  {
  private:
    /// master object
    Object* p_mobj;
    /// thread specific map
    struct s_thrmap* p_tmap;

  public:
    /// create a new thread map
    Thrmap (void);

    /// destroy this thread map
    ~Thrmap (void);

    /// @return the class name
    String repr (void) const;

    /// set a thread specific object
    /// @param object the object to set
    void set (Object* object);

    /// @return a thread specific object
    Object* get (void) const;

  private:
    // make the copy constructor private
    Thrmap (const Thrmap&);
    // make the assignment operator private
    Thrmap& operator = (const Thrmap&);
  };
}

#endif
