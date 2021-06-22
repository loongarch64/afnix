// ---------------------------------------------------------------------------
// - Sockopt.hpp                                                          -
// - afnix:net module - socket options class definition                      -
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

#ifndef  AFNIX_SOCKOPT_HPP
#define  AFNIX_SOCKOPT_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Sockopt classs is a base class that defines the socket options.
  /// The socket options can be used directly by the socket or by the socket
  /// parameters object.
  /// @author amaury darsch

  class Sockopt : public virtual Object {
  public:
    /// the socket options
    enum t_so {
      SO_RADR = 0,  // enable address reuse
      SO_RPRT = 1,  // enable port reuse
      SO_BCST = 2,  // enable packet broadcast
      SO_DNTR = 3,  // bypass routing table
      SO_KLIV = 4,  // set connection test
      SO_LIGT = 5,  // set linger time
      SO_RSIZ = 6,  // receive buffer size
      SO_SSIZ = 7,  // send buffer size
      SO_SHOP = 8,  // socket hop limit
      SO_MLBK = 9,  // multicast use lopback
      SO_MHOP = 10, // multicast hop limit
      SO_MSIZ = 11, // tcp maximum segment size
      SO_NDLY = 12  // disable naggle algorithm
    };
    
  public:
    /// create a default socket option
    Sockopt (void);
    
    /// copy move this socket option
    /// @param that the object to move
    Sockopt (Sockopt&& that) noexcept;

    /// move a socket option into this one
    /// @param that the ojbect to move
    Sockopt& operator = (Sockopt&& that) noexcept;

    /// set a socket option
    /// @param opt the socket option
    /// @param val the option value
    virtual bool setopt (const t_so opt, const bool val) =0;

    /// set a socket option with a value
    /// @param opt the socket option
    /// @param val the value to set
    virtual bool setopt (const t_so opt, const long val) =0;

    /// get a socket option
    /// @param opt the socket option
    /// @param val the option value
    virtual bool getbopt (const t_so opt) const =0;

    /// get a socket option with a value
    /// @param opt the socket option
    /// @param val the value to set
    virtual long getlopt (const t_so opt) const =0;

  public:
    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
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
