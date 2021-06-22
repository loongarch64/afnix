// ---------------------------------------------------------------------------
// - Rpi.hpp                                                                 -
// - afnix:mth module - real point interface definitions                     -
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

#ifndef  AFNIX_RPI_HPP
#define  AFNIX_RPI_HPP

#ifndef  AFNIX_API_HPP
#include "Api.hpp"
#endif

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif

namespace afnix {

  /// This Rpi class is an abstract class that models the behavior of a
  /// real based point. The class extends the abstract point interface with
  /// specific real point methods.
  /// @author amaury darsch

  class Rpi : public Api {
  public:
    /// create a null point
    Rpi (void) =default;

    /// create a point by size
    /// @param size the point size
    Rpi (const t_long size);

    /// copy construct this point
    /// @param that the point to copy
    Rpi (const Rpi& that);
    
    /// copy move this point
    /// @param that the point to move
    Rpi (Rpi&& that) noexcept;
    
    /// assign a point into this one
    /// @param that the point to assign
    Rpi& operator = (const Rpi& that);

    /// move a point into this one
    /// @param that the point to move
    Rpi& operator = (Rpi&& that) noexcept;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// reset this point
    void reset (void) override;

    /// clear this point
    void clear (void) override;

    /// @return true if the point is nil
    bool isnil (void) const override;
    
    /// compare two points
    /// @param  x the point argument
    /// @return true if they are equals
    virtual bool operator == (const Rpi& x) const;

    /// compare two points
    /// @param  x the point argument
    /// @return true if they are not equals
    virtual bool operator != (const Rpi& x) const;

    /// set a point by value
    /// @param val the value to set
    virtual void set (const t_real val);

    /// set a point by position
    /// @param pos the point position
    /// @param val the value to set
    virtual void set (const t_long pos, const t_real val);

    /// get a point value by position
    /// @param pos the point position
    virtual t_real get (const t_long pos) const;

  public:
    /// no lock - set a point by position
    /// @param pos the point position
    /// @param val the value to set
    virtual void nlset (const t_long pos, const t_real val) =0;

    /// no lock - get a point value by position
    /// @param pos the point position
    virtual t_real nlget (const t_long pos) const =0;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
