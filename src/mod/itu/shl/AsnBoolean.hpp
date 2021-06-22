// ---------------------------------------------------------------------------
// - AsnBoolean.hpp                                                          -
// - afnix:itu module - asn boolean node class definition                    -
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

#ifndef  AFNIX_ASNBOOLEAN_HPP
#define  AFNIX_ASNBOOLEAN_HPP

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnBoolean class is the asn object class that encodes the boolean
  /// primitive. This primitive has a unique encoding with the CER or DER
  /// rule, but the BER rule can support any byte value for the true value.
  /// @author amaury darsch

  class AsnBoolean : public AsnNode {
  private:
    /// the byte bool value
    t_byte d_bval;

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
    /// create a default asn boolean
    AsnBoolean (void);

    /// create an asn boolean by value
    /// @param bval the boolean value to set
    AsnBoolean (const bool bval);

    /// create a boolean node by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnBoolean (const AsnNode& node, const Buffer& cbuf);

    /// copy construct this asn boolean
    /// @param that the asn boolean to copy
    AsnBoolean (const AsnBoolean& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn boolean node to this one
    /// param that the asn boolean node to assign
    AsnBoolean& operator = (const AsnBoolean& that);

    /// reset this boolean node
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// @return the node boolean value
    bool tobool (void) const;

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
