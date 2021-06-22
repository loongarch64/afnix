// ---------------------------------------------------------------------------
// - Error.hpp                                                               -
// - standard object library - error class definition                        -
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

#ifndef AFNIX_ERROR_HPP
#define AFNIX_ERROR_HPP

#ifndef  AFNIX_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace afnix {

  /// The Error class is an extension of the exception class designed for
  /// system specific error which are identified by an integer code. Error
  /// objects are meant to be caught during processing but can still be left
  /// in the stack for high level interception.
  /// @author amaury darsch

  class Error : public Exception {
  public:
    // generic system error
    static const long CODE_OTHR;
    // interrupt received
    static const long CODE_INTR;
    // privilege violations
    static const long CODE_PRIV;
    // broken piep
    static const long CODE_PIPE;
    // name is too long
    static const long CODE_LONG;
    // connection refused
    static const long CODE_CREF;
    // network is unreachable
    static const long CODE_UNRE;
    // resource has timed-out
    static const long CODE_TOUT;
    // resource already in use
    static const long CODE_IUSE;
    // cannot find protocol
    static const long CODE_POTO;
    // invalid inet address
    static const long CODE_ADDR;
    // end point is not connected
    static const long CODE_UCON;
    // connection reset by peer
    static const long CODE_PRST;
    // invalid arguments
    static const long CODE_IARG;
    
  private:
    /// the error code
    long d_code;

  public:
    /// create a default error
    Error (void);
    
    /// create an error with an id, a reason and a code
    /// @param eid    the exception eid
    /// @param reason the reason for this exception
    /// @param code   the error code
    Error (const String& eid, const String& reason, const long code);

    /// copy construct this object
    /// @param that the error to copy
    Error (const Error& that);

    /// assign an error to this one
    /// @param that the error to assign
    Error& operator = (const Error& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;
    
    /// @return the error code
    virtual long getcode (void) const;

  private:
    // make the assignment operator private
    Exception& operator = (const Exception&);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// evaluate an object member by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark) override;    
  };
}

#endif
