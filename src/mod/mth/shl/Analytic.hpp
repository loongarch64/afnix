// ---------------------------------------------------------------------------
// - Analytic.hpp                                                            -
// - afnix:mth module - system analytic class definition                     -
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

#ifndef  AFNIX_ANALYTIC_HPP
#define  AFNIX_ANALYTIC_HPP

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif

#ifndef  AFNIX_RMI_HPP
#include "Rmi.hpp"
#endif

#ifndef  AFNIX_LOGGER_HPP
#include "Logger.hpp"
#endif

namespace afnix {

  /// The Analytic class is specialized class design for storing analytic data
  /// as well as logging pertinent information about a system candidate. In
  /// particular, the analytic provides the methods for checking and logging
  /// a system consistenncy prior being solved.
  /// @author amaury darsch

  class Analytic : public Object {
  protected:
    /// the analytic logger
    Logger* p_slg;

  public:
    /// create a default analytic
    Analytic (void);

    /// create an analytic with a logger
    /// @param slg the system logger
    Analytic (Logger* slg);
 
    /// copy construct this analytic
    /// @param that the object to copy
    Analytic (const Analytic& that);

    /// destroy this analytic
    ~Analytic (void);

    /// assign an analytic to this one
    /// @param that the object to assign
    Analytic& operator = (const Analytic& that);

    /// @return the class name
    String repr (void) const;

    /// set the analytic logger
    /// @param slg the system logger
    virtual void setslg (Logger* slg);

    /// @return the analytic logger
    virtual Logger* getslg (void) const;

    /// check a vector for nan and log
    /// @param x the vector to check
    virtual bool isnan (const Rvi& x) const;

    /// check a matric for null row and log
    /// @param m the matrix to check
    virtual bool isnrow (const Rmi& m) const;

    /// check a matric for null column and log
    /// @param m the matrix to check
    virtual bool isncol (const Rmi& m) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
