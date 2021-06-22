// ---------------------------------------------------------------------------
// - Rfunction.hpp                                                           -
// - afnix:mth module - real function class definitions                      -
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

#ifndef  AFNIX_RFUNCTION_HPP
#define  AFNIX_RFUNCTION_HPP

#ifndef  AFNIX_RFI_HPP
#include "Rfi.hpp"
#endif

namespace afnix {

  /// The Rfunction class is a class that encapuslates the functionality
  /// of a single real function. The class provides the support for the
  /// real function interface by binding a pointer to a function. Optionnaly
  /// a pointer to the function derivate cal also be provided. By default
  /// a null function that always returns 0 is bound at construction.
  /// @author amaury darsch

  class Rfunction : public Rfi {
  private:
    /// the function pointer
    t_rfi p_fptr;
    /// the derivate pointer
    t_rfi p_dptr;

  public:
    /// create a default null function
    Rfunction (void);

    /// create a function by pointer
    /// @param fptr the function pointer
    Rfunction (t_rfi fptr);

    /// create a function by pointers
    /// @param fptr the function pointer
    /// @param dptr the derivate pointer
    Rfunction (t_rfi fptr, t_rfi dptr);

    /// copy construct this real function
    /// @param that the object to copy
    Rfunction (const Rfunction& that);
    
    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a real function to this one
    /// @param that the object to assign
    Rfunction& operator = (const Rfunction& that);

    /// compute the function by value
    /// @param x the evaluation real point
    t_real compute (const t_real x) const;

    /// compute the derivate by value
    /// @param x the evaluation real point
    t_real derivate (const t_real x) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
