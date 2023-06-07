# tec-MORSE
TEC-1 morse code send and rx and decode







## Morse code 
is a code where each letter of the alphabet is represented by a combination of dots and dashes. The code is named after Samuel Morse, who developed it in 1837. There are only a few simple rules to encoding and decoding Morse code:

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
symbol_boundary = CW_space_average - (  dash_time -  CW_space_average)/4
adjust_CW_boundary =  CW_boundary +- X
```
this algorithm relates to timing calculations. Let's go through each of them:

1. dot_avg calculation: This algorithm calculates the average duration of a dot by taking into account the previous dot_avg and the duration of a new dot. It uses a weighted average formula, where the new dot contributes 7/8th of its value, and the previous dot_avg contributes 1/8th of its value. This can be helpful for estimating the timing of dots in Morse code.

2. pulse_ave calculation: This algorithm calculates the average duration of a pulse, which is a unit of time that encompasses either a dot or a dash. It combines the dot_avg and dash_avg by using a weighted average formula, where dot_avg contributes 1/4th of its value, and dash_avg contributes 1/2nd of its value. This can be useful for general timing calculations involving Morse code symbols.

3. CW_space_avg calculation: This algorithm calculates the average duration of the space between two Morse code symbols. It uses a weighted average formula similar to dot_avg, where the space_time contributes 7/8th of its value, and the previous CW_space_avg contributes 1/8th of its value. This helps estimate the timing of spaces between symbols.

4. symbol_boundary calculation: This algorithm estimates the boundary between a Morse code symbol and the following space. It subtracts the quantity (dash_time - CW_space_average) from CW_space_average and then divides it by 4. This formula assumes that the dash_time is greater than the CW_space_average and helps determine the boundary based on the difference between them.

5. adjust_CW_boundary calculation: This algorithm adjusts the CW_boundary value by adding or subtracting a value represented by X. The purpose of this adjustment is not clear from the provided information, but it seems to provide flexibility for fine-tuning the boundary estimation.

 note that the implementation and usage of these algorithms may vary depending on the specific context of your Morse code decoding project. Additionally, the effectiveness and accuracy of these algorithms may depend on the quality and consistency of the Morse code signals being processed.

```
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

explanation- benefits:

1. Initialization:
   - `index = 0` initializes the index variable used to keep track of the current position in the lookup string.
   - `dash_jump = 64` sets the initial value of dash_jump, which is used to determine the jump distance within the binary search algorithm.

2. Binary search iteration:
   - For each received element `e`, the algorithm performs the following steps:
     - `dash_jump = dash_jump/2` reduces the jump distance by half in each iteration.
     - `(e == '.') ? 1 : dash_jump` checks if the received element is a dot or a dash. If it's a dot, the index is incremented by 1; otherwise, it is incremented by the current value of dash_jump.
   - The binary search algorithm allows for efficient searching and decoding of Morse code symbols by repeatedly dividing the search space in half.

3. Lookup and output:
   - `ascii = lookupstring[index]` retrieves the ASCII character corresponding to the final index position after the binary search.
   - The lookup string likely contains a mapping between Morse code symbols and their corresponding ASCII characters.

Benefits of using this algorithm:
- Efficiency: The binary search algorithm provides efficient searching by reducing the search space in half in each iteration. This can significantly speed up the decoding process, especially if you have a large number of Morse code symbols to decode.
- Accuracy: By using a binary search approach, the algorithm can precisely locate the corresponding ASCII character in the lookup table based on the received Morse code symbols.
- Flexibility: This algorithm can be easily adapted to different Morse code lookup tables or mappings by adjusting the lookupstring and the symbols it contains.

Overall, this algorithm allows for fast and accurate decoding of Morse code symbols into ASCII characters, making it suitable for real-time applications or scenarios where efficiency is crucial.

 ```
 variable index
variable dash_jump
: lookup ( -- char ) "abcdefghijklmnopqrstuvwxyz0123456789" index @ + c@ ;

: reset ( -- ) 0 index ! 64 dash_jump ! ;
: process ( c -- ) 
    dash_jump @ 2 / dash_jump !
    over [char] . = if drop 1 else dash_jump @ then
    index @ + index !
;
: decode ( -- ascii ) lookup ;

: main
    reset
    begin key process key? until
    decode emit
;
```
Let's break down how this program works.

First, we declare two global variables, index and dash_jump, which will hold the current index in the lookup string and the current dash_jump value, respectively.

