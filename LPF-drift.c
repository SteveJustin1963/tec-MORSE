\ simple low-pass filter with frequency drift. However, there are some issues and areas of confusion in your code. 
assuming A is a pointer to the input sample array, 
DE holds the address of the previous filtered sample, 
HL holds the initial cutoff frequency, 
and BC holds the drift value.
assuming that you want to do integer division by the number of taps, which will lead to rounding towards zero.
code  depend on additional factors such as the memory layout of your system, 
the type and size of the data  working with,
how we initialized your registers, 
and specific requirements for the low-pass filter.   \
    
    LowPassFilterWithDrift:
    LD B, 4       ; Number of filter taps
    LD C, B       ; Counter for the number of filter taps

FilterLoop:
    LD A, (DE)    ; Load the previous filtered sample
    SUB (HL)      ; Compare it with the current cutoff frequency
    JR NC, Skip   ; If above cutoff frequency, skip filtering

    ADD A, (DE)   ; Accumulate the current and previous samples
    INC DE
    DEC C
    JR NZ, FilterLoop

    PUSH HL       ; Preserve HL
    LD H, B       ; Copy the number of taps to HL
    CALL DivideByB
    POP HL        ; Restore HL

    DEC DE
    LD (DE), A    ; Store the filtered sample back into memory (decrement DE to point to the previous sample)

    LD A, H       ; Load the current cutoff frequency
    ADD A, C      ; Update the cutoff frequency with the drift value
    LD H, A       ; Store back the updated cutoff frequency
    RET

DivideByB:
    PUSH BC       ; Preserve BC
    PUSH DE       ; Preserve DE

    XOR A         ; Clear A register for quotient
    LD D, A       ; Clear D register for remainder
    LD E, B       ; Copy the number of taps to E register

DivideLoop:
    INC A         ; Increment A (quotient)
    CP E          ; Compare A with E (number of taps)
    JR NZ, DivideLoop

    POP DE        ; Restore DE
    POP BC        ; Restore BC

    RET

Skip:
    INC DE        ; Skip filtering, but still move to the next sample
    DEC C
    JR NZ, FilterLoop

    RET
