// ---------------------------------------------------------------------------
// - Rgivens.hpp                                                             -
// - afnix:mth module - real givens matrix class definitions                 -
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

#ifndef  AFNIX_RGIVENS_HPP
#define  AFNIX_RGIVENS_HPP

#ifndef  AFNIX_RMD_HPP
#include "Rmd.hpp"
#endif

namespace afnix {

  /// The Rgivens class is the real givens matrix class that provides
  /// the essential mechanism to nullify a particular position in a matrix.
  /// The givens matrix is an essential component of the QR factorization
  /// process. The givens matrix is represented by two coefficients (c) and
  /// (s). The (c) coefficients is at coordinates (row, row) and (col,col)
  /// while the (-s) coefficient is at (row, col) (s) is at (col, row).
  /// A givens matrix is essentially an orthogonal rotation matrix.
  /// @author amaury darsch

  class Rgivens : public Object {
  public:
    /// the givens rotation
    struct s_givens {
      /// the row datum
      t_long d_row;
      /// the column datum
      t_long d_col;
      /// the givens c coefficients
      t_real d_c;
      /// the givens s coefficients
      t_real d_s;
      /// create a givens rotation
      s_givens (void);
      /// create a givens rotation by datum
      /// @param rmr the matrix row datum
      /// @param rmc the matrix col datum
      s_givens (const Rmd& rmr, const Rmd& rmc);
      /// create a givens rotation by values
      /// @param row the row coordinates
      /// @param col the col coordinate
      /// @param rcv the row/col value
      /// @param ccv the col/col value
      s_givens (const t_long row, const t_long col, 
		const t_real rcv, const t_real ccv);
      /// set a givens rotation by datum
      /// @param rmr the matrix row datum
      /// @param rmc the matrix col datum
      void set (const Rmd& rmr, const Rmd& rmc);
      /// set a givens rotation by values
      /// @param row the row coordinates
      /// @param col the col coordinate
      /// @param rcv the row/col value
      /// @param ccv the col/col value
      void set (const t_long row, const t_long col, 
		const t_real rcv, const t_real ccv);
    };

  protected:
    /// the givens size
    long d_size;
    /// the givens length
    long d_glen;
    ///  the givens rotations
    s_givens* p_grot;

  public:
    /// create a default givens array
    Rgivens (void);

    /// destroy this object
    ~Rgivens (void);

    /// @return the class name
    String repr (void) const;

    /// @return the givens array length
    virtual long length (void) const;

    /// add a givens rotation
    /// @param gr the givens rotation
    virtual long add (const s_givens& gr);

    /// add a givens rotation by datum
    /// @param rmr the matrix row datum
    /// @param rmc the matrix col datum
    virtual long add (const Rmd& rmr, const Rmd& rmc);
    
    /// add a givens rotation by values
    /// @param row the row coordinates
    /// @param col the col coordinate
    /// @param xr  the matrix row value
    /// @param xr  the matrix col value
    virtual long add (const t_long row, const t_long col, 
		      const t_real xr,  const t_real xc);
    
    /// @return the givens rotation by index
    virtual s_givens get (const long index) const;

    /// @return the givens row by index
    virtual t_long getrow (const long index) const;

    /// @return the givens column by index
    virtual t_long getcol (const long index) const;

    /// @return the givens c value by index
    virtual t_real getc (const long index) const;

    /// @return the givens s value by index
    virtual t_real gets (const long index) const;

  public:
    /// @return the givens rotation by index (no lock)
    virtual s_givens nlget (const long index) const;

    /// @return the givens row by index (no lock)
    virtual t_long nlgrow (const long index) const;

    /// @return the givens column by index (no lock)
    virtual t_long nlgcol (const long index) const;

    /// @return the givens c value by index (no lock)
    virtual t_real nlgc (const long index) const;

    /// @return the givens s value by index (no lock)
    virtual t_real nlgs (const long index) const;

  private:
    // make the copy constructor private
    Rgivens (const Rgivens&);
    // make the assignement operator private
    Rgivens& operator = (const Rgivens&);

  public:
    /// create a new object in a generic way
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
