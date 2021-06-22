# ---------------------------------------------------------------------------
# - SEC0002.als                                                             -
# - afnix:sec module test unit                                              -
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

# @info   key test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# ---------------------------------------------------------------------------
# - ksym key type                                                           -
# ---------------------------------------------------------------------------

# create ksym type
const KNIL afnix:sec:Key:KNIL
const KSYM afnix:sec:Key:KSYM

# create a default key
const  key1 (afnix:sec:Key)
assert true (afnix:sec:key-p key1)

# check the type
assert KNIL (key1:get-type)

# create an octet string key
const  kstr "0123456789ABCDEF"
const  key2 (afnix:sec:Key kstr)
assert true (afnix:sec:key-p key2)

# get the key type and value
assert KSYM (key2:get-type)
assert 64   (key2:get-bits)
assert 8    (key2:get-size)

# check the key bytes
assert (Byte 0x01) (key2:get-byte 0)
assert (Byte 0x23) (key2:get-byte 1)
assert (Byte 0x45) (key2:get-byte 2)
assert (Byte 0x67) (key2:get-byte 3)
assert (Byte 0x89) (key2:get-byte 4)
assert (Byte 0xAB) (key2:get-byte 5)
assert (Byte 0xCD) (key2:get-byte 6)
assert (Byte 0xEF) (key2:get-byte 7)

# create a typed ksym key
const  key3 (afnix:sec:Key KSYM)
assert true (afnix:sec:key-p key3)

# get the key type and value
assert KSYM (key3:get-type)
assert 128  (key3:get-bits)
assert 16   (key3:get-size)

# ---------------------------------------------------------------------------
# - krsa key type                                                           -
# ---------------------------------------------------------------------------

# create krsa type
const KRSA afnix:sec:Key:KRSA

# create a typed krsa key by size
const  key4 (afnix:sec:Key KRSA 256)
assert true (afnix:sec:key-p key4)

# get the key type and value
assert KRSA (key4:get-type)
assert 256  (key4:get-bits)
assert 32   (key4:get-size)

# extract rsa key component
const  pmod (key4:get-relatif-key afnix:sec:Key:RSA-MODULUS)
const  pexp (key4:get-relatif-key afnix:sec:Key:RSA-PUBLIC-EXPONENT)
const  sexp (key4:get-relatif-key afnix:sec:Key:RSA-SECRET-EXPONENT)

# create a new rsa key by number vector
const  key5 (afnix:sec:Key KRSA (Vector pmod pexp sexp))
assert KRSA (key5:get-type)
assert 256  (key5:get-bits)
assert 32   (key5:get-size)
assert pmod (key5:get-relatif-key afnix:sec:Key:RSA-MODULUS)
assert pexp (key5:get-relatif-key afnix:sec:Key:RSA-PUBLIC-EXPONENT)
assert sexp (key5:get-relatif-key afnix:sec:Key:RSA-SECRET-EXPONENT)

# ---------------------------------------------------------------------------
# - kmac key type                                                           -
# ---------------------------------------------------------------------------

# create kmac type
const KMAC afnix:sec:Key:KMAC

# create an octet string key
const  key6 (afnix:sec:Key KMAC kstr)
assert true (afnix:sec:key-p key6)

# get the key type and value
assert KMAC (key6:get-type)
assert 64   (key6:get-bits)
assert 8    (key6:get-size)

# check the key bytes
assert (Byte 0x01) (key6:get-byte 0)
assert (Byte 0x23) (key6:get-byte 1)
assert (Byte 0x45) (key6:get-byte 2)
assert (Byte 0x67) (key6:get-byte 3)
assert (Byte 0x89) (key6:get-byte 4)
assert (Byte 0xAB) (key6:get-byte 5)
assert (Byte 0xCD) (key6:get-byte 6)
assert (Byte 0xEF) (key6:get-byte 7)

# ---------------------------------------------------------------------------
# - kdsa key type                                                           -
# ---------------------------------------------------------------------------

# create kdsa type
const KDSA afnix:sec:Key:KDSA

# the kdsa key values
const p 11184273624106017745668320055568040296262172596287667637170184215522◀
       ▶11644322071600041606911347857812632210884625974282325995214057201964◀
       ▶71809442632530597812256533479985063923349274008145373227113781476027◀
       ▶49071285969649066781227028265551899153044441267995815740916022439676◀
       ▶3538444647292704799887044108899783011R
const q 1126612938219026988436677875090713967438866401713R
const s 702595380471395552670150686056244850096650324318R
const k 10773145396945336545265655982446048728361860740013215842192739521905◀
       ▶63437421814154750398567521089682942084852986735249846314519202935870◀
       ▶78219710375048770423686702155472417831165330675790439296199523795444◀
       ▶09360726669465701037081772749234676773538391365198972671630818826041◀
       ▶4302251880156452058996335441797722834R
const g 99287620411493013936883020366240234188806852123124491478735177684192◀
       ▶20121959935866937558939085925449921060208232563144217232118381221727◀
       ▶61805400010681584056395168597431064375695533680888127098319463682978◀
       ▶22631984135323631021866128119076904311159316019851270113404626798246◀
       ▶159200287843786154715498174783354777R

# create a typed kdsa key
const  key7 (afnix:sec:Key KDSA (Vector p q g s k))
assert true (afnix:sec:key-p key7)
assert p (key7:get-relatif-key afnix:sec:Key:DSA-P-PRIME)
assert q (key7:get-relatif-key afnix:sec:Key:DSA-Q-PRIME)
assert g (key7:get-relatif-key afnix:sec:Key:DSA-PUBLIC-GENERATOR)
assert s (key7:get-relatif-key afnix:sec:Key:DSA-SECRET-KEY)
assert k (key7:get-relatif-key afnix:sec:Key:DSA-PUBLIC-KEY)

# get the key type and value
assert KDSA (key7:get-type)
assert 1024 (key7:get-bits)
assert 128  (key7:get-size)

# get key prime values
const  l (- p 1)
assert 0R (l:mod q)
