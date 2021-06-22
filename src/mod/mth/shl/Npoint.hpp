// ---------------------------------------------------------------------------
// - Npoint.hpp                                                              -
// - afnix:mth module - numeral point definitions                            -
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

#ifndef  AFNIX_NPOINT_HPP
#define  AFNIX_NPOINT_HPP

#ifndef  AFNIX_NPI_HPP
#include "Npi.hpp"
#endif
 
#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif
 
namespace afnix {

  /// This Npoint class is the default implementation of the numeral point
  /// interface. Internally, the point is represented as an array of integer
  /// or floats.
  /// @author amaury darsch

  class Npoint : public Npi, public Viewable {
  protected:
    /// the point elements
    union {
      /// the integer point
      int*    p_itab;
      /// the float point
      float*  p_ftab;
      /// the byte point
      t_byte* p_btab;
    };
    
  public:
    /// create a null point
    Npoint (void);

    /// create a point by size
    /// @param size the point size
    Npoint (const t_long size);

    /// create a point by size and type
    /// @param size the point size
    /// @param numt the point type
    Npoint (const t_long size, const Numeral::t_numt numt);

    /// copy construct this point
    /// @param that the object to copy
    Npoint (const Npoint& that);

    /// destroy this point
    ~Npoint (void);

    /// assign a point to this one
    /// @param that the object to assign
    Npoint& operator = (const Npoint& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// reset this point
    void reset (void);

    /// clear this point
    void clear (void);

    /// resize this point
    /// @param size the new point size
    void resize (const t_long size);

    /// preset this point
    void preset (void);

    /// @return the viewable size
    long tosize (void) const;

    /// @return the viewable data
    t_byte* tobyte (void);
    
    /// @return the viewable data
    const t_byte* tobyte (void) const;
    
  public:
    /// no lock - set a point by position
    /// @param pos the point position
    /// @param val the value to set
    void nlset (const t_long pos, const Numeral& val);

    /// no lock - get a point value by position
    /// @param pos the point position
    Numeral nlget (const t_long pos) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