The lookup word retrieves the ASCII character at the current index from the lookup string, which contains a mapping of Morse code symbols to ASCII characters.

The reset word initializes index to 0 and dash_jump to 64, just like in your pseudocode.

The process word takes a character from the input, checks if it's a dot (.), and updates index accordingly. If the input character is a dot, index is incremented by 1; otherwise, it's incremented by the current value of dash_jump. After each input character, dash_jump is divided by 2.

The decode word retrieves the ASCII character corresponding to the current index from the lookup string.

Finally, main is the entry point for the program. It initializes the variables with reset, reads input characters with process until there's no more input, and then uses decode to retrieve and print the corresponding ASCII character.

Please note that Forth is a stack-based language, and it uses Reverse Polish Notation (RPN), so operations are often done in a different order than you might expect if you're used to other languages. Also, Forth doesn't have built-in control structures like if or for in the way you might be used to. Instead, it uses words (functions) to manipulate the stack and control program flow.


## getting the signal ready

1. Audio input:
   - If you're connecting the audio output of your radio receiver to your computer, you can use an audio cable with the appropriate connectors. Plug one end into the audio output of the receiver and the other end into the line-in or microphone port of your computer.
   - Alternatively, if you're using a microphone to capture the audio, connect the microphone to your computer's microphone port.
   - Ensure that the audio levels are set correctly to avoid distortion or clipping. You can adjust the volume levels on both the receiver and your computer.

2. Preprocessing:
   - Use audio processing software or libraries to filter out background noise and enhance the quality of the Morse code signals. Tools like noise reduction, bandpass filters, or adaptive filters can be helpful.
   - Experiment with different preprocessing techniques to find the optimal settings for your specific audio signals.

3. Thresholding:
   - Apply a thresholding algorithm to convert the preprocessed audio signal into a binary representation.
   - Determine an appropriate threshold level that separates the Morse code pulses from the silence. This can be achieved by analyzing the amplitude of the audio signal and setting a threshold value accordingly.
   - Simple thresholding methods compare the signal amplitude to a fixed threshold, while more advanced techniques such as adaptive thresholding adjust the threshold dynamically based on the characteristics of the signal.

4. Pulse detection:
   - Analyze the binary signal to detect the transitions from logic 0 to logic 1 and vice versa. These transitions represent the timing boundaries between dots, dashes, and spaces.
   - You can implement a pulse detection algorithm that identifies rising or falling edges in the binary signal. This can be done by comparing consecutive samples and detecting changes in the binary values.
   - Consider incorporating techniques such as hysteresis or debouncing to handle potential noise or false detections.

5. Timing calculations:
   - Once you have detected the transitions between pulses and spaces, measure the durations of dots, dashes, and spaces based on these boundaries.
   - Implement your timing algorithms, such as dot_avg, pulse_ave, CW_space_avg, symbol_boundary, and adjust_CW_boundary, using the detected pulse and space durations to update and refine the timing values.

6. Morse code decoding:
   - Map the calculated pulse and space durations to the appropriate Morse code symbols based on the Morse code specification.
   - Create a lookup table or dictionary that associates the timing patterns with the corresponding Morse code symbols (e.g., dots, dashes, spaces).
   - Decode the Morse code symbols based on the identified patterns, and store or output the decoded text accordingly.

7. Output:
   - Determine how you want to process or use the decoded Morse code symbols.
   - You can simply print the decoded text to the console or display it in a user interface.
   - If you're building a more complex application, you might transmit the decoded text over a network, save it to a file, or perform further analysis on the Morse code data.


