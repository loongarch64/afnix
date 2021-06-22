// ---------------------------------------------------------------------------
// - Cpi.hpp                                                                 -
// - afnix:mth module - combinatoric permutation interface definitions       -
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

#ifndef  AFNIX_CPI_HPP
#define  AFNIX_CPI_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif
 
namespace afnix {

  /// This Cpi class is the interface class for the permutation object. The
  /// class defines the permutation size as well as the accessor and mutator
  /// methods.
  /// @author amaury darsch

  class Cpi : public virtual Serial {
  protected:
    /// the permutation size
    t_long  d_size;

  public:
    /// create a null permutation
    Cpi (void);

    /// create a permutation by size
    /// @param size the permutation size
    Cpi (const t_long size);

    /// copy move this permutation
    /// @param that the permutation  move
    Cpi (Cpi&& that) noexcept;

    /// move a permutation into this one
    /// @param that the permutation to move
    Cpi& operator = (Cpi&& that) noexcept;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// reset this permutation
    virtual void reset (void);

    /// clear this permutation
    virtual void clear (void);

    /// resize this permutation
    /// @param size the new permutation size
    virtual void resize (const t_long size) =0;

    /// @return the permutation size
    virtual t_long getsize (void) const;

    /// set the permutation by position
    /// @param pos the permutation index
    /// @param val the permutation value
    virtual void set (const t_long pos, const t_long val);

    /// get the permuation value by position
    /// @param pos the permutation position
    virtual t_long get (const t_long pos) const;

    /// reverse the permuation value by position
    /// @param pos the permutation position
    virtual t_long rget (const t_long pos) const;

  public:
    /// no lock - set a permutation by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void nlset (const t_long pos, const t_long val) =0;

    /// no lock - get a permutation value by position
    /// @param pos the vector position
    virtual t_long nlget (const t_long pos) const =0;

    /// no lock - get a reverse permutation value by position
    /// @param pos the vector position
    virtual t_long nlrget (const t_long pos) const =0;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
