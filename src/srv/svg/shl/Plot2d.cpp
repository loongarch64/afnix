// ---------------------------------------------------------------------------
// - Plot2d.cpp                                                              -
// - afnix:svg service - 2D svg plot class implementation                    -
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

#include "Math.hpp"
#include "Plot2d.hpp"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Palette.hpp"
#include "SvgLine.hpp"
#include "SvgRect.hpp"
#include "Evaluable.hpp"
#include "Transform.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SvgPolyline.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default plot view box 
  static const long   P2D_TLX_DEF = 0L;
  static const long   P2D_TLY_DEF = 0L;
  static const long   P2D_BRX_DEF = 1000L;
  static const long   P2D_BRY_DEF = 1000L;
  // the default box margin
  static const long   P2D_LXCV_DEF = 10L;
  static const long   P2D_RXCV_DEF = 10L;
  static const long   P2D_TYCV_DEF = 10L;
  static const long   P2D_BYCV_DEF = 10L;
  // the default viewbox parameters
  static const bool   P2D_VBD_DEF = true;
  static const long   P2D_VBW_DEF = 1L;
  static const String P2D_VBL_DEF = "gray";
  static const String P2D_VBF_DEF = "none";
  // the default axis parameters
  static const long   P2D_AXW_DEF = 1L;
  static const String P2D_AXC_DEF = "gray";
  // the default polyline parameters
  static const long   P2D_PLW_DEF = 1L;
  static const String P2D_PLF_DEF = "none";

  // this procedure computes the absolute value of a real
  static inline t_real abs (const t_real x) {
    return (x >= 0) ? x : -x;
  }

  // this procedure computes the absolute minimum of two reals
  static inline t_real amin (const t_real x, const t_real y) {
    t_real ax = abs (x);
    t_real ay = abs (y);
    return (ax < ay) ? ax : ay;
  }
  
  // this procedure returns a normalized signed value
  static inline t_real nmin (const t_real x, const t_real m) {
    return (x >= 0) ? m : -m;
  }

  // the axis coordinate transformation structure
  struct s_act {
    t_real pac;
    t_real nac;
    t_real zbc;
    s_act (void) {
      pac = 0.0;
      nac = 0.0;
      zbc = 0.0;
    }
    // map a coordiate by real value
    long toaxis (const t_real val) const {
      long result = zbc;
      result += (val >= 0) ? pac * val : nac * val;
      return result;
    }
  };

  // this procedure computes the x-axis coordinate transform
  static s_act p2d_get_xct (const t_real hmin, const t_real hmax,
			    const long   xmin, const long   xmax,
			    const long   box) {
    // the result structure
    s_act xct;
    // right quadrant
    if (hmin >= 0.0) {
      xct.pac = ((t_real) (xmax - box)) / hmax;
      xct.zbc = box;
      return xct;
    }
    // left quadrant
    if (hmax <= 0.0) {
      xct.nac = ((t_real) (xmin - box)) / hmin;
      xct.zbc = box;
      return xct;
    }
    // left-right quadrant
    t_real pac = ((t_real) (xmax - box)) / hmax;
    t_real nac = ((t_real) (xmin - box)) / hmin;
    t_real mac = amin (pac, nac);
    xct.pac = nmin (pac, mac);
    xct.nac = nmin (nac, mac);
    xct.zbc = box;
    return xct;
  }

  // this procedure computes the y-axis coordinate transform
  static s_act p2d_get_yct (const t_real vmin, const t_real vmax,
			    const long   ymin, const long   ymax,
			    const long   boy) {
    // the result structure
    s_act yct;
    // top quadrant
    if (vmin >= 0.0) {
      yct.pac = ((t_real) (ymin - boy)) / vmax;
      yct.zbc = boy;
      return yct;
    }
    // bottom quadrant
    if (vmax <= 0.0) {
      yct.nac = ((t_real) (ymax - boy)) / vmin;
      yct.zbc = boy;
      return yct;
    }
    // top-bottom quadrant
    t_real pac = ((t_real) (ymin - boy)) / vmax;
    t_real nac = ((t_real) (ymax - boy)) / vmin;
    t_real mac = amin (pac, nac);
    yct.pac = nmin (pac, mac);
    yct.nac = nmin (nac, mac);
    yct.zbc = boy;
    return yct;
  }

  // this procedure computes the signed minimum columns
  static t_real p2d_min_sc (const long cnum, const long* cols,
			    const Rsamples& data) {
    t_real result = Math::CV_NAN;
    for (long i = 0; i < cnum; i++) {
      t_real min = data.minsc (cols[i]);
      if (Math::isnan (min) == true) continue;
      if (Math::isnan (result) == true) result = data.minsc (cols[i]);
      else if (min < result) result = min;
    }
    return result;
  }
  
  // this procedure computes the signed maximum columns
  static t_real p2d_max_sc (const long cnum, const long* cols,
			    const Rsamples& data) {
    t_real result = Math::CV_NAN;
    for (long i = 0; i < cnum; i++) {
      t_real max = data.maxsc (cols[i]);
      if (Math::isnan (max) == true) continue;
      if (Math::isnan (result) == true) result = data.maxsc (cols[i]);
      else if (max < result) result = max;
    }
    return result;
  }

  // this procedure compute the polyline from the data samples
  static SvgPolyline* p2d_get_pl (const Rsamples data, 
				  const long     hcol, const long   vcol,
				  const s_act&    xct, const s_act&  yct) {
    // get the number of rows
    long rows = data.getrows ();
    if (rows == 0) return nullptr;
    // create a new polyline
    SvgPolyline* pl = new SvgPolyline;
    try {
      // loop in the rows
      for (long row = 0; row < rows; row++) {
	// get the horizontal value
	t_real h = (hcol >= 0) ? data.get (row, hcol) : data.gettime (row);
	// get the vertical value
	t_real v = data.get (row, vcol);
	// compute x and y ipp
	long x = xct.toaxis (h);
	long y = yct.toaxis (v);
	// update the polyline
	pl->addipp (x, y);
      }
      // here we are
      return pl;
    } catch (...) {
      delete pl;
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null plot

  Plot2d::Plot2d (void) {
    // set data samples
    p_data = nullptr;
    // reset everything
    reset ();
  }

  // create a plot by data

  Plot2d::Plot2d (Rsamples* data) {
    // set data samples
    Object::iref (p_data = data);
    // reset everything
    reset ();
  }

  // destroy this plot

  Plot2d::~Plot2d (void) {
    Object::dref (p_data);
  }
  
  // return the class name

  String Plot2d::repr (void) const {
    return "Plot2d";
  }

  // reset the plot

  void Plot2d::reset (void) {
    wrlock ();
    try {
      // set the default viewbox
      setvbx (P2D_TLX_DEF, P2D_TLY_DEF, P2D_BRX_DEF, P2D_BRY_DEF);
      // set the default margin
      setvbm (P2D_LXCV_DEF, P2D_RXCV_DEF, P2D_TYCV_DEF, P2D_BYCV_DEF);
      // set the viewbox parameters
      d_vbdf = P2D_VBD_DEF;
      d_vblw = P2D_VBW_DEF;
      d_vblc = P2D_VBL_DEF;
      d_vbfc = P2D_VBF_DEF;
      // set the axis parameters
      d_axew = P2D_AXW_DEF;
      d_axec = P2D_AXC_DEF;
      // set the polyline parameters
      d_plyw =  P2D_PLW_DEF;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the samples data

  Rsamples* Plot2d::getdata (void) const {
    rdlock ();
    try {
      Rsamples* result = p_data;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the plot viewbox attribute

  void Plot2d::setvbx (const long tlx, const long tly, 
		       const long brx, const long bry) {
    wrlock ();
    try {
      // save the viewbox data
      d_tlx = tlx; d_tly = tly;
      d_brx = brx; d_bry = bry;
      // update the fragment
      SvgFragment::setvbx (tlx, tly, brx, bry);
      // recompute the box origin
      setbo ((brx + tlx) / 2, (bry + tly) / 2);
      d_bof = true;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the plot margin attributes

  void Plot2d::setvbm (const long lxm, const long rxm, 
		       const long tym, const long bym) {
    wrlock ();
    try {
      // save the margin data
      d_lxm = lxm; d_rxm = rxm;
      d_tym = tym; d_bym = bym;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the plot box origin

  void Plot2d::setbo (const long box, const long boy) {
    wrlock ();
    try {
      // check for valid coordinates
      if ((box < d_tlx) || (box > d_brx) || (boy < d_tly) || (boy > d_bry)) {
	throw Exception ("plot-error", "invalid origin for viewbox");
      }
      // save the coordinates
      d_box = box; d_boy = boy;
      // set the box flag
      d_bof = true;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a whole plot

  void Plot2d::create (void) {
    wrlock ();
    try {
      // reset the fragment
      clrchild ();
      // check for data
      if (p_data == nullptr) {
	unlock ();
	return;
      }
      // check if the data are stamped
      if (p_data->stamped () == true) {
	// get the number of columns
	long cnum = p_data->getcols ();
	if (cnum == 0) {
	  unlock ();
	  return;
	}
	long cols[cnum];
	for (long i = 0; i < cnum; i++) cols[i] = i;
	// create the the plot
	create (-1, cnum, cols);
      } else {
	// get the number of columns
	long cnum = p_data->getcols ();
	if (cnum < 2) {
	  unlock ();
	  return;
	}
	long hcol = 0;
	long cols[cnum-1];
	for (long i = 1; i < cnum; i++) cols[i-1] = i;
	// create the the plot
	create (hcol, cnum-1, cols);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a plot by column indexes

  void Plot2d::create (const long hcol, const long cnum, const long* cols) {
    wrlock ();
    try {
      // reset the fragment
      clrchild ();
      // check for data
      if (p_data == nullptr) {
	unlock ();
	return;
      }
      // get the horizontal range
      t_real hmin = (hcol >= 0) ? p_data->minsc (hcol) : p_data->minst ();
      t_real hmax = (hcol >= 0) ? p_data->maxsc (hcol) : p_data->maxst ();
      if ((Math::isnan (hmin) == true) || (Math::isnan (hmax) == true)) {
	throw Exception ("plot-error", "invalid horizontal range in create");
      }
      // get the vertical range
      t_real vmin = p2d_min_sc (cnum, cols, *p_data);
      t_real vmax = p2d_max_sc (cnum, cols, *p_data);
      if ((Math::isnan (vmin) == true) || (Math::isnan (vmax) == true)) {
	throw Exception ("plot-error", "invalid vertical range in create");
      }
      // compute the x and y ranges
      long xmin = d_tlx + d_lxm; long xmax = d_brx - d_rxm;
      long ymin = d_tly + d_tym; long ymax = d_bry - d_bym;
      // create a viewbox if any
      if (d_vbdf == true) {
	SvgRect* vb = new SvgRect ((d_brx-d_tlx), (d_bry-d_tly), d_tlx, d_tly);
	vb->setswth (d_vblw); vb->setstrk (d_vblc); vb->setfill (d_vbfc);
	addchild (vb);
      }
      // create the axis
      if (d_bof == true) {
	// create the x axis
	SvgLine* xa = new SvgLine (xmin, d_boy, xmax, d_boy);
	xa->setswth (d_axew); xa->setstrk (d_axec);
	addchild (xa);
	// create the y axis
	SvgLine* ya = new SvgLine (d_box, ymin, d_box, ymax);
	ya->setswth (d_axew); ya->setstrk (d_axec);
	addchild (ya);
      }
      // get axis transformation structure
      s_act xct = p2d_get_xct (hmin, hmax, xmin, xmax, d_box);
      s_act yct = p2d_get_yct (vmin, vmax, ymin, ymax, d_boy);
      // loop in the columns
      for (long i = 0; i < cnum; i++) {
	// get the column number
	long vcol = cols[i];
	// create a polyline
	SvgPolyline* pl = p2d_get_pl (*p_data, hcol, vcol, xct, yct);
	// set the polyline style
	pl->setswth (d_plyw); pl->setstrk (Palette::getrgb (i));
	pl->setfill (P2D_PLF_DEF);
	// add the polyline
	if (pl != nullptr) addchild (pl);
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETBO   = zone.intern ("set-box-origin");
  static const long QUARK_FPLOT   = zone.intern ("full-plot");
  static const long QUARK_TPLOT   = zone.intern ("time-plot");
  static const long QUARK_CPLOT   = zone.intern ("column-plot");
  static const long QUARK_SETVBX  = zone.intern ("set-box");
  static const long QUARK_SETVBM  = zone.intern ("set-box-margin");
  static const long QUARK_GETDATA = zone.intern ("get-data");

  // create a new object in a generic way

  Object* Plot2d::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Plot2d;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Rsamples* data = dynamic_cast <Rsamples*> (obj);
      if (data == nullptr) {
	throw Exception ("type-error", "invalid object for plot",
			 Object::repr (obj));
      }
      return new Plot2d (data);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with plot object");
  }
  
  // return true if the given quark is defined
  
  bool Plot2d::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? SvgFragment::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Plot2d::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // processs generic quark
    if (quark == QUARK_TPLOT) {
      if (argc == 0) {
	throw Exception ("plot-error", "missing arguments with time-plot");
      }
      long cols[argc];
      for (long i = 0; i < argc; i++) cols[i] = argv->getlong (i);
      create (-1, argc, cols);
      return nullptr;
    }
    if (quark == QUARK_CPLOT) {
      if (argc < 2) {
	throw Exception ("plot-error", "missing arguments with column-plot");
      }
      long cols[argc];
      long hcol = argv->getlong (0);
      for (long i = 1; i < argc; i++) cols[i] = argv->getlong (i);
      create (hcol, argc-1, cols);
      return nullptr;
    }

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_FPLOT) {
	create ();
	return nullptr;
      }
      if (quark == QUARK_GETDATA) {
	rdlock ();
	try {
	  Object* result = getdata ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETBO) {
	long box = argv->getlong (0);
	long boy = argv->getlong (1);
	setbo (box, boy);
	return nullptr;
      }
    }
    // dispatch 4 arguments
    if (argc == 4) {
      if (quark == QUARK_SETVBX) {
	long tlx = argv->getlong (0);
	long tly = argv->getlong (1);
	long brx = argv->getlong (2);
	long bry = argv->getlong (3);
	setvbx (tlx, tly, brx, bry);
	return nullptr;
      }
      if (quark == QUARK_SETVBM) {
	long lxm = argv->getlong (0);
	long rxm = argv->getlong (1);
	long tym = argv->getlong (2);
	long bym = argv->getlong (3);
	setvbm (lxm, rxm, tym, bym);
	return nullptr;
      }
    }
    // call the svg fragment method
    return SvgFragment::apply (zobj, nset, quark, argv);
  }
}
