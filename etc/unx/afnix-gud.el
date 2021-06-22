;;; afnix-gud.el --- afnix debugger for emacs

;;  This program is  free software;  you can  redistribute it and/or modify
;;  it under the  terms of the GNU General Public License  as published  by
;;  the  Free Software Foundation; either version  2 of the License, or any
;;  later version.
;;  This  program  is  distributed in the hope  that it will be useful, but
;;  without  any  warranty;  without  even   the   implied    warranty   of
;;  merchantability or fitness for a particular purpose.
;;  See the GNU General Public License for more details.
;;
;;  copyright (c) 1999-2021 Amaury Darsch

(require 'gud)
(defvar gud-axd-history nil)

(defun gud-axd-massage-args (file args)
  (cons "-f" (cons "emacs" args)))

(defun gud-axd-marker-filter (string)
  (setq gud-marker-acc (concat gud-marker-acc string))
  (let ((output ""))

    ;; Process all the complete markers in this chunk.
    (while (string-match "\032\032\\(\\([a-zA-Z]:\\)?[^:\n]*\\):\\([0-9]*\\)\n"
			 gud-marker-acc)
      (setq

       ;; Extract the frame position from the marker.
       gud-last-frame
       (cons (substring gud-marker-acc (match-beginning 1) (match-end 1))
	     (string-to-int (substring gud-marker-acc
				       (match-beginning 3)
				       (match-end 3))))

       ;; Append any text before the marker to the output we're going
       ;; to return - we don't include the marker in this text.
       output (concat output
		      (substring gud-marker-acc 0 (match-beginning 0)))

       ;; Set the accumulator to the remaining text.
       gud-marker-acc (substring gud-marker-acc (match-end 0))))

    ;; Does the remaining text look like it might end with the
    ;; beginning of another marker?  If it does, then keep it in
    ;; gud-marker-acc until we receive the rest of it.  Since we
    ;; know the full marker regexp above failed, it's pretty simple to
    ;; test for marker starts.
    (if (string-match "\032.*\\'" gud-marker-acc)
	(progn
	  ;; Everything before the potential marker start can be output.
	  (setq output (concat output (substring gud-marker-acc
						 0 (match-beginning 0))))

	  ;; Everything after, we save, to combine with later input.
	  (setq gud-marker-acc
		(substring gud-marker-acc (match-beginning 0))))

      (setq output (concat output gud-marker-acc)
	    gud-marker-acc ""))

    output))

(defun gud-axd-find-file (f)
  (find-file-noselect f))

(defcustom gud-axd-command-name "axd"
  "File name for executing the Afnix debugger."
  :type 'string
  :group 'gud)

;;; autoload
(defun axd (command-line)
  "Run axd on program FILE in buffer *gud-FILE*."
  (interactive
   (list (gud-query-cmdline 'axd)))

  (gud-common-init command-line 'gud-axd-massage-args
		   'gud-axd-marker-filter 'gud-axd-find-file)
  (set (make-local-variable 'gud-minor-mode) 'axd)
   
  (gud-def gud-run    "axd:run"             "\C-r" "Run the program.")
  (gud-def gud-next   "axd:next"            "\C-n" "Next line.")
  (gud-def gud-cont   "axd:continue"        "\C-c" "Continue execution.")
  (gud-def gud-break  "axd:break \"%f\" %l" "\C-b" "Set breakpoint at line.")

  (local-set-key [menu-bar debug run] '("Run" . gud-run))

  (setq comint-prompt-regexp "^(.*axd[+]?) *")
  (setq paragraph-start comint-prompt-regexp)
  (run-hooks 'axd-mode-hook)
  )
