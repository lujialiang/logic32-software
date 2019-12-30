
(define_constants
  [ (LOGIC32_SP 29) ]
)

;; Attributes
;; define attribute mode
(define_attr "mode" "unknown,none,QI,HI,SI,SF" (const_string "unknown"))

;; define attribute type
(define_attr "type" "move,unary,binary,compare,shift,mult,div,arith,logical,uncond_branch,branch,call,call_no_delay_slot,nop,jmp" (const_string "binary"))

;; data move instructions

(define_insn "store_reg"
	[(set (mem:SI (plus:SI (reg:SI LOGIC32_SP) (match_operand:SI 0 "immediate_operand" "i")))
		(match_operand:SI 1 "register_operand" "r"))]
	""
	"sw %0($sp), %1	; store from sp"
	[(set_attr "type" "arith") (set_attr "mode" "SI")])

(define_insn "load_reg"
	[(set (match_operand:SI 0 "register_operand" "=r")
		(mem:SI (plus:SI (reg:SI LOGIC32_SP) (match_operand:SI 1 "immediate_operand" "i"))))]
	""
	"lw %0, %1($sp) ; load form sp"
	[(set_attr "type" "arith") (set_attr "mode" "SI")])

;; If the destination is a MEM and the source is a
;; MEM or an CONST_INT move the source into a register.
(define_expand "movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "general_operand" ""))]
  ""
  "{
  if (!reload_in_progress
      && !reload_completed
      && GET_CODE(operands[0]) == MEM
      && (GET_CODE (operands[1]) == MEM
	  || immediate_operand (operands[1], SImode)))
     operands[1] = copy_to_mode_reg (SImode, operands[1]);
  }"
)

;; mov instruction templater
(define_insn "movsi_internal"
	[(set (match_operand:SI 0 "nonimmediate_operand" "=r,r,r,r,m,m")
		(match_operand:SI 1 "general_operand" "r,J,i,m,J,r"))]
	""
	"@
	mov %0, %1
	mov %0, $zero
	ldi %0, %1
	lw %0, %1 ; load word
	sw %0, $zero
	sw %0, %1 ; movsi"
	[(set_attr "type" "arith") (set_attr "mode" "SI")])

(define_insn "movqi"
	[(set (match_operand:QI 0 "nonimmediate_operand" "=r,r,r,r,m")
		(match_operand:QI 1 "general_operand" "r,J,i,m,r"))]
	"1"
	"@
	mov %0, %1	; movb
	mov %0, $zero
	ldi %0, %1
	lb %0, %1
	sb %0, %1"
	[(set_attr "type" "arith") (set_attr "mode" "SI")])

(define_insn "movhi"
	[(set (match_operand:HI 0 "nonimmediate_operand" "=r,r,r,m")
		(match_operand:HI 1 "general_operand" "r,i,m,r"))]
	"1"
	"@
	mov %0, %1	; movh
	ldi %0, %1
	lh %0, %1
	sh %0, %1"
	[(set_attr "type" "arith") (set_attr "mode" "SI")])


;; arithmetic instructions

;; ADD, SUB, MULT, DIV, ADDF, SUBF, MULTF, DIVF instruction templater
(define_insn "addsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(plus:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "arith_operand" "r,I")))]
	""
	"@
	add %0, %1, %2
	addi %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])

(define_insn "subsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(minus:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "arith_operand" "r,I")))]
	""
	"@
	sub %0, %1, %2
	subi %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])

(define_insn "mulsi3"
	[(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI (match_operand:SI 1 "register_operand" "r") (match_operand:SI 2 "register_operand" "r")))]
	""
	"mul %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])

