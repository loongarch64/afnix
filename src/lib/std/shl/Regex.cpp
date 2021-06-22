// ---------------------------------------------------------------------------
// - Regex.cpp                                                               -
// - standard object library - regex class implementation                    -
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

#include "Regex.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "Unitabler.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputString.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // check for a blank character (blank or tab)
  static inline bool re_check_blank (const t_quad c) {
    if ((c == blkq) || (c == tabq)) return true;
    return false;
  }

  // check for a newline/cariage return or end of file
  static inline bool re_check_eos (const t_quad c) {
    if ((c == eolq) || (c == eosq) || (c == crlq)) return true;
    return false;
  }

  // check for a newline or cariage return
  static inline bool re_check_newln (const t_quad c) {
    if ((c == eolq) || (c == crlq)) return true;
    return false;
  }

  // check for a combining alphanumeric
  static inline bool re_check_can (const t_quad c) {
    return Unicode::iscan (c);
  }

  // check for a alpha-numeric character
  static inline bool re_check_alpha (const t_quad c) {
    return Unicode::isalpha (c);
  }

  // check for a digit
  static inline bool re_check_digit (const t_quad c) {
    return Unicode::isdigit (c);
  }

  // check for a lower character
  static inline bool re_check_lower (const t_quad c) {
    return  Unicode::islower (c);
  }

  // check for a upper character
  static inline bool re_check_upper (const t_quad c) {
    return Unicode::isupper (c);
  }

  // check for a letter character
  static inline bool re_check_letter (const t_quad c) {
    return Unicode::isletter (c);
  }

  // check for a hexadecimal character
  static inline bool re_check_hexa (const t_quad c) {
    return Unicode::ishexa (c);
  }

  // check for a word constituent
  static inline bool re_check_word (const char c) {
    return Unicode::iswcc (c);
  }

  // check for a afnix constituent
  static inline bool re_check_afnix (const char c) {
    return Unicode::isafnix (c);
  }

  // check for a braket
  static inline bool re_check_braket (const char c) {
    char cval = Unicode::tochar (c);
    return ((cval == '[') || (cval == ']'));
  }
  
  // check a meta character against a character
  static bool re_check_meta (const t_quad meta, const t_quad c) {
    switch (meta) {
    case 'a':
      return  re_check_alpha (c);
      break;
    case 'A':
      return !re_check_alpha (c);
      break;
    case 'b':
      return  re_check_blank (c);
      break;
    case 'B':
      return !re_check_blank (c);
      break;
    case 'c':
      return  re_check_can (c);
      break;
    case 'C':
      return !re_check_can (c);
      break;
    case 'd':
      return  re_check_digit (c);
      break;
    case 'D':
      return !re_check_digit (c);
      break;
    case 'e':
      return  re_check_eos (c);
      break;
    case 'E':
      return !re_check_eos (c);
      break;
    case 'k':
      return  re_check_braket (c);
      break;
    case 'K':
      return !re_check_braket (c);
      break;
    case 'l':
      return  re_check_lower (c);
      break;
    case 'L':
      return !re_check_lower (c);
      break;
    case 'n':
      return  re_check_newln (c);
      break;
    case 'N':
      return !re_check_newln (c);
      break;
    case 's':
      return  re_check_letter (c);
      break;
    case 'S':
      return !re_check_letter (c);
      break;
    case 'u':
      return  re_check_upper (c);
      break;
    case 'U':
      return !re_check_upper (c);
      break;
    case 'v':
      return  re_check_afnix (c);
      break;
    case 'V':
      return  !re_check_afnix (c);
      break;
    case 'w':
      return  re_check_word (c);
      break;
    case 'W':
      return !re_check_word (c);
      break;
    case 'x':
      return  re_check_hexa (c);
      break;
    case 'X':
      return !re_check_hexa (c);
      break;
    default:
      return (meta == c);
      break;
    }
    return false;
  }

  // the regex character set structure
  struct s_recset {
    // unicode table
    Unitabler* p_utbl;
    // meta character
    t_quad d_meta;
    // inverted set flag
    bool   d_iflg;
    // next set in list
    s_recset* p_next;
    // simple constructor
    s_recset (void) {
      p_utbl = nullptr;
      d_meta = nilq;
      d_iflg = false;
      p_next = nullptr;
    }
    // simple destructor
    ~s_recset (void) {
      delete p_utbl;
      delete p_next;
    }
    // add a meta character to this set
    void meta (const t_quad c) {
      // check this node first
      if (d_meta == nilq) {
	d_meta = c;
	return;
      }
      // find the last character set
      s_recset* cset = this;
      while (cset->p_next != nullptr) cset = cset->p_next;
      // create a new node and link
      s_recset* node = new s_recset;
      node->d_meta   = c;
      cset->p_next   = node;
    }
    // mark the character set with a character
    void mark (const t_quad c) {
      // eventually create a unicode table
      if (p_utbl == nullptr) p_utbl = new Unitabler;
      p_utbl->add (c, nullptr);
    }
    // complement the whole character set
    void iset (const bool iflg) {
      d_iflg = iflg;
      if (p_next != nullptr) p_next->iset (iflg);
    }
    // return true if a character belongs
    // to the character set recursively
    bool valid (const t_quad c) {
      // check meta
      bool mflg = (d_meta != nilq) ? re_check_meta (d_meta, c) : false;
      if ((mflg == true) && (d_iflg == false)) return true;
      // check the unicode table
      bool uflg = (p_utbl != nullptr) ? p_utbl->exists (c) : false;
      // compute status and eventually revert the result
      bool status = mflg || uflg;
      if (d_iflg == true) status = !status;
      if ((status == true) || (p_next == nullptr)) return status;
      // compute next value
      return p_next->valid (c);
    }
  };

  // check a character set against a character
  static bool re_check_cset (s_recset* cset, const t_quad c) {
    if (cset == nullptr) return false;
    return cset->valid (c);
  }

  // the regex block node type
  enum t_renode {
    RE_CHAR, // character node
    RE_META, // meta character node 
    RE_CSET, // character set node
    RE_BLOK, // sub blok node
    RE_GMRK, // group mark
    RE_GSET, // group accept
    RE_OPRD  // operand node
  };

  // the regex operator type
  enum t_reoper {
    RE_NONE, // no operator
    RE_PLUS, // + operator
    RE_MULT, // * operator
    RE_ZONE, // ? operator
    RE_ALTN, // | operator
    RE_CTRL  // control node
  };

  // the regex node
  struct s_renode {
    // operator control
    t_reoper d_oper;
    // the node type
    t_renode d_type;
    // node data
    union {
      t_quad    d_cval;
      s_recset* p_cset;
      s_renode* p_nval;
    };
    // operand node
    s_renode* p_oprd;
    // next node in list
    s_renode* p_next;
    // protection flag
    bool d_prot;
    // create a new node by operator
    s_renode (t_renode type) {
      if (type != RE_CSET) {
	d_oper = RE_CTRL;
	d_type = type;
	d_cval = nilq;
	p_oprd = nullptr;
	p_next = nullptr;
	d_prot = false;
      } else {
	d_oper = RE_NONE;
	d_type = type;
	p_cset = new s_recset;
	p_oprd = nullptr;
	p_next = nullptr;
	d_prot = false;
      }
    }
    // create a new node by type and character
    s_renode (t_renode type, const t_quad c) {
      d_oper = RE_NONE;
      d_type = type;
      d_cval = c;
      p_oprd = nullptr;
      p_next = nullptr;
      d_prot = false;
    }
    // create a new block node
    s_renode (s_renode* node) {
      d_oper = RE_NONE;
      d_type = RE_BLOK;
      p_nval = node;
      p_oprd = nullptr;
      p_next = nullptr;
      d_prot = false;
    }
    // delete this node
    ~s_renode (void) {
      if (d_type == RE_CSET) {
	delete p_cset;
	p_cset = nullptr;
      }
      if ((d_oper == RE_ALTN) && (p_next != nullptr)) p_next->d_prot = true;
      if ((d_type == RE_BLOK) || (d_type == RE_OPRD)) {
	if ((p_nval != nullptr) && (p_nval->d_prot == false)) delete p_nval;
      }
      if (d_type == RE_OPRD) {
	if ((p_oprd != nullptr) && (p_oprd->d_prot == false)) delete p_oprd;
      }
      if ((d_oper == RE_ALTN) && (p_next != nullptr)) p_next->d_prot = false;
      if ((p_next != nullptr) && (p_next->d_prot == false)) delete p_next;
    }

    // mark a node as an operator - move node data if alternate
    void mark (t_reoper oper) {
      // check for alternate
      if (oper == RE_ALTN) {
	if (d_oper == RE_ALTN) {
	  throw Exception ("regex-error", "invalid dual | operator");
	}
	// copy node
	s_renode* node = new s_renode (d_type);
	node->d_oper = d_oper;
	node->d_type = d_type;
	node->p_nval = p_nval;
	node->p_oprd = p_oprd;
	node->p_next = nullptr;
	node->d_prot = d_prot;
	// re-adjust node
	d_oper = RE_ALTN;
	d_type = RE_OPRD;
	p_nval = node;
	p_oprd = nullptr;
	p_next = nullptr;
	return;
      }
      if (d_oper != RE_NONE) {
	throw Exception ("regex-error", "invalid operator position");
      }
      d_oper = oper;
    }
  };

  // the regex structure
  struct s_regex {
    // the root node
    s_renode* p_root;
    // the last node
    s_renode* p_last;
    // the reference count
    long d_rcount;
    // initialize the regex structure
    s_regex (void) {
      p_root = nullptr;
      p_last = nullptr;
      d_rcount = 1;
    }
  };

  // read a character in the buffer - possibly escaped
  static t_quad re_escape_char (InputString& buf) {
    // check for escaped
    t_quad c = buf.getu ();
    if (c != (t_quad) '\\') return c;
    // the character is escaped
    c = buf.getu ();
    if (c == eosq) throw Exception ("regex-error", "invalid eos character");
    switch (c) {
    case 'n':  return eolq;
    case 't':  return tabq;
    case '"':  return (t_quad) '"';
    case '\\': return (t_quad) '\\';
    default:   return c;
    }
  }

  // find the last node in a node chain
  static s_renode* re_find_last (s_renode* node) {
    if (node == nullptr) return nullptr;
    while (node->p_next != nullptr) node = node->p_next;
    return node;
  }

  // append a node to a node chain
  static void re_append_node (s_renode** root, s_renode** last, 
			      s_renode* node) {
    if (*root == nullptr) {
      *root = node;
      *last = node;
      return;
    }
    // check if we have an alternate node
    if ((*last)->d_oper == RE_ALTN) {
      // mark operand node
      if ((*last)->p_oprd == nullptr) {
	(*last)->p_oprd = node;
	return;
      }
      // add next node at the end of the alternate node
      s_renode* nlast = re_find_last ((*last)->p_nval);
      nlast->p_next = node;
      nlast = re_find_last ((*last)->p_oprd);
      nlast->p_next = node;
    }
    // at the end node
    (*last)->p_next = node;
    *last = node;
  }

  // regex context structure
  struct s_rectx {
    // input stream
    InputStream* p_is;
    // string data 
    String  d_str;
    // input mode
    bool    d_imod;
    // string length
    long  d_len;
    // start index
    long  d_sidx;
    // current index;
    long  d_cidx;
    // group mark
    long  d_gmrk;
    // group vector
    Vector* p_grpv;
    // create a new string context at position 0
    s_rectx (const String& s, Vector* grpv) {
      p_is   = nullptr;
      d_str  = s;
      d_imod = false;
      d_len  = s.length ();
      d_sidx = 0;
      d_cidx = d_sidx;
      d_gmrk = d_sidx;
      Object::iref (p_grpv = grpv);
    }
    // create a new string context at a given position
    s_rectx (const String& s, const long sidx, Vector* grpv) {
      p_is   = nullptr;
      d_str  = s;
      d_imod = false;
      d_len  = s.length ();
      d_sidx = (sidx < d_len) ? sidx : d_len;
      d_cidx = d_sidx;
      d_gmrk = d_sidx;
      Object::iref (p_grpv = grpv);
    }
    // create an input stream context at a given position
    s_rectx (InputStream* is, Vector* grpv) {
      p_is   = is;
      d_str  = "";
      d_imod = true;
      d_len  = 0;
      d_sidx = 0;
      d_cidx = d_sidx;
      d_gmrk = d_sidx;
      Object::iref (p_grpv = grpv);
    }
    // create an input stream context with a prefix at a given position
    s_rectx (InputStream* is, const String& ps, Vector* grpv) {
      p_is   = is;
      d_str  = ps;
      d_imod = true;
      d_len  = ps.length ();
      d_sidx = 0;
      d_cidx = d_sidx;
      d_gmrk = d_sidx;
      Object::iref (p_grpv = grpv);
    }
    // copy construct this context
    s_rectx (const s_rectx& ctx) {
      p_is = ctx.p_is;
      d_str  = ctx.d_str;
      d_imod = ctx.d_imod;
      d_len  = ctx.d_len;
      d_sidx = ctx.d_sidx;
      d_cidx = ctx.d_cidx;
      d_gmrk = ctx.d_gmrk;
      Object::iref (p_grpv = ctx.p_grpv);
    }
    // destroy this context
    ~s_rectx (void) {
      Object::dref (p_grpv);
    }
    // assign a context to this one
    s_rectx& operator = (const s_rectx& ctx) {
      p_is   = ctx.p_is;
      d_str  = ctx.d_str;
      d_imod = ctx.d_imod;
      d_len  = ctx.d_len;
      d_sidx = ctx.d_sidx;
      d_cidx = ctx.d_cidx;
      d_gmrk = ctx.d_gmrk;
      Object::iref (ctx.p_grpv);
      Object::dref (p_grpv);
      p_grpv = ctx.p_grpv;
      return *this;
    }
    // restore the old context with a new one
    s_rectx& restore (const s_rectx& ctx) {
      // check for good mode
      if ((d_imod == false) || (ctx.d_imod == false)) return *this;
      // make sure the we have more characters to push back
      if (ctx.d_len <= d_len) return *this;
      for (long i = d_len; i < ctx.d_len; i++) p_is->pushback (ctx.d_str[i]);
      return *this;
    }
    // return the context string
    String getstring (void) const {
      return d_str;
    }
    // read a character from this context
    t_quad getu (void) {
      if ((d_imod == true) && (d_cidx == d_len)) {
	if ((p_is != nullptr) && (p_is->iseos () == false)) {
	  t_quad c = p_is->getu ();
	  d_str = d_str + c;
	  d_len++;
	}
      }
      return (d_cidx == d_len) ? eosc : d_str[d_cidx++];
    }
    // return true if we are at the end
    bool iseos (void) const {
      if (d_imod == true) {
	return (p_is == nullptr) ? true : p_is->iseos ();
      } else {
	return (d_cidx == d_len);
      }
    }
    // mark the group start
    void gmrk (void) {
      d_gmrk = d_cidx;
    }
    // set a group result
    void gset (void) {
      long  len = d_cidx - d_gmrk;
      if (len <= 0) len = 0;
      t_quad* buf = new t_quad[len + 1];
      for (long i = 0; i < len; i++) buf[i] = d_str[i+d_gmrk];
      buf[len] = nilq;
      p_grpv->add (new String (buf));
      delete [] buf;
    }
    // pop a last entry in the group
    void gpop (void) {
      Object::cref (p_grpv->rml ());
    }
    // return the string match
    String subs (void) const {
      return d_str.substr (d_sidx, d_cidx);
    }
  };

  // forward declaration for node execution
  static bool re_exec      (s_renode* node, s_rectx& ctx);
  static bool re_exec_root (s_renode* node, s_rectx& ctx);
  static bool re_exec_node (s_renode* node, s_rectx& ctx);
  static bool re_exec_loop (s_renode* node, s_rectx& ctx);
  static bool re_exec_plus (s_renode* node, s_rectx& ctx);
  static bool re_exec_mult (s_renode* node, s_rectx& ctx);
  static bool re_exec_zone (s_renode* node, s_rectx& ctx);
  static bool re_exec_altn (s_renode* node, s_rectx& ctx);
  static bool re_exec_ctrl (s_renode* node, s_rectx& ctx);

  // execute at a particular node
  static bool re_exec (s_renode* node, s_rectx& ctx) {
    // without node - we succed
    if (node == nullptr) return true;
    // initialize the status and save context
    s_rectx bctx = ctx;
    bool status  = false;
    // dispatch based on node operation
    switch (node->d_oper) {
    case RE_NONE:
      status = re_exec_node (node, ctx);
      if (status == false) {
	ctx = bctx.restore (ctx);
	break;
      }
      status = re_exec (node->p_next, ctx);
      break;
    case RE_PLUS:
      status = re_exec_plus (node, ctx);
      break;
    case RE_MULT:
      status = re_exec_mult (node, ctx);
      break;
    case RE_ZONE:
      status = re_exec_zone (node, ctx);
      break;
    case RE_ALTN:
      status = re_exec_altn (node, ctx);
      break;
    case RE_CTRL:
      status = re_exec_ctrl (node, ctx);
    }
    return status;
  }

  // execute the root node
  static bool re_exec_root (s_renode* node, s_rectx& ctx) {
    // without node - we succed
    if (node == nullptr) return true;
    // the special case is the alternate node, since at the root
    // the sucess condition is attached with an end of file
    if (node->d_oper != RE_ALTN) return re_exec (node, ctx);
    // here we have an alternate node, so initialize the status 
    // and save context
    s_rectx bctx = ctx;
    bool status  = false;
    // try with first node
    status = re_exec (node->p_nval, ctx);
    if ((status == true) && (ctx.iseos () == true)) return true;
    // try with second node
    ctx = bctx.restore (ctx);
    status = re_exec (node->p_oprd, ctx);
    // restore context if failure
    if (status == false) ctx = bctx.restore (ctx);
    return status;
  }

  // execute a particular node
  static bool re_exec_node (s_renode* node, s_rectx& ctx) {
    // check for node and succed
    if (node == nullptr) return true;
    bool status = false;
    // dispatch according to node type
    switch (node->d_type) {
    case RE_CHAR:
      status = (ctx.getu () == node->d_cval);
      break;
    case RE_META:
      status = re_check_meta (node->d_cval, ctx.getu ());
      break;
    case RE_CSET:
      status = re_check_cset (node->p_cset, ctx.getu ());
      break;
    case RE_BLOK:
      status = re_exec (node->p_nval, ctx);
      break;
    default:
      throw Exception ("regex-error", "internal exec node error");
    }
    return status;
  }

  //  execute a loop with fall back
  static bool re_exec_loop (s_renode* node, s_rectx& ctx) {
    // save the context and execute the node
    s_rectx bctx = ctx;
    bool status  = ctx.iseos () ? false : re_exec_node (node, ctx);
    // restore context and check for fallback
    if (status == false) {
      ctx = bctx.restore (ctx);
      // try to fall back
      status = re_exec (node->p_next, ctx);
      if (status == false) {
	ctx = bctx.restore (ctx);
	return false;
      }
      return status;
    }
    status = re_exec_loop (node,ctx);
    if (status == false) {
      ctx = bctx.restore (ctx);
      // try to fall back
      status = re_exec (node->p_next, ctx);
      if (status == false) {
	ctx = bctx.restore (ctx);
	return false;
      }
      return status;
    }
    return status;
  }

  // execute a node with a plus operator
  static bool re_exec_plus (s_renode* node, s_rectx& ctx) {
    // save the context and reset status
    s_rectx bctx = ctx;
    bool status  = false;
    // execute the node once
    status = re_exec_node (node, ctx);
    if (status == false) {
      ctx = bctx.restore (ctx);
      return false;
    }
    // execute the loop with fallback
    return re_exec_loop (node, ctx);
  }

  // execute a node with a mult operator
  static bool re_exec_mult (s_renode* node, s_rectx& ctx) {
    return re_exec_loop (node,ctx);
  }

  // execute a node with a zone operator
  static bool re_exec_zone (s_renode* node, s_rectx& ctx) {
    // save the context and reset status
    s_rectx zctx = ctx;
    bool status  = false;
    // execute the node once
    status = re_exec_node (node, ctx);
    // fallback if wrong
    if (status == false) {
      ctx = zctx;
      return re_exec (node->p_next, ctx);
    }
    // try with one
    status = re_exec (node->p_next, ctx);
    if (status == false) {
      ctx = zctx;
      return re_exec (node->p_next, ctx);
    }
    return status;
  }

  // execute an alternate node
  static bool re_exec_altn (s_renode* node, s_rectx& ctx) {
    // save the context and reset status
    s_rectx bctx = ctx;
    bool status  = false;

    // try with first node
    status = re_exec (node->p_nval, ctx);
    if (status == false) {
      ctx = bctx.restore (ctx);
      status = re_exec (node->p_oprd, ctx);
      if (status == false) {
	ctx = bctx.restore (ctx);
	return status;
      }
    }
    return status;
  }

  // execute a control node
  static bool re_exec_ctrl (s_renode* node, s_rectx& ctx) {
    // save the context and reset status
    s_rectx bctx = ctx;
    bool status  = false;
    // dispatch from node type
    switch (node->d_type) {
    case RE_GMRK:
      ctx.gmrk ();
      status = re_exec (node->p_next, ctx);
      break;
    case RE_GSET:
      ctx.gset ();
      status = re_exec (node->p_next, ctx);
      if (status == false) ctx.gpop ();
      break;
    default:
      throw Exception ("regex-error", "internal regex control node error");
      break;
    }
    if (status == false) ctx = bctx.restore (ctx);
    return status;
  }

  // get or create the group vector
  static Vector* re_get_grpv (Thrmap& gmap) {
    Object* vobj = gmap.get ();
    if (vobj == nullptr) gmap.set (vobj = new Vector);
    return dynamic_cast <Vector*> (vobj);
  }

  // fixup the root node by removing the block node
  static s_renode* re_remove_root_block (s_renode* root) {
    // check for blok node
    if (root == nullptr) return nullptr;
    if (root->d_type != RE_BLOK) return root;
    if (root->p_next != nullptr)    return root;
    // save result node and fixup root
    s_renode* result = root->p_nval;
    root->p_nval = nullptr;
    delete root;
    return result;
  }

  // verify the root node tree
  static void re_verify_node (s_renode* root) {
    // initialize the node
    s_renode* node = root;
    // loop in the nodes
    while (node != nullptr) {
      // check operator node
      if (node->d_type == RE_OPRD) {
	// check we have a node value
	if (node->p_nval == nullptr) {
	  throw Exception ("regex-error", "missing operand in regex");
	}
	// verify the node
	re_verify_node (node->p_nval);
	// check alternate node
	if (node->d_oper == RE_ALTN) {
	  if (node->p_oprd == nullptr) {
	    throw Exception ("regex-error", "missing operand in alternate");
	  }
	  re_verify_node (node->p_oprd);
	} else {
	  if (node->p_oprd != nullptr) {
	    throw Exception ("regex-error", "invalid operand in regex");
	  }
	}
      }
      // check next node
      node = node->p_next;
    }
  }

  // compile from a stream and return a root node
  static s_renode* re_compile (InputString& is, const bool bflag) {
    // check for data
    if (is.iseos () == true) return nullptr;

    // initialize the root and last node
    s_renode* root = nullptr;
    s_renode* last = nullptr;
    s_renode* node = nullptr;

    // flag for balancing checks
    long bcount = bflag ? 1 : 0;
    long gcount = 0;
    // flags for character set complement
    bool flag = false;
    bool iflg = false;
    // loop in the expressions
    while (is.iseos () == false) {
      t_quad c = is.getu ();
      switch (c) {
      case eosq:
	if (bcount != 0) {
	  delete root;
	  throw Exception ("regex-error", "unbalanced [] in expression");
	}
	if (gcount != 0) {
	  delete root;
	  throw Exception ("regex-error", "unbalanced () in expression");
	}
	return root;
      case ']':
	bcount--;
	if (bcount != 0) {
	  delete root;
	  throw Exception ("regex-error", "unbalanced [] in expression");
	}
	if (gcount != 0) {
	  delete root;
	  throw Exception ("regex-error", "unbalanced () in expression");
	}
	return root;	
      case '[':
	node = new s_renode (re_compile (is, true));
	re_append_node (&root, &last, node);
	break;
      case '(':
	gcount++;
	node = new s_renode (RE_GMRK);
	re_append_node (&root, &last, node);
	break;
      case ')':
	gcount--;
	node = new s_renode (RE_GSET);
	re_append_node (&root, &last, node);
	break;
      case '$':
	if ((c = is.getu ()) == eosq) {
	  delete root;
	  throw Exception ("regex-error", "end of regex with $");
	}
	node = new s_renode (RE_META, c);
	re_append_node (&root, &last, node);
	break;
      case '"':
	while ((is.iseos () == false) && (is.get () != '"')) {
	  t_quad ec = re_escape_char (is);
	  node = new s_renode (RE_CHAR, ec);
	  re_append_node (&root, &last, node);
	}
	// consume last double quote
	if ((c = is.getu ()) != (t_quad) '"') {
	  delete root;
	  throw Exception ("regex-error", "unbalanced double quote");
	}
	break;
      case '<':
	node = new s_renode (RE_CSET);
	while (is.get () != '>') {
	  t_quad c = is.getu ();
	  if (flag == false) {
	    flag = true;
	    if (c == (t_quad) '^') {
	      iflg = true;
	      continue;
	    }
	  }
	  if (c == eosq) {
	    delete node;
	    delete root;
	    throw Exception ("regex-error", "end of regex with <");
	  }
	  if (c == (t_quad) '$') {
	    c = is.getu ();
	    if (c == eosq) {
	      delete node;
	      delete root;
	      throw Exception ("regex-error", "end of regex with $");
	    }
	    node->p_cset->meta (c);
	  } else {
	    node->p_cset->mark (c);
	  } 
	}
	// consume last character - eventually complement and append
	is.getu ();
	node->p_cset->iset (iflg);
	re_append_node (&root, &last, node);
	break;
      case '+':
	if (last == nullptr) {
	  delete root;
	  throw Exception ("regex-error", "invalid + start");
	}
	last->mark (RE_PLUS);
	break;
      case '*':
	if (last == nullptr) {
	  delete root;
	  throw Exception ("regex-error", "invalid * start");
	}
	last->mark (RE_MULT);
	break;
      case '?':
	if (last == nullptr) {
	  delete root;
	  throw Exception ("regex-error", "invalid ? start");
	}
	last->mark (RE_ZONE);
	break;
      case '|':
	if (last == nullptr) {
	  delete root;
	  throw Exception ("regex-error", "invalid | start");
	}
	last->mark (RE_ALTN);
	break;
      default:
	node = new s_renode (RE_CHAR, c);
	re_append_node (&root, &last, node);
	break;
      }
    }
    // check for balancing
    if (bcount != 0) {
      delete root;
      throw Exception ("regex-error", "unbalanced [] in expression");
    }
    if (gcount != 0) {
      delete root;
      throw Exception ("regex-error", "unbalanced () in expression");
    }
    // trim the root node
    s_renode* result = re_remove_root_block (root);
    // verify the result node
    try {
      re_verify_node (result);
    } catch (...) {
      delete result;
      throw;
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null regex

  Regex::Regex (void) {
    p_recni = new s_regex;
  }

  // create a regex from a string

  Regex::Regex (const char* re) {
    p_recni = new s_regex;
    if (re != nullptr) compile (re);
  }

  // create a regex from a string

  Regex::Regex (const String& re) {
    p_recni = new s_regex;
    compile (re);
  }

  // copy construct this regex
  
  Regex::Regex (const Regex& that) {
    that.rdlock ();
    try {
      d_reval = that.d_reval;
      p_recni = that.p_recni;
      p_recni->d_rcount++;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this regex

  Regex::~Regex (void) {
    if (--p_recni->d_rcount == 0) {
      delete p_recni->p_root;
      delete p_recni;
    }
  }

  // return the class name

  String Regex::repr (void) const {
    return "Regex";
  }

  // return a clone of this object

  Object* Regex::clone (void) const {
    return new Regex (*this);
  }

  // clear this regex

  void Regex::clear (void) {
    wrlock ();
    try {
      d_reval = "";
      delete p_recni; p_recni = new s_regex;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of this regex

  String Regex::tostring (void) const {
    rdlock ();
    try {
      String result = d_reval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this regex

  String Regex::toliteral (void) const {
    rdlock ();
    try {
      String result = (d_reval[0] == '[')
	? d_reval
	: (String ("[") + d_reval + "]");
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the regex did

  t_word Regex::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the regex sid

  t_word Regex::getsid (void) const {
    return SRL_REGX_SID;
  }

  // serialize this regex

  void Regex::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      d_reval.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this regex

  void Regex::rdstream (InputStream& is) {
    wrlock ();
    try {
      String sval;
      sval.rdstream (is);
      compile (sval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a regex to this one

  Regex& Regex::operator = (const Regex& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy regex value
      d_reval = that.d_reval;
      // reference the regex structure
      if (--p_recni->d_rcount == 0) {
	delete p_recni->p_root;
	delete p_recni;      
      }
      p_recni = that.p_recni;
      p_recni->d_rcount++;
      // unlock everything
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // assign a regex description to this regex

  Regex& Regex::operator = (const char* re) {
    wrlock ();
    try {
      if (re != nullptr) compile (re);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a regex description to this regex

  Regex& Regex::operator = (const String& re) {
    wrlock ();
    try {
      compile (re);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compile a string as a regex

  void Regex::compile (const String& re) {
    wrlock ();
    try {
      // clean the old compiled node info
      if (--p_recni->d_rcount == 0) {
	delete p_recni->p_root;
	delete p_recni;
	p_recni = new s_regex;
      }
      // create an input stream
      InputString is (re);
      // get the root and last node
      p_recni->p_root = re_compile   (is, false);
      p_recni->p_last = re_find_last (p_recni->p_root);
      // check for consistency
      if (is.iseos () == false) {
	delete p_recni->p_root;
	p_recni->p_root = nullptr;
	p_recni->p_last = nullptr;
	throw Exception ("regex-error", "regex syntax error", re);
      }
      // save the string regex
      d_reval = re;
      // unlock the regex
      unlock ();
    } catch (...) {
      delete p_recni->p_root;
      p_recni->p_root = nullptr;
      p_recni->p_last = nullptr;
      unlock ();
      throw;
    }
  }

  // match this regex against a string

  bool Regex::operator == (const String& s) const {
    // initialize group vector
    Vector* grpv = nullptr;
    // lock and compare
    rdlock ();
    try {
      // get the group vector
      grpv = re_get_grpv (d_gmap);
      if (grpv != nullptr) grpv->reset ();
      // create a regex context
      s_rectx ctx (s, 0, grpv);
      bool result = re_exec_root (p_recni->p_root, ctx) & ctx.iseos ();
      unlock ();
      return result;
    } catch (...) {
      if (grpv != nullptr) grpv->reset ();
      unlock ();
      throw;
    }
  }

  // match this regex partially against a string

  bool Regex::operator < (const String& s) const {
    // initialize group vector
    Vector* grpv = nullptr;
    // lock and compare
    rdlock ();
    try {
      // get the group vector
      grpv = re_get_grpv (d_gmap);
      // process along the string
      long len = s.length ();
      for (long i = 0; i < len; i++) {
	// reset the group vector
	if (grpv != nullptr) grpv->reset ();
	// create a regex context
	s_rectx ctx (s, i, grpv);
	bool result = re_exec (p_recni->p_root, ctx);
	if (result == false) continue;
	unlock ();
	return result;
      }
      unlock ();
      return false;
    } catch (...) {
      if (grpv != nullptr) grpv->reset ();
      unlock ();
      throw;
    }
  }

  // reverse matching the regex

  bool Regex::operator != (const String& s) const {
    rdlock ();
    try {
      bool result = (*this == s) ? false : true;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // match this regex with an input stream

  String Regex::match (InputStream* is) const {
    // check for nil first
    if (is == nullptr) return "";
    // lock and match
    rdlock ();
    try {
      String result = match (is, "");
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // match this regex with an input stream and an initial string

  String Regex::match (InputStream* is, const String& ps) const {
    // initialize group vector
    Vector* grpv = nullptr;
    // lock and match
    rdlock ();
    try {
      // get the group vector
      grpv = re_get_grpv (d_gmap);
      // protect the input stream
      Object::iref (is);
      // reset the group vector
      if (grpv != nullptr) grpv->reset ();
      // create a regex context
      s_rectx ctx (is, ps, grpv);
      // execute the regex
      bool status = re_exec (p_recni->p_root, ctx);
      String result = status ? ctx.getstring () : "";
      // unportect the stream
      Object::tref (is);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      // reset the group vector
      if (grpv != nullptr) grpv->reset ();
      // unportect the stream
      Object::tref (is);
      // unlock and throw
      unlock ();
      throw;
    }
  }

  // match this regex partially and return the matching string

  String Regex::match (const String& s) const {
    // initialize group vector
    Vector* grpv = nullptr;
    // lock and match
    rdlock ();
    try {
      // get the group vector
      grpv = re_get_grpv (d_gmap);
      // process along the string
      long len = s.length ();
      for (long i = 0; i < len; i++) {
	// reset the group vector
	if (grpv != nullptr) grpv->reset ();
	// create a regex context
	s_rectx ctx (s, i, grpv);
	if (re_exec (p_recni->p_root, ctx) == false) continue;
	String result = ctx.subs ();
	unlock ();
	return result;
      }
      unlock ();
      return "";
    } catch (...) {
      if (grpv != nullptr) grpv->reset ();
      unlock ();
      throw;
    }
  }

  // replace a match with another string

  String Regex::replace (const String& s, const String& val) const {
    // initialize group vector
    Vector* grpv = nullptr;
    // lock and match
    rdlock ();
    try {
      // get the group vector
      grpv = re_get_grpv (d_gmap);
      // initialize result buffer
      Buffer result (Encoding::EMOD_UTF8);
      // process along the string
      long len = s.length ();
      for (long i = 0; i < len; i++) {
	// reset the group vector
	if (grpv != nullptr) grpv->reset ();
	// create a regex context
	s_rectx ctx (s, i, grpv);
	if (re_exec_root (p_recni->p_root, ctx) == false) {
	  result.add (s[i]);
	  continue;
	}
	result.add (val);
	i = ctx.d_cidx - 1;
      }
      unlock ();
      return result.tostring ();
    } catch (...) {
      if (grpv != nullptr) grpv->reset ();
      unlock ();
      throw;
    }
  }
  
  // get the length of the group vector

  long Regex::length (void) const {
    rdlock ();
    try {
      // get the group vector
      Vector* grpv = re_get_grpv (d_gmap);
      // get its length
      long result = (grpv == nullptr) ? 0 : grpv->length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object at certain index

  Object* Regex::getobj (const long index) const {
    rdlock ();
    try {
      // get the group vector
      Vector* grpv = re_get_grpv (d_gmap);
      Object* result =  (grpv == nullptr) ? nullptr : grpv->get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a string at certain index

  String Regex::getstring (const long index) const {
    rdlock ();
    try {
      // get the group vector
      Vector* grpv = re_get_grpv (d_gmap);
      if (grpv == nullptr) {
	throw Exception ("regex-error", "out of bound group access");
      }
      String result = grpv->getstring (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an integer at certain index

  t_long Regex::getlong (const long index) const {
    rdlock ();
    try {
      // get the group vector
      Vector* grpv = re_get_grpv (d_gmap);
      if (grpv == nullptr) {
	throw Exception ("regex-error", "out of bound group access");
      }
      t_long result = Utility::tolong ((grpv->getstring (index)));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a real at certain index

  t_real Regex::getreal (const long index) const {
    try {
      // get the group vector
      Vector* grpv = re_get_grpv (d_gmap);
      if (grpv == nullptr) {
	throw Exception ("regex-error", "out of bound group access");
      }
      t_real result = Utility::toreal ((grpv->getstring (index)));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GET      = zone.intern ("get");
  static const long QUARK_MATCH    = zone.intern ("match");
  static const long QUARK_LENGTH   = zone.intern ("length");
  static const long QUARK_REPLACE  = zone.intern ("replace");

  // create a new object in a generic way

  Object* Regex::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Regex;
    // check for 1 argument
    if (argc == 1) {
      String str = argv->getstring (0);
      return new Regex (str);
    }
    throw Exception ("argument-error", "too many argument with regex");
  }

  // return true if the given quark is defined

  bool Regex::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Literal::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Regex::oper (t_oper type, Object* object) {
    // get a literal object
    Literal* lobj = dynamic_cast <Literal*> (object);
    switch (type) {
    case Object::OPER_EQL:
    case Object::OPER_QEQ:
      if (lobj != nullptr) return new Boolean (*this == lobj->tostring ());
      break;
    case Object::OPER_NEQ:
      if (lobj != nullptr) return new Boolean (*this != lobj->tostring ());
      break;
    case Object::OPER_LTH:
      if (lobj != nullptr) return new Boolean (*this < lobj->tostring ());
      break;      
    default:
      break;
    }
    throw Exception ("type-error", "invalid operand with regex",
		     Object::repr (object));
  }

  // set an object to this regex

  Object* Regex::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      String* sobj = dynamic_cast <String*> (object);
      if (sobj != nullptr) {
	compile (*sobj);
	zobj->post (this);
	unlock ();
	return this;
      }
      Regex* gobj = dynamic_cast <Regex*> (object);
      if (gobj != nullptr) {
	*this = *gobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with regex vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Regex::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
    } 

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  Object* result = getobj (argv->getlong (0));
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MATCH) {
	Object*  obj = argv->get (0);
	// check for a stream
	InputStream* iobj = dynamic_cast <InputStream*> (obj);
	if (iobj != nullptr) return new String (match (iobj));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new String (match (*sobj));
	// argument error
	throw Exception ("type-error", "invalid object with match ", 
			 Object::repr (obj));
      }
    }

    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_REPLACE) {
	String s   = argv->getstring (0);
	String val = argv->getstring (1);
	return new String (replace (s, val));
      }
      if (quark == QUARK_MATCH) {
	// get the input stream
	Object* obj = argv->get (0);
	InputStream* iobj = dynamic_cast <InputStream*> (obj);
	// get the prefix
	String ps = argv->getstring (1);
	if (iobj != nullptr) return new String (match (iobj, ps));
	throw Exception ("type-error", "invalid object with match ", 
			 Object::repr (obj));
      }
    }    
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
