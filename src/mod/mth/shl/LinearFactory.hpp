// ---------------------------------------------------------------------------
// - LinearFactory.hpp                                                       -
// - afnix:mth module - linear solver factory class definitions              -
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

#ifndef  AFNIX_LINEARFACTORY_HPP
#define  AFNIX_LINEARFACTORY_HPP

#ifndef  AFNIX_SOLVERFACTORY_HPP
#include "SolverFactory.hpp"
#endif

namespace afnix {

  /// The LinearFactory class is an implementation of the solver factory
  /// for the linear solver object. The class holds the necessary flags to
  /// create a new linear solver
  /// @author amaury darsch

  class LinearFactory : public SolverFactory {    
  protected:
    /// the verification flag
    bool d_avf;
    /// the precondition flag
    bool d_pcf;
    /// the solver type
    String d_lst;

  public:
    /// create a default linear factory
    LinearFactory (void);

    /// create a linear factory by logger
    /// @param slg the system logger
    LinearFactory (Logger* slg);

    /// create a linear factory by type
    /// @param lst the solver type
    LinearFactory (const String& lst);

    /// create a linear factory by flags
    /// @param avf the verification flag
    /// @param pcf the preconditionning flag
    LinearFactory (const bool avf, const bool pcf);

    /// create a linear factory by flags and type
    /// @param avf the verification flag
    /// @param pcf the preconditionning flag
    /// @param lst the solver type
    LinearFactory (const bool avf, const bool pcf, const String& lst);

    /// copy construct this factory
    /// @param that the object to copy
    LinearFactory (const LinearFactory& that);

    /// assign a factory to this one
    /// @param that the object to assign
    LinearFactory& operator = (const LinearFactory& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return a new solver
    Solver* newslv (void) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
