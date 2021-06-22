// ---------------------------------------------------------------------------
// - TlsData.hpp                                                             -
// - afnix:tls service - tls appplication data class definition              -
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

#ifndef  AFNIX_TLSDATA_HPP
#define  AFNIX_TLSDATA_HPP

#ifndef  AFNIX_TLSMESSAGE_HPP
#include "TlsMessage.hpp"
#endif

namespace afnix {

  /// The TlsData class is the application data class for the tls
  /// protocol. The class is valid for both client and server side of
  /// the tls and is used to for the transmission of secured data.
  /// @author amaury darsch

  class TlsData : public TlsMessage {
  public:
    /// create an empty message
    TlsData (void);

    /// create a data message by record
    /// @param rcd the tls record
    TlsData (TlsRecord* rcs);

    /// create a data message by version
    /// @param vmaj the major version
    /// @param vmin the minor version
    TlsData (const t_byte vmaj, const t_byte vmin);

    /// copy construct this message
    /// @param that the object to copy
    TlsData (const TlsData& that);

    /// assign a message to this one
    /// @param that the record to assign
    TlsData& operator = (const TlsData& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this message
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
