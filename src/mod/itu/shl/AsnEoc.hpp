// ---------------------------------------------------------------------------
// - AsnEoc.hpp                                                              -
// - afnix:itu module - asn eoc node class definition                        -
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

#ifndef  AFNIX_ASNEOC_HPP
#define  AFNIX_ASNEOC_HPP

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnEoc class is the asn object class that encodes the eoc
  /// or end-of-content primitive. This primitive is almost never used
  /// but its encoding is used with the indefinite length encoding.
  /// @author amaury darsch

  class AsnEoc : public AsnNode {
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
    /// create a default eoc
    AsnEoc (void);

    /// copy construct this asn eoc
    /// @param that the asn eoc to copy
    AsnEoc (const AsnEoc& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn eoc node to this one
    /// param that the asn eoc node to assign
    AsnEoc& operator = (const AsnEoc& that);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
