// ---------------------------------------------------------------------------
// - TlsSocket.hpp                                                           -
// - afnix:tls service - tls socket class definition                         -
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

#ifndef  AFNIX_TLSSOCKET_HPP
#define  AFNIX_TLSSOCKET_HPP

#ifndef  AFNIX_TLSINPUT_HPP
#include "TlsInput.hpp"
#endif

#ifndef  AFNIX_TLSOUTPUT_HPP
#include "TlsOutput.hpp"
#endif

#ifndef  AFNIX_TCPSOCKET_HPP
#include "TcpSocket.hpp"
#endif

#ifndef  AFNIX_SOCKETSTREAM_HPP
#include "SocketStream.hpp"
#endif

namespace afnix {

  /// The TlsSocket class is the socket stream object which is implemented
  /// with the help of the tls input and output objects. The class is built
  /// with the help of a regular socket and a tls state. Thus the tls socket
  /// provides the same socket stream method, but operates in the context of
  /// the tls. Note that the socket stream interface provides also support
  /// for socket information.
  /// @author amaury darsch

  class TlsSocket : public SocketStream, public TlsInfos {
  protected:
    /// the tcp socket
    TcpSocket* p_tcps;
    /// the tls state
    TlsState* p_ssta;
    /// the input stream
    TlsInput* p_is;
    /// the output stream
    TlsOutput* p_os;

  public:
    /// create a null tls socket
    TlsSocket (void);
    
    /// create a tls socket by socket and state
    /// @param tcps the tcp socket
    /// @param ssta the tls state
    TlsSocket (TcpSocket* tcps, TlsState* ssta);

    /// destroy this tls socket
    ~TlsSocket (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this object
    void reset (void) override;

    /// @return the input stream channel
    InputStream* getis (void) override;

    /// @return the output stream channel
    OutputStream* getos (void) override;

    /// @return the info as a plist
    Plist getinfo (void) const override;

    /// close this socket stream
    bool close (void) override;
    
    /// @return true if we are at the eos
    bool iseos (void) const override;
    
    /// check if we can read one character
    bool valid (void) const override;

    /// @return the next available character
    char read (void) override;

    /// write one character on the socket stream.
    /// @param value the character to write  
    long write (const char value) override;

    /// write a data buffer to the socket stream
    /// @param data the data to write
    long write (const char* data) override;
    
    /// @return true if we have an ipv6 socket
    bool isipv6 (void) const override;

    /// @return true if we can broadcast messages
    bool isbcast (void) const override;

    /// @return true if the socket is open
    bool isopen (void) const override;

    /// @return the socket address
    Address* getsockaddr (void) const override;

    /// @return the socket port
    t_word getsockport (void) const override;

    /// @return the socket authority
    String getsockauth (void) const override;

    /// @return the peer address
    Address* getpeeraddr (void) const override;

    /// @return the peer port
    t_word getpeerport (void) const override;

    /// @return the peer authority
    String getpeerauth (void) const override;

    /// @return the socket protocol
    String getprotocol (void) const override;
    
    /// @return the tls state
    virtual TlsState* getstate (void) const;

  protected:
    /// bind the tls socket
    /// @param tcps the tcp socket
    /// @param ssta the tls state
    virtual void bind (TcpSocket* tcps, TlsState* ssta);
    
  private:
    // make the copy constructor private
    TlsSocket (const TlsSocket&) =delete;
    // make the assignment operator private
    TlsSocket& operator = (const TlsSocket&) =delete;

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
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
