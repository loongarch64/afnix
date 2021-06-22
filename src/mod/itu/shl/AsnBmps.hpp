// ---------------------------------------------------------------------------
// - AsnBmps.hpp                                                             -
// - afnix:itu module - asn bmp string node class definition                 -
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

#ifndef  AFNIX_ASNBMPS_HPP
#define  AFNIX_ASNBMPS_HPP

#ifndef  AFNIX_ASNOCTETS_HPP
#include "AsnOctets.hpp"
#endif

namespace afnix {

  /// The AsnBmps class is the asn object class that encodes the asn bmp
  /// string primitive also known as the UCS-2 type string. This string
  /// is implemented, after conversion as an octet string. Consequently
  /// the rules for encoding in CER and DER modes are applied.
  /// @author amaury darsch

  class AsnBmps : public AsnOctets {
  public:
    /// create a default asn string
    AsnBmps (void);

    /// create an asn string by value
    /// @param sval the string value
    AsnBmps (const String& sval);

    /// create a string node by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnBmps (const AsnNode& node, const Buffer& cbuf);

    /// create a string node by node and node vector
    /// @param node the base asn node
    /// @param vnod the content vector
    AsnBmps (const AsnNode& node, const Vector& vnod);

    /// copy construct this string node
    /// @param that the string node to copy
    AsnBmps (const AsnBmps& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this asn string
    void reset (void);

    /// @return a string representation
    String tostring (void) const;

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