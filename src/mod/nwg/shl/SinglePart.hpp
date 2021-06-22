// ---------------------------------------------------------------------------
// - SinglePart.hpp                                                          -
// - afnix:nwg module - single part content class definition                 -
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

#ifndef  AFNIX_SINGLEPART_HPP
#define  AFNIX_SINGLEPART_HPP

#ifndef  AFNIX_HEADERPART_HPP
#include "HeaderPart.hpp"
#endif

namespace afnix {

  /// The SinglePart class is a buffer class designed to support a single
  /// portion of a multipart representation. The single part is splitted
  /// between a header and a data buffer.
  /// @author amaury darsch

  class SinglePart : public HeaderPart {
  private:
    /// the part buffer
    Buffer* p_pbuf;

  public:
    /// create an empty single part
    SinglePart (void);

    /// copy construct this single part
    /// @param that the object copy
    SinglePart (const SinglePart& that);

    /// destroy this object
    ~SinglePart (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a single part to this one
    /// @param that the part to assign
    SinglePart& operator = (const SinglePart& that);

    /// set the single part buffer
    /// @param pbuf the buffer to set
    virtual void setpbuf (Buffer* pbuf);

    /// @return the single part buffer
    virtual Buffer* getpbuf (void) const;

    /// @return the single part buffer string
    virtual String tostring (void) const;

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
