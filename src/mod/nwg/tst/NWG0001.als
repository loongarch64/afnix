# ---------------------------------------------------------------------------
# - NWG0001.als                                                             -
# - afnix:nwg module test unit                                              -
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

# @info   uri test unit
# @author amaury darsch

# get the module
interp:library "afnix-nwg"

# check string uri
trans  snm  (afnix:nwg:to-string-uri "http" "www.afnix.org" 80)
assert snm  "http://www.afnix.org:80"
assert true (afnix:nwg:string-uri-p snm)

# check the normalized uri
trans  unm "http://www.afnix.org"
assert unm (afnix:nwg:normalize-uri-name unm)
assert unm (afnix:nwg:normalize-uri-name "www.afnix.org")
assert unm (afnix:nwg:normalize-uri-name "//www.afnix.org")

# check the path normalization
trans  unm "/"
assert unm (afnix:nwg:path-uri-name "http://www.afnix.org")
assert unm (afnix:nwg:path-uri-name "/")
trans  unm "../afnix"
assert unm (afnix:nwg:path-uri-name "../afnix")
assert unm (afnix:nwg:path-uri-name "../afnix?lang=als")
assert unm (afnix:nwg:path-uri-name "../afnix?lang=als#tester")

# check for a simple uri
trans  unm "http://www.afnix.org"
trans  uri (afnix:nwg:Uri unm)
assert true                (afnix:nwg:uri-p uri)
assert unm                 (uri:get-name)

# check components
assert "http"              (uri:get-scheme)
assert "www.afnix.org"     (uri:get-authority)
assert "www.afnix.org"     (uri:get-host)
assert ""                  (uri:get-path)
assert ""                  (uri:get-path-target)
assert ""                  (uri:get-query)
assert ""                  (uri:get-fragment)

# check for a simple uri
trans  unm "http://www.afnix.org/"
trans  uri (afnix:nwg:Uri unm)
assert "http"              (uri:get-scheme)
assert "www.afnix.org"     (uri:get-authority)
assert "www.afnix.org"     (uri:get-host)
assert "/"                 (uri:get-path)
assert unm                 (uri:get-hname)

# check for a simple uri with a port
trans  uri (afnix:nwg:Uri "http" "www.afnix.org" 888)
assert "http"              (uri:get-scheme)
assert "www.afnix.org:888" (uri:get-authority)

# check for a simple uri
trans  unm "http://www.afnix.org/fr/âllô"
trans  rnm "http://www.afnix.org/fr/a%CC%82llo%CC%82"
trans  uri (afnix:nwg:Uri (unm:to-normal-form))
assert "http"              (uri:get-scheme)
assert "www.afnix.org"     (uri:get-authority)
assert rnm                 (uri:get-rname)

trans  unm "http://www.afnix.org/fr/"
trans  uri (afnix:nwg:Uri unm)
assert "http"              (uri:get-scheme)
assert "www.afnix.org"     (uri:get-authority)
assert "/fr/"              (uri:get-path)
assert unm                 (uri:get-hname)

# check add-path
trans  uri (uri:add-path "/hello/")
assert "/hello/"           (uri:get-path)
assert "hello"             (uri:get-path-target)

# check for a uri with a path
trans  unm "http://www.afnix.org/example"
trans  bnm "http://www.afnix.org"
trans  hnm "http://www.afnix.org/example"
trans  uri (afnix:nwg:Uri unm)
assert unm                 (uri:get-name)
assert bnm                 (uri:get-base)
assert hnm                 (uri:get-hname)
assert "http"              (uri:get-scheme)
assert "www.afnix.org"     (uri:get-authority)
assert "/example"          (uri:get-path)
assert "example"           (uri:get-path-target)
assert ""                  (uri:get-query)
assert ""                  (uri:get-fragment)

# check add-path
trans  uri (uri:add-path "/hello")
assert "/hello"            (uri:get-path)
trans  uri (uri:add-path "../")
assert "/"                 (uri:get-path)
trans  uri (uri:add-path "../")
assert "/"                 (uri:get-path)
trans  uri (uri:add-path "/hello/./world/./example/..")
assert "/hello/world"      (uri:get-path)

# check for a uri with a path and a query
trans  unm "http://www.afnix.org/example?val=hello+world" 
trans  uri (afnix:nwg:Uri unm)
assert unm                 (uri:get-name)
assert "http"              (uri:get-scheme)
assert "www.afnix.org"     (uri:get-authority)
assert "/example"          (uri:get-path)
assert "val=hello+world"   (uri:get-query)
assert ""                  (uri:get-fragment)
assert "www.afnix.org"     (uri:get-host)
assert 80                  (uri:get-port)

