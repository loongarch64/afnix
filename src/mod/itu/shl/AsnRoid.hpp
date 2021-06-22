// ---------------------------------------------------------------------------
// - AsnRoid.hpp                                                           -
// - afnix:itu module - asn relative oid node class definition               -
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

#ifndef  AFNIX_ASNROID_HPP
#define  AFNIX_ASNROID_HPP

#ifndef  AFNIX_OID_HPP
#include "Oid.hpp"
#endif

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnRoid class is the asn object class that encodes the object
  /// relative identifier primitive. This primitive has a unique encoding with
  /// the CER or DER rule. The oid is built as a vector of subidentifiers (sid).
  /// Each sid is represented as an octa (64 bits) value. The difference with
  /// the oid object is to be found in the encoding of the first 2 sid.
  /// @author amaury darsch

  class AsnRoid : public AsnNode {
  private:
    /// the roid object
    Oid d_roid;

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
    /// create a default asn oid
    AsnRoid (void);

    /// create an oid node by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnRoid (const AsnNode& node, const Buffer& cbuf);

    /// copy construct this asn oid
    /// @param that the asn oid to copy
    AsnRoid (const AsnRoid& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn oid node to this one
    /// param that the asn oid node to assign
    AsnRoid& operator = (const AsnRoid& that);

    /// reset this asn roid node
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// @return the number of sid
    long getnsid (void) const;

    /// @return a sid by index
    t_octa getsid (const long index) const;

    /// add a sid to the oid vector
    /// @param sid the sid to add
    void add (const t_octa sid);

    /// @return the associated oid object
    Oid getoid (void) const;

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
