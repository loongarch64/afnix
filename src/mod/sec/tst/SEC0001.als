# ---------------------------------------------------------------------------
# - SEC0001.als                                                             -
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

# @info   hasher test unit
# @author amaury darsch

# get the module
interp:library "afnix-sec"

# ---------------------------------------------------------------------------
# - MD2 test section                                                        -
# ---------------------------------------------------------------------------

# get the md2 digest
const md-2 (afnix:sec:Md2)

# check the name
assert "MD-2" (md-2:get-name)

# check with a string
assert "8350E5A3E24C153DF2275C9F80692773" (md-2:compute "")
assert "32EC01EC4A6DAC72C0AB96FB34C0B5D1" (md-2:compute "a")
assert "DA853B0D3F88D99B30283A69E6DED6BB" (md-2:compute "abc")
assert "D9CCE882EE690A5C1CE70BEFF3A78C77" (md-2:compute "hello world")

# check with a buffer
assert "8350E5A3E24C153DF2275C9F80692773" (md-2:compute (Buffer ""))
assert "32EC01EC4A6DAC72C0AB96FB34C0B5D1" (md-2:compute (Buffer "a"))
assert "DA853B0D3F88D99B30283A69E6DED6BB" (md-2:compute (Buffer "abc"))
assert "D9CCE882EE690A5C1CE70BEFF3A78C77" (md-2:compute (Buffer "hello world"))

# check format value
assert true (md-2:hash-p (md-2:compute ""))

# ---------------------------------------------------------------------------
# - MD4 test section                                                        -
# ---------------------------------------------------------------------------

# get the md4 digest
const md-4 (afnix:sec:Md4)

# check the name
assert "MD-4" (md-4:get-name)

# check with a string
assert "31D6CFE0D16AE931B73C59D7E0C089C0" (md-4:compute "")
assert "BDE52CB31DE33E46245E05FBDBD6FB24" (md-4:compute "a")
assert "A448017AAF21D8525FC10AE87AA6729D" (md-4:compute "abc")
assert "AA010FBC1D14C795D86EF98C95479D17" (md-4:compute "hello world")

# check with a buffer
assert "31D6CFE0D16AE931B73C59D7E0C089C0" (md-4:compute (Buffer ""))
assert "BDE52CB31DE33E46245E05FBDBD6FB24" (md-4:compute (Buffer "a"))
assert "A448017AAF21D8525FC10AE87AA6729D" (md-4:compute (Buffer "abc"))
assert "AA010FBC1D14C795D86EF98C95479D17" (md-4:compute (Buffer "hello world"))

# check format value
assert true (md-4:hash-p (md-4:compute ""))

# ---------------------------------------------------------------------------
# - MD5 test section                                                        -
# ---------------------------------------------------------------------------

# get a default digest (MD5)
const md-5 (afnix:sec:Md5)

# check the name
assert "MD-5" (md-5:get-name)

# check with a string
assert "D41D8CD98F00B204E9800998ECF8427E" (md-5:compute "")
assert "0CC175B9C0F1B6A831C399E269772661" (md-5:compute "a")
assert "900150983CD24FB0D6963F7D28E17F72" (md-5:compute "abc")
assert "5EB63BBBE01EEED093CB22BB8F5ACDC3" (md-5:compute "hello world")

# check with a buffer
assert "D41D8CD98F00B204E9800998ECF8427E" (md-5:compute (Buffer ""))
assert "0CC175B9C0F1B6A831C399E269772661" (md-5:compute (Buffer "a"))
assert "900150983CD24FB0D6963F7D28E17F72" (md-5:compute (Buffer "abc"))
assert "5EB63BBBE01EEED093CB22BB8F5ACDC3" (md-5:compute (Buffer "hello world"))

# check format value
assert true (md-5:hash-p (md-5:compute ""))

# ---------------------------------------------------------------------------
# - SHA-1 test section                                                      -
# ---------------------------------------------------------------------------

# get a SHA-1 digest 
const sha-1 (afnix:sec:Sha1)

# check the name
assert "SHA-1" (sha-1:get-name)

