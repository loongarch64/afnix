// ---------------------------------------------------------------------------
// - Pem.hpp                                                                 -
// - afnix:uri service - pem codec class definition                          -
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

#ifndef  AFNIX_PEM_HPP
#define  AFNIX_PEM_HPP

#ifndef  AFNIX_BASEXX_HPP
#include "Basexx.hpp"
#endif

namespace afnix {

  /// The Pem class is a codec that implements the support for PEM encoding
  /// withe the help of the base 64 codec. The class is primarily designed
  /// to code and decode der object.
  /// @author amaury darsch

  class Pem : public Basexx {
  public:
    enum t_pemc
      {
       PEMC_NONE, // unknown data
       PEMC_RSAP, // rsa private key
       PEMC_RSAC, // rsa public key
       PEMC_CERT  // certificate
      };
    
  private:
    /// the pem content
    t_pemc d_pemc;
    
  public:
    /// create a default codec
    Pem (void);

    /// create a codec by content
    /// @param pemc the content type
    Pem (const t_pemc pemc);

    /// @return the class name
    String repr (void) const;

    /// reset the pem encoder
    virtual void reset (void);

    /// @return the pem content type
    virtual t_pemc getpemc (void) const;
    
  protected:
    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, Buffer& ib);

    /// encode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, InputStream& is);

    /// encode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long encode (OutputStream& os, InputStream& is);

    /// decode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long decode (Buffer& ob, Buffer& ib);

    /// decode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long decode (Buffer& ob, InputStream& is);

    /// decode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long decode (OutputStream& os, InputStream& is);
    
  private:
    // make the copy constructor private
    Pem (const Pem&) =delete;
    // make the assignement operator private
    Pem& operator = (const Pem&) =delete;

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
