// ---------------------------------------------------------------------------
// - Nvector.hpp                                                             -
// - afnix:mth module - numeral vector definitions                           -
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

#ifndef  AFNIX_NVECTOR_HPP
#define  AFNIX_NVECTOR_HPP

#ifndef  AFNIX_NVI_HPP
#include "Nvi.hpp"
#endif
 
#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif
 
namespace afnix {

  /// This Nvector class is the default implementation of the numeral vector
  /// interface. Internally, the vector is represented as an array of integer
  /// or floats.
  /// @author amaury darsch

  class Nvector : public Nvi, public Viewable {
  public:
    /// generate a random vector by size
    /// @param size the vector size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Nvector* random (const t_long   size, 
			    const Numeral& nmin, const Numeral& nmax);

  protected:
    /// the vector elements
    union {
      /// the integer vector
      int*    p_itab;
      /// the float vector
      float*  p_ftab;
      /// the byte vector
      t_byte* p_btab;
    };
    
  public:
    /// create a null vector
    Nvector (void);

    /// create a vector by size
    /// @param size the vector size
    Nvector (const t_long size);

    /// create a vector by size and type
    /// @param size the vector size
    /// @param numt the numeral type
    Nvector (const t_long size, const Numeral::t_numt numt);

    /// copy construct this vector
    /// @param that the object to copy
    Nvector (const Nvector& that);

    /// destroy this vector
    ~Nvector (void);

    /// assign a vector to this one
    /// @param that the object to assign
    Nvector& operator = (const Nvector& that);
    
    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// reset this vector
    void reset (void) override;

    /// clear this vector
    void clear (void) override;

    /// resize this vector
    /// @param size the new vector size
    void resize (const t_long size) override;

    /// preset this vector
    void preset (void) override;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;
    
    /// @return the viewable data
    const t_byte* tobyte (void) const override;
    
    /// convert a numeral vector by type
    /// @param numt the target type
    Numeral::t_numt convert (const Numeral::t_numt numt) override;
    
  public:
    /// no lock - set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    void nlset (const t_long pos, const Numeral& val) override;

    /// no lock - get a vector value by position
    /// @param pos the vector position
    Numeral nlget (const t_long pos) const override;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
