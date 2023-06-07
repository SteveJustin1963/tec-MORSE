; Moving Average Filtering with Drift
; Assume A points to the input sample, DE holds the previous filtered sample
; Assume B holds the number of adjacent samples to average (window size)
; Assume HL holds the initial drift value
; Assume BC holds the drift value change per iteration

MovingAverageFilterWithDrift:
    PUSH BC       ; Preserve the value of BC
    LD C, B       ; Counter for the number of samples to average
    LD B, 4       ; Number of filter taps

FilterLoop:
    LD A, (DE)    ; Load the previous filtered sample
    SUB H         ; Compare it with the current drift value
    JR NC, Skip   ; If above drift value, skip filtering

    LD HL, 0      ; Initialize accumulator

AverageLoop:
    ADD A, (DE)   ; Accumulate the current and previous samples
    INC DE
    DJNZ AverageLoop

    LD A, B       ; Divide the accumulated sum by the number of samples
    CALL DivideByB

    LD (DE-1), A  ; Store the filtered sample back into memory (decrement DE to point to the previous sample)

    ADD H, C      ; Update the drift value with the drift value change

    RET

Skip:
    INC DE        ; Skip filtering, but still move to the next sample

    DJNZ FilterLoop

    POP BC        ; Restore the value of BC
    RET

DivideByB:
    XOR H         ; Clear H register for quotient
    LD B, A       ; Copy dividend (accumulator) to B register
    XOR A         ; Clear A register for remainder

DivideLoop:
    CP B          ; Compare remainder with dividend
    JR C, DivideEnd ; If remainder < dividend, division complete

    SUB B         ; Subtract dividend from remainder
    INC H        
