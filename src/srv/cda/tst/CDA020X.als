# ---------------------------------------------------------------------------
# - CDA020X.als                                                             -
# - afnix:cda service test unit                                              -
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

# @info   streamer test unit
# @author amaury darsch

# get the module
interp:library "afnix-sps"
interp:library "afnix-cda"

# create a new sheet
const new-sps-sheet nil {
  # create a new sheet transfer
  const st (afnix:sps:SpsTransit)
  # import a csv into a sheet
  st:import "CDA020X.csv"
  st:get-sheet
}

# create a new sps streamer
const new-sps-streamer (col) {
  # get the sps sheet
  const sht (new-sps-sheet)
  # create a sps streamer
  const sps (afnix:cda:Spsds sht)
  sps:set-col-index col
  eval sps
}

# create a new linear streamer
const new-lnr-streamer (col) {
  # get the sps streamer
  const stm (new-sps-streamer col)
  # create a linear streamer
  afnix:cda:Lnrds stm
}
