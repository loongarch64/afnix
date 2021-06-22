// ---------------------------------------------------------------------------
// - MthTransit.hpp                                                          -
// - afnix:mth module - math object transit class definition                 -
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

#ifndef  AFNIX_MTHTRANSIT_HPP
#define  AFNIX_MTHTRANSIT_HPP

#ifndef  AFNIX_IVI_HPP
#include "Ivi.hpp"
#endif

#ifndef  AFNIX_RMI_HPP
#include "Rmi.hpp"
#endif

namespace afnix {

  /// The MthTransit class is a math object import/export. The class is
  /// designed to support various mathematical format but support may vary
  /// with the object.
  /// @author amaury darsch

  class MthTransit : public Object {
  public:
    /// the transit mode
    enum t_tmod
      {
       TMOD_AUTO  // mode auto
      };
       
  protected:
    /// the transit mode
    t_tmod d_tmod;
    /// the output stream
    OutputStream* p_os;
    
  public:
    /// create a default transit
    MthTransit (void);
 
    /// create a transit by output stream
    /// @param os the output stream
    MthTransit (OutputStream* os);
 
    /// destroy this transit
    ~MthTransit (void);
    
    /// @return the class name
    String repr (void) const override;

    /// reset this transit
    virtual void reset (void);

    /// export a vector to an output stream
    /// @param v the vector to export
    virtual bool xport (const Ivi& v);

    /// export a vector to an output stream
    /// @param v the vector to export
    virtual bool xport (const Rvi& v);

    /// export a matrix to an output stream
    /// @param m the matrix to export
    virtual bool xport (const Rmi& m);

    /// export a vector by symbol
    /// @param v the vector to export
    /// @param s the vector symbol
    virtual bool xport (const Ivi& v, const String& s);
    
    /// export a vector by symbol
    /// @param v the vector to export
    /// @param s the vector symbol
    virtual bool xport (const Rvi& v, const String& s);
    
    /// export a matrix by symbol
    /// @param m the matrix to export
    /// @param s the matrix symbol
    virtual bool xport (const Rmi& m, const String& s);
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
