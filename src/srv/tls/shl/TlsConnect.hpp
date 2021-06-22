// ---------------------------------------------------------------------------
// - TlsConnect.hpp                                                          -
// - afnix:tls service - tls connect class definition                        -
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

#ifndef  AFNIX_TLSCONNECT_HPP
#define  AFNIX_TLSCONNECT_HPP

#ifndef  AFNIX_TLSSTATE_HPP
#include "TlsState.hpp"
#endif

namespace afnix {

  /// The TlsConnect class is the common connection class for the
  /// tls protocol. The class is designed to negotiate a tls connection
  /// which result in the generation of a tls state object. It is here
  /// the handshake is performed. The TlsConnect class is a complex state
  /// machine those operations varies with the protocol version as well as
  /// the runtime parameters.
  /// @author amaury darsch

  class TlsConnect : public Object {
  protected:
    /// the server flag
    bool  d_sflg;
    /// the tls parameters
    TlsParams d_prms;
    
  public:
    /// create a tls connect by server flag
    /// @param sflg the server flag
    TlsConnect (const bool sflg);

    /// create a tls connect by server flag and parameters
    /// @param sflg the server flag
    /// @param prms the tls parameters
    TlsConnect (const bool sflg, const TlsParams& prms);

    /// @return the class name
    String repr (void) const;

    /// set the tls parameters
    /// @param prms the tls parameters
    virtual void setprms (const TlsParams& prms);
    
    /// connect a tls socket by streams
    /// @param is the input stream
    /// @param os the output stream
    virtual TlsState* connect (InputStream* is, OutputStream* os);

  protected:
    /// @return the initial tls server state
    virtual TlsState* iniss (InputStream* is);
    /// @return the tls server state
    virtual TlsState* getss (InputStream* is, OutputStream* os);
    /// @return the initial tls client state
    virtual TlsState* inics (InputStream* is, OutputStream* os);
    /// @return the tls client state
    virtual TlsState* getcs (InputStream* is, OutputStream* os);

  private:
    // make the copy constructor private
    TlsConnect (const TlsConnect&) =delete;
    // make the assignment operator private
    TlsConnect& operator = (const TlsConnect&) =delete;

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