(define_insn "divsi3"
	[(set (match_operand:SI 0 "register_operand" "=r")
	(div:SI (match_operand:SI 1 "register_operand" "r") (match_operand:SI 2 "register_operand" "r")))]
	""
	"div %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])

(define_insn "modsi3"
	[(set (match_operand:SI 0 "register_operand" "=r")
	(mod:SI (match_operand:SI 1 "register_operand" "r") (match_operand:SI 2 "register_operand" "r")))]
	""
	"mod %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])

; unsigned
;(define_insn "umulsi3"
	;[(set (match_operand:SI 0 "register_operand" "=r")
	;(umult:SI (match_operand:SI 1 "register_operand" "r") (match_operand:SI 2 "register_operand" "r")))]
	;""
	;"mulu %0, %1, %2"
	;[(set_attr "type" "arith")
	;(set_attr "mode" "SI")])

(define_insn "udivsi3"
	[(set (match_operand:SI 0 "register_operand" "=r")
	(udiv:SI (match_operand:SI 1 "register_operand" "r") (match_operand:SI 2 "register_operand" "r")))]
	""
	"divu %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])

(define_insn "umodsi3"
	[(set (match_operand:SI 0 "register_operand" "=r")
	(umod:SI (match_operand:SI 1 "register_operand" "r") (match_operand:SI 2 "register_operand" "r")))]
	""
	"modu %0, %1, %2"
	[(set_attr "type" "arith")
	(set_attr "mode" "SI")])


;; ---------------------------------
;;      zero extension 
;; ---------------------------------

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (zero_extend:HI (match_operand:QI 1 "register_operand" "r")))]
  ""
  "andi %0, %1, 0xff"
  [(set_attr "type" "arith")]  
)

(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (zero_extend:SI (match_operand:QI 1 "register_operand" "r")))]
  ""
  "andi %0, %1, 0xff"
  [(set_attr "type" "arith")]  
)

(define_insn "zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (zero_extend:SI (match_operand:HI 1 "register_operand" "r")))]
  ""
  "andi %0, %1, 0xffff"
  [(set_attr "type" "arith")]
)

;; booleam instructions

(define_insn "andsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(and:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "uns_arith_operand" "r,i")))]
	""
	"@
	and %0, %1, %2
	andi %0, %1, %2"
	[(set_attr "type" "logical")
	(set_attr "mode" "SI")])

(define_insn "iorsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(ior:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "uns_arith_operand" "r,i")))]
	""
	"@
	orr %0, %1, %2
	ori %0, %1, %2"
	[(set_attr "type" "logical")
	(set_attr "mode" "SI")])

(define_insn "xorsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(xor:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "uns_arith_operand" "r,i")))]
	""
	"@
	xor %0, %1, %2
	xori %0, %1, %2"
	[(set_attr "type" "logical")
	(set_attr "mode" "SI")])

;; NOT instruction templater
(define_insn "one_cmplsi2"
	[(set (match_operand:SI 0 "register_operand" "=r") (not:SI (match_operand:SI 1 "register_operand" "r")))]
	""
	"not %0, %1"
	[(set_attr "type" "logical")
	(set_attr "mode" "SI")])

;; arith shift left
(define_insn "ashlsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(ashift:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "arith_operand" "r,I")))]
	""
	"@
	lsl %0, %1, %2	; asl
	lsli %0, %1, %2	; asli"
	[(set_attr "type" "shift")
	(set_attr "mode" "SI")])

;; arith shift right
(define_insn "ashrsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "arith_operand" "r,I")))]
	""
	"@
	asr %0, %1, %2
	asri %0, %1, %2"
	[(set_attr "type" "shift")
	(set_attr "mode" "SI")])

;; logic shift right
(define_insn "lshrsi3"
	[(set (match_operand:SI 0 "register_operand" "=r,r")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "r,r") (match_operand:SI 2 "arith_operand" "r,I")))]
	""
	"@
	lsr %0, %1, %2
	lsri %0, %1, %2"
	[(set_attr "type" "shift")
	(set_attr "mode" "SI")])

(define_insn "cmpsi"
  [(set (cc0)
	(compare (match_operand:SI 0 "register_operand" "r,r,r")
		 (match_operand:SI 1 "arith_operand" "r,J,I")))]
  ""
  "@
  cmp %0, %1
  cmp %0, $zero
  subi $zero, %0, %1"
)

