;;; afnix-mode.el --- major mode for editing afnix source in emacs

;;  This program is  free software;  you can  redistribute it and/or modify
;;  it under the  terms of the GNU General Public License  as published  by
;;  the  Free Software Foundation; either version  2 of the License, or any
;;  later version.
;;  This  program  is  distributed in the hope  that it will be useful, but
;;  without  any  warranty;  without  even   the   implied    warranty   of
;;  merchantability or fitness for a particular purpose.
;;  See the GNU General Public License for more details.
;;
;;  copyright (c) 1999-2001 Philippe Troin
;;  copyright (c) 2002-2021 Amaury Darsch

(eval-when-compile 
  (require 'font-lock))

;;  first things first - basic definitions

(defgroup afnix nil
  "Major mode for editing Afnix source in Emacs"
  :group 'languages)

(defvar afnix-mode-abbrev-table nil
  "Abbreviation table used in Afnix mode")
(define-abbrev-table 'afnix-mode-abbrev-table ())

(defvar afnix-mode-map nil
  "Keymap used in Afnix mode")

(defvar afnix-mode-syntax-table nil
  "Syntax table used in Afnix mode")

(defvar afnix-indent-level 2
  "Amount by which afnix subexpressions are indented.")

;; mode map definition

(unless afnix-mode-map
  (setq afnix-mode-map (make-sparse-keymap))
  (define-key afnix-mode-map "\C-c\C-c" 'afnix-comment-region))

;; syntax table definition

(unless afnix-mode-syntax-table
  (setq afnix-mode-syntax-table (make-syntax-table))
  (modify-syntax-entry ?+   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?-   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?*   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?/   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?\.  "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?:   "_"  afnix-mode-syntax-table)
  (modify-syntax-entry ?=   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?<   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?>   "w"  afnix-mode-syntax-table)
  (modify-syntax-entry ?\{  "(}" afnix-mode-syntax-table)
  (modify-syntax-entry ?\}  "){" afnix-mode-syntax-table)
  (modify-syntax-entry ?\(  "()" afnix-mode-syntax-table)  
  (modify-syntax-entry ?\)  ")(" afnix-mode-syntax-table)
  (modify-syntax-entry ?\[  "(]" afnix-mode-syntax-table)  
  (modify-syntax-entry ?\]  ")[" afnix-mode-syntax-table)
  (modify-syntax-entry ?\#  "<"  afnix-mode-syntax-table)
  (modify-syntax-entry ?\n  ">"  afnix-mode-syntax-table))

;; font-lock support

(defconst afnix-font-lock-keywords
  (eval-when-compile
    (list
     ;; keywords
     (cons
      (concat 
       "\\b\\("
       (mapconcat (lambda (x) x) 
		  '("assert"  "block"   "class"   "const"   "delay"
		    "do"      "else"    "enum"    "errorln" "eval"    "for"
                    "force"   "gamma"   "if"      "lambda"  "launch"  "loop"
                    "nameset" "println" "protect" "return"  "sync"    "switch"
                    "throw"   "trans"   "try"     "while"   "unref"   "this"
                    "what"    "args")
		  "\\|")
       "\\)\\b")
      font-lock-keyword-face)
     ;; Built-in functions (this face is not supported by X-Emacs)
     (unless (featurep 'xemacs)
       (cons
	(concat 
	 "\\b\\("
	 (mapconcat (lambda (x) x) 
		    '("\\sw+-p")
		    "\\|")
	 "\\)\\b")
	font-lock-builtin-face))
     ;; Built-in constants (this face is not supported by X-Emacs)
     (unless (featurep 'xemacs)
       (cons
	(concat 
	 "\\b\\("
	 (mapconcat (lambda (x) x) 
		    '("nil" "true" "false" "self" "this")
		    "\\|")
	 "\\)\\b")
	font-lock-constant-face))
     ;; Built-in types (this face is not supported by X-Emacs)
     (unless (featurep 'xemacs)
       (cons
	(concat 
	 "\\b\\("
	 (mapconcat (lambda (x) x) 
		    '("Literal"   "item"      "Boolean" "Integer"   "Relatif"
		      "Real"      "Character" "String"  "Regex"     "Numeral"
		      "Cons"      "Enum"      "List"    "Vector"    "Set"
		      "Node"      "Edge"      "Graph"   "Queue"     "BitSet"
		      "Buffer"
		      "Object"    "Interp"    "Thread"  "Condvar"   "Lexical"
		      "Qualified" "Symbol"    "Closure" "Librarian" "Resolver"
		      "PrintTable")
		    "\\|")
	 "\\)\\b")
	font-lock-constant-face))
     ;; Function declaration
     '("\\b\\(trans\\|const\\)\\s-+\\(\\(\\sw\\|\\s_\\)+\\)\\s-*\\((\\(([^)
]*)\\|[^()
]\\)*)\\|nil\\)\\s-*[({]" 
       2 font-lock-function-name-face keep)
     ;; Variable declaration
     '("\\b\\(const\\|trans\\)\\s-+\\(\\(\\sw\\|\\s_\\)+\\)" 
       2 font-lock-variable-name-face keep)
     ))
  "afnix-mode font lock definitions")

;; major mode starts here

(defun afnix-mode ()
  "This is the major mode for editing afnix code in Emacs"

  ; house-keeping first
  (interactive)
  (kill-all-local-variables)

  ; install keymaps
  (use-local-map afnix-mode-map)

  ; set local abbreviation table
  (setq local-abbrev-table afnix-mode-abbrev-table)

  ; set syntax table
  (set-syntax-table afnix-mode-syntax-table)

  ; indentation command
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'afnix-indent-line)

  ; set comment start & end
  (make-local-variable 'comment-start)
  (setq comment-start "# ")
  (make-local-variable 'comment-end)
  (setq comment-end "")

  ; sexp ignore comment
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments t)

  ; define paragraph
  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "$\\|" page-delimiter))

  ; install font lock
  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults '(afnix-font-lock-keywords nil nil))

  ; let emacs know who we are
  (setq mode-name "afnix")
  (setq major-mode 'afnix-mode)

  ; run a hook if it exists
  (run-hooks 'afnix-mode-hook))

;; compute the line indentation

(defun afnix-count-blanks-at-bol (&optional pos blank-is-zero)
  "Returns the number of blanks at the beginning of POS (or the point if POS
is nil). If line is only blank-filled and BLANK-IS-ZERO is non-nil, returns 0."
  (save-excursion
    (save-match-data
      (if pos (goto-char pos))
      (beginning-of-line)
      (if blank-is-zero
	  (while (looking-at "^\\s-*$")
	    (beginning-of-line 0)))
      (re-search-forward "^\\s-*")
      (if (and (eolp) blank-is-zero)
	  0
	(current-column)))))

(defun afnix-get-indent-delta-of-region (begin end)
  "Returns the delta in indent-level between BEGIN and END."
  (save-excursion
    (save-match-data
      (goto-char begin)
      (while (looking-at "\\s-*$")
	(beginning-of-line 0))
      (re-search-forward "^\\s-*\\s)*")
      (let ((count 0))
	(while (re-search-forward "\\s(\\|\\s)" end t)
	  (let 
	      ((matched-char-syntax 
		(char-syntax (string-to-char (match-string 0)))))
	    (cond
	     ((eq matched-char-syntax ?\() (setq count (1+ count)))
	     ((eq matched-char-syntax ?\)) (setq count (1- count))))))
	count))))

(defun afnix-get-indent-delta-of-cc-at-bol (&optional pos)
  "Returns the (always negative) indent level taking into account
only closing parenthesis characters at the beginning of the line containing
by POS (or the point if POS is nil)."
  (save-excursion
    (save-match-data
    (if pos (goto-char pos))
    (beginning-of-line)
    (re-search-forward "^\\s-*")
    (let ((level 0))
      (while (eq (char-syntax (let ((ca (char-after)))
				(or ca 0)))
		 ?\))
	(setq level (1- level))
	(forward-char))
      level))))

(defun afnix-put-point-at-bol-after-blanks (&optional pos)
  "Put the point in the current buffer at the beginning of the line
contained by POS (or the point if POS is nil), but after the leading blanks."
  (if pos (goto-char pos))
  (beginning-of-line)
  (re-search-forward "^\\s-*"))
  
(defun afnix-get-theorical-indent-of-line (&optional pos)
  "Returns the indentation offset of line at POS (or the point)."
  (save-excursion
    (if pos (goto-char pos))
    (let (c-line-pos p-line-pos)
    (beginning-of-line)
    (setq c-line-pos (point))
    (if (eq (point) (point-min))
	0
      (beginning-of-line 0)
      (setq p-line-pos (point))
      (+ (afnix-count-blanks-at-bol p-line-pos t)
	 (* afnix-indent-level
	    (+ (afnix-get-indent-delta-of-region p-line-pos c-line-pos)
	       (afnix-get-indent-delta-of-cc-at-bol c-line-pos))))))))

(defun afnix-indent-line ()
  "Indent current line as afnix code."
  (let ((c-indent (afnix-count-blanks-at-bol))
	(t-indent (afnix-get-theorical-indent-of-line)))
    (save-excursion
      (if (< t-indent 0) 
	  (setq t-indent 0))
      (afnix-put-point-at-bol-after-blanks)
      (cond
       ((< c-indent t-indent)
	(insert-char ?\  (- t-indent c-indent)))
       ((> c-indent t-indent)
	(backward-delete-char-untabify (- c-indent t-indent)))))
    (if (looking-at "\\s-*$")
	(end-of-line))))

;; and tell emacs we are here

(provide 'afnix-mode)
