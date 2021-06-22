// ---------------------------------------------------------------------------
// - AsnNode.hpp                                                             -
// - afnix:itu module - asn base node class definition                       -
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

#ifndef  AFNIX_ASNNODE_HPP
#define  AFNIX_ASNNODE_HPP

#ifndef  AFNIX_ITEM_HPP
#include "Item.hpp"
#endif

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The AsnNode class is the base class used to represent the asn tree.
  /// The structure of the node is defined in ITU-T X.690 recommendation.
  /// This implementation supports 64 bits tag number with natural machine
  /// length encoding. The Canonical Encoding Rule (CER) and Distinguished
  /// Encoding Rule (DER) are defined by the class. Since ASN.1 provides
  /// several encoding schemes, the class is designed to be as generic as 
  /// possible but does not provides the mechanism for changing from one 
  /// representation to another although it is perfectly valid to read a 
  /// DER representation and write it in the CER form.
  /// @author amaury darsch

  class AsnNode : public virtual Object {
  public:
    /// the encoding rules
    enum t_encr {
      ASN_BER,  // basic encoding rule
      ASN_CER,  // canonical encoding rule
      ASN_DER   // distinguished encoding rule
    };

    /// the node class
    enum t_ncls {
      CLS_UNIV, // class 00 : universal class
      CLS_APPL, // class 01 : application class
      CLS_CTXS, // class 10 : context specific class
      CLS_PRIV  // class 11 : private class
    };

    /// the primitive boolean tag
    static const t_octa ASN_UNIV_BOOL = 0x0000000000000001ULL;
    /// the primitive integer tag
    static const t_octa ASN_UNIV_INTG = 0x0000000000000002ULL;
    /// the primitive bits string tag
    static const t_octa ASN_UNIV_BITS = 0x0000000000000003ULL;
    /// the primitive octet string tag
    static const t_octa ASN_UNIV_OCTS = 0x0000000000000004ULL;
    /// the primitive null tag
    static const t_octa ASN_UNIV_NULL = 0x0000000000000005ULL;
    /// the primitive oid tag
    static const t_octa ASN_UNIV_OIDN = 0x0000000000000006ULL;
    /// the primitive descriptor tag
    static const t_octa ASN_UNIV_ODES = 0x0000000000000007ULL;
    /// the primitive instance tag
    static const t_octa ASN_UNIV_INST = 0x0000000000000008ULL;
    /// the primitive real tag
    static const t_octa ASN_UNIV_REAL = 0x0000000000000009ULL;
    /// the primitive enumeration tag
    static const t_octa ASN_UNIV_ENUM = 0x000000000000000AULL;
    /// the primitive pdv tag
    static const t_octa ASN_UNIV_EPDV = 0x000000000000000BULL;
    /// the primitive utf string tag
    static const t_octa ASN_UNIV_UTFS = 0x000000000000000CULL;
    /// the primitive roid tag
    static const t_octa ASN_UNIV_ROID = 0x000000000000000DULL;
    /// the primitive sequence tag
    static const t_octa ASN_UNIV_SEQO = 0x0000000000000010ULL;
    /// the primitive set tag
    static const t_octa ASN_UNIV_SETO = 0x0000000000000011ULL;
    /// the primitive numeric string tag
    static const t_octa ASN_UNIV_NUMS = 0x0000000000000012ULL;
    /// the primitive printable string
    static const t_octa ASN_UNIV_PRTS = 0x0000000000000013ULL;
    /// the primitive teletex string
    static const t_octa ASN_UNIV_TLXS = 0x0000000000000014ULL;
    /// the primitive video text string tag
    static const t_octa ASN_UNIV_VIDS = 0x0000000000000015ULL;
    /// the primitive ias5 string tag
    static const t_octa ASN_UNIV_IA5S = 0x0000000000000016ULL;
    /// the primitive utc time tag
    static const t_octa ASN_UNIV_UTCS = 0x0000000000000017ULL;
    /// the primitive gmt time tag
    static const t_octa ASN_UNIV_GTMS = 0x0000000000000018ULL;
    /// the primitive graphic string tag
    static const t_octa ASN_UNIV_GRPS = 0x0000000000000019ULL;
    /// the primitive visible string tag
    static const t_octa ASN_UNIV_VISS = 0x000000000000001AULL;
    /// the primitive general string tag
    static const t_octa ASN_UNIV_GENS = 0x000000000000001BULL;
    /// the primitive universal string tag
    static const t_octa ASN_UNIV_UNVS = 0x000000000000001CULL;
    /// the primitive bmp string tag
    static const t_octa ASN_UNIV_BMPS = 0x000000000000001EULL;
    
    /// map an item into an encoding rule type
    /// @param item the item to map
    static t_encr toencr (const Item& item);

    /// map an item into a node class type
    /// @param item the item to map
    static t_ncls toncls (const Item& item);

  protected:
    /// the node class
    t_ncls d_ncls;
    /// the constructed flag
    bool   d_cstf;
    /// the tag number
    t_octa d_tagn;
    /// the indefinite content length flag
    bool   d_iclf;

    /// create a null node
    AsnNode (void);

    /// create a node by primitive tag
    /// @param tagn the tag number
    AsnNode (const t_octa tagn);

    /// create a node by primitive tag and flag
    /// @param tagn the tag number
    /// @param cstf the constructed flag
    AsnNode (const t_octa tagn, const bool cstf);

    /// copy construct this asn node
    /// @param that the node to copy
    AsnNode (const AsnNode& that);

    /// assign a node to this one
    /// @param that the node to assign
    AsnNode& operator = (const AsnNode& that);

    /// write the node header into a buffer
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    virtual void whead (const t_encr encr, Buffer& buf) const;

    /// write a node header into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    virtual void whead (const t_encr encr, OutputStream& os) const;

    /// write the node footer into a buffer
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    virtual void wfoot (const t_encr encr, Buffer& buf) const;

    /// write a node footer into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    virtual void wfoot (const t_encr encr, OutputStream& os) const;

    /// write the node body into a buffer
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    virtual void wbody (const t_encr encr, Buffer& buf) const =0;

    /// write a node body into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    virtual void wbody (const t_encr encr, OutputStream& os) const =0;

  public:
    /// @return the class name
    String repr (void) const;

    /// reset this node
    virtual void reset (void);

    /// @return the node class
    virtual t_ncls getcls (void) const;

    /// @return true if the node is primitive
    virtual bool isprm (void) const;

    /// @return true if the node is constructed
    virtual bool iscst (void) const;

    /// @return true if the indefinite content length is set
    virtual bool isicl (void) const;

    /// @return the node tag number
    virtual t_octa gettagn (void) const;

    /// get the node content length
    virtual t_long getclen (void) const;

    /// get the node content length
    /// @param encr the encoding rule
    virtual t_long getclen (const t_encr encr) const =0;

    /// get the total node length
    virtual t_long length (void) const;
    
    /// get the total node length
    /// @param encr the encoding rule
    virtual t_long length (const t_encr encr) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    virtual void write (Buffer& buf) const;

    /// write a node into a buffer by encoding rule
    /// @param encr the encoding rule
    /// @param buf  the buffer to write
    virtual void write (const t_encr encr, Buffer& buf) const;

    /// write a node into an output stream
    /// @param os the output stream to write
    virtual void write (OutputStream& os) const;

    /// write a node into an output stream
    /// @param encr the encoding rule
    /// @param os   the output stream to write
    virtual void write (const t_encr encr, OutputStream& os) const;

  public:

    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
