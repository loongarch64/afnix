// ---------------------------------------------------------------------------
// - TlsCspec.hpp                                                            -
// - afnix:tls service - tls change [cipher spec] class definition           -
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

#ifndef  AFNIX_TLSCSPEC_HPP
#define  AFNIX_TLSCSPEC_HPP

#ifndef  AFNIX_TLSMESSAGE_HPP
#include "TlsMessage.hpp"
#endif

namespace afnix {

  /// The TlsCspec class is the change cipher spec class for the tls
  /// protocol. The class is valid for both client and server side of
  /// the tls and is used to instruct the engine to switch to its newly
  /// negotiated cipher.
  /// @author amaury darsch

  class TlsCspec : public TlsMessage {
  public:
    /// create an empty message
    TlsCspec (void);

    /// create a message by record
    /// @param rcd the tls record
    TlsCspec (TlsRecord* rcs);

    /// create a message by version
    /// @param vmaj the major version
    /// @param vmin the minor version
    TlsCspec (const t_byte vmaj, const t_byte vmin);

    /// copy construct this message
    /// @param that the object to copy
    TlsCspec (const TlsCspec& that);

    /// assign a message to this one
    /// @param that the record to assign
    TlsCspec& operator = (const TlsCspec& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this cspec
    void reset (void);

    /// @return the message info as a plist
    Plist getinfo (void) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
