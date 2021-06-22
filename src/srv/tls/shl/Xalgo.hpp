// ---------------------------------------------------------------------------
// - Xalgo.hpp                                                               -
// - afnix:tls service - X509 algorithm class definition                     -
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

#ifndef  AFNIX_XALGO_HPP
#define  AFNIX_XALGO_HPP

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The Xalgo class is a generic class design to hold the X509 algorithm
  /// structure. The oid object is the algorithm identifier. An optional
  /// algorithm parameter object, in the form of an asn node can also be
  /// attached. It is up to the certificate profile to decode the oid and
  /// the optional parameters.
  /// @author amaury darsch

  class Xalgo : public virtual Object {
  protected:
    /// the algorithm identifier
    String   d_aid;
    /// the algorithm parameters
    AsnNode* p_aop;

  public:
    /// create an empty algorithm
    Xalgo (void);

    /// create the algorithm by oid and parameters
    /// @param aid the algorithm oid
    /// @param aop the algorithm parameters
    Xalgo (const String& aid, AsnNode* aop);

    /// copy construct this obejct
    /// @param that the object to copy
    Xalgo (const Xalgo& that);

    /// destroy this algorithm
    ~Xalgo (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an object to this one
    /// @param that the object to assign
    Xalgo& operator = (const Xalgo& that);

    /// reset this algorithm identifier
    virtual void reset (void);

    /// @return the algorithm identifier
    virtual String getaid (void) const;

    /// set the algorithm by oid and parameters
    /// @param aid the algorithm oid
    /// @param aop the algorithm parameters
    virtual void set (const String& aid, AsnNode* aop);

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
