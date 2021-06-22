// ---------------------------------------------------------------------------
// - JsonMime.hpp                                                            -
// - afnix:wax service - json mime class definition                          -
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

#ifndef  AFNIX_JSONMIME_HPP
#define  AFNIX_JSONMIME_HPP

#ifndef  AFNIX_MIME_HPP
#include "Mime.hpp"
#endif

#ifndef  AFNIX_JSON_HPP
#include "Json.hpp"
#endif

namespace afnix {

  /// The JsonMime class is a generic javascript object notation generic
  /// mime writer. The class is filled by object.
  /// @author amaury darsch

  class JsonMime : public Json, public Mime {
  public:
    /// create a default json mime
    JsonMime (void);

    /// @return the class name
    String repr (void) const override;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const override;
    
    /// write a node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const override;

  private:
    // make the copy constructor private
    JsonMime (const JsonMime&) =delete;
    // make the assignment operator private
    JsonMime& operator = (const JsonMime&) =delete;
    
  public:
    /// create an object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
