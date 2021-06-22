// ---------------------------------------------------------------------------
// - Permute.hpp                                                             -
// - afnix:mth module - combinatoric permutation object definitions          -
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

#ifndef  AFNIX_PERMUTE_HPP
#define  AFNIX_PERMUTE_HPP

#ifndef  AFNIX_CPI_HPP
#include "Cpi.hpp"
#endif
 
namespace afnix {

  /// This Permute class is the default implementation of the combinatoric
  /// permutation interface. Internally, the permutation is represented as
  /// an array of integers.
  /// @author amaury darsch

  class Permute : public Cpi {
  protected:
    /// the permutation array
    t_long* p_prow;
    /// the reverse permutation
    t_long* p_revp;

  public:
    /// create a null permutation
    Permute (void);

    /// create a permutation by size
    /// @param size the permutation size
    Permute (const t_long size);

    /// copy construct this object
    /// @param that the object to copy
    Permute (const Permute& that);

    /// destroy this object
    ~Permute (void);

    /// assign an object to this one
    /// @param that the object to copy
    Permute& operator = (const Permute& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// reset this permutation
    void reset (void);

    /// clear this permutation
    void clear (void);

    /// resize this permutation
    /// @param size the new vector size
    void resize (const t_long size);

  public:
    /// no lock - set the permutation by position
    /// @param pos the permutation index
    /// @param val the permutation value
    void nlset (const t_long pos, const t_long val);

    /// no lock - get the permuation value by position
    /// @param pos the permutation position
    t_long nlget (const t_long pos) const;

    /// no lock - reverse the permuation value by position
    /// @param pos the permutation position
    t_long nlrget (const t_long pos) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
