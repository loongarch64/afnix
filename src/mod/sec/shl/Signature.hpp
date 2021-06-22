// ---------------------------------------------------------------------------
// - Signature.hpp                                                           -
// - afnix:sec module - message signature class definition                   -
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

#ifndef  AFNIX_SIGNATURE_HPP
#define  AFNIX_SIGNATURE_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

namespace afnix {

  /// The Signature class is a container class designed to store a message
  /// signature. The signature object is produced by a signing process,
  /// implemented in the form of a digital signature algorithm such like
  /// RSA or DSA.
  /// @author amaury darsch

  class Signature : public virtual Serial {
  public:
    /// the signature type
    enum t_sign : t_byte {
      SIGN_SNIL = 0x00U, // null signature
      SIGN_SDSA = 0x01U  // DSA signature
    };

    /// the signature index accessor
    enum t_isgn {
      SDSA_SCMP, // dsa s component
      SDSA_RCMP  // dsa r component
    };

  private:
    /// the signature type
    t_sign d_type;
    /// the signature structure
    union {
      struct s_snil* p_snil;
      struct s_sdsa* p_sdsa;
    };
    /// the public key
    Key d_pubk;
    
  public:
    /// create a default signature
    Signature (void);

    /// create a signature by type and object vector
    /// @param type the signature type
    /// @param ovec the object vector
    Signature (const t_sign type, const Vector& ovec);
    
    /// copy construct this signature
    /// @param that the signature to copy
    Signature (const Signature& that);

    /// destroy the signature
    ~Signature (void);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// assign a signature to this one
    /// @param that the signature to assign
    Signature& operator = (const Signature& that);

    /// @return the serial did
    t_word getdid (void) const override;
    
    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this key
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this key
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// reset this signature
    virtual void reset (void);

    /// @return the signature type
    virtual t_sign gettype (void) const;
    
    /// @return a formatted component by type
    virtual String format (const t_isgn type) const;

    /// @return a relatif component by type
    virtual Relatif getrcmp (const t_isgn type) const;

    /// set the signature public key
    /// @param pubk the public key
    virtual void setpubk (const Key& pubk);

    /// @return the signature public key
    virtual Key getpubk (void) const;

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
