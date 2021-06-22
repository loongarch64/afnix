// ---------------------------------------------------------------------------
// - Narray.hpp                                                              -
// - afnix:mth module - numeral array array definitions                     -
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

#ifndef  AFNIX_NARRAY_HPP
#define  AFNIX_NARRAY_HPP

#ifndef  AFNIX_NAI_HPP
#include "Nai.hpp"
#endif
 
namespace afnix {

  /// This Narray class is the array implementation of the numeral array
  /// interface. The array is designed to support an array representation
  /// without any optimization for sparsity. Although the class is very
  /// efficient, it is not designed for large, dense array.
  /// @author amaury darsch

  class Narray : public Nai, public Viewable {
  protected:
    /// the array block
    union {
      /// the integer array
      int* p_iblk;
      /// the float array
      float* p_fblk;
      /// the byte array
      t_byte* p_bblk;
    };
    
  public:
    /// create a null array
    Narray (void);

    /// create an array by size
    /// @param size the array size
    Narray (const t_long size);

    /// create an array by size
    /// @param size the array size
    /// @param strd the array stride
    Narray (const t_long size, const long strd);

    /// create an array by stride and type
    /// @param strd the array stride
    /// @param numt the numeral type
    Narray (const long strd, const Numeral::t_numt numt);

    /// create an array by size and type
    /// @param size the array size
    /// @param strd the array stride
    /// @param numt the numeral type
    Narray (const t_long size, const long strd, const Numeral::t_numt numt);

    /// copy construct this array
    /// @param that the array to copy
    Narray (const Narray& that);

    /// destroy this array
    ~Narray (void);

    /// assign a array to this one
    /// @param that the array to assign
    Narray& operator = (const Narray& that);
    
    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;

    /// reset this array
    void reset (void) override;

    /// clear this array
    void clear (void) override;

    /// resize this array
    /// @param size the new array size
    void resize (const t_long size) override;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;
    
    /// @return the viewable data
    const t_byte* tobyte (void) const override;
    
  public:
    /// no lock - set an array by position
    /// @param aidx the array index
    /// @param sidx the stride index
    /// @param nval the value to set
    void nlset (const t_long aidx,const long sidx,const Numeral& val) override;

    /// no lock - get an array value by position
    /// @param aidx the array index
    /// @param sidx the stride index
    Numeral nlget (const t_long aidx, const long sidx) const override;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