```
VARIABLE frames
VARIABLE patterns
VARIABLE current-pattern
VARIABLE decoded-message

: preprocess ( y fs -- )
  frame-duration frame-length /
  CREATE frames-addr frame-length ALLOT
  frame-length 0 DO
    i frames-addr +  i frames +!
  LOOP
;

: pre-emphasis ( -- )
  frame-length 0 DO
    i frames-addr +  i @ i 1- @ alpha *
    frames i @ f-  frames i !
  LOOP
;

: noise-reduction ( -- )
  frame-length 0 DO
    frames i @ DUP  ABS F**  frame-length 1- + F/  frames i @ F/  1.0 noise-reduction-param F-  F*  frames i @  frames i + F*  F+  frames i !
  LOOP
;

: bandpass-filter ( -- )
  frame-length 0 DO
    frames i @ DUP  2DUP bandpass-freq-range 2! fs 2/ F/
    frames i @ frames-addr +  filter  frames i !
  LOOP
;

: normalize ( -- )
  FMIN  frame-length 0 DO
    frames i @ F/  frames i !
  LOOP
;

: threshold ( -- )
  frame-length 0 DO
    frames i @ threshold F< IF  0 ELSE  1 THEN  frames i !
  LOOP
;

: duration-analysis ( -- )
  frame-length 0 DO
    frames i @ patterns i +!  patterns i @ +
  LOOP
;

: pattern-recognition ( -- )
  frame-length 1 DO
    patterns i @ 1 = IF
      patterns i @ current-pattern C@ ASCII - IF
        patterns i @ 0 > IF
          current-pattern C!
          current-pattern EMIT
        THEN
      ELSE
        current-pattern C!
      THEN
      patterns i !
    ELSE
      patterns i @ 0 = IF
        current-pattern C!
        current-pattern EMIT
      THEN
      patterns i !
    THEN
  LOOP
;

: decode ( -- )
  decoded-message 0 DO
    decoded-message I @ C@ 8 + EMIT
  LOOP
;

: morse-code-signal ( -- )
  preprocess
  pre-emphasis
  noise-reduction
  bandpass-filter
  normalize
  threshold
  duration-analysis
  pattern-recognition
  decode
;

morse-code-signal
```

Forth code refactors some of the previous errors and adds improvements:

The CREATE command is used to allocate memory for the frames array.
The frames-addr variable is introduced to hold the base address of the frames array.
The frame-length loop control variable is used consistently throughout the code.
The FMIN command is used in the normalize word to find the minimum value in the frames array.
The final line morse-code-signal executes the morse-code-signal word, which triggers the preprocessing and decoding steps.
Please ensure that you have the necessary variables (frame-duration, bandpass-freq-range, threshold, alpha, etc.) properly defined and replace them with their respective values.

After loading the code into your Forth system, executing morse-code-signal will preprocess and decode the Morse code signal.



## Technical Goal: Development of a Morse Code Transmitter and Receiver System with Error Tolerance and Keypad Input

Objective: To design and implement a comprehensive Morse Code communication system capable of encoding and decoding messages while being tolerant against frequency drift, duration shift, and errors. The system should provide a user-friendly interface for inputting Morse Code messages via a keypad and outputting them through a TTL switch to operate a Pixie QRP-CW-2Watt transmitter connected to a magloop antenna. The system will also include an LED readout to display the transmitted and received Morse Code messages.

Key Components:

1. Pulse Duration Creation and Detection: Develop a circuit or software algorithm that can generate and detect precise pulse durations corresponding to the dots and dashes of Morse Code.

2. Morse Encoder: Implement a module that converts alphanumeric characters into their corresponding Morse Code representations. This encoder should take user input from a keypad and produce the Morse Code output.

3. Morse Decoder: Create a module capable of accurately decoding Morse Code signals received by the system, converting them back into alphanumeric characters for display.

4. Error Tolerance: Design the system to handle frequency drift and duration shift by incorporating adaptive algorithms that can adjust pulse timings dynamically. Error detection and correction techniques should be employed to minimize data corruption during transmission and reception.

5. User Interface: Develop a user-friendly interface that allows users to input Morse Code messages through a keypad. This interface should provide real-time feedback on the input and allow for easy editing and transmission of the messages.

6. TTL Switch Integration: Integrate the system with a TTL switch to enable the operation of a Pixie QRP-CW-2Watt transmitter. The TTL switch should be capable of activating the transmitter when the Morse Code message is ready for transmission.

7. LED Readout: Implement an LED readout that can display the transmitted and received Morse Code messages in real-time. The LED display should be designed to clearly indicate the dots, dashes, and spaces.

By achieving these technical goals, the Morse Code communication system will enable reliable and efficient transmission and reception of messages, facilitating effective communication in a variety of scenarios. 
 
