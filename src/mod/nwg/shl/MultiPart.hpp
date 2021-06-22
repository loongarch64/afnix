// ---------------------------------------------------------------------------
// - MultiPart.hpp                                                           -
// - afnix:nwg module - multipart content class definition                   -
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

#ifndef  AFNIX_MULTIPART_HPP
#define  AFNIX_MULTIPART_HPP

#ifndef  AFNIX_SINGLEPART_HPP
#include "SinglePart.hpp"
#endif

namespace afnix {

  /// The MultiPart class is a buffer class designed to support the 
  /// multipart representation used especially in the form file upload.
  /// This implementation is conformant to RFC 1867/2388. Normally, this
  /// object is contructed when parsing a http request. The multi part 
  /// structure is a collection of single part objects.
  /// @author amaury darsch

  class MultiPart : public virtual Object {
  private:
    /// the boundary string
    String d_bnds;
    /// the multipart vector
    Vector d_mprt;

  public:
    /// create a multipart with a boundary string
    /// @param bnds the boundary string
    MultiPart (const String& bnds);

    /// create a multipart by string and buffer
    /// @param bnds the boundary string
    /// @param mbuf the multipart buffer
    MultiPart (const String& bnds, Buffer& mbuf);

    /// @return the class name
    String repr (void) const;

    /// reset this multipart content
    void reset (void);

    /// @return the boundary string
    String getbnds (void) const;

    /// parse a multipart buffer
    /// @param mbuf the buffer to parse
    void parse (Buffer& mbuf);

    /// @return the number of single part objects
    long length (void) const;

    /// get a single part object by index
    /// @param index the single part index
    SinglePart* get (const long index) const;

  private:
    // make the copy constructor private
    MultiPart (const MultiPart&);
    // make the assignement operator private
    MultiPart& operator = (const MultiPart&);
    
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
