// ---------------------------------------------------------------------------
// - X509.hpp                                                                -
// - afnix:tls service - X509 certificate class definition                   -
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

#ifndef  AFNIX_X509_HPP
#define  AFNIX_X509_HPP

#ifndef  AFNIX_PKI_HPP
#include "Pki.hpp"
#endif

#ifndef  AFNIX_XKEY_HPP
#include "Xkey.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_XALGO_HPP
#include "Xalgo.hpp"
#endif

#ifndef  AFNIX_XEXTN_HPP
#include "Xextn.hpp"
#endif

namespace afnix {

  /// The X509 class is the base class used to represent a certificate
  /// as defined by the ITU X509 specification. The class does not make
  /// any assumption about the content and does not follow a particular
  /// profile, which therefore makes it a good candidate for reading a
  /// certificate, but certainly not for verifying it. Additional information
  /// may be found in RFC 5280 and RFC 4055.
  /// @author amaury darsch

  class X509 : public Pki {
  protected:
    /// the certificate version
    long    d_vers;
    /// the serial number
    Relatif d_snum;
    /// the signature algorithm
    Xalgo   d_slgo;
    /// the issuer plist
    Plist   d_ilst;
    /// the subject plist
    Plist   d_slst;
    /// the validity before
    t_long  d_tvlb;
    /// the validity after
    t_long  d_tvla;
    /// the subject key algorithm
    Xalgo   d_spka;
    /// the subject key info
    Bitset  d_spki;
    /// the issuer unique id
    Bitset* p_iuid;
    /// the subject unique id
    Bitset* p_suid;
    /// the extension vector
    Vector  d_exts;
    /// the signature algorithm
    Xalgo   d_csgo;
    /// the signature bit string
    Bitset  d_sign;

  public:
    /// create an empty certificate
    X509 (void);

    /// create a certificate by path
    /// @param path the input file path
    X509 (const String& path);

    /// create a certificate by buffer
    /// @param xbuf the buffer to decode
    X509 (const Buffer& xbuf);
    
    /// copy construct this certificate
    /// @param the certificate to copy
    X509 (const X509& that);

    /// copy move this certificate
    /// @param the certificate to move
    ///X509 (X509&& that) noexcept;

    /// destroy this certificate
    ~X509 (void);

    /// assign a certificate to this one
    /// @param that the certificate to assign
    X509& operator = (const X509& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this certificate
    void reset (void);

    /// @return the certificate version
    virtual long getvers (void) const;

    /// @return the certificate serial number
    virtual Relatif getsnum (void) const;

    /// @return the signature algorithm
    virtual Xalgo getslgo (void) const;

    /// @return the issuer plist
    virtual Plist getilst (void) const;

    /// @return the subject plist
    virtual Plist getslst (void) const;

    /// @return the validity before time
    virtual t_long gettvlb (void) const;

    /// @return the validity after time
    virtual t_long gettvla (void) const;

    /// @return the subject key algorithm
    virtual Xalgo getspka (void) const;

    /// @return the subject key info
    virtual Bitset getspki (void) const;

    /// @return the subject key info
    virtual Xkey getskey (void) const;

    /// @return the issuer unique id
    virtual Bitset* getiuid (void) const;

    /// @return the subject unique id
    virtual Bitset* getsuid (void) const;

    /// @return the certificate extension vector
    virtual Vector getexts (void) const;

  protected:
    // encode a pki buffer
    bool encode (void);
    
    /// decode a pki buffer
    bool decode (void);
    
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