# check with a string
assert "A9993E364706816ABA3E25717850C26C9CD0D89D" (sha-1:compute "abc")
assert "FAD1FD98E882B0013DD257DF063BF4A2926EDA17" (sha-1:compute "afnix")

# check format value
assert true (sha-1:hash-p (sha-1:compute ""))

# ---------------------------------------------------------------------------
# - SHA-256 test section                                                    -
# ---------------------------------------------------------------------------

# get a SHA-256 digest 
const sha-256 (afnix:sec:Sha256)

# check the name
assert "SHA-256" (sha-256:get-name)

# check with a string
trans  MD1 "BA7816BF8F01CFEA414140DE5DAE2223"
trans  MD2 "B00361A396177A9CB410FF61F20015AD"
trans  MDS (+ MD1 MD2)
assert MDS (sha-256:compute "abc")

trans  MD1 "DEBDB627802EECBF0171E229D9470CB6"
trans  MD2 "E94DD7E713D80E61D0B877A01B56AF2F"
trans  MDS (+ MD1 MD2)
assert MDS (sha-256:compute "afnix")

# check format value
assert true (sha-256:hash-p (sha-256:compute ""))

# ---------------------------------------------------------------------------
# - SHA-224 test section                                                    -
# ---------------------------------------------------------------------------

# get a SHA-224 digest 
const sha-224 (afnix:sec:Sha224)

# check the name
assert "SHA-224" (sha-224:get-name)

# check with a string
trans  MDS "23097D223405D8228642A477BDA255B32AADBCE4BDA0B3F7E36C9DA7"
assert MDS (sha-224:compute "abc")

trans  MDS "7EC37060130877BE6D449C761012D1BFAC19CE2AD2265F1FCF0E6990"
assert MDS (sha-224:compute "afnix")

# check format value
assert true (sha-224:hash-p (sha-224:compute ""))

# ---------------------------------------------------------------------------
# - SHA-384 test section                                                    -
# ---------------------------------------------------------------------------

# get a SHA-384 digest 
const sha-384 (afnix:sec:Sha384)

# check the name
assert "SHA-384" (sha-384:get-name)

# check with a string
trans  MD1 "CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED163"
trans  MD2 "1A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7"
trans  MDS (+ MD1 MD2)
assert MDS (sha-384:compute "abc")

trans  MD1 "33ABEC90674F5DC0823578EF189498FF8B98B8A82FB7CEFD"
trans  MD2 "ACBF39428AAD03213ADCDC1317E884A86A9DEBD2FFF6B3D4"
trans  MDS (+ MD1 MD2)
assert MDS (sha-384:compute "afnix")

# check format value
assert true (sha-384:hash-p (sha-384:compute ""))

# ---------------------------------------------------------------------------
# - SHA-512 test section                                                    -
# ---------------------------------------------------------------------------

# get a SHA-512 digest 
const sha-512 (afnix:sec:Sha512)

# check the name
assert "SHA-512" (sha-512:get-name)

# check with a string
trans  MD1 "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A"
trans  MD2 "2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F"
trans  MDS (+ MD1 MD2)
assert MDS (sha-512:compute "abc")

# check with a string
trans  MD1 "40B11B89056BBB0BD2266AD30E1ADE9DEE2CECFFB8950A23AE4FE666EC80E054"
trans  MD2 "10727E900029313F1D10269F1609638FB0CB82D419D82743A96167AA1E80E21E"
trans  MDS (+ MD1 MD2)
assert MDS (sha-512:compute "afnix")

# check format value
assert true (sha-512:hash-p (sha-512:compute ""))

# ---------------------------------------------------------------------------
# - SHA-3-224 test section                                                  -
# ---------------------------------------------------------------------------

# get a SHA-3-224 digest 
const sha-3224 (afnix:sec:Sha3224)

# check the name
assert "SHA-3-224" (sha-3224:get-name)

# check empty string
trans  MDS "6B4E03423667DBB73B6E15454F0EB1ABD4597F9A1B078E3F5B5A6BC7"
assert MDS (sha-3224:compute "")

