// ---------------------------------------------------------------------------
// - TlsCerts.hpp                                                            -
// - afnix:tls service - tls certificate list class definition               -
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

#ifndef  AFNIX_TLSCERTS_HPP
#define  AFNIX_TLSCERTS_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_TLSHBLOCK_HPP
#include "TlsHblock.hpp"
#endif

namespace afnix {

  /// The TlsCerts class is the tls certificate handshake block class. The
  /// class is derived from the handshake message. Note that several
  /// certificates might be part of message.
  /// @author amaury darsch

  class TlsCerts : public TlsInfos {
  private:
    /// the certificate list name
    String d_cert;
    /// the certificate private key
    String d_prvk;
    /// the vector of buffer
    Vector d_vbuf;
    
  public:
    /// create an empty certificate list
    TlsCerts (void);

    /// create a certificate list by names
    /// @param cert the certificate list
    TlsCerts (const String& cert);

    /// create a certificate list by names and key
    /// @param cert the certificate list
    /// @param prvk the private key
    TlsCerts (const String& cert, const String& prvk);

    /// create a certificate list by handshake block
    /// @aparam hblk the handshake block
    TlsCerts (TlsHblock* hblk);
    
    /// @return the class name
    String repr (void) const;

    /// reset this block
    void reset (void);

    /// @return the block info as a plist
    Plist getinfo (void) const;

    /// @return the number of certificates
    virtual long length (void) const;
    
    /// add a certificate list by name
    /// @param cert the list to add
    virtual void addcert (const String& cert);

    /// set the certificate private key
    /// @param prvk the private key
    virtual void setprvk (const String& prvk);
    
    /// get the certificate private key
    virtual Key* getprvk (void) const;

    /// get the certificate public key
    virtual Key* getpubk (void) const;

    /// decode a handshake block
    /// @param hblk the block to decode
    virtual void decode (TlsHblock* hblk);
    
    /// map a certificate to a chunk
    /// @param cidx the certificate index
    virtual TlsChunk tochunk (void) const;

  private:
    // make the copy constructor private
    TlsCerts (const TlsCerts&);
    // make the assignment operator private
    TlsCerts& operator = (const TlsCerts&);

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
