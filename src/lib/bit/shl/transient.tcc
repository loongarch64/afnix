// ---------------------------------------------------------------------------
// - transient.tcc                                                           -
// - afnix nuts and bolts - transient pointer template                       -
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

#ifndef  AFNIX_TRANSIENT_TCC
#define  AFNIX_TRANSIENT_TCC

namespace afnix {

  /// The transient template is a generic pointer encapsulation designed
  /// to manage the object destruction when leaving the allocation scope.
  /// As such the allocated pointr is therefore transient with respect to
  /// execution scope.
  /// @author amaury darsch

  template <typename T>
  class t_transient {
  protected:
    /// the transient pointer
    T* p_ptr;

  public:
    /// create a null transient
    t_transient(void) {
      p_ptr = nullptr;
    }

    /// create a transient object
    /// @param ptr the pointer object
    t_transient(T* ptr) {
      p_ptr = ptr;
    }

    /// copy move this transient pointer
    /// @param that the object to copy move
    t_transient (t_transient&& that) noexcept {
      p_ptr = that.p_ptr; that.p_ptr = nullptr;
    }
    
    /// destroy this transient object
    ~t_transient (void) {
      delete p_ptr;
    }

    /// move this transient pointer
    /// @param that the object to move
    t_transient& operator = (t_transient&& that) noexcept {
      if (this == &that) return *this;
      delete p_ptr; p_ptr = that.p_ptr; that.p_ptr = nullptr;
      return *this;
    }

    /// reset this transient pointer
    void reset (void) {
      delete p_ptr; p_ptr = nullptr;
    }
    
    /// @return true is the pointer is valid
    bool isvalid (void) const {
      return (p_ptr == nullptr) ? false : true;
    }
    
    /// @return the transient pointer
    T* operator * (void) {
      return p_ptr;
    }

    /// @return the transient pointer
    const T* operator * (void) const {
      return p_ptr;
    }

    /// @return the transient pointer
    T* operator -> (void) {
      return p_ptr;
    }

    /// @return the transient pointer
    const T* operator -> (void) const {
      return p_ptr;
    }

    /// @return a reference by index
    T& operator [] (const long pidx) {
      if (p_ptr == nullptr) throw "null pointer in transient access";
      return p_ptr[pidx];
    }

    /// @return a reference by index
    const T& operator [] (const long pidx) const {
      if (p_ptr == nullptr) throw "null pointer in transient access";
      return p_ptr[pidx];
    }

    /// detach this transient object
    T* detach (void) {
      T* result = p_ptr; p_ptr = nullptr;
      return result;
    }
    
  private:
    // make the copy constructor private
    t_transient (const t_transient&) =delete;
    // make the assignement operator private
    t_transient& operator = (const t_transient&) =delete; 
  };
}

#endif
