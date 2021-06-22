// ---------------------------------------------------------------------------
// - AsnOctets.hpp                                                           -
// - afnix:itu module - asn octet string node class definition               -
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

#ifndef  AFNIX_ASNOCTETS_HPP
#define  AFNIX_ASNOCTETS_HPP

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnOctets class is the asn object class that encodes the octet
  /// string type. This type can be encoded either as a primitive or as 
  /// constructed at sender's option. In CER form, the primitive form is
  /// used when the content length is less than 1000 octets, and the 
  /// constructed form is used otherwise. The DER form will always use
  /// the primitive form.
  /// @author amaury darsch

  class AsnOctets : public AsnNode {
  protected:
    /// the octet buffer
    Buffer d_octs;

    /// create an octet string by primitive tag
    /// @param tagn the tag number
    AsnOctets (const t_octa tagn);

    /// write the node body into a buffer
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    void wbody (const t_encr encr, Buffer& buf) const;

    /// write a node body into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    void wbody (const t_encr encr, OutputStream& os) const;

  public:
    /// create a default asn octet string
    AsnOctets (void);
    
    /// create an asn octet string by string
    /// @param sval the initial buffer string
    AsnOctets (const String& sval);

    /// create an asn octet string by buffer
    /// @param obuf the initial octet buffer
    AsnOctets (const Buffer& obuf);

    /// create an octet string node by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnOctets (const AsnNode& node, const Buffer& cbuf);

    /// create an octet string node by node and node vector
    /// @param node the base asn node
    /// @param vnod the content vector
    AsnOctets (const AsnNode& node, const Vector& vnod);

    /// copy construct this asn octet string
    /// @param that the asn octet string to copy
    AsnOctets (const AsnOctets& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an octet string node to this one
    /// param that the asn octet string node to assign
    AsnOctets& operator = (const AsnOctets& that);

    /// reset this asn string
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// write a node into a buffer by encoding rule
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    void write (const t_encr encr, Buffer& buf) const;

    /// write a node into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    void write (const t_encr encr, OutputStream& os) const;

    /// @return a buffer representation
    virtual Buffer tobuffer (void) const;

    /// @return a string representation
    virtual String tostring (void) const;

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
