// ---------------------------------------------------------------------------
// - AsnInteger.hpp                                                          -
// - afnix:itu module - asn integer node class definition                    -
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

#ifndef  AFNIX_ASNINTEGER_HPP
#define  AFNIX_ASNINTEGER_HPP

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnInteger class is the asn object class that encodes the integer
  /// primitive. This primitive has a unique encoding with the CER or DER
  /// rule. All encoding use a signed 2-complement form.
  /// @author amaury darsch

  class AsnInteger : public AsnNode {
  private:
    /// the integer value
    Relatif d_rval;

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
    /// create a default asn integer
    AsnInteger (void);

    /// create an asn integer by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnInteger (const AsnNode& node, const Buffer& cbuf);

    /// create an asn integer by value
    /// @param rval the integer value to set
    AsnInteger (const Relatif& rval);

    /// copy construct this asn integer
    /// @param that the asn integer to copy
    AsnInteger (const AsnInteger& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn integer node to this one
    /// param that the asn boolean node to assign
    AsnInteger& operator = (const AsnInteger& that);

    /// reset this asn integer
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// @return the node relatif value
    Relatif torelatif (void) const;

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
