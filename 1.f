\ Fixed-point arithmetic based definitions
: dot 256 ;      \ 256 to represent 1 unit of time
: dash 768 ;     \ 3 times dot
: char-space 256 ; \ space between characters in a letter
: letter-space 768 ; \ space between letters
: word-space 1792 ; \ space between words (7 units)

\ Error state
: error 9999 ;

\ Initialize stack with 0 for time accumulation
: init 0 ;

\ Add a unit of time to accumulator
: add-time ( time -- time') OVER + ;

\ Decode a single Morse code character from a string
: decode-char ( c-addr u -- )
  init
  0 DO
    I C@ CASE
      46 OF \ ASCII for '.'
        add-time dot
      ENDOF
      45 OF \ ASCII for '-'
        add-time dash
      ENDOF
      32 OF \ ASCII for ' '
        add-time word-space
      ENDOF
      DROP error \ Unrecognized character
    ENDCASE
  LOOP ;

\ Main decoding loop
: decode ( c-addr u -- )
  DO
    I decode-char
  LOOP ;

