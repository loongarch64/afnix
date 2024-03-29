// ---------------------------------------------------------------------------
// - AsnGtm.hpp                                                              -
// - afnix:itu module - asn generalized time node class definition           -
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

#ifndef  AFNIX_ASNGTM_HPP
#define  AFNIX_ASNGTM_HPP

#ifndef  AFNIX_ASNNODE_HPP
#include "AsnNode.hpp"
#endif

namespace afnix {

  /// The AsnGtm class is the asn object class that encodes the generalized
  /// time primitive. This primitive is encoded from its equivalent string
  /// representation. Although, the constructed mode is authorized, it does
  /// not make that much sense to use it.
  /// @author amaury darsch

  class AsnGtm : public AsnNode {
  private:
    /// the time clock
    t_long d_tclk;
    /// the fraction part
    long   d_frac;
    /// the utc flag
    bool   d_utcf;
    /// the zone flag
    bool   d_zflg;
    /// the zone part
    long   d_zone;

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
    /// create a default asn generalized time
    AsnGtm (void);

    /// create an asn generalized time by value
    /// @param sval the string value to use
    AsnGtm (const String& sval);

    /// create an asn generalized time by node and buffer
    /// @param node the base asn node
    /// @param cbuf the content buffer
    AsnGtm (const AsnNode& node, const Buffer& cbuf);

    /// copy construct this asn generalize time
    /// @param that the asn generalized time to copy
    AsnGtm (const AsnGtm& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an asn generalized time node to this one
    /// param that the asn generalized time node to assign
    AsnGtm& operator = (const AsnGtm& that);

    /// reset this node
    void reset (void);

    /// get the node content length
    /// @param encr the encoding rule
    t_long getclen (const t_encr encr) const;

    /// @return true if the utc flag is set
    bool isutc (void) const;

    /// set the date by value
    /// @param sval the date to set
    void setdate (const String& sval);

    /// @return the node time value
    t_long totime (void) const;

    /// @return the node string value
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
