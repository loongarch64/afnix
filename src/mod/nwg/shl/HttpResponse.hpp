// ---------------------------------------------------------------------------
// - HttpResponse.hpp                                                        -
// - afnix:nwg module - http response class definition                       -
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

#ifndef  AFNIX_HTTPRESPONSE_HPP
#define  AFNIX_HTTPRESPONSE_HPP

#ifndef  AFNIX_HTTPPROTO_HPP
#include "HttpProto.hpp"
#endif

#ifndef  AFNIX_COOKIEJAR_HPP
#include "CookieJar.hpp"
#endif

namespace afnix {

  /// The HttpResponse class is a base class designed to handle a http
  /// response. The class operates with the protocol version 1.1 
  /// as defined by RFC 2616. For a client response, the response is built by
  /// reading an input stream and setting the response status code with its 
  /// associated header. For a server response, the response is formatted 
  /// with a response status and additional header information. In both cases,
  /// the header is filled automatically depending on the response side and
  /// the base http protocol object type set. By default, a server response
  /// is built unless an input stream is used, which in this case corresponds
  /// to a client response.
  /// @author amaury darsch

  class HttpResponse : public HttpProto {
  protected:
    /// the status code
    long d_code;
    /// the cookie jar
    CookieJar d_cjar;

  public:
    /// create a default http response
    HttpResponse (void);

    /// create a http response by status code
    /// @param code the status code to set
    HttpResponse (const long code);

    /// create a http response by status code and type
    /// @param code the status code to set
    /// @param type the content to set
    HttpResponse (const long code, const String& type);

    /// create a response by input stream
    /// @param is the input stream to parse
    HttpResponse (InputStream& is);
    
    /// copy construct this http response
    /// @param the object to copy
    HttpResponse (const HttpResponse& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a http response to this one
    /// @param the object to assign
    HttpResponse& operator = (const HttpResponse& that);

    /// reset the response
    void reset (void);

    /// parse the response
    /// @param is the input stream to parse
    void parse (InputStream& is);

    /// write the response header to an output stream
    /// @param os the output stream
    void hwrite (OutputStream& os) const;

    /// write the response header to a buffer
    /// @param buf the target buffer
    void hwrite (Buffer& buf) const;

    /// write the http response to an output stream
    /// @param os the output stream
    void write (OutputStream& os) const;

    /// write the http response to a buffer
    /// @param buf the target buffer
    void write (Buffer& buf) const;

    /// @return true if the status is ok
    bool isok (void) const;

    /// @return true if the error code is set
    bool iserror (void) const;

    /// @return true if the status code is set
    bool isstatus (void) const;

    /// set the response status by code
    /// @param code the status code to set
    void setstatus (const long code); 

    /// set the response status by code and version
    /// @param code the status code to set
    /// @param vers the response version
    void setstatus (const long code, const String& vers);

    /// @return the response status code
    long getstatus (void) const;

    /// map the response status code
    String mapstatus (void) const;

    /// @return true if another location is defined
    bool ishloc (void) const;

    /// set a header location as a redirect
    /// @param uri the target uri
    void sethloc (const String& uri);

    /// @return the header location value
    String gethloc (void) const;

    /// add a cookie in the response jar
    /// @param cook the cookie to add
    void addcookie (const Cookie& cook);

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
