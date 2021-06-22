// ---------------------------------------------------------------------------
// - UriStream.hpp                                                           -
// - afnix:uri service - uri stream class definition                         -
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

#ifndef  AFNIX_URISTREAM_HPP
#define  AFNIX_URISTREAM_HPP

#ifndef  AFNIX_URI_HPP
#include "Uri.hpp"
#endif

#ifndef  AFNIX_STREAM_HPP
#include "Stream.hpp"
#endif

namespace afnix {

  /// The UriStream class is a stream generator that operates with uri.
  /// Depending on the uri scheme, a stream is opened and ready for data
  /// operations. By default, an input file stream is created with
  /// uri of file scheme. If the uri scheme is http, an http request is
  /// performed and the payload can be consumed normaly.
  /// @author amaury darsch

  class UriStream {
  public:
    /// the io stream mapper
    using t_iosm = Object* (*) (const String&, const long);

  private:
    /// the io function mapper
    t_iosm p_iosm;
    
  public:
    /// create a null uri stream
    UriStream (void);

    /// create a uri stream by mapper
    /// @param isnm the io stream mapper
    UriStream (t_iosm iosm);
    
    /// create an input stream by uri
    /// @param uri the uri to use
    InputStream* istream (const Uri& uri) const;

    /// create an input stream by string uri
    /// @param suri the string uri to use
    InputStream* istream (const String& suri) const;

    /// create an output stream by uri
    /// @param uri the uri to use
    OutputStream* ostream (const Uri& uri) const;

    /// create an outout stream by string uri
    /// @param suri the string uri to use
    OutputStream* ostream (const String& suri) const;

  private:
    // make the copy constructor private
    UriStream (const UriStream&) =delete;
    // make the assignement operator private
    UriStream& operator = (const UriStream&) =delete; 
  };
}

#endif
