// ---------------------------------------------------------------------------
// - TlsClient.hpp                                                           -
// - afnix:tls service - tls client class definition                         -
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

#ifndef  AFNIX_TLSCLIENT_HPP
#define  AFNIX_TLSCLIENT_HPP

#ifndef  AFNIX_TLSSOCKET_HPP
#include "TlsSocket.hpp"
#endif

namespace afnix {

  /// The TlsClient class is the client socket class for the tls protocol.
  /// The class is a wrapper around the tls socket with extra information
  /// needed to setup a secure channel. Once a connection is accepted (in
  /// the tcp sense) the handshake can start. The class is designed to
  /// act as a tls socket object, once the full handshake is completed.
  /// @author amaury darsch

  class TlsClient : public TlsSocket {
  protected:
    /// the tls parameters
    TlsParams d_prms;
    
  public:
    /// create a tls client with a host and port
    /// @param host the host to bind the client
    /// @param port the port to listen
    TlsClient (const String& host, t_word port);

    /// create a tls client by parameters
    /// @param prms the tls parameters
    TlsClient (const TlsParams& prms);
    
    /// @return the class name
    String repr (void) const;
    
    /// @return the tls parameters
    virtual TlsParams getprms (void) const;

  private:
    // make the copy constructor private
    TlsClient (const TlsClient&);
    // make the assignment operator private
    TlsClient& operator = (const TlsClient&);

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
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
