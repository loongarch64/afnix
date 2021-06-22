// ---------------------------------------------------------------------------
// - Act.hpp                                                                 -
// - afnix:csm service - notary act class definition                         -
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

#ifndef  AFNIX_ACT_HPP
#define  AFNIX_ACT_HPP

#ifndef  AFNIX_IDENTITY_HPP
#include "Identity.hpp"
#endif

#ifndef  AFNIX_SIGNATURE_HPP
#include "Signature.hpp"
#endif

namespace afnix {

  /// The Act class is a blob used for registering an authenticated act
  /// produced by a notary. The act is divided in 3 parts, the protocol
  /// (aka preamble), which contais the notary identification, the parties,
  /// the data of execution as well as the valid dates for the act. The
  /// corpus (aka main part) contains the act itself, while the eschatocol
  /// (aka conclusion) contains the signature of the act by the notary.
  /// The act as produced by a notary can also be verified by a notary, not
  /// necessarily the same, and can establish the status of the act which
  /// can be valid, verified, invalid and/or cannot be verified.
  /// @author amaury darsch

  class Act : public virtual Serial {
  protected:
    /// the signing identity
    Identity d_idty;
    /// the requesting party
    Identity d_rqst;
    /// the signing time
    t_long d_sgnt;
    /// the validty before
    t_long d_tvlb;
    /// the validty after
    t_long d_tvla;
    /// the act signature
    Signature d_sign;
    
  public:
    /// create a null act
    Act (void);

    /// create an act by identity and requester
    /// @param idty the signer identity
    /// @param rqst the requester identity
    Act (const Identity& idty, const Identity& rqst);

    /// copy construct this act
    /// @param that the object to copy
    Act (const Act& that);

    /// assign an act to this one
    /// @param that the object to assign
    Act& operator = (const Act& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// set the validity before time
    /// @param tvlb the before time
    virtual void settvlb (const t_long tvlb);

    /// @return the validity before time
    virtual t_long gettvlb (void) const;
    
    /// set the validity after time
    /// @param tvla the after time
    virtual void settvla (const t_long tvla);

    /// @return the validity after time
    virtual t_long gettvla (void) const;
    
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
