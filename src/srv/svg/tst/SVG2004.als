# ---------------------------------------------------------------------------
# - SVG2004.als                                                             -
# - afnix:svg service test unit                                             -
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

# @info   svg plot2d test unit
# @author amaury darsch

# get the module
interp:library "afnix-sio"
interp:library "afnix-mth"
interp:library "afnix-svg"

# this procedure add data to a samples
const rsa-add-data (rsa x y) {
  # create a new row
  const row (rsa:new-row)
  # add the data
  rsa:set row 0 x
  rsa:set row 1 y
}

# create a 2 column samples
const rsa (afnix:mth:Rsamples 2)
# add the data
rsa-add-data rsa -1.0000000000e+00 -8.8386143690e-22
rsa-add-data rsa -9.9000000022e-01 -2.5579538491e-11
rsa-add-data rsa -9.8000000045e-01 -2.5579538490e-11
rsa-add-data rsa -9.6999999974e-01 -2.5579538489e-11
rsa-add-data rsa -9.5999999996e-01 -2.5579538488e-11
rsa-add-data rsa -9.5000000019e-01 -2.5579538492e-11
rsa-add-data rsa -9.4000000041e-01 -2.5579538486e-11
rsa-add-data rsa -9.3000000063e-01 -2.5579538496e-11
rsa-add-data rsa -9.1999999993e-01 -2.5579538494e-11
rsa-add-data rsa -9.1000000015e-01 -2.5579538490e-11
rsa-add-data rsa -9.0000000037e-01 -2.5636381908e-11
rsa-add-data rsa -8.9000000060e-01 -2.5636381911e-11
rsa-add-data rsa -8.7999999989e-01 -2.5636381909e-11
rsa-add-data rsa -8.7000000011e-01 -2.5636381904e-11
rsa-add-data rsa -8.6000000034e-01 -2.5636381903e-11
rsa-add-data rsa -8.5000000009e-01 -2.5636381904e-11
rsa-add-data rsa -8.4000000032e-01 -2.5636381905e-11
rsa-add-data rsa -8.3000000007e-01 -2.5636381900e-11
rsa-add-data rsa -8.2000000030e-01 -2.5636381902e-11
rsa-add-data rsa -8.1000000006e-01 -2.5636381904e-11
rsa-add-data rsa -8.0000000028e-01 -2.5636381903e-11
rsa-add-data rsa -7.9000000004e-01 -2.5636381901e-11
rsa-add-data rsa -7.8000000026e-01 -2.5636381909e-11
rsa-add-data rsa -7.7000000002e-01 -2.5636381897e-11
rsa-add-data rsa -7.6000000024e-01 -2.5636381906e-11
rsa-add-data rsa -7.5000000000e-01 -2.5636381897e-11
rsa-add-data rsa -7.4000000022e-01 -2.5636381908e-11
rsa-add-data rsa -7.2999999998e-01 -2.5636381910e-11
rsa-add-data rsa -7.2000000020e-01 -2.5636381896e-11
rsa-add-data rsa -7.0999999996e-01 -2.5636381895e-11
rsa-add-data rsa -7.0000000019e-01 -2.5636381909e-11
rsa-add-data rsa -6.8999999994e-01 -2.5636381906e-11
rsa-add-data rsa -6.7999999993e-01 -2.5636381898e-11
rsa-add-data rsa -6.6999999993e-01 -2.5636381906e-11
rsa-add-data rsa -6.5999999992e-01 -2.5636381910e-11
rsa-add-data rsa -6.4999999991e-01 -2.5636381904e-11
rsa-add-data rsa -6.3999999990e-01 -2.5636381906e-11
rsa-add-data rsa -6.2999999989e-01 -2.5607960193e-11
rsa-add-data rsa -6.1999999988e-01 -2.5607960195e-11
rsa-add-data rsa -6.0999999987e-01 -2.5607960192e-11
rsa-add-data rsa -5.9999999986e-01 -2.5607960191e-11
rsa-add-data rsa -5.8999999985e-01 -2.5607960199e-11
rsa-add-data rsa -5.7999999984e-01 -2.5607960202e-11
rsa-add-data rsa -5.6999999983e-01 -2.5607960195e-11
rsa-add-data rsa -5.5999999982e-01 -2.5607960194e-11
rsa-add-data rsa -5.5000000005e-01 -2.5607960192e-11
rsa-add-data rsa -5.4000000004e-01 -2.5607960193e-11
rsa-add-data rsa -5.3000000003e-01 -2.5607960191e-11
rsa-add-data rsa -5.2000000002e-01 -2.5607960188e-11
rsa-add-data rsa -5.1000000001e-01 -2.5607960191e-11
rsa-add-data rsa -5.0000000000e-01 -2.5607960199e-11
rsa-add-data rsa -4.9000000011e-01 -2.5607960200e-11
rsa-add-data rsa -4.8000000010e-01 -2.5607960189e-11
rsa-add-data rsa -4.7000000009e-01 -2.5607960196e-11
rsa-add-data rsa -4.6000000008e-01 -2.5607960201e-11
rsa-add-data rsa -4.5000000007e-01 -2.5607960198e-11
rsa-add-data rsa -4.4000000006e-01 -2.5607960195e-11
rsa-add-data rsa -4.2999999999e-01 -2.5607960194e-11
rsa-add-data rsa -4.1999999998e-01 -2.5607960195e-11
rsa-add-data rsa -4.0999999997e-01 -2.5607960205e-11
rsa-add-data rsa -4.0000000002e-01 -2.5607960202e-11
rsa-add-data rsa -3.9000000001e-01 -2.5607960204e-11
rsa-add-data rsa -3.8000000000e-01 -2.5607960200e-11
rsa-add-data rsa -3.7000000000e-01 -2.5607960192e-11
rsa-add-data rsa -3.5999999999e-01 -2.5607960192e-11
rsa-add-data rsa -3.4999999998e-01 -2.5607960197e-11
rsa-add-data rsa -3.4000000000e-01 -2.5607960199e-11
rsa-add-data rsa -3.2999999999e-01 -2.5607960200e-11
rsa-add-data rsa -3.2000000001e-01 -2.5607960197e-11
rsa-add-data rsa -3.1000000000e-01 -2.5600854771e-11
rsa-add-data rsa -2.9999999999e-01 -2.5600854771e-11
rsa-add-data rsa -2.9000000001e-01 -2.5600854767e-11
rsa-add-data rsa -2.8000000000e-01 -2.5600854767e-11
rsa-add-data rsa -2.6999999999e-01 -2.5600854775e-11
rsa-add-data rsa -2.5999999999e-01 -2.5600854777e-11
rsa-add-data rsa -2.5000000000e-01 -2.5600854766e-11
rsa-add-data rsa -2.3999999999e-01 -2.5572433059e-11
rsa-add-data rsa -2.3000000000e-01 -2.5565327632e-11
rsa-add-data rsa -2.2000000000e-01 -2.5554669504e-11
rsa-add-data rsa -2.1000000000e-01 -2.5536905928e-11
rsa-add-data rsa -2.0000000000e-01 -2.5515589641e-11
rsa-add-data rsa -1.9000000000e-01 -2.5487167930e-11
rsa-add-data rsa -1.8000000000e-01 -2.5451640789e-11
rsa-add-data rsa -1.7000000000e-01 -2.5405455518e-11
rsa-add-data rsa -1.6000000000e-01 -2.5341506672e-11
rsa-add-data rsa -1.5000000000e-01 -2.5258017900e-11
rsa-add-data rsa -1.4000000000e-01 -2.5151436495e-11
rsa-add-data rsa -1.3000000000e-01 -2.5011104299e-11
rsa-add-data rsa -1.2000000000e-01 -2.4826363186e-11
rsa-add-data rsa -1.1000000000e-01 -2.4585666846e-11
rsa-add-data rsa -9.9999999999e-02 -2.4271251667e-11
rsa-add-data rsa -9.0000000000e-02 -2.3860025064e-11
rsa-add-data rsa -8.0000000000e-02 -2.3320900767e-11
rsa-add-data rsa -7.0000000000e-02 -2.2617019363e-11
rsa-add-data rsa -6.0000000000e-02 -2.1695978345e-11
rsa-add-data rsa -5.0000000000e-02 -2.0491830456e-11
rsa-add-data rsa -4.0000000000e-02 -1.8916646029e-11
rsa-add-data rsa -3.0000000000e-02 -1.6856516183e-11
rsa-add-data rsa -2.0000000000e-02 -1.4162004900e-11
rsa-add-data rsa -1.0000000000e-02 -1.0637782322e-11
rsa-add-data rsa 2.1238869546e-31 -6.0285040848e-12
rsa-add-data rsa 1.0000000000e-02 2.1837568171e-21
rsa-add-data rsa 2.0000000000e-02 7.8846928985e-12
rsa-add-data rsa 3.0000000000e-02 1.8197110484e-11
rsa-add-data rsa 4.0000000000e-02 3.1684765921e-11
rsa-add-data rsa 5.0000000000e-02 4.9325432624e-11
rsa-add-data rsa 6.0000000000e-02 7.2397643435e-11
rsa-add-data rsa 7.0000000000e-02 1.0257394934e-10
rsa-add-data rsa 8.0000000000e-02 1.4204148969e-10
rsa-add-data rsa 9.0000000000e-02 1.9366019899e-10
rsa-add-data rsa 1.0000000000e-01 2.6117419338e-10
rsa-add-data rsa 1.1000000000e-01 3.4947511551e-10
rsa-add-data rsa 1.2000000000e-01 4.6496495542e-10
rsa-add-data rsa 1.3000000000e-01 6.1601390655e-10
rsa-add-data rsa 1.4000000000e-01 8.1357143245e-10
rsa-add-data rsa 1.5000000000e-01 1.0719585219e-09
rsa-add-data rsa 1.6000000000e-01 1.4099033052e-09
rsa-add-data rsa 1.7000000000e-01 1.8519017431e-09
rsa-add-data rsa 1.8000000000e-01 2.4299922074e-09
rsa-add-data rsa 1.9000000000e-01 3.1860807326e-09
rsa-add-data rsa 2.0000000000e-01 4.1749714796e-09
rsa-add-data rsa 2.1000000000e-01 5.4683475526e-09
rsa-add-data rsa 2.2000000000e-01 7.1599544071e-09
rsa-add-data rsa 2.3000000001e-01 9.3724210615e-09
rsa-add-data rsa 2.4000000001e-01 1.2266099247e-08
rsa-add-data rsa 2.5000000000e-01 1.6050762497e-08
rsa-add-data rsa 2.6000000001e-01 2.1000730044e-08
rsa-add-data rsa 2.7000000000e-01 2.7474818864e-08
rsa-add-data rsa 2.8000000000e-01 3.5942292698e-08
rsa-add-data rsa 2.9000000001e-01 4.7016932569e-08
rsa-add-data rsa 3.0000000002e-01 6.1501495452e-08
rsa-add-data rsa 3.1000000000e-01 8.0445907713e-08
rsa-add-data rsa 3.2000000001e-01 1.0522336424e-07
rsa-add-data rsa 3.3000000002e-01 1.3762988260e-07
rsa-add-data rsa 3.4000000000e-01 1.8001448154e-07
rsa-add-data rsa 3.4999999998e-01 2.3544943417e-07
rsa-add-data rsa 3.5999999999e-01 3.0795297334e-07
rsa-add-data rsa 3.7000000000e-01 4.0278058577e-07
rsa-add-data rsa 3.8000000000e-01 5.2680586293e-07
rsa-add-data rsa 3.9000000001e-01 6.8901886152e-07
rsa-add-data rsa 4.0000000002e-01 9.0117761431e-07
rsa-add-data rsa 4.1000000003e-01 1.1786604546e-06
rsa-add-data rsa 4.1999999998e-01 1.5415806445e-06
rsa-add-data rsa 4.2999999999e-01 2.0162442240e-06
rsa-add-data rsa 4.3999999994e-01 2.6370566388e-06
rsa-add-data rsa 4.4999999995e-01 3.4490164040e-06
rsa-add-data rsa 4.5999999996e-01 4.5109763249e-06
rsa-add-data rsa 4.6999999997e-01 5.8999086718e-06
rsa-add-data rsa 4.7999999998e-01 7.7164831680e-06
rsa-add-data rsa 4.8999999999e-01 1.0092361634e-05
rsa-add-data rsa 5.0000000000e-01 1.3199737452e-05
rsa-add-data rsa 5.0999999989e-01 1.7263809895e-05
rsa-add-data rsa 5.1999999990e-01 2.2579095599e-05
rsa-add-data rsa 5.2999999991e-01 2.9530756038e-05
rsa-add-data rsa 5.3999999992e-01 3.8622479906e-05
rsa-add-data rsa 5.4999999993e-01 5.0512932091e-05
rsa-add-data rsa 5.6000000006e-01 6.6063392225e-05
rsa-add-data rsa 5.7000000007e-01 8.6400005351e-05
rsa-add-data rsa 5.8000000007e-01 1.1299510260e-04
rsa-add-data rsa 5.9000000008e-01 1.4777339912e-04
rsa-add-data rsa 6.0000000009e-01 1.9325060762e-04
rsa-add-data rsa 6.1000000010e-01 2.5271425375e-04
rsa-add-data rsa 6.2000000011e-01 3.3045934674e-04
rsa-add-data rsa 6.3000000012e-01 4.3209521800e-04
rsa-add-data rsa 6.4000000013e-01 5.6494445585e-04
rsa-add-data rsa 6.5000000014e-01 7.3856061886e-04
rsa-add-data rsa 6.6000000015e-01 9.6539845799e-04
rsa-add-data rsa 6.7000000016e-01 1.2616787862e-03
rsa-add-data rsa 6.8000000017e-01 1.6484997941e-03
rsa-add-data rsa 6.9000000018e-01 2.1532570072e-03
rsa-add-data rsa 6.9999999995e-01 2.8114439910e-03
rsa-add-data rsa 7.0999999996e-01 3.6689129556e-03
rsa-add-data rsa 7.1999999997e-01 4.7846742345e-03
rsa-add-data rsa 7.2999999998e-01 6.2342981200e-03
rsa-add-data rsa 7.3999999999e-01 8.1139414942e-03
rsa-add-data rsa 7.5000000000e-01 1.0544935052e-02
rsa-add-data rsa 7.6000000001e-01 1.3678714426e-02
rsa-add-data rsa 7.6999999990e-01 1.7701633027e-02
rsa-add-data rsa 7.8000000003e-01 2.2838840894e-02
rsa-add-data rsa 7.9000000001e-01 2.9355969031e-02
rsa-add-data rsa 8.0000000000e-01 3.7556909748e-02
rsa-add-data rsa 8.1000000000e-01 4.7775751764e-02
rsa-add-data rsa 8.2000000000e-01 6.0361251959e-02
rsa-add-data rsa 8.3000000000e-01 7.5653529314e-02
rsa-add-data rsa 8.4000000000e-01 9.3955088652e-02
rsa-add-data rsa 8.5000000000e-01 1.1550128060e-01
rsa-add-data rsa 8.6000000000e-01 1.4043735118e-01
rsa-add-data rsa 8.7000000000e-01 1.6880847182e-01
rsa-add-data rsa 8.8000000000e-01 2.0056502829e-01
rsa-add-data rsa 8.9000000000e-01 2.3558003984e-01
rsa-add-data rsa 9.0000000000e-01 2.7367226500e-01
rsa-add-data rsa 9.1000000000e-01 3.1462887681e-01
rsa-add-data rsa 9.2000000000e-01 3.5822409615e-01
rsa-add-data rsa 9.3000000000e-01 4.0423266249e-01
rsa-add-data rsa 9.4000000000e-01 4.5243855391e-01
rsa-add-data rsa 9.5000000000e-01 5.0263997879e-01
rsa-add-data rsa 9.6000000000e-01 5.5465172699e-01
rsa-add-data rsa 9.7000000000e-01 6.0830578711e-01
rsa-add-data rsa 9.8000000000e-01 6.6345091193e-01
rsa-add-data rsa 9.9000000000e-01 7.1995158767e-01

# create a plot2d object
const ptd (afnix:svg:Plot2d rsa)

# create a full plot
ptd:full-plot

# create a svg root node
const root (afnix:svg:SvgRoot ptd)
 
# create an output file and write
const os (afnix:sio:OutputFile "SVG2004.svg")
root:write os
os:close

# release the node
root:release