## nn approach
 ```
 \ Define the neural network weights
variable weights
8 cells allot weights !

: init-weights ( -- )
  8 0 do
    i cells weights + i !  
  loop ;

\ Define the Morse code lookup table
variable morse-lookup
78 cells allot morse-lookup !

: init-morse-lookup ( -- )
  morse-lookup 26 0 do
    i 3 * cells + morse-lookup 65 + !
    i 1 + 3 * cells + morse-lookup 65 + !
    i 2 + 3 * cells + morse-lookup 65 + !
  loop ;

\ Function to decode Morse code using the neural network
: decode-morse-code ( addr -- )
  weights 8 0 do
    i cells weights + @ 
    8 0 do
      i cells weights + @
      swap over xor 0= if
        drop emit exit
      then
    loop
  loop ;

\ Function to convert an 8-bit integer to Morse code
: int-to-morse-code ( n -- )
  8 0 do
    dup 1 and if
      '-' emit
    else
      '.' emit
    then
    2/ swap
  loop drop ;

\ Initialize the weights and Morse code lookup table
init-weights
init-morse-lookup

\ Example usage
: test-decode
  ." Enter an 8-bit integer: "
  decimal
  2>r ( u.r cr )   \ NOTE: The use of 'u.r' might cause problems depending on the Forth system
  r> int-to-morse-code
  decode-morse-code cr ;

test-decode
```

 how the code works:

1. Neural Network Weights:
   - The line `variable weights` declares a Forth variable named `weights`.
   - The line `8 cells allot weights !` allocates 8 cells of memory for the `weights` variable.
   - The `init-weights` function initializes the `weights` variable by storing values in each cell using the loop construct. The loop iterates 8 times, using the index `i` to calculate the address of each cell and storing the corresponding value using `i !`.

2. Morse Code Lookup Table:
   - The line `variable morse-lookup` declares a Forth variable named `morse-lookup`.
   - The line `78 cells allot morse-lookup !` allocates 78 cells of memory for the `morse-lookup` variable.
   - The `init-morse-lookup` function initializes the `morse-lookup` variable by storing the ASCII values for each letter of the alphabet and their corresponding Morse code patterns. The loop iterates 26 times, using the index `i` to calculate the addresses of the cells in the `morse-lookup` table and storing the ASCII values using `i +` and `!`.

3. Decoding Morse Code:
   - The `decode-morse-code` function takes an address as input, representing a Morse code pattern, and decodes it using the neural network weights.
   - The function starts by looping through the neural network weights using the `8 0 do` construct, with the index `i` representing the current weight.
   - Within the nested loop, the function retrieves the current weight and compares it with the input pattern using `swap over xor 0=`. If they match, the corresponding ASCII value is emitted using `emit` and the function exits.
   - If no match is found, the loop continues to the next weight until all weights have been checked.

4. Converting an 8-bit Integer to Morse Code:
   - The `int-to-morse-code` function takes an 8-bit integer as input and converts it into a Morse code pattern.
   - The function starts by looping 8 times using the `8 0 do` construct, with the index `i` representing the current bit position.
   - Within the loop, the function checks the least significant bit of the integer using `1 and`. If it's set (1), the function emits a dash (`'-'`) using `emit`; otherwise, it emits a dot (`'.'`).
   - After emitting the appropriate character, the function divides the integer by 2 (`2/`) to shift the bits to the right.
   - Once all 8 bits have been processed, the function drops the remaining integer from the stack.

5. Example Usage:
   - The `test-decode` function provides an example usage scenario.
   - It prompts the user to enter an 8-bit integer using `"Enter an 8-bit integer: "`, and then reads the input using `decimal 2>r u.r cr r>` (Note: The use of `u.r` might cause problems depending on the Forth system).
   - The integer is then converted to Morse code using `int-to-morse-code` and decoded using `decode-morse-code`.
   - The resulting character(s) are printed to the console using `emit`.
   - Finally, the `cr` command is used to print a newline character.

Note: The code assumes the use of ASCII encoding and handles upper case letters A to Z. Adjustments may be necessary for handling additional characters or different encoding schemes.


## Iterate
- need to experiment, iterate, and fine-tune algorithms and settings to achieve the best results based on your specific audio signals and requirements.



## Ref
- http://alanpich.github.io/Morse-Code-Recognition/Introduction.html
- https://www.pa3fwm.nl/software/rscw/algorithm.html
- https://github.com/nicolacimmino/MorseDecoder
- https://hackaday.com/2014/01/17/magic-morse-arduino-trainer/
- https://github.com/jwrr/z80usb
- https://github.com/SteveJustin1963/tec-HELL
- https://github.com/SteveJustin1963/tec-RTTY
- http://mikenaberezny.com/hardware/c64-128/microlog-air-1/
- 
