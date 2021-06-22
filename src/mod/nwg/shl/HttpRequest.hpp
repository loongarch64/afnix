// ---------------------------------------------------------------------------
// - HttpRequest.hpp                                                         -
// - afnix:nwg module - http request class definition                        -
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

#ifndef  AFNIX_HTTPREQUEST_HPP
#define  AFNIX_HTTPREQUEST_HPP

#ifndef  AFNIX_URI_HPP
#include "Uri.hpp"
#endif

#ifndef  AFNIX_HTTPPROTO_HPP
#include "HttpProto.hpp"
#endif

#ifndef  AFNIX_MULTIPART_HPP
#include "MultiPart.hpp"
#endif

namespace afnix {

  /// The HttpRequest class is a base class designed to handle a http
  /// request. The class operates with the protocol version 1.1 
  /// as defined by RFC 2616. For a server request, the request is built by
  /// reading an input stream and setting the request command with its 
  /// associated header. For a client request, the request is formatted with a
  /// request command and a eventually a uri. In both cases, the header is
  /// filled automatically depending on the request side and the base http 
  /// protocol object type set. By default, a client response is built unless
  /// an input stream is used, which in this case corresponds to a server
  /// request.
  /// @author amaury darsch

  class HttpRequest : public HttpProto {
  protected:
    /// the request method
    String d_rmth;
    /// the request uri
    String d_ruri;
    /// the request buffer
    Buffer* p_rbuf;

  public:
    /// create a default request
    HttpRequest (void);

    /// create a request by method
    /// @param rmth the request method
    HttpRequest (const String& rmth);

    /// create a request by uri
    /// @param uri the requested uri 
    HttpRequest (const Uri& uri);

    /// create a request by method and request uri
    /// @param rmth the request method
    /// @param ruri the request uri
    HttpRequest (const String& rmth, const String& ruri);

    /// create a request by method and uri
    /// @param rmth the request method
    /// @param uri the requested uri 
    HttpRequest (const String& rmth, const Uri& uri);

    /// create a request by input stream
    /// @param is the input stream to parse
    HttpRequest (InputStream& is);

    /// copy construct this http request
    /// @param the object to copy
    HttpRequest (const HttpRequest& that);

    /// destroy this object
    ~HttpRequest (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a http request to this one
    /// @param the object to assign
    HttpRequest& operator = (const HttpRequest& that);

    /// reset the response
    void reset (void);

    /// parse an input stream
    /// @param is the input stream to parse
    void parse (InputStream& is);

    /// write the http request to an output stream
    /// @param os the output stream
    void write (OutputStream& os) const;

    /// write the http request to a buffer
    /// @param buf the target buffer
    void write (Buffer& buf) const;

    /// set the request method
    /// @param rmth the request method
    virtual void setrmth (const String& rmth);

    /// @return the request method
    virtual String getrmth (void) const;

    /// set the request uri
    /// @param ruri the request uri to set
    virtual void setruri (const String& ruri);

    /// @return the request uri
    virtual String getruri (void) const;

    /// @return the request query string
    virtual String getrqry (void) const;

    /// @return true if a cookie is set
    virtual bool iscookie (void) const;

    /// get a cookie string from the request
    virtual String getcookie (void) const;

    /// get the request buffer if any
    virtual Buffer* getrbuf (void) const;

    /// the multipart content if any
    virtual MultiPart* getmpart (void);

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
