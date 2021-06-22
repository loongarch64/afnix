// ---------------------------------------------------------------------------
// - HttpStream.hpp                                                          -
// - afnix:nwg module - http stream class definition                         -
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

#ifndef  AFNIX_HTTPSTREAM_HPP
#define  AFNIX_HTTPSTREAM_HPP

#ifndef  AFNIX_HTTPPROTO_HPP
#include "HttpProto.hpp"
#endif

#ifndef  AFNIX_INPUTBOUND_HPP
#include "InputBound.hpp"
#endif

namespace afnix {

  /// The HttpStream class is an input stream class bound to a http protocol
  /// object. The class can be built from a http request or response object.
  /// Normally, the class operates as a container with an input stream built
  /// from the protocol object. The protocol type is accessible from the
  /// protocol object itself while the stream object is directly accessible.
  /// @author amaury darsch

  class HttpStream : public InputBound {
  protected:
    /// the protocol object
    HttpProto* p_hpto;

  public:
    /// create a default http stream
    HttpStream (void);

    /// create a http stream by protocol and stream
    /// @param pto the http protocol object
    /// @param is  the input stream to bind
    HttpStream (HttpProto* pto, InputStream* is);

    /// destroy this http stream object
    ~HttpStream (void);

    /// @return the class name
    String repr (void) const;

    /// @return the http protocol object
    HttpProto* getproto (void) const;

    /// @return the http stream media type
    String getmedia (void) const;

  private:
    // make the copy constructor private
    HttpStream (const HttpStream&);
    // make the assignment operator private
    HttpStream& operator = (const HttpStream&);

  public:
    /// create a new object in a generic object
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
