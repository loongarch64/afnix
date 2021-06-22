// ---------------------------------------------------------------------------
// - Uri.cpp                                                                 -
// - afnix:www module - uri class implementation                             -
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

#include "Uri.hpp"
#include "Ascii.hpp"
#include "Regex.hpp"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Buffer.hpp"
#include "System.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Unicode.hpp"
#include "Pathname.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the uri name separator
  static const char URI_PATH_CSEP = '/';
  // the default ftp port
  static const long URI_XFTP_PORT = 21;
  // the default ssh port
  static const long URI_XSSH_PORT = 22;
  // the default smtp port
  static const long URI_SMTP_PORT = 25;
  // the default http port
  static const long URI_HTTP_PORT = 80;
  // the default https port
  static const long URI_HTPS_PORT = 443;

  // this procedure validate the uri encode ascii character
  static bool uri_uenc_p (const char c) {
    if (Ascii::isalpha (c) == true) return false;
    if (Ascii::isdigit (c) == true) return false;
    switch (c) {
    case ';':
    case ',':
    case '/':
    case '?':
    case ':':
    case '@':
    case '&':
    case '=':
    case '+':
    case '$':
    case '-':
    case '_':
    case '.':
    case '!':
    case '~':
    case '*':
    case '\'':
    case '(':
    case ')':
    case '#':
      return false;
      break;
    default:
	break;
    }
    return true;
  }

  // this procedure validate the percent encode ascii character
  static bool uri_penc_p (const char c) {
    if (Ascii::isalpha (c) == true) return false;
    if (Ascii::isdigit (c) == true) return false;
    switch (c) {
    case ';':
    case ',':
    case '/':
    case '?':
    case ':':
    case '@':
    case '&':
    case '=':
    case '+':
    case '$':
    case '-':
    case '_':
    case '.':
    case '!':
    case '~':
    case '*':
    case '\'':
    case '(':
    case ')':
    case '#':
      return false;
      break;
    case '[':
    case ']':
    case '%':
      return false;
      break;
    default:
	break;
    }
    return true;
  }

  // this procedure validate the component encode ascii character
  static bool uri_cenc_p (const char c) {
    if (Ascii::isalpha (c) == true) return false;
    if (Ascii::isdigit (c) == true) return false;
    switch (c) {
    case '-':
    case '_':
    case '.':
    case '!':
    case '~':
    case '*':
    case '\'':
    case '(':
    case ')':
      return false;
      break;
    default:
	break;
    }
    return true;
  }

  // the percent encoding predicate
  using t_penc = bool (*) (const char);

  // percent-encode a name
  static String uri_pencode (const String& name, t_penc penc) {
    // do nothing with nil string
    if (name.isnil () == true) return name;
    // get a quad buffer
    t_quad* sbuf = name.toquad ();
    if (sbuf == nullptr) return String();
    try {
      // prepare buffer 
      Buffer result;
      // loop in the quad buffer
      t_quad* p = sbuf;
      while (*p != nilq) {
	// check for an ascii character
	if (Unicode::isascii (*p) == true) {
	  char c = Unicode::tochar (*p);
	  // check for escape
	  if (penc (c) == true) {
	    result.add ('%');
	    result.add (Ascii::btoc (c, false));
	    result.add (Ascii::btoc (c, true));
	  } else result.add (c);
	} else {
	  // non-ascii needs percent encoding
	  long size = 0L;
	  t_byte* cbuf = Unicode::qtob (size, *p);
	  if ((size == 0L) || (cbuf == nullptr)) continue;
	  // loop and add
	  for (long k = 0L; k < size; k++) {
	    result.add ('%');
	    result.add (Ascii::btoc (cbuf[k], false));
	    result.add (Ascii::btoc (cbuf[k], true));
	  }
	  delete [] cbuf;
	}
	// next position
	p++;
      }
      delete [] sbuf;
      return result.tostring ();
    } catch (...) {
      delete [] sbuf;
      throw;
    }
  }
  
  // this procedure normalize a path
  static String normalize_path (const String& path) {
    // check for special case
    if (path.isnil () == true) return path;
    if (path == "/") return path;
    if (path.first () != Unicode::toquad ('/')) return path;
    // extract the path components
    Strvec svec = Strvec::split (path, "/");
    long slen = svec.length ();
    if (slen < 2) return path;
    // rebuild a new vector path
    Strvec rvec;
    for (long i = 0; i < slen ; i++) {
      // get the string element
      String elem = svec.get (i);
      // check for special case
      if (elem.isnil () == true) {
	if (i == slen-1) rvec.add (elem);
	continue;
      }
      if (elem == ".") continue;
      if (elem == "..") {
	if (rvec.empty () == false) rvec.rml ();
	continue;
      }
      // save in result vector
      rvec.add (elem);
    }
    // rebuild result path
    long rlen = rvec.length ();
    if (rlen == 0) return "/";
    String result = "";
    for (long i = 0; i < rlen; i++) {
      result += "/";
      result += rvec.get (i);
    }
    // evntually fix the last /
    if (path.last () == Unicode::toquad ('/')) result += "/";
    return result;
  }

  // this procedure get the base name from a path if it does not finish
  // with a slash - so it can be used as a base for adding a path
  static String get_base_path (const String& name) {
    if (name.last () == Unicode::toquad ('/')) return name;
    // get name length and check
    long len = name.length ();
    if (len == 0) return name;
    // find the first slash index
    for (long i = len - 1; i >= 0; i--) {
      if (name[i] == Unicode::toquad ('/')) {
	return name.lsubstr (i);
      }
    }
    return name;
  }

  // get a system path from a uri path

  static String get_sys_path (const String& path) {
    //check that the path starts with a '/' character
    if (path.first () != Unicode::toquad ('/')) {
      throw Exception ("path-error", "invalid uri path to convert", path);
    }
    // check special case with root only
    if (path == "/") return System::getrdir ();
    // split the path
    Strvec svec = Strvec::split (path, "/");
    if (svec.length () < 2) {
      throw Exception ("internal-error", "cannot convert uri path", path);
    }
    // build first component
    String result = System::getrdir ();
    // add the remaining elements
    long slen = svec.length ();
    for (long i = 1; i < slen; i++) {
      result = System::join (result, svec.get (i));
    }
    return Uri::pencode (result);
  }

  // this procedure returns a authority host
  static String get_auth_host (const String& auth) {
    // check for nil name
    if (auth.isnil () == true) return auth;
    // the authority regex
    Regex re ("([[<$a-._~>+]|[$k<$x:>+$k]]?)[:$d+]?");
    if (re == auth) {
      // get the host name
      String host = re.getstring (0);
      // check for ip numerical syntax
      if ((host.first () == '[') && (host.last () == ']')) {
	long hlen = host.length ();
	host = host.substr (1, hlen - 1L);
      }
      return host;
    }
    // invalid authority
    throw Exception ("uri-error", "invalid authority name", auth);
  }

  // this procedure returns a mail host
  static String get_mail_host (const String& path) {
    // check for nil name
    if (path.isnil () == true) return path;
    // the authority regex
    Regex re ("<$a+-_.%:>*@(<$a-._~>*)");
    if (re == path) return re.getstring (0);
    // invalid authority
    throw Exception ("uri-error", "invalid mail path name", path);
  }

  // this procedure returns an authority port if any
  static long  get_auth_port (const String& auth) {
    // check for nil name
    if (auth.isnil () == true) return 0;
    // the authority regex
    Regex re ("[[<$a-._~>+]|[$k<$x:>+$k]]?:($d+)");
    if (re == auth) return Utility::tolong (re.getstring (0));
    return 0;
  }

  // this procedure returns an authority by host and port
  static String get_auth_name (const String& host, const long port) {
    // check the host name
    Regex rhst ("[<$a-._~>+]|[$k<$x:>+$k]");
    if (rhst != host) {
      throw Exception ("name-error", "invalid uri host name", host);
    }
    // check the port
    if (port < 0) {
      throw Exception ("port-error", "invalid uri negative port");
    }
    // build the authority
    String result = host;
    result += ':';
    result += port;
    return result;
  }

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // check for a string uri

  bool Uri::isuri (const String& suri) {
    try {
      Uri uri (suri);
      return true;
    } catch (...) {
      return false;
    }
  }

  // create a string uri by scheme, host and port

  String Uri::tosuri (const String& schm,
		      const String& host, const long port) {
    // check if the host contains some : character
    bool bflg = false;
    // rebuild the uri string
    String result = schm + "://";
    if (bflg == true) {
      result = result + '[' + host + ']';
    } else {
      result = result + host;
    }
    // check for valid port
    if (port < 0) {
      throw Exception ("uri-error", "invalid negative uri port");
    }
    if (port > 0L) result = result + ':' + port;
    // full uri is here
    return result;
  }
  
  // percent encode a name

  String Uri::pencode (const String& name) {
    return uri_pencode (name, uri_penc_p);
  }
  
  // uri encode a name

  String Uri::uencode (const String& name) {
    return uri_pencode (name, uri_uenc_p);
  }

  // component encode a name

  String Uri::cencode (const String& name) {
    return uri_pencode (name, uri_cenc_p);
  }
  
  // percent-decode a uri name

  String Uri::pdecode (const String& name) {
    // check for nil name
    if (name.isnil () == true) return name;
    // create a working buffer
    Buffer buf;
    // get the name length and loop
    long    size = name.length ();
    t_quad* data = name.toquad ();
    try {
      for (long i = 0; i < size; i++) {
	t_quad c = data[i];
	if (c == Unicode::toquad ('%')) {
	  t_byte val = nilc;
	  if (++i < size) val  = Ascii::htob (data[i]) << 4;
	  if (++i < size) val += Ascii::htob (data[i]);
	  if (val == nilc) {
	    throw Exception ("uri-error", "unterminated escape sequence");
	  }
	  buf.add ((char) val);
	  continue;
	}
	if (Unicode::isascii (c) == false) {
	  throw Exception ("uri-error", 
			   "non-ascii uri sequence to decode", name);
	}
	buf.add (c);
      }
      delete [] data;
      return buf.tostring ();
    } catch (...) {
      delete [] data;
      throw;
    }
  }

  // normalize the uri name by looking at various scheme for incomplete name

  String Uri::nrmname (const String& name) {
    // check if we have a scheme
    Regex sre ("($l$a*):($N+)");
    if (sre == name) return name;
    // check if we have a qualified host
    Regex hre ("[<$a-_~>+.]+$s+[/$N*]?");
    if (hre == name) {
      String result = "http://";
      result += name;
      return result;
    }
    // check if we have a qualified host with //
    Regex qre ("//[<$a-_~>+.]+$s+[/$N*]?");
    if (qre == name) {
      String result = "http:";
      result += name;
      return result;
    }
    // here we can assume that we have a file since there is no scheme and
    // host name - we just check for an absolute path
    if (name.first () == Unicode::toquad ('/')) {
      String result = "file://";
      result += name;
      return result;
    }
    // here there is nothing we can do - so return the original name
    return name;
  }

  // normalize the uri name by prioritizing the system name
  
  String Uri::sysname (const String& name) {
    // check if we have a scheme
    Regex sre ("($l$a*):($N+)");
    if (sre == name) return name;
    // check if we have a system path
    Pathname path = name;
    String pnam = path.getsysp ();
    if (System::isfile (pnam) == true) {
      String result = "file://";
      result += pnam;
      return result;
    }
    // go back to the regular normalization
    return Uri::nrmname (name);
  }

  // normalize a path by uri name

  String Uri::pthname (const String& name) {
    // check for a uri
    try {
      Uri uri (name);
      String result = uri.getpath ();
      if (result.isnil () == true) result = "/";
      return result;
    } catch (...) {
      // here we might have a partial path - this is typically found
      // in http request
      // now extract the path
      Regex re ("(<$a+-_=.%!:@*,;/\\'`~()&^[]{}$<$>>*)(<?#>?$N*)");
      if (re == name) return re.getstring (0);
    }
    // do nothing here
    return name;
  }

  // normalize a host name by authority
  
  String Uri::nrmhost (const String& auth) {
    return get_auth_host (auth);
  }

  // normalize a port by authority

  long Uri::nrmport (const String& auth) {
    return get_auth_port (auth);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default uri

  Uri::Uri (void) {
    reset ();
  }

  // create a uri by name

  Uri::Uri (const String& uri) {
    reset ();
    parse (uri);
  }

  // create a uri by scheme, host and port
  
  Uri::Uri (const String& schm, const String& host, const long port) {
    reset ();
    Regex rsch ("$l$a*");
    if (rsch != schm) {
      throw Exception ("scheme-error", "invalid uri scheme name", schm);
    }
    d_schm = schm;
    d_amrk = "//";
    d_auth = get_auth_name (host, port);
  }

  // copy constructs this uri

  Uri::Uri (const Uri& that) {
    that.rdlock ();
    try {
      d_schm  = that.d_schm;
      d_amrk  = that.d_amrk;
      d_auth  = that.d_auth;
      d_path  = that.d_path;
      d_query = that.d_query;
      d_frag  = that.d_frag;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Uri::repr (void) const {
    return "Uri";
  }

  // return a clone of this object

  Object* Uri::clone (void) const {
    return new Uri (*this);
  }

  // asssign an uri to this one

  Uri& Uri::operator = (const Uri& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_schm  = that.d_schm;
      d_amrk  = that.d_amrk;
      d_auth  = that.d_auth;
      d_path  = that.d_path;
      d_query = that.d_query;
      d_frag  = that.d_frag;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset an uri information

  void Uri::reset (void) {
    wrlock ();
    try {
      d_schm  = "";
      d_amrk  = "";
      d_auth  = "";
      d_path  = "";
      d_query = "";
      d_frag  = "";
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri original name

  String Uri::getname (void) const {
    rdlock ();
    try {
      String result = d_schm;
      result += ":";
      result += d_amrk;
      result += d_auth;
      result += d_path;
      // add optional query
      if (d_query.isnil () == false) {
	result += '?';
	result += d_query;
      }
      // add optional fragment
      if (d_frag.isnil () == false) {
	result += '#';
	result += d_frag;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse an uri and update the uri data structure

  void Uri::parse (const String& uri) {
    wrlock ();
    try {
      // reset everythnig
      reset  ();
      // check for nothing first
      if (uri.isnil () == true) {
	unlock ();
	return;
      }
      // percent encode the string
      String puri = Uri::pencode (uri);
      // the reminding string
      String rest;
      // extract the scheme
      Regex rsch ("($l$a*):($N+)");
      if (rsch == puri) {
	d_schm = rsch.getstring (0);
	rest   = rsch.getstring (1);
      } else {
	throw Exception ("uri-error", "empty scheme uri name", uri);
      }
      // now extract the  authority
      Regex raut ("//([[<$a-._~>+]|[$k<$x:>+$k]]?[:$d+]?)(</?#>?$N*)");
      if (raut == rest) {
	// extract the authority
	d_amrk = "//";
	d_auth = raut.getstring (0);
	rest   = raut.getstring (1);
	// now extract the path
	Regex rpth ("(/<$a+-_=.%!:@*,;/\\'`~()&^[]{}$<$>>*)(<?#>?$N*)");
	if (rpth == rest) {
	  d_path = rpth.getstring (0);
	  rest   = rpth.getstring (1);
	}
      } else {
	// now extract the path
	Regex rpth ("(<$a+-_=.%!:@*,;/\\'`~()&^[]{}$<$>>*)(<?#>?$N*)");
	if (rpth == rest) {
	  d_path = rpth.getstring (0);
	  rest   = rpth.getstring (1);
	}
      }
      // now extract the query
      Regex rqry ("$?(<$a+-_.%/=;&:,@()>*)(<$#>?$N*)");
      if (rqry == rest) {
	d_query = rqry.getstring (0);
	rest    = rqry.getstring (1);
      }
      // now extract the fragment
      Regex rfrg ("#($N*)");
      if (rfrg == rest) {
	d_frag = rfrg.getstring (0);
	rest = "";
      }
      if (rest.length () != 0) {
	throw Exception ("uri-error", "incorrect uri format", uri);
      }
      unlock ();
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // normalize the uri authority

  void Uri::nrmauth (void) {
    wrlock ();
    try {
      d_auth = d_auth.tolower ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the uri scheme
  
  String Uri::getscheme (void) const {
    rdlock ();
    try {
      String result = d_schm;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the uri authority
  
  String Uri::getauth (void) const {
    rdlock ();
    try {
      String result = d_auth;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the uri path
  
  String Uri::getpath (void) const {
    rdlock ();
    try {
      String result = Uri::pdecode (d_path);
      if (d_amrk == "//") result = normalize_path (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the uri query
  
  String Uri::getquery (void) const {
    rdlock ();
    try {
      String result = d_query;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the uri fragment
  
  String Uri::getfragment (void) const {
    rdlock ();
    try {
      String result = Uri::pdecode (d_frag);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri base name (scheme+authority)

  String Uri::getbase (void) const {
    rdlock ();
    try {
      String result = d_schm;
      result += ":";
      result += d_amrk;
      result += d_auth;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri rname (scheme+authority+path percent encoded)

  String Uri::getrnam (void) const {
    rdlock ();
    try {
      String result = getbase ();
      if ((d_path.isnil () == true) && (d_amrk == "//")) {
	result += "/";
      } else {
	if (d_amrk == "//") {
	  result += normalize_path (d_path);
	} else {
	  result += d_path;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri hname (scheme+authority+path)

  String Uri::gethnam (void) const {
    rdlock ();
    try {
      String result = Uri::pdecode (getrnam ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the almost uri name (scheme+authority+path+query percent encoded)

  String Uri::getanam (void) const {
    rdlock ();
    try {
      // get the reference name
      String result = getrnam ();
      // add optional query
      if (d_query.isnil () == false) {
	result += '?';
	result += d_query;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the path target name

  String Uri::getptnm (void) const {
    rdlock ();
    try {
      String result = System::xbase (getpath (), URI_PATH_CSEP);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a path to this uri

  Uri Uri::addpath (const String& path) const {
    rdlock ();
    try {
      // fix the path
      String epth = Uri::pencode (path);
      // check if the path is valid
      Regex re ("([<$a+-_=.%:@,;/\\'`~()&^[]{}$<$>>+]?)[$?$N+]?[$#$N+]?");
      if (re != epth) {
	throw Exception ("path-error", "invalid path to add to uri", path);
      }
      String rpth = re.getstring (0);
      if (rpth.isnil () == true) {
	Uri result = *this;
	unlock ();
	return result;
      }
      // rebuild the uri
      String uri = getbase ();
      // check if have an absolute path
      if (rpth.first () == Unicode::toquad ('/')) {
	uri += rpth;
      } else {
	// add the base path
	uri += get_base_path (d_path);
	// check if we add a slash
	if (uri.last () == Unicode::toquad ('/')) {
	  if (rpth.first () == Unicode::toquad ('/')) {
	    uri += rpth.rsubstr (1);
	  } else {
	    uri += rpth;
	  }
	} else {
	  if (rpth.first () == Unicode::toquad ('/')) {
	    uri += rpth;
	  } else {
	    uri += '/';
	    uri += rpth;
	  }
	}
	// add optional query
	if (d_query.isnil () == false) {
	  uri += '?';
	  uri += d_query;
	}
	// add optional fragment
	if (d_frag.isnil () == false) {
	  uri += '?';
	  uri += d_frag;
	}
      }
      // build the result uri
      Uri result = uri;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a new uri by path - if the path is a uri the new uri is returned
  // otherwise, a new uri path is built
  
  Uri Uri::gethref (const String& path) const {
    rdlock ();
    try {
      // check for nil first
      if (path.isnil () == true) {
	Uri result = *this;
	unlock ();
	return result;
      }
      // select from path scheme
      Regex rsch = "($l$a*):($N+)";
      Uri result = (rsch == path) ? path : addpath (path);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a system path if the scheme is a file

  String Uri::getsysp (void) const {
    rdlock ();
    try {
      // check that we have a file scheme
      if (d_schm != "file") {
	throw Exception ("scheme-error", 
			 "cannot convert to system path without file scheme");
      }
      String result = get_sys_path (getpath ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the path encoded name

  String Uri::getpenc (void) const {
    rdlock ();
    try {
      String result = d_path;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri host if any

  String Uri::gethost (void) const {
    rdlock ();
    try {
      // initialize result
      String result;
      // select host from the scheme
      if (d_schm == "mailto") 
	result = get_mail_host (d_path);
      else
	result = get_auth_host (d_auth);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the uri port if any

  long Uri::getport (void) const {
    rdlock ();
    try {
      long result = 0;
      // check for ftp scheme
      if (d_schm == "ftp") {
	result = get_auth_port (d_auth);
	if (result == 0) result = URI_XFTP_PORT;
      } else
      // check for ssh scheme
      if (d_schm == "ssh") {
	result = get_auth_port (d_auth);
	if (result == 0) result = URI_XSSH_PORT;
      } else
      // check for http
      if (d_schm == "http") {
	result = get_auth_port (d_auth);
	if (result == 0) result = URI_HTTP_PORT;
      } else
      // check for https
      if (d_schm == "https") {
	result = get_auth_port (d_auth);
	if (result == 0) result = URI_HTPS_PORT;
      } else
      // check for mailto or default
      if (d_schm == "mailto") 
	result = URI_SMTP_PORT;
      else
	result = get_auth_port (d_auth);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 19;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the uri supported quarks
  static const long QUARK_PARSE       = zone.intern ("parse");
  static const long QUARK_GETNAME     = zone.intern ("get-name");
  static const long QUARK_GETBASE     = zone.intern ("get-base");
  static const long QUARK_GETRNAM     = zone.intern ("get-rname");
  static const long QUARK_GETHNAM     = zone.intern ("get-hname");
  static const long QUARK_GETANAM     = zone.intern ("get-aname");
  static const long QUARK_ADDPATH     = zone.intern ("add-path");
  static const long QUARK_GETHREF     = zone.intern ("get-href");
  static const long QUARK_GETAUTH     = zone.intern ("get-authority");
  static const long QUARK_GETPATH     = zone.intern ("get-path");
  static const long QUARK_GETPTNM     = zone.intern ("get-path-target");
  static const long QUARK_GETSYST     = zone.intern ("get-system-path");
  static const long QUARK_GETPENC     = zone.intern ("get-path-encoded");
  static const long QUARK_GETHOST     = zone.intern ("get-host");
  static const long QUARK_GETPORT     = zone.intern ("get-port");
  static const long QUARK_NRMAUTH     = zone.intern ("normalize-authority");
  static const long QUARK_GETQUERY    = zone.intern ("get-query");
  static const long QUARK_GETSCHEME   = zone.intern ("get-scheme");
  static const long QUARK_GETFRAGMENT = zone.intern ("get-fragment");

  // create a new object in a generic way

  Object* Uri::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Uri;
    // check for 1 argument
    if (argc == 1) {
      String uri = argv->getstring (0);
      return new Uri (uri);
    }
    // check for 3 arguments
    if (argc == 3) {
      String schm = argv->getstring (0);
      String host = argv->getstring (1);
      long   port = argv->getlong (2);;
      return new Uri (schm, host, port);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with uri object");
  }

  // return true if the given quark is defined

  bool Uri::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Uri::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME)     return new String  (getname     ());
      if (quark == QUARK_GETBASE)     return new String  (getbase     ());
      if (quark == QUARK_GETRNAM)     return new String  (getrnam     ());
      if (quark == QUARK_GETHNAM)     return new String  (gethnam     ());
      if (quark == QUARK_GETANAM)     return new String  (getanam     ());
      if (quark == QUARK_GETAUTH)     return new String  (getauth     ());
      if (quark == QUARK_GETPATH)     return new String  (getpath     ());
      if (quark == QUARK_GETPTNM)     return new String  (getptnm     ());
      if (quark == QUARK_GETSYST)     return new String  (getsysp     ());
      if (quark == QUARK_GETPENC)     return new String  (getpenc     ());
      if (quark == QUARK_GETHOST)     return new String  (gethost     ());
      if (quark == QUARK_GETPORT)     return new Integer (getport     ());
      if (quark == QUARK_GETQUERY)    return new String  (getquery    ());
      if (quark == QUARK_GETSCHEME)   return new String  (getscheme   ());
      if (quark == QUARK_GETFRAGMENT) return new String  (getfragment ());
      if (quark == QUARK_NRMAUTH) {
	nrmauth ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	parse (argv->getstring (0));
	return nullptr;
      }
      if (quark == QUARK_ADDPATH) {
	String path = argv->getstring (0);
	return new Uri (addpath (path));
      }
      if (quark == QUARK_GETHREF) {
	String path = argv->getstring (0);
	return new Uri (gethref (path));
      }
    }
    // call the nameable object
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
