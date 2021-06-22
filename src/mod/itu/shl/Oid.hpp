// ---------------------------------------------------------------------------
// - Oid.hpp                                                                 -
// - afnix:itu module - oid class definition                                 -
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

#ifndef  AFNIX_OID_HPP
#define  AFNIX_OID_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Oid class is a base class that represents the X500 object identifier
  /// which is used in the ASN.1 encoding and in the X509 standard. An oid
  /// is simply represented by a vector of subidentifiers.
  /// @author amaury darsch

  class Oid : public virtual Object{
  private:
    /// the number of sid
    long d_nsid;
    /// the sid vector
    t_octa* p_sidv;

  public:
    /// create a default oid
    Oid (void);

    /// copy construct this oid
    /// @param that the oid to copy
    Oid (const Oid& that);

    /// destroy this oid
    ~Oid (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an oid to this one
    /// param that the oid to assign
    Oid& operator = (const Oid& that);

    /// reset this oid object
    void reset (void);

    /// @return the number of sid
    long length (void) const;

    /// @return a sid by index
    t_octa get (const long index) const;

    /// add a sid to the oid vector
    /// @param sid the sid to add
    void add (const t_octa sid);

    /// format the oid as a string
    String format (void) const;

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
