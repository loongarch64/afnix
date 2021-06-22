// ---------------------------------------------------------------------------
// - AsnSet.hpp                                                              -
// - afnix:itu module - asn set node class definition                        -
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

#ifndef  AFNIX_ASNSET_HPP
#define  AFNIX_ASNSET_HPP

#ifndef  AFNIX_SET_HPP
#include "Set.hpp"
#endif

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnSet class is the asn object class that encodes the set
  /// constructed type. The order of elements is not important in a set.
  /// @author amaury darsch

  class AsnSet : public AsnNode {
  private:
    /// the element set
    Set d_eset;

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
    /// create a default asn set
    AsnSet (void);

    /// create an asn set by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnSet (const AsnNode& node, const Buffer& cbuf);

    /// create an asn set by node and node vector
    /// @param node the base asn node
    /// @param vnod the content vector
    AsnSet (const AsnNode& node, const Vector& vnod);

    /// copy construct this asn set
    /// @param that the asn set to copy
    AsnSet (const AsnSet& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn integer node to this one
    /// param that the asn boolean node to assign
    AsnSet& operator = (const AsnSet& that);

    /// reset this asn set
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// @return the number of nodes
    long getnlen (void) const;

    /// @return a node by index
    AsnNode* getnode (const long index) const;

    /// add an asn node to the set
    /// @param node the asn node to add
    void add (AsnNode* node);

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
