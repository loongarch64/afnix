// ---------------------------------------------------------------------------
// - Plot2d.hpp                                                              -
// - afnix:svg service - 2D svg plot class definition                        -
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

#ifndef  AFNIX_PLOT2D_HPP
#define  AFNIX_PLOT2D_HPP

#ifndef  AFNIX_SVGFRAGMENT_HPP
#include "SvgFragment.hpp"
#endif

#ifndef  AFNIX_RSAMPLES_HPP
#include "Rsamples.hpp"
#endif

namespace afnix {

  /// The Plot2d class is a svg fragment designed for the automatic
  /// drawing of a two dimensional plot. The plot data are obtained
  /// from the real samples object. By default, the viewbox is set to
  /// 1000x1000 but can be changed manually.
  /// @author amaury darsch

  class Plot2d : public SvgFragment {
  private:
    /// the top left x vb
    long d_tlx;
    /// the top left y vb
    long d_tly;    
    /// the bottom right x vb
    long d_brx;
    /// the bottom right y vb
    long d_bry;
    /// the left margin
    long d_lxm;
    /// the right margin
    long d_rxm;
    /// the top margin
    long d_tym;
    /// the bottom margin
    long d_bym;
    /// the box origin flag
    bool d_bof;
    /// the viewbox origin x
    long d_box;
    /// the viewbox origin y
    long d_boy;
    /// the viewbox display flag
    bool d_vbdf;
    /// the viewbox line width
    long d_vblw;
    /// the viewbox line color
    String d_vblc;
    /// the viewbox fill color
    String d_vbfc;
    /// the axis width
    long   d_axew;
    /// the axis color
    String d_axec;
    /// the polyline width
    long   d_plyw;
    /// the real samples
    Rsamples* p_data;

  public:
    /// create a null plot
    Plot2d (void);

    /// create a plot by data
    /// @param data the data samples
    Plot2d (Rsamples* data);

    /// destroy this plot
    ~Plot2d (void);

    /// @return the class name
    String repr (void) const;

    /// reset this plot
    void reset (void);

    /// @return the plot data samples
    Rsamples* getdata (void) const;

    /// set the plot viewbox attribute
    /// @param tlx the top left x coordinate
    /// @param tly the top left y coordinate
    /// @param brx the bottom right x coordinate
    /// @param bry the bottom right y coordinate
    void setvbx (const long tlx, const long tly, 
		 const long brx, const long bry);

    /// set the plot margin attribute
    /// @param lxm the left margin
    /// @param rxm the right margin
    /// @param tym the top margin
    /// @param bym the bottom margin
    void setvbm (const long lxm, const long rxm, 
		 const long tym, const long bym);

    /// set the plot box origin
    /// @param box the box origin x
    /// @param boy the box origin y
    void setbo (const long box, const long boy);

    /// create a a whole plot
    void create (void);

    /// create a plot by columns
    /// @param hcol the horizontal column
    /// @param cnum the number of columns
    /// @param cnum the number of columns
    /// @param cols the column indexes
    void create (const long hcol, const long cnum, const long* cols);

  private:
    // make the copy constructor private
    Plot2d (const Plot2d&);
    // make the assignment operator private
    Plot2d& operator = (const Plot2d&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
