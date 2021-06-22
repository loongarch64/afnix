// ---------------------------------------------------------------------------
// - QuarkZone.hpp                                                           -
// - standard object library - quark zone class definition                   -
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

#ifndef  AFNIX_QUARKZONE_HPP
#define  AFNIX_QUARKZONE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The QuarkZone class is an administrative class designed to manage
  /// the class quarks. The class is fed by adding quark in the form of
  /// string internation. Once created, a quark is locally stored by the
  /// zone which can report whether or not a quark exists
  /// @author amaury darsch

  class QuarkZone {
  private:
    /// the zone size
    long  d_size;
    /// the zone length
    long  d_zlen;
    /// the array of quark
    long* p_zone;

  public:
    /// create an empty quark zone
    QuarkZone (void);
  
    /// create a quark zone with an initial size
    /// @param size the initial size
    QuarkZone (const long size);

    /// copy construct this quark zone
    QuarkZone (const QuarkZone& that);

    /// destroy the quark zone. 
    ~QuarkZone (void);

    /// assign a quark zone to this one
    QuarkZone& operator = (const QuarkZone& that);

    /// reset the quark zone
    void reset (void);

    /// @return the number of elements
    long length (void) const;

    /// @return true if the quark exists
    bool exists (const long quark) const;

    /// intern the string argument and return the associated quark
    /// @param value the string to intern
    long intern (const String& value);

    /// @return the interned quark by index
    long get (const long index) const;

    /// @return the interned string by index
    String tostring (const long index) const;
  };
}

#endif
