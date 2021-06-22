// ---------------------------------------------------------------------------
// - Xoid.hxx                                                                -
// - afnix:tls service - resistered oid definitions                          -
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

#ifndef  AFNIX_XOID_HXX
#define  AFNIX_XOID_HXX

namespace afnix {

  // the tls oid definition
  static const char* XOID_MEMB_RSA  = "1.2.840.113549";
  static const char* XOID_ALGO_PKCS = "1.2.840.113549.1";
  static const char* XOID_ALGO_PKS1 = "1.2.840.113549.1.1";
  static const char* XOID_ALGO_RSAE = "1.2.840.113549.1.1.1";
  static const char* XOID_ALGO_RSA2 = "1.2.840.113549.1.1.2";
  static const char* XOID_ALGO_RSA4 = "1.2.840.113549.1.1.3";
  static const char* XOID_ALGO_RSA5 = "1.2.840.113549.1.1.4";
  static const char* XOID_ALGO_RSAS = "1.2.840.113549.1.1.5";
  static const char* XOID_ATTR_MAIL = "1.2.840.113549.1.9.1";
  static const char* XOID_ATTR_CTYP = "1.2.840.113549.1.9.3";
  static const char* XOID_ATTR_DGST = "1.2.840.113549.1.9.4";
  static const char* XOID_ALGO_DGST = "1.2.840.113549.2";
  static const char* XOID_ALGO_CPTO = "1.2.840.113549.3";

  static const char* XOID_PORG_FIFS = "1.3.6.1.4.1.28509";
  static const char* XOID_PORG_ETHX = "1.3.6.1.4.1.40112";

  static const char* XOID_ATTR_CN   = "2.5.4.3";
  static const char* XOID_ATTR_SRN  = "2.5.4.4";
  static const char* XOID_ATTR_SN   = "2.5.4.5";
  static const char* XOID_ATTR_C    = "2.5.4.6";
  static const char* XOID_ATTR_L    = "2.5.4.7";
  static const char* XOID_ATTR_ST   = "2.5.4.8";
  static const char* XOID_ATTR_STR  = "2.5.4.9";
  static const char* XOID_ATTR_O    = "2.5.4.10";
  static const char* XOID_ATTR_OU   = "2.5.4.11";
  static const char* XOID_ATTR_GN   = "2.5.4.42";
  static const char* XOID_ATTR_INI  = "2.5.4.43";
  static const char* XOID_ATTR_GQN  = "2.5.4.44";
  static const char* XOID_ATTR_DN   = "2.5.4.49";
  static const char* XOID_ATTR_PSD  = "2.5.4.65";

  // the oid information structure
  struct t_xoid {
    // the oid string
    const char* d_xoid;
    // the oid name
    const char* d_name;
    // the oid info
    const char* d_info;
  };

  // the oid array
  static t_xoid TLS_XOID_BASE[] = {
    {"0",            "itu-t",                   "ITU-T"},
    {"1",            "iso",                     "ISO"},
    {"2",            "joint-iso-itu-t",         "JOINT-ISO-ITU-T"},
    // iso
    {"1.2",          "member-body",             "ISO member body"},
    {"1.2.840",      "member-body-us",          "US member body"},
    {XOID_MEMB_RSA,  "rsadsi",                  "RSA Data Security Inc"},
    {XOID_ALGO_PKCS, "pkcs"             
     "Public Key Cryptographic Standard"},
    {XOID_ALGO_PKS1, "pkcs-1",          
     "Public Key Cryptographic Standard 1"},
    {XOID_ALGO_RSAE, "rsa",                     "RSA Encryption"},
    {XOID_ALGO_RSA2, "md2-rsa",                 "MD2 with RSA Encryption"},
    {XOID_ALGO_RSA4, "md4-rsa",                 "MD4 with RSA Encryption"},
    {XOID_ALGO_RSA5, "md5-rsa",                 "MD5 with RSA Encryption"},
    {XOID_ALGO_RSAS, "sha1-rsa",                "SHA1 with RSA Signature"},
    {XOID_ATTR_MAIL, "email-address",           "Email Address"},
    {XOID_ATTR_CTYP, "content-type",            "Content Type"},
    {XOID_ATTR_DGST, "message-digest",          "Message Digest"},
    {XOID_ALGO_DGST, "digest",                  "Digest algorithm"},
    {XOID_ALGO_CPTO, "encryption",              "Encryption algorithm"},
    {"1.3",          "identified-organization", "ISO organization"},
    {"1.3.6",        "dod",                     "US DOD"},
    {"1.3.6.1",      "internet",                "US DOD Internet"},
    {"1.3.6.1.4",    "private",                 "Internet Organization"},
    {"1.3.6.1.4.1",  "enterprise",              "Internet Enterprise"},
    {XOID_PORG_FIFS, "fifsource",               "Fif Source"},
    {XOID_PORG_ETHX, "ethernix",                "The Ethernix Company"},
    // joint-iso-itu-t
    {"2.5",          "directory service",       "Director Service"},
    {"2.5.4",        "attribute-type",          "Attribute Type"},
    {"2.5.4.1",      "aliased-entry-name",      "Aliased Entry Name"},
    {"2.5.4.2",      "knowledge information",   "Knowledge Information"},
    {XOID_ATTR_CN,   "common-name",             "Common Name"},
    {XOID_ATTR_SRN,  "surname",                 "Surname"},
    {XOID_ATTR_SN,   "serial-number",           "Serial Number"},
    {XOID_ATTR_C,    "country",                 "Country Name"},
    {XOID_ATTR_L,    "locality",                "Locality Name"},
    {XOID_ATTR_ST,   "state",                   "State or Province Name"},
    {XOID_ATTR_STR,  "street",                  "Street Address"},
    {XOID_ATTR_O,    "organization",            "Organization Name"},
    {XOID_ATTR_OU,   "organization-unit",       "Organization Unit Name"},
    {XOID_ATTR_GN,   "given-name",              "Given Name"},
    {XOID_ATTR_INI,  "initials",                "Initials"},
    {XOID_ATTR_GQN,  "generation-qualifier",    "Generation Qualifier"},
    {XOID_ATTR_DN,   "distinguished-name",      "Distinguished Name"},
    {XOID_ATTR_PSD,  "pseudonym",               "Pseudonym"}
  };

  // the oid tls table size
  static const long TLS_XOID_SIZE = sizeof (TLS_XOID_BASE) / sizeof (t_xoid);
}

#endif
