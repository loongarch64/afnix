# ---------------------------------------------------------------------------
# - XSIO001.als                                                             -
# - afnix example : sio module example                                      -
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

# cat a file on the output terminal
# usage: afnix XSIO001.als [file]

# get the module
interp:library "afnix-sio"

# cat a file
const cat (name) {
  const f (afnix:sio:InputFile name)
  while (f:valid-p) (println (f:readln))
  f:close
}

# get the file 
if (== 0 (interp:argv:length)) {
  errorln "usage: afnix net-0001.als file"
} {
  cat (interp:argv:get 0)
}