(define_insn "beq"
  [(set (pc)
	(if_then_else (eq (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "beq %l0")

(define_insn "bne"
  [(set (pc)
	(if_then_else (ne (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bne %l0")

(define_insn "bgt"
  [(set (pc)
	(if_then_else (gt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bgt %l0")

(define_insn "bge"
  [(set (pc)
	(if_then_else (ge (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bge %l0")

(define_insn "blt"
  [(set (pc)
	(if_then_else (lt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "blt %l0")

(define_insn "ble"
  [(set (pc)
	(if_then_else (le (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "ble %l0")

(define_insn "bgtu"
  [(set (pc)
	(if_then_else (gtu (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bgtu %l0")

(define_insn "bgeu"
  [(set (pc)
	(if_then_else (geu (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bgeu %l0")

(define_insn "bltu"
  [(set (pc)
	(if_then_else (ltu (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bltu %l0")

(define_insn "bleu"
  [(set (pc)
	(if_then_else (leu (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bleu %l0")

;; uncondition jump JUMP instruction templater
(define_insn "jump"
	[(set (pc) (label_ref (match_operand 0 "" "")))]
	""
	"jmp %l0"
)

;; indirect jump J instruction templater
(define_insn "indirect_jump"
	[(set (pc) (match_operand:SI 0 "address_operand" "r,g"))]
	""
	"@
	jmpr %0
	jmp %0"
)


;; Subroutine call instruction returning no value.  Operand 0 is the function
;; to call; operand 1 is the number of bytes of arguments pushed (in mode
;; `SImode', except it is normally a `const_int'); operand 2 is the number of
;; registers used as operands.

;; On most machines, operand 2 is not actually stored into the RTL pattern.  It
;; is supplied for the sake of some RISC machines which need to put this
;; information into the assembler code; they can put it in the RTL instead of
;; operand 1.

(define_expand "call"
  ;; operands[1] is stack_size_rtx
  ;; operands[2] is next_arg_register
  [(parallel [(call (match_operand:SI 0 "call_operand" "")
		    (match_operand 1 "" ""))
	     (clobber (reg:SI 31))])]
  ""
  "")

(define_insn "*call_via_reg"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "r"))
	 (match_operand 1 "" ""))
   (clobber (reg:SI 31))]
  ""
  "jalr %0"
  )

(define_insn "*call_via_label"
  [(call (mem:SI (match_operand:SI 0 "call_address_operand" ""))
	 (match_operand 1 "" ""))
   (clobber (reg:SI 31))]
  ""
  ; The %~ is necessary in case this insn gets conditionalized and the previous
  ; insn is the cc setter.
  "jal %0"
  )

(define_expand "call_value"
  ;; operand 2 is stack_size_rtx
  ;; operand 3 is next_arg_register
  [(parallel [(set (match_operand 0 "register_operand" "=r")
		   (call (match_operand:SI 1 "call_operand" "")
			 (match_operand 2 "" "")))
	     (clobber (reg:SI 31))])]
  ""
  "")

(define_insn "*call_value_via_reg"
  [(set (match_operand 0 "register_operand" "=r")
	(call (mem:SI (match_operand:SI 1 "register_operand" "r"))
	      (match_operand 2 "" "")))
   (clobber (reg:SI 31))]
  ""
  "jalr %1"
  )

(define_insn "*call_value_via_label"
  [(set (match_operand 0 "register_operand" "=r")
	(call (mem:SI (match_operand:SI 1 "call_address_operand" ""))
	      (match_operand 2 "" "")))
   (clobber (reg:SI 31))]
  ""
  ; The %~ is necessary in case this insn gets conditionalized and the previous
  ; insn is the cc setter.
  "jal %1"
  )

;; nop NOP instruction templater
(define_insn "nop"
	[(const_int 0)]
	""
	"nop ; no operantion"
)

;; dummy test instruction
(define_insn "dummy_pattern"
	[(reg:SI 0)]
	"1"
	"This is just empty!"
)


(define_expand "prologue"
	[(const_int 1)] 
	""  
	"
	{
		logic32_expand_prologue ();
		DONE;
	}"
)

(define_expand "epilogue"
	[(return)]
	""
	"
	{
		logic32_expand_epilogue ();
		DONE;
	}"
)

;; RETURN instruction templater
(define_insn "return_from_func"
	[(set (pc) (return)) (use (reg:DI 31))]
	"reload_completed"
	"ret"
)

(define_insn "negsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(neg:SI (match_operand:SI 1 "register_operand" "r")))]
  ""
  "sub %0, $0, %1	; neg"
  ;;"neg %0, %1"
  [(set_attr "type" "unary")])

;;--------------------------------------------------------------------
;;-  Table jump
;;--------------------------------------------------------------------
;;
;; Operand 0 is the address of the table element to use
;; operand 1 is the CODE_LABEL for the table
;;--------------------------------------------------------------------
(define_insn "tablejump"
  [(set (pc)
	(match_operand:SI 0 "register_operand" "r"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "jmpr	%0"
  [(set_attr "type" "jmp")])

