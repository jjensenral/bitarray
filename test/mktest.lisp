;;;; Make test files
;;;; Only required to (re)build the test code

;;; Who tests the testers?  If rt is loaded prior to reading this code, (some) regression tests are defined.
;;; These can be run with (rt:do-tests)


(defparameter +test-file-name+ #P"bitarray.test.cc")


(defparameter +file-header+ "// Testing ~A
// This test file is generated with mktest.lisp
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <span>
// guarded shift tests the underlying shift implementation, that it does not alter memory outside of the words it shifts
bool guarded_shift(std::initializer_list<u_int32_t> const &, signed int shift, std::initializer_list<u_int32_t> const &);
")

(defparameter +test-header+ "TEST_CASE( \"~A\", \"[~:*~A]\" ) {
")
(defparameter +test-footer+ "} // ~A~%")

(defparameter +file-footer+ "
bool guarded_shift(std::initializer_list<u_int32_t> const &mydata, signed int shift, std::initializer_list<u_int32_t> const &mytarget)
{
    std::span data(mydata), target(mytarget);
    std::array<u_int32_t, 5> in, out;
    const u_int32_t lo_guard = 0xa0ef09af;
    const u_int32_t hi_guard = 0xaee91fe9;
    /* Note the initializer has msw first, lsw last, so we fill the sandwich values backwards */
    in[0] = lo_guard; in[1] = data[2]; in[2] = data[1]; in[3] = data[0]; in[4] = hi_guard;
    out[0] = lo_guard; out[1] = target[2]; out[2] = target[1]; out[3] = target[0]; out[4] = hi_guard;
    std::span shiftme{in.begin()+1, 3};
    if(shift < 0)
	_private::bitarray_do_shift_right(shiftme, -shift);
    else
	_private::bitarray_do_shift_left(shiftme, shift);
    return in == out;
}
 // End of ~A~%")




(defvar *magic-1* #x8aa0e2231256affe10f9e3a1)

(defun make-number (words return-type seed)
  "Make a \"random\" number consisting of word 32-bit words from a given seed (which is *not* altered).  If it matters to the return type, the MSW is placed first."
  ;; First make the number.  It is not possible to initialise a random state to a known implementation-independent state, portably.
  ;; However, the sbcl docs state that the initial state is the same every time sbcl is started, so that at least gives consistency
  ;; if used carefully.
  (let* ((wordsize 32)
	 (the-word (ash 1 wordsize))
	 (state (make-random-state seed))
	 (number (loop repeat words collecting (random the-word state))))
    ;; and now see if we can coerce it into the required type
    (ecase return-type
      (list number)
      (integer (reduce (lambda (a b) (logior (ash a wordsize) b)) number :initial-value 0))
      (string (format nil "~{~8,'0x~}" number)))))

;; sbcl startup values used for rt
#+rt
(rt:deftest (make-number 1) (make-number 5 'string *random-state*)
  "E7E1FAEED5C31F792082352CF807B7DFE9D30005")
#+rt
(rt:deftest (make-number 2) (make-number 5 'string *random-state*)
  ;; repeat of above tests that random state is unaltered
  "E7E1FAEED5C31F792082352CF807B7DFE9D30005")

#+rt
(rt:deftest (make-number 3) (make-number 5 'list *random-state*)
  ;; same number again, different format
  (3890346734 3586334585 545404204 4161255391 3922919429))

#+rt
(rt:deftest (make-number 4) (make-number 5 'integer *random-state*)
  ;; same number again, different format
  1323816395072801286890685414170339662235283161093)

#+rt
(rt:deftest (make-number 4) (eql 1323816395072801286890685414170339662235283161093 #XE7E1FAEED5C31F792082352CF807B7DFE9D30005)
  t)



(defun shift-left (k)
  ;; y is the magic number; m is a three double word (96 bits) long mask
  (let ((y *magic-1*) (m (1- (ash 1 (ash 3 5)))))
    (format nil "~24,'0X" (logand m (ash y k)))))

(defun shift-right (k)
  ;; y is the magic number; m is a three double word (96 bits) long mask
  (let ((y *magic-1*) (m (1- (ash 1 (ash 3 5)))))
    (format nil "~24,'0X" (logand m (ash y (- k))))))

(defun hex-string-to-dwords (str &optional (at 8))
  "Convert string to substrings of length at, shorter first"
  (let* ((strings nil)
	 (a (length str)))
    (do* ((i 0 (+ i r))
	  ;; r may be shorter than at initially, but not zero
	  (r (let ((r1 (mod a at))) (if (zerop r1) at r1)) at))
	 ((= i a) (nreverse strings))
      (push (subseq str i (+ i r)) strings))))

#+rt
(rt:deftest (hex-string-to-dwords 1)
    (hex-string-to-dwords "abcdefghijklmnopqrstuvwxyz")
  ("ab" "cdefghij" "klmnopqr" "stuvwxyz"))

#+rt
(rt:deftest (hex-string-to-dwords 2)
    (hex-string-to-dwords "01234567")
  ("01234567"))

#+rt
(rt:deftest (hex-string-to-dwords 3)
    (hex-string-to-dwords "foo")
  ("foo"))

#+rt
(rt:deftest (hex-string-to-dwords 4)
    (hex-string-to-dwords "")
  NIL)


(defun hex-string-to-variable (str &optional name len)
  "From a hex string return a constructor as a string; if the name of a variable is given construct the variable, otherwise a temporary rvalue (and no semicolon).  len is the number of words to use in the constructor and defaults to the number of dwords in the string.  Using an empty string results in a call to the default constructor."
  (let* ((data (hex-string-to-dwords str))
	 
	 (len-1 (or len (length data))))
    (if (endp data)
	(format nil "bitarray<~D>~A" len-1 (if name (format nil " ~A;" name) "()"))
	(format nil "bitarray<~D>~A{0x~A~{,0x~A~}}~A" len-1
		(if name (format nil " ~A" name) "") ; if a name is present, it must be preceded by a space
		(car data) (cdr data) (if name ";\n" "")))))



(defun write-file-header (file deps)
  "Write the file header; deps should be the list of header files to be included with the last one the one to be tested"
  (format file "~?~{#include \"~A\"~%~}using namespace bitarrays;~%" +file-header+ (last deps) deps))

(defun write-file-footer (file name)
  (format file +file-footer+ name))

(defun write-test-header (file name)
  (format file +test-header+ name))

(defun write-test-footer (file name)
  (format file +test-footer+ name))

(defmacro write-test (file name &body body)
  `(progn (write-test-header ,file ,name) ,@body (write-test-footer ,file ,name)))


;;; *main* / *entrypoint*
(defun make-test-file nil
  (with-open-file (test-file +test-file-name+ :direction :output
			     :if-exists :overwrite
			     :if-does-not-exist :create)
    (let ((shifts '( 0 1 2 3 23 32 33 34 60 64 65 66 95 96 100 ))
	  (three "{0x~A,0x~A,0x~A}"))
      (write-file-header test-file (list "bitarray.hh"))
      (write-test test-file "constructor"
		  (format test-file "~:@{    REQUIRE( ~A == ~A ); //~A~%~}"
			  (list (hex-string-to-variable "1234567") (hex-string-to-variable "01234567") "leading zero")
			  (list (hex-string-to-variable "1234567" nil 3) (hex-string-to-variable "000000000000000001234567") "short number")
			  (list (hex-string-to-variable "ff00112233445566778899aabbccddee" nil 3) (hex-string-to-variable "33445566778899aabbccddee") "trunc lead digits")))

      (write-test test-file "guard left shift"
		  (dolist (k shifts)
		    (format test-file "    REQUIRE( guarded_shift(~?,~D,~?) );~%" three (hex-string-to-dwords (format nil "~x" *magic-1*)) k three (hex-string-to-dwords (shift-left k)))))

      (write-test test-file "left shift"
			 (dolist (k shifts)
			   (format test-file "    REQUIRE( (~A << ~D ) == ~A );~%" (hex-string-to-variable (format nil "~x" *magic-1*)) k (hex-string-to-variable (shift-left k)))))

      (write-test test-file "guard right shift"
		  (dolist (k shifts)
		    (format test-file "    REQUIRE( guarded_shift(~?,~D,~?) );~%" three (hex-string-to-dwords (format nil "~x" *magic-1*)) (- k) three (hex-string-to-dwords (shift-right k)))))

      (write-test test-file "right shift"
			 (dolist (k shifts)
			   (format test-file "    REQUIRE( (~A >> ~D ) == ~A );~%" (hex-string-to-variable (format nil "~x" *magic-1*)) k (hex-string-to-variable (shift-right k)))))

      (write-test test-file "and or xor"
		  (let* ((ab (make-number 24 'list *random-state*))
			 (a (subseq ab 0 12))
			 (b (subseq ab 12))
			 (ops (list (cons #\& #'logand) (cons #\| #'logior) (cons #\^  #'logxor))))
		    (dolist (op ops)
		      ;; Note the inner parens in test to catch annoying operator precedence in C++
		      (format test-file "    REQUIRE( (~A ~C ~A) == ~A );~%"
			      (hex-string-to-variable (format nil "~{~8,'0X~}" a))
			      (car op)
			      (hex-string-to-variable (format nil "~{~8,'0X~}" b))
			      (hex-string-to-variable (format nil "~{~8,'0X~}" (mapcar (cdr op) a b)))))))
      (princ "    bitarray<7> fred;
" test-file)
      (write-test test-file "one bit"
		  (princ "    REQUIRE( fred.is_zero() );
    fred.bitset(0);
    REQUIRE( !fred.is_zero() );
//    for(int i = 0; i < 7*32-2; ++i) {
	REQUIRE( fred.lsb() == i );
	REQUIRE( fred.msb() == i );
	fred <<= 1;
//    }
    fred.bitclr(7*32-1);
    REQUIRE( fred.is_zero() );
" test-file))
      )
    (write-file-footer test-file +test-file-name+)))
