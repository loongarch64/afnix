# ---------------------------------------------------------------------------
# - TLS0509.als                                                             -
# - afnix:tls module test unit                                              -
# ---------------------------------------------------------------------------
# - This program is free software;  you can redistribute it  and/or  modify -
# - it provided that this copyright notice is kept intact.                  -
# -                                                                         -
# - This program  is  distributed in  the hope  that it will be useful, but -
# - without  any  warranty;  without  even   the   implied    warranty   of -
# - merchantability or fitness for a particular purpose.  In no event shall -
# - the copyright holder be liable for any  direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.     -
# ---------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                   -
# ---------------------------------------------------------------------------

# @info   X509 test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"
interp:library "afnix-tls"

# ---------------------------------------------------------------------------
# - PKCS 1 test                                                             -
# ---------------------------------------------------------------------------

# create a pkcs key by path
const krsa (afnix:tls:Pkcs1 "RSA0509.der")
const pkey (krsa:get-key)
const pmod (pkey:get-relatif-key afnix:sec:Key:RSA-MODULUS)
const pexp (pkey:get-relatif-key afnix:sec:Key:RSA-PUBLIC-EXPONENT)

assert afnix:sec:Key:KRSA (pkey:get-type)
assert 2048 (pkey:get-bits)

# ---------------------------------------------------------------------------
# - X509 test                                                               -
# ---------------------------------------------------------------------------

# create a certificate by path
const cert (afnix:tls:X509 "TLS0509.der")

# check representation
assert true   (afnix:tls:pkcs-p cert)
assert true   (afnix:tls:x509-p cert)
assert "X509" (cert:repr)

# check version
assert 2  (cert:get-version)
# check serial number
assert 1R (cert:get-serial-number)
# check signature algorithm (sha256WithRSAEncryption)
const slgo (cert:get-signature-algorithm)
assert "1.2.840.113549.1.1.11" (slgo:get-identifier)
# get the issuer plist
const ilst  (cert:get-issuer-plist)
assert   6  (ilst:length)
# check the oid
assert "FR"  (ilst:get-value "2.5.4.6")
assert "BZH" (ilst:get-value "2.5.4.8")
assert "LORIENT" (ilst:get-value "2.5.4.7")

# get the subject plist
const slst  (cert:get-subject-plist)
assert   6  (slst:length)
# check the oid
assert "FR" (slst:get-value "2.5.4.6")
assert "BZH" (slst:get-value "2.5.4.8")
assert "LORIENT" (slst:get-value "2.5.4.7")

# check the validity sequence
assert 63752026405 (cert:get-validity-before)
assert 63776650405 (cert:get-validity-after)

# check subject public key algorithm (RSAEncryption)
const spka (cert:get-subject-key-algorithm)
assert "1.2.840.113549.1.1.1" (spka:get-identifier)
# check subject public key info
const spki  (cert:get-subject-key-info)
assert true (bitset-p spki)
assert 2160 (spki:length)
# check the key
const  skey (cert:get-subject-key)
const  rsae (skey:to-key)
assert afnix:sec:Key:KRSA (rsae:get-type)
assert 2048 (rsae:get-bits)

# check the unique id
assert nil (cert:get-issuer-unique-id)
assert nil (cert:get-subject-unique-id)

# check extensions
const  exts (cert:get-extension-vector)
assert 3    (exts:length)

# ---------------------------------------------------------------------------
# - consistency test                                                        -
# ---------------------------------------------------------------------------

assert pmod (rsae:get-relatif-key afnix:sec:Key:RSA-MODULUS)
assert pexp (rsae:get-relatif-key afnix:sec:Key:RSA-PUBLIC-EXPONENT)
