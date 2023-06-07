; Low-Pass Filtering with Drift

; Assume A points to the input sample, DE holds the previous filtered sample
; Assume HL holds the initial cutoff frequency
; Assume BC holds the drift value

LowPassFilterWithDrift:
    LD B, 4       ; Number of filter taps
    LD C, B       ; Counter for the number of filter taps

FilterLoop:
    LD A, (DE)    ; Load the previous filtered sample
    SUB H         ; Compare it with the current cutoff frequency
    JR NC, Skip   ; If above cutoff frequency, skip filtering

    ADD A, (DE)   ; Accumulate the current and previous samples
    INC DE
    DJNZ FilterLoop

    LD A, B       ; Divide the accumulated sum by the number of taps
    CALL DivideByB

    LD (DE-1), A  ; Store the filtered sample back into memory (decrement DE to point to the previous sample)

    ADD H, C      ; Update the cutoff frequency with the drift value

    RET

DivideByB:
    XOR H         ; Clear H register for quotient
    LD B, A       ; Copy dividend (accumulator) to B register
    XOR A         ; Clear A register for remainder

DivideLoop:
    CP B          ; Compare remainder with dividend
    JR C, DivideEnd ; If remainder < dividend, division complete

    SUB B         ; Subtract dividend from remainder
    INC H         ; Increment quotient

    JR DivideLoop ; Repeat division

DivideEnd:
    RET
