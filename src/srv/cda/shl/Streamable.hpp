// ---------------------------------------------------------------------------
// - Streamable.hpp                                                          -
// - afnix:cda service - streamable interface definitions                    -
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

#ifndef  AFNIX_STREAMABLE_HPP
#define  AFNIX_STREAMABLE_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// This Streamable class is an abstract class that models the behavior of
  /// an object streamer. A streamable object can deliver various objects
  /// in a sequential form with an optional position. The streamable object
  /// has also capabilities that can be queried. The basic streamable object
  /// is a file which produces data bytes.
  /// @author amaury darsch

  class Streamable : public virtual Object {
  public:
    /// create a default streamable
    Streamable (void);
    
    /// copy move this streamable
    /// @param that the streamable to move
    Streamable (Streamable&& that) noexcept;

    /// move an streamable into this one
    /// @param that the streamable to move
    Streamable& operator = (Streamable&& that) noexcept;

    /// reset this streamer
    virtual void reset (void) =0;

    /// @return the streamer departure position
    virtual t_real departure (void) const;

    /// @return the streamer arrival position
    virtual t_real arrival (void) const;
    
    /// @return the streamer position
    virtual t_real locate (void) const;
    
    /// move the streamer to the departure position
    virtual t_real begin (void);

    /// move the streamer to the arrival position
    virtual t_real end (void);

    /// move the streamer to the next position
    virtual t_real next (void);

    /// move the streamer to the next position
    /// @param incr the increment position
    virtual t_real next (const t_real incr);

    /// move the streamer to the previous position
    virtual t_real prev (void);
    
    /// move the streamer to the previous position
    /// @param decr the decrement position
    virtual t_real prev (const t_real decr);

    /// set the streamer position
    /// @param pos the position to set
    virtual t_real move (const t_real pos);
    
    /// @return the streamer boolean data
    virtual bool getbool (void) const;

    /// @return the boolean data by position
    virtual bool getbool (const t_real pos);

    /// @return the streamer integer data
    virtual long getlong (void) const;

    /// @return the integer data by position
    virtual long getlong (const t_real pos);
    
    /// @return the streamer real data
    virtual t_real getreal (void) const;

    /// @return the real data by position
    virtual t_real getreal (const t_real pos);

    /// forward the streamer to the next boolean
    /// @param pval the boolean value to find
    virtual t_real forward (const bool pval);

    /// forward the streamer to the next integer
    /// @param pval the integer value to find
    virtual t_real forward (const long pval);

    /// forward the streamer to the next real
    /// @param pval the real value to find
    virtual t_real forward (const t_real pval);

    /// backward the streamer to the previous boolean
    /// @param pval the boolean value to find
    virtual t_real backward (const bool pval);

    /// backward the streamer to the previous integer
    /// @param pval the integer value to find
    virtual t_real backward (const long pval);

    /// backward the streamer to the previous real
    /// @param pval the real value to find
    virtual t_real backward (const t_real pval);
    
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
