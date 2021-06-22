// ---------------------------------------------------------------------------
// - Uri.hpp                                                                 -
// - afnix:www module - uri class definition                                 -
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

#ifndef  AFNIX_URI_HPP
#define  AFNIX_URI_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The Uri class is a base object used to parse or build a uniform
  /// resource identifer as defined by RFC 3986. The URI can be built by 
  /// specifiying each component or by parsing a string. When a string is
  /// given in the constructor, the class parses the string and extract all
  /// components. The URI components are the scheme, the authority,
  /// the path, the query and the fragment. The uri base name is the scheme
  /// combined with the authority. The uri hname is the base name combined
  /// with the path. The base name and the hname are not part of the RFC
  /// 3986, although they are convenient concepts
  /// @author amaury darsch

  class Uri : public Nameable {
  public:
    /// check for a uri string
    /// @param suri the uri string to check
    static bool isuri (const String& suri);

    /// create a string uri by scheme, host and port
    /// @param schm the uri scheme
    /// @param host the uri host
    /// @param port the uri port
    static String tosuri (const String& schm,
			  const String& host, const long port);
    
    /// percent encode a name
    /// @param name the name to encode
    static String pencode (const String& name);

    /// uri encode a name
    /// @param name the name to encode
    static String uencode (const String& name);

    /// component encode a name
    /// @param name the name to encode
    static String cencode (const String& name);

    /// percent decode a  name
    /// @param name the name to decode
    static String pdecode (const String& name);
    
    /// normalize a uri name by adding a scheme if necessary
    /// @param name the name to fix
    static String nrmname (const String& name);

    /// get a uri name  by prioritizing the system path
    /// @param name the name to fix
    static String sysname (const String& name);

    /// normalize a path by uri name
    /// @param name the name to fix
    static String pthname (const String& name);

    /// normalize a host name by authority
    /// @param auth the authority to normalize
    static String nrmhost (const String& auth);

    /// normalize a port by authority
    /// @param auth the authority to normalize
    static long nrmport (const String& auth);
    
  private:
    /// the uri scheme
    String d_schm;
    /// the uri marker
    String d_amrk;
    /// the uri authority
    String d_auth;
    /// the uri path
    String d_path;
    /// the uri query
    String d_query;
    /// the uri fragment
    String d_frag;

  public:
    /// create an empty uri
    Uri (void);

    /// create a uri by string
    /// @param uri the string to parse
    Uri (const String& uri);

    /// create a uri by scheme, host and port
    /// @param schm the uri scheme
    /// @param host the uri host
    /// @param port the uri port
    Uri (const String& schm, const String& host, const long port);

    /// copy construct this uri
    /// @param that the uri to copy
    Uri (const Uri& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a uri to this one
    /// @param that the uri to assign
    Uri& operator = (const Uri& that);

    /// reset an uri to its default
    void reset (void);

    /// @return the uri original name
    String getname (void) const;

    /// parse an uri by name
    /// @param uri the uri to parse
    void parse (const String& uri);

    /// normalize the uri authority
    void nrmauth (void);

    /// @return this uri scheme
    String getscheme (void) const;

    /// @return this uri authority
    String getauth (void) const;

    /// @return this uri path
    String getpath (void) const;

    /// @return this uri path target name
    String getptnm (void) const;

    /// @return this uri query
    String getquery (void) const;

    /// @return this uri fragment
    String getfragment (void) const;

    /// @return the base uri name
    String getbase (void) const;

    /// @return the reference uri name
    String getrnam (void) const;

    /// @return the decoded reference uri name
    String gethnam (void) const;

    /// @return the almost uri name
    String getanam (void) const;

    /// add a path to this uri
    /// @param path the path to add
    Uri addpath (const String& path) const;
    
    /// get a new uri from a path
    /// @param path the path to add or replace
    Uri gethref (const String& path) const;

    /// @return the system path from a uri path
    String getsysp (void) const;

    /// @return the encode path
    String getpenc (void) const;

    /// @return the uri host if any
    String gethost (void) const;

    /// @return the uri port if any
    long getport (void) const;

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
