// ---------------------------------------------------------------------------
// - AsnBuffer.hpp                                                           -
// - afnix:itu module - asn buffer node class definition                     -
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

#ifndef  AFNIX_ASNBUFFER_HPP
#define  AFNIX_ASNBUFFER_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_BITSET_HPP
#include "Bitset.hpp"
#endif

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The AsnBuffer class is the asn object class that provides a generic
  /// implementation of an asn structure. The class acts as a simple encoder
  /// and decoder with special facilities to retarget the buffer content.
  /// @author amaury darsch

  class AsnBuffer : public AsnNode {
  public:
    /// parse a buffer as a node vector
    /// @param buf the buffer to parse
    static Vector tovnod (const Buffer& buf);

  private:
    /// the content buffer
    Buffer d_cbuf;
    /// the content vector
    Vector d_vnod;

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
    /// create a default asn buffer
    AsnBuffer (void);

    /// create an asn buffer by stream
    /// @param is the input stream to read
    AsnBuffer (InputStream* is);

    /// create an asn buffer by buffer
    /// @param buf the buffer to read
    AsnBuffer (const Buffer& buf);

    /// create an asn buffer by bitset
    /// @param bset the bitset to read
    AsnBuffer (const Bitset& bset);

    /// copy construct this asn buffer
    /// @param that the asn buffer to copy
    AsnBuffer (const AsnBuffer& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn buffer node to this one
    /// param that the asn buffer node to assign
    AsnBuffer& operator = (const AsnBuffer& that);

    /// reset this asn buffer node
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// parse an input stream
    /// @param is the stream to parse
    bool parse (InputStream* is);

    /// parse a buffer
    /// @param buf the buffer to parse
    bool parse (const Buffer& buf);

    /// parse a bitset as a buffer
    /// @param bset the bitset to parse
    bool parse (const Bitset& bset);

    /// @return an asn node mapping of this asn buffer
    AsnNode* mapnode (void) const;

    /// map an asn node by tag number
    /// @param tagn the tag number to map the node
    AsnNode* mapnode (const t_octa tagn) const;

    /// map an asn node by class and tag number
    /// @param ncls the node class
    /// @param tagn the tag number to map the node
    AsnNode* mapnode (const t_ncls ncls, const t_octa tagn) const;

    /// @return the content buffer
    Buffer* getcbuf (void) const;

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
