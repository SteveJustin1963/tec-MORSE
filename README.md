# tec-MORSE
TEC-1 morse code send and rx and decode





### project
- build a encoder/ decoder. code for freq and pulse duration detection like tec-DAT instead for morse code and display on led readout. 
- send morse from keypad, and output to ttl switch to operate pixie qrp-cw-2watt on magloop antenna
- 
 

## Method
Morse code is a code where each letter of the alphabet is represented by a combination of dots and dashes. The code is named after Samuel Morse, who developed it in 1837. There are only a few simple rules to encoding and decoding Morse code:

1. A dot is one unit of time.
2. A dash is three units of time.
3. A space between dots and dashes in the same character is one unit of time.
4. A space between characters is three units of time.
5. A space between words is seven units of time.

Here is an example of the word "help" encoded in Morse code: `.... . .-.. .--. .-.-.-`

The first letter, "h", is represented by four dots. The next letter, "e", is represented by one dot and one dash. The next letter, "l", is represented by two dots. The next letter, "p", is represented by one dash and three dots. To decode a Morse code message, you simply need to know the code for each letter and follow the rules.

A decoder tree can be used like;

![](https://github.com/SteveJustin1963/tec-MORSE/blob/master/pics/mortree1.png)


## Algorithm 1

```
dot_avg = dot_avg + new_dot/8 - dot_avg/8
pulse_ave = dot_avg/4 + dash_avg/2
CW_space_avg = CW_space_avg - CW_space_avg/8 + space_time/8
symbol_bounday = CW_space_average - (  dash_time -  CW_space_average)/4
adjust_CW_boundary =  CW_boundary +- X
```
 ## using this logic we derive ...
 ```
cw-1
```



##  Algorithms2
```
index=0
    dash_jump=64
    for each received element e
      dash_jump=dash_jump/2
      index = index + (e=='.')?1:dash_jump
    endfor
    ascii = lookupstring[index]
```

 

 


## Iterate
- https://github.com/SteveJustin1963/tec-HELL
- https://github.com/SteveJustin1963/tec-RTTY

## Ref
- http://alanpich.github.io/Morse-Code-Recognition/Introduction.html
- https://www.pa3fwm.nl/software/rscw/algorithm.html
- https://github.com/nicolacimmino/MorseDecoder
- https://hackaday.com/2014/01/17/magic-morse-arduino-trainer/
- https://github.com/jwrr/z80usb
- 
- 
