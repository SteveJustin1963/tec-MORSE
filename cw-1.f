\ Stack: --
\ Variable initialization
variable dot_avg 0 dot_avg !
variable dash_avg 0 dash_avg !
variable CW_space_avg 0 CW_space_avg !
variable CW_boundary 0 CW_boundary !
variable space_time 0 space_time !
variable dash_time 0 dash_time !

: calc_dot_avg ( new_dot -- )
  8 /              \ Compute new_dot/8
  dot_avg @ 8 /    \ Compute dot_avg/8
  dot_avg @ +       \ Add dot_avg and new_dot/8
  -                 \ Subtract dot_avg/8
  dot_avg ! ;       \ Update dot_avg

: calc_pulse_avg ( -- avg )
  dot_avg @ 4 /     \ Compute dot_avg/4
  dash_avg @ 2 /    \ Compute dash_avg/2
  + ;               \ Return dot_avg/4 + dash_avg/2

: calc_CW_space_avg ( -- )
  CW_space_avg @ 8 /  \ Compute CW_space_avg/8
  space_time @ 8 /    \ Compute space_time/8
  CW_space_avg @ +    \ Add CW_space_avg and space_time/8
  -                   \ Subtract CW_space_avg/8
  CW_space_avg ! ;    \ Update CW_space_avg

: calc_symbol_boundary ( -- )
  dash_time @                \ Push dash_time
  CW_space_avg @ -           \ Subtract CW_space_avg
  4 /                        \ Divide by 4
  CW_space_avg @ -           \ Subtract from CW_space_avg
  CW_boundary ! ;            \ Update CW_boundary

: adjust_CW_boundary ( X -- )
  CW_boundary @ +            \ Add X to CW_boundary
  CW_boundary ! ;            \ Update CW_boundary