# check for a complete uri
trans  unm "http://a.b.org:2000/example?val=hello+world#good"
trans  anm "http://a.b.org:2000/example?val=hello+world"
trans  uri (afnix:nwg:Uri unm)
assert unm                 (uri:get-name)
assert anm                 (uri:get-aname)
assert "http"              (uri:get-scheme)
assert "a.b.org:2000"      (uri:get-authority)
assert "a.b.org"           (uri:get-host)
assert 2000                (uri:get-port)
assert "/example"          (uri:get-path)
assert "val=hello+world"   (uri:get-query)
assert "good"              (uri:get-fragment)

# check the normalized uri
trans  unm "file:///home/afnix"
assert unm (afnix:nwg:normalize-uri-name unm)
assert unm (afnix:nwg:normalize-uri-name "/home/afnix")

# check for a file uri
trans  unm "file:///home/afnix"
trans  uri (afnix:nwg:Uri unm)
assert unm                 (uri:get-name)
assert "file"              (uri:get-scheme)
assert ""                  (uri:get-authority)
assert "/home/afnix"       (uri:get-path)
assert "afnix"             (uri:get-path-target)

# check for a mail uri
trans  unm "mailto:support@afnix.org"
trans  uri (afnix:nwg:Uri unm)
assert unm                 (uri:get-name)
assert "mailto"            (uri:get-scheme)
assert ""                  (uri:get-authority)
assert "support@afnix.org" (uri:get-path)
assert "support@afnix.org" (uri:get-path-target)
assert "afnix.org"         (uri:get-host)
assert 25                  (uri:get-port)

# check for a urn
trans  unm "urn:example:afnix"
trans  uri (afnix:nwg:Uri unm)
assert unm                 (uri:get-name)
assert "urn"               (uri:get-scheme)
assert ""                  (uri:get-authority)
assert "example:afnix"     (uri:get-path)
assert "example:afnix"     (uri:get-path-target)

# test uri with encoded path
#trans  uri (afnix:nwg:Uri "file:///%c3%a7")
#assert "/%c3%a7" (uri:get-path-encoded)

# test uri with special characters
trans  uri (afnix:nwg:Uri "http://www.afnix.org/programming_&_language")
assert "www.afnix.org" (uri:get-authority)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data^")
assert "/data^" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data_[A-Z]/")
assert "/data_[A-Z]/" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data_<A-Z>;")
assert "/data_<A-Z>;" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data_A=Z")
assert "/data_A=Z" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/`data")
assert "/`data" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/{data}")
assert "/{data}" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data\\")
assert "/data\\" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data!")
assert "/data!" (uri:get-path)
trans  uri (afnix:nwg:Uri "http://www.afnix.org/data*")
assert "/data*" (uri:get-path)

# check for a simple uri with an ip address
trans  unm "http://127.0.0.1/"
trans  uri (afnix:nwg:Uri unm)
assert "http"       (uri:get-scheme)
assert "127.0.0.1"  (uri:get-authority)
assert "/"          (uri:get-path)
assert unm          (uri:get-hname)

# check for a simple uri with an ip address
trans  unm "http://[::1]/"
trans  uri (afnix:nwg:Uri unm)
assert "http"   (uri:get-scheme)
assert "[::1]"  (uri:get-authority)
assert "::1"    (uri:get-host)
assert "/"      (uri:get-path)
assert unm      (uri:get-hname)
# check more complex uri
trans  unm "http://[::1]:2020/home/afnix"
trans  uri (afnix:nwg:Uri unm)
assert "http"        (uri:get-scheme)
assert "[::1]:2020"  (uri:get-authority)
assert "::1"         (uri:get-host)
assert 2020          (uri:get-port)
assert "/home/afnix" (uri:get-path)
assert unm           (uri:get-hname)

# check for normalize authority
assert "localhost"   (afnix:nwg:normalize-uri-host "localhost")
assert 0             (afnix:nwg:normalize-uri-port "localhost")
assert "localhost"   (afnix:nwg:normalize-uri-host "localhost:2020")
assert 2020          (afnix:nwg:normalize-uri-port "localhost:2020")
assert "127.0.0.1"   (afnix:nwg:normalize-uri-host "127.0.0.1")
assert 0             (afnix:nwg:normalize-uri-port "127.0.0.1")
assert "127.0.0.1"   (afnix:nwg:normalize-uri-host "127.0.0.1:2020")
assert 2020          (afnix:nwg:normalize-uri-port "127.0.0.1:2020")
assert "::1"         (afnix:nwg:normalize-uri-host "[::1]")
assert 0             (afnix:nwg:normalize-uri-port "[::1]")
assert "::1"         (afnix:nwg:normalize-uri-host "[::1]:2020")
assert 2020          (afnix:nwg:normalize-uri-port "[::1]:2020")
assert "2001:db8::"  (afnix:nwg:normalize-uri-host "[2001:db8::]")
assert 0             (afnix:nwg:normalize-uri-port "[2001:db8::]")
assert "2001:db8::"  (afnix:nwg:normalize-uri-host "[2001:db8::]:2020")
assert 2020          (afnix:nwg:normalize-uri-port "[2001:db8::]:2020")
