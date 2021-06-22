// ---------------------------------------------------------------------------
// - Xextn.hpp                                                               -
// - afnix:tls service - X509 extension class definition                     -
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

#ifndef  AFNIX_XEXTN_HPP
#define  AFNIX_XEXTN_HPP

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The Xextn class is a generic class design to hold the X509 extension
  /// structure. The oid object is the extension identifier. An optional
  /// critical flag is associaed with the structure which comes along with
  /// the octet string structure associated with oid. Standard certificate
  /// extensions are described in RFC 5280.
  /// @author amaury darsch

  class Xextn : public virtual Object {
  protected:
    /// the extension identifier
    String   d_eid;
    /// the critical flag
    bool     d_ecf;
    /// the extension octet node
    AsnNode* p_eon;

  public:
    /// create an empty extension
    Xextn (void);

    /// create the extension by oid and node
    /// @param eid the extension oid
    /// @param eon the extension node
    Xextn (const String& eid, AsnNode* eon);

    /// create the extension by oid, flag  and node
    /// @param eid the extension oid
    /// @param ecf the extension critical flag
    /// @param eon the extension node
    Xextn (const String& eid, const bool ecf, AsnNode* eon);

    /// copy construct this object
    /// @param that the object to copy
    Xextn (const Xextn& that);

    /// destroy this extension
    ~Xextn (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an object to this one
    /// @param that the object to assign
    Xextn& operator = (const Xextn& that);

    /// reset this extension identifier
    virtual void reset (void);

    /// @return the extension oid
    virtual String geteid (void) const;

    /// @return the critical flag 
    virtual bool getecf (void) const;

    /// set the extension by oid and object
    /// @param eid the extension oid
    /// @param ecf the extension critical flag
    /// @param eon the extension node
    virtual void set (const String& eid, const bool ecf, AsnNode* eon);

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
