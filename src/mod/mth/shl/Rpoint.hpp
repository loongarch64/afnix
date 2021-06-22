// ---------------------------------------------------------------------------
// - Rpoint.hpp                                                              -
// - afnix:mth module - real point definitions                               -
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

#ifndef  AFNIX_RPOINT_HPP
#define  AFNIX_RPOINT_HPP

#ifndef  AFNIX_RPI_HPP
#include "Rpi.hpp"
#endif
 
namespace afnix {

  /// This Rpoint class is the default implementation of the real point
  /// interface. Internally, the point is represented as an array of t_real.
  /// @author amaury darsch

  class Rpoint : public Rpi {
  protected:
    /// the point element
    t_real* p_ptab;

  public:
    /// create a null point
    Rpoint (void);

    /// create a point by size
    /// @param size the point size
    Rpoint (const t_long size);

    /// copy construct this point
    /// @param that the object to copy
    Rpoint (const Rpoint& that);

    /// destroy this point
    ~Rpoint (void);

    /// assign a point to this one
    /// @param that the object to assign
    Rpoint& operator = (const Rpoint& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;
    
    /// @return the serial sid
    t_word getsid (void) const;

    /// resize this point
    /// @param size the new point size
    void resize (const t_long size);

    /// reset this point
    void reset (void);

    /// clear this point
    void clear (void);

  public:
    /// no lock - set a point by position
    /// @param pos the point position
    /// @param val the value to set
    void nlset (const t_long pos, const t_real val);

    /// no lock - get a point value by position
    /// @param pos the point position
    t_real nlget (const t_long pos) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument point
    static Object* mknew (Vector* argv);
  };
}

#endif