# check with a string
trans  MDS "E642824C3F8CF24AD09234EE7D3C766FC9A3A5168D0C94AD73B46FDF"
assert MDS (sha-3224:compute "abc")

# check with a string
trans  MDS "96125C56F1723A8281C1F33FBC31BF14D526AFE5215D7ABE03A59D0B"
assert MDS (sha-3224:compute "afnix")

# check format value
assert true (sha-224:hash-p (sha-224:compute ""))

# ---------------------------------------------------------------------------
# - SHA-3-256 test section                                                  -
# ---------------------------------------------------------------------------

# get a SHA-3-256 digest 
const sha-3256 (afnix:sec:Sha3256)

# check the name
assert "SHA-3-256" (sha-3256:get-name)

# check empty string
trans  MDS "A7FFC6F8BF1ED76651C14756A061D662F580FF4DE43B49FA82D80A4B80F8434A"
assert MDS (sha-3256:compute "")

# check with a string
trans  MDS "3A985DA74FE225B2045C172D6BD390BD855F086E3E9D525B46BFE24511431532"
assert MDS (sha-3256:compute "abc")

# check with a string
trans  MDS "90AA563D9E5AB90D9A5BC13257C2E3DC22485A312D15E139026B1B17509E9246"
assert MDS (sha-3256:compute "afnix")

# check format value
assert true (sha-256:hash-p (sha-256:compute ""))

# ---------------------------------------------------------------------------
# - SHA-3-384 test section                                                  -
# ---------------------------------------------------------------------------

# get a SHA-3-384 digest 
const sha-3384 (afnix:sec:Sha3384)

# check the name
assert "SHA-3-384" (sha-3384:get-name)

# check empty string
trans  MD1 "0C63A75B845E4F7D01107D852E4C2485C51A50AAAA94FC61"
trans  MD2 "995E71BBEE983A2AC3713831264ADB47FB6BD1E058D5F004"
trans  MDS (+ MD1 MD2)
assert MDS (sha-3384:compute "")

# check with a string
trans  MD1 "EC01498288516FC926459F58E2C6AD8DF9B473CB0FC08C25"
trans  MD2 "96DA7CF0E49BE4B298D88CEA927AC7F539F1EDF228376D25"
trans  MDS (+ MD1 MD2)
assert MDS (sha-3384:compute "abc")

# check with a string
trans  MD1 "7BACC957EDD0BB1325F1E54C024C94F490FF24AD22907DC7"
trans  MD2 "F96FE5F7440362A13222FBACEB94B879C661CB2E4101842A"
trans  MDS (+ MD1 MD2)
assert MDS (sha-3384:compute "afnix")

# check format value
assert true (sha-384:hash-p (sha-384:compute ""))

# ---------------------------------------------------------------------------
# - SHA-3-512 test section                                                  -
# ---------------------------------------------------------------------------

# get a SHA-3-512 digest 
const sha-3512 (afnix:sec:Sha3512)

# check the name
assert "SHA-3-512" (sha-3512:get-name)

# check empty string
trans  MD1 "A69F73CCA23A9AC5C8B567DC185A756E97C982164FE25859E0D1DCC1475C80A6"
trans  MD2 "15B2123AF1F5F94C11E3E9402C3AC558F500199D95B6D3E301758586281DCD26"
trans  MDS (+ MD1 MD2)
assert MDS (sha-3512:compute "")

# check with a string
trans  MD1 "B751850B1A57168A5693CD924B6B096E08F621827444F70D884F5D0240D2712E"
trans  MD2 "10E116E9192AF3C91A7EC57647E3934057340B4CF408D5A56592F8274EEC53F0"
trans  MDS (+ MD1 MD2)
assert MDS (sha-3512:compute "abc")

# check with a string
trans  MD1 "1C38B8BEA6BB09107635F058F0FDCB04DFEB324A38F520BE0EE0F0382CA8B981"
trans  MD2 "2E7A71BCA7635FACF1D65B399A30885AAFD5C28C0ACBAA9C74CDC2B4BC082574"
trans  MDS (+ MD1 MD2)
assert MDS (sha-3512:compute "afnix")

# check format value
assert true (sha-512:hash-p (sha-512:compute ""))
