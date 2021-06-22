// ---------------------------------------------------------------------------
// - AsnBits.hpp                                                             -
// - afnix:itu module - asn bit string node class definition                 -
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

#ifndef  AFNIX_ASNBITS_HPP
#define  AFNIX_ASNBITS_HPP

#ifndef  AFNIX_BITSET_HPP
#include "Bitset.hpp"
#endif

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnBits class is the asn object class that encodes the bit string
  /// type. This type can be encoded either as a primitive or as constructed
  /// at sender's option. In CER form, the primitive form is used when the
  /// content length is less than 1000 octets, and the constructed form is
  /// used otherwise. The DER form will always use the primitive form.
  /// @author amaury darsch

  class AsnBits : public AsnNode {
  private:
    /// the bit string
    Bitset d_bits;

  protected:
    /// write the node body into a buffer
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    void wbody (const t_encr encr, Buffer& buf) const;

    /// write a node body into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    void wbody (const t_encr encr, OutputStream& os) const;

  public:
    /// create a default asn bit string
    AsnBits (void);

    /// create an asn bit string by string
    /// @param sval the initial bitset string
    AsnBits (const String& sval);

    /// create an asn bit string by bitset
    /// @param bset the initial bitset
    AsnBits (const Bitset& bset);

    /// create a bit string node by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnBits (const AsnNode& node, const Buffer& cbuf);

    /// create a bit string node by node and node vector
    /// @param node the base asn node
    /// @param vnod the content vector
    AsnBits (const AsnNode& node, const Vector& vnod);

    /// copy construct this asn bit string
    /// @param that the asn bit string to copy
    AsnBits (const AsnBits& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn bit string node to this one
    /// param that the asn bit string node to assign
    AsnBits& operator = (const AsnBits& that);

    /// reset this asn bit string
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

    /// @return a bitset reprsentation
    Bitset tobits (void) const;

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
