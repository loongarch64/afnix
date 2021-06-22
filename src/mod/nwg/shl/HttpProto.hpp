// ---------------------------------------------------------------------------
// - HttpProto.hpp                                                           -
// - afnix:nwg module - http protocol base class definition                  -
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

#ifndef  AFNIX_HTTPPROTO_HPP
#define  AFNIX_HTTPPROTO_HPP

#ifndef  AFNIX_HEADERPART_HPP
#include "HeaderPart.hpp"
#endif

#ifndef  AFNIX_BLOCKBUFFER_HPP
#include "BlockBuffer.hpp"
#endif

namespace afnix {

  /// the HttpProto class is a base class that ease the deployement of the
  /// http protocol. The base class is built with a property list which is
  /// used to define the message header. The class also defines the write
  /// methods which are used to write a message either on an output stream
  /// or into a buffer.
  /// @author amaury darsch

  class HttpProto : public HeaderPart {
  public:
    /// the protocol object type
    enum t_ptyp {
      HTTP_SERVER, // protocol server side
      HTTP_CLIENT  // protocol client side
    };

    /// the header field
    enum t_hfld {
      HEAD_ACPT, // Accept
      HEAD_ACHS, // Accept-Charset
      HEAD_AENC, // Accept-Encoding
      HEAD_ALNG, // Accept-Language
      HEAD_ARNG, // Accept-Ranges
      HEAD_BAGE, // Age
      HEAD_ALOW, // Allow
      HEAD_AUTH, // Authorization
      HEAD_CCTL, // Cache-Control
      HEAD_CONN, // Connection
      HEAD_CENC, // Content-Encoding
      HEAD_CLNG, // Content-Language
      HEAD_CLEN, // Content-Length
      HEAD_CLOC, // Content-Location
      HEAD_CMD5, // Content-MD5
      HEAD_CRNG, // Content-Range
      HEAD_CTYP, // Content-Type
      HEAD_DATE, // Date
      HEAD_ETAG, // ETag
      HEAD_EXPT, // Expect
      HEAD_EXPR, // Expires
      HEAD_FROM, // From
      HEAD_HOST, // Host
      HEAD_IFMT, // If-Match
      HEAD_IFMS, // If-Modified-Since
      HEAD_IFNM, // If-None-Match
      HEAD_IFRG, // If-Range
      HEAD_IFUS, // If-Unmodified-Since
      HEAD_LMOD, // Last-Modified
      HEAD_MLOC, // Location
      HEAD_MFWD, // Max-Forwards
      HEAD_PRGM, // Pragma
      HEAD_PXTH, // Proxy-Authenticate
      HEAD_PXAT, // Proxy-Authorization
      HEAD_BRNG, // Range
      HEAD_REFR, // Referer
      HEAD_RAFT, // Retry-After
      HEAD_SERV, // Server
      HEAD_BDTE, // TE
      HEAD_TRLR, // Trailer
      HEAD_TENC, // Transfer-Encoding
      HEAD_UPGR, // Upgrade
      HEAD_UAGT, // User-Agent
      HEAD_VARY, // Vary
      HEAD_RVIA, // Via
      HEAD_WRNG, // Warning
      HEAD_WWWA, // WWW-Authenticate
      HEAD_COK0, // Set-Cookie
      HEAD_COK1  // Set-Cookie2
    };

    /// @return the default http version
    static String getxdef (void);

    /// map a header field to a string
    /// @param hflg the header field to map
    static String hfldtos (const t_hfld hfld);

    /// map a http status code to a string
    /// @param code the code to map
    static String codetos (const long code);

  protected:
    /// the protocol version
    String d_vers;
    /// the protocol type
    t_ptyp d_ptyp;

  public:
    /// create a default http protocol
    HttpProto (void);

    /// create a default http protocol by type
    /// @param ptyp the protocol type
    HttpProto (const t_ptyp ptyp);

    /// copy construct this http protocol
    /// @param that the object to copy
    HttpProto (const HttpProto& that);

    /// assign a http protocol to this one
    /// @param that the object to assign
    HttpProto& operator = (const HttpProto& that);

    /// reset this protocol object
    virtual void reset (void);

    /// parse an input stream
    /// @param is the input stream to parse
    virtual void parse (InputStream& is) =0;

    /// set the protocol version
    /// @param vers the version to set
    virtual void setvers (const String& vers);

    /// @return the protocol version
    virtual String getvers (void) const;

    /// @return true if a header property exists by field
    virtual bool hexists (const t_hfld hfld) const;
    
    /// set a header property by field and litteral
    /// @param hfld the header field
    /// @param lval the property value
    virtual void hset (const t_hfld hfld, const Literal& pval);

    /// @return a header property by field
    virtual Property* hfind (const t_hfld hfld) const;

    /// @return a header property by field or throw an exception
    virtual Property* hlookup (const t_hfld hfld) const;

    /// @return a header property value by field
    virtual String hmap (const t_hfld hfld) const;

    /// @return true if the media type is defined
    virtual bool ismedia (void) const;

    /// @return the response media type
    virtual String getmedia (void) const;

    /// @return true if the encoding mode is defined
    virtual bool isemod (void) const;

    /// @return the response encoding mode
    virtual String getemod (void) const;

    /// @return true if the content length is defined
    virtual bool isclen (void) const;

    /// @return the header content length
    virtual long getclen (void) const;

    /// get the content string
    /// @param is the input stream to parse
    virtual String getcstr (InputStream& is);

    /// write the http proto to an output stream
    /// @param os the output stream
    virtual void write (OutputStream& os) const;

    /// write the http proto to a buffer
    /// @param buf the target buffer
    virtual void write (Buffer& buf) const;

  public:
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
