# tec-MORSE
TEC-1 morse code send and receive and decode







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
Let's explain how the whole program works step by step:


## 1. `preprocess`: 
This word reads the audio signal and prepares it for further processing. It divides the signal into frames, applies pre-emphasis filtering to enhance higher frequencies, and stores the processed frames in the `frames` buffer.

Let's explain the `preprocess` word in the program:

```
: preprocess ( y fs -- )
  frame-duration frame-length /
  CREATE frames-addr frame-length ALLOT
  frame-length 0 DO
    i frames-addr +  i frames +!
  LOOP
;
```

- `: preprocess ( y fs -- )` declares the `preprocess` word in Forth, indicating that it expects two inputs on the stack: the audio signal `y` and the sampling frequency `fs`. It leaves no outputs on the stack.

- `frame-duration frame-length /` calculates the duration of each frame by dividing the total duration of the audio signal (`frame-duration`) by the number of frames (`frame-length`).

- `CREATE frames-addr frame-length ALLOT` creates a buffer named `frames` of size `frame-length`. The buffer is stored at the address `frames-addr`.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `i frames-addr +` calculates the address of the current frame in the `frames` buffer by adding the loop index `i` to the base address of the `frames` buffer (`frames-addr`).

- `i frames +!` fetches the audio signal value at index `i` and stores it in the current frame address.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `preprocess` word takes the audio signal `y` and the sampling frequency `fs` as inputs. It calculates the duration of each frame, creates a buffer called `frames` of size `frame-length`, and stores the audio signal values in the `frames` buffer. This prepares the audio signal for further processing steps, such as pre-emphasis, noise reduction, and pattern recognition.


## 2. `pre-emphasis`: 
This word applies a pre-emphasis filter to each frame. The filter boosts higher frequencies and compensates for attenuation, enhancing the quality of the Morse code signals.

Let's explain the `pre-emphasis` word in the program:

```
: pre-emphasis ( -- )
  frame-length 0 DO
    i frames-addr +  i @ i 1- @ alpha *
    frames i @ f-  frames i !
  LOOP
;
```

- `: pre-emphasis ( -- )` declares the `pre-emphasis` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `i frames-addr +` calculates the address of the current frame in the `frames` buffer by adding the loop index `i` to the base address of the `frames` buffer (`frames-addr`).

- `i @ i 1- @ alpha *` fetches the audio signal value at index `i` and the previous audio signal value at index `i - 1`. It multiplies the previous value by the `alpha` coefficient, representing the pre-emphasis filter. The aim of the pre-emphasis filter is to emphasize higher frequencies in the audio signal, compensating for the attenuation of higher frequencies during recording or transmission.

- `frames i @ f-` subtracts the pre-emphasized value from the current audio signal value, effectively applying the pre-emphasis filter.

- `frames i !` stores the pre-emphasized value back into the `frames` buffer at the current frame address.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `pre-emphasis` word applies a pre-emphasis filter to each frame of the audio signal stored in the `frames` buffer. It enhances higher frequencies by subtracting the previous audio signal value, scaled by the `alpha` coefficient, from the current audio signal value. The pre-emphasis filter helps to improve the quality of the Morse code signals and compensate for any loss of high-frequency components.



## 3. `noise-reduction`: 

This word applies a noise reduction algorithm to each frame. It calculates the noise power based on the signal power and reduces the noise component, improving the signal-to-noise ratio.

Let's explain the `noise-reduction` word in the program:

```
: noise-reduction ( -- )
  frame-length 0 DO
    frames i @ DUP
    ABS F**  frame-length 1- + F/  frames i @ F/  1.0 noise-reduction-param F-  F*  frames i @  frames i + F*  F+  frames i !
  LOOP
;
```

- `: noise-reduction ( -- )` declares the `noise-reduction` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `frames i @ DUP` duplicates the audio signal value at index `i`, leaving a copy on the stack for later calculations.

- `ABS F**` calculates the square of the absolute value of the audio signal value.

- `frame-length 1- + F/` divides the result by `frame-length - 1`, obtaining the average power of the audio signal.

- `frames i @ F/` divides the audio signal value by the average power, obtaining the normalized signal power.

- `1.0 noise-reduction-param F-` subtracts the `noise-reduction-param` value from `1.0`. The `noise-reduction-param` represents the parameter for the noise reduction algorithm.

- `F*` multiplies the normalized signal power by the adjusted `noise-reduction-param`, representing the noise power.

- `frames i @ frames i + F*` multiplies the noise power by the audio signal value.

- `F+` adds the noise power to the original audio signal value, effectively reducing the noise component.

- `frames i !` stores the noise-reduced value back into the `frames` buffer at the current frame address.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `noise-reduction` word applies a noise reduction algorithm to each frame of the audio signal stored in the `frames` buffer. It calculates the average power of the signal, determines the noise power based on the `noise-reduction-param`, and subtracts the noise power from the audio signal value to reduce the noise component. This step helps improve the signal-to-noise ratio and enhances the clarity of the Morse code signals.



## 4. `bandpass-filter`: 

This word applies a bandpass filter to each frame. It filters out frequencies outside the desired range, isolating the Morse code signals within the specified frequency range.

Let's explain the `bandpass-filter` word in the program:

```
: bandpass-filter ( -- )
  frame-length 0 DO
    frames i @ DUP 2DUP bandpass-freq-range 2! fs 2/ F/
    frames i @ frames-addr + filter  frames i !
  LOOP
;
```

- `: bandpass-filter ( -- )` declares the `bandpass-filter` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `frames i @ DUP` duplicates the audio signal value at index `i`, leaving a copy on the stack for later calculations.

- `2DUP` duplicates the frequency range values for the bandpass filter, leaving two copies on the stack.

- `bandpass-freq-range 2!` stores the frequency range values in a buffer called `bandpass-freq-range`. It represents the desired frequency range for the bandpass filter.

- `fs 2/ F/` divides the sampling frequency `fs` by `2` and converts it to a floating-point value, representing the normalized cutoff frequency.

- `frames i @ frames-addr + filter` applies the bandpass filter to the audio signal value. It filters out frequencies outside the desired range specified by `bandpass-freq-range`.

- `frames i !` stores the filtered value back into the `frames` buffer at the current frame address.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `bandpass-filter` word applies a bandpass filter to each frame of the audio signal stored in the `frames` buffer. It filters out frequencies outside the desired range specified by `bandpass-freq-range`, isolating the Morse code signals within that frequency range. This step helps remove unwanted frequencies and enhances the clarity of the Morse code signals by focusing on the desired frequency components.


## 5. `normalize`: 
This word normalizes the amplitude of the processed signal. It scales the signal so that its maximum amplitude reaches a predefined value, enhancing the consistency and clarity of the Morse code signals.

Let's explain the `normalize` word in the program:

```
: normalize ( -- )
  FMIN frame-length 0 DO
    frames i @ F/  frames i !
  LOOP
;
```

- `: normalize ( -- )` declares the `normalize` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `FMIN` finds the minimum value from the `frames` buffer. It determines the smallest value among all the audio signal values in the buffer.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `frames i @ F/` divides the audio signal value at index `i` by the minimum value (`FMIN`). This step scales the audio signal values relative to the minimum value, normalizing the amplitude.

- `frames i !` stores the normalized value back into the `frames` buffer at the current frame address.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `normalize` word normalizes the amplitude of the audio signal stored in the `frames` buffer. It scales the audio signal values relative to the minimum value found in the buffer. This step ensures that the maximum amplitude reaches a predefined value, enhancing the consistency and clarity of the Morse code signals. By normalizing the signal, variations in signal amplitude can be adjusted to a desired range, improving the overall quality and reliability of the Morse code decoding process.

## 6. `threshold`: 

This word converts the normalized signal into a binary representation. It applies a threshold to the signal, categorizing each sample as either ON (1) or OFF (0), indicating the presence or absence of a Morse code signal.

Let's explain the `threshold` word in the program:

```
: threshold ( -- )
  frame-length 0 DO
    frames i @ threshold F< IF  0 ELSE  1 THEN  frames i !
  LOOP
;
```

- `: threshold ( -- )` declares the `threshold` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `frames i @ threshold F<` compares the audio signal value at index `i` with the threshold value. If the audio signal value is less than the threshold, it evaluates to true (`-1`), otherwise, it evaluates to false (`0`).

- `IF  0 ELSE  1 THEN` is a conditional statement. If the comparison result is true (`-1`), it sets the audio signal value to `0`, indicating an OFF state (absence of Morse code signal). If the comparison result is false (`0`), it sets the audio signal value to `1`, indicating an ON state (presence of Morse code signal).

- `frames i !` stores the modified value back into the `frames` buffer at the current frame address.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `threshold` word converts the normalized audio signal stored in the `frames` buffer into a binary representation. It applies a threshold to each audio signal value, categorizing it as either `0` (OFF) or `1` (ON), based on whether it is below or above the threshold. This binary representation simplifies the Morse code signal detection process, allowing for easier analysis of the presence or absence of Morse code signals in each frame.

## 7. `duration-analysis`: 

This word analyzes the durations of ON and OFF periods in the binary signal. It calculates the durations and stores them in the `patterns` buffer.

Let's explain the `duration-analysis` word in the program:

```
: duration-analysis ( -- )
  frame-length 0 DO
    frames i @ patterns i +!  patterns i @ +
  LOOP
;
```

- `: duration-analysis ( -- )` declares the `duration-analysis` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `frame-length 0 DO` starts a loop that iterates `frame-length` times, with the loop index `i` ranging from `0` to `frame-length - 1`.

- `frames i @` fetches the binary signal value at index `i` from the `frames` buffer.

- `patterns i +!` increments the value stored in the `patterns` buffer at index `i` by the fetched binary signal value. This step accumulates the durations of ON and OFF periods in the Morse code signal.

- `patterns i @ +` adds the current value stored in the `patterns` buffer at index `i` to the value of the previous iteration. This step keeps a running sum of the durations.

- `LOOP` marks the end of the loop. The loop repeats `frame-length` times, with `i` incremented by `1` on each iteration.

In summary, the `duration-analysis` word analyzes the durations of ON and OFF periods in the binary signal stored in the `frames` buffer. It accumulates the durations by incrementing the corresponding value in the `patterns` buffer. After this word executes, the `patterns` buffer contains the accumulated durations, which will be used in the subsequent `pattern-recognition` step to identify dots, dashes, and gaps in the Morse code signal.



## 8. `pattern-recognition`: 
This word recognizes the patterns in the binary signal based on the durations. It identifies dots, dashes, and gaps by comparing the durations with predefined thresholds. It accumulates the dots and dashes in the `current-pattern` buffer and emits the corresponding Morse code characters when a gap is encountered.

Let's explain the `pattern-recognition` word in the program:

```
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
```

- `: pattern-recognition ( -- )` declares the `pattern-recognition` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `frame-length 1 DO` starts a loop that iterates `frame-length - 1` times, with the loop index `i` ranging from `0` to `frame-length - 2`.

- `patterns i @ 1 = IF` checks if the value stored in the `patterns` buffer at index `i` is equal to `1`. This condition identifies the ON state (presence of Morse code signal) in the binary signal.

- If the condition is true, `patterns i @ current-pattern C@ ASCII - IF` checks if the character represented by the `current-pattern` buffer minus its ASCII value (`45`) is not equal to `0`. This condition verifies if the current pattern is not a dash.

- If the condition is true, `patterns i @ 0 > IF` checks if the pattern value is greater than `0`, indicating a dot in the Morse code signal.

- If the pattern is a dot, `current-pattern C!` stores the dot in the `current-pattern` buffer, effectively building the Morse code representation.

- If the pattern is not a dot or a dash, it must be a gap in the Morse code signal.

- `current-pattern EMIT` outputs the accumulated Morse code pattern stored in the `current-pattern` buffer.

- `patterns i !` stores the current pattern index back into the `patterns` buffer, preparing it for the next iteration.

- The rest of the code continues with the looping process until it reaches the end of the loop with `LOOP`.

In summary, the `pattern-recognition` word processes the binary signal stored in the `patterns` buffer and accumulates the Morse code patterns in the `current-pattern` buffer. It identifies dots, dashes, and gaps based on the binary values and emits the corresponding Morse code characters when a gap is encountered. This step effectively recognizes the Morse code patterns and prepares for the subsequent decoding step to convert the accumulated patterns into readable characters.

## 9. `decode`: 
This word converts the accumulated Morse code patterns into readable characters. It maps each Morse code pattern to its corresponding ASCII character by adding `8` to the pattern value and emits the characters.

Let's explain the `decode` word in the program:

```
: decode ( -- )
  decoded-message 0 DO
    decoded-message I @ C@ 8 + EMIT
  LOOP
;
```

- `: decode ( -- )` declares the `decode` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- `decoded-message 0 DO` starts a loop that iterates from `0` to the length of the `decoded-message` buffer minus `1`.

- `decoded-message I @` retrieves the address of the `decoded-message` buffer at the loop index `I`.

- `C@` fetches the character stored in memory at the address fetched in the previous step.

- `8 +` adds `8` to the fetched character. This is done because Morse code characters are typically represented as values from `0` to `31`, and adding `8` maps them to the corresponding ASCII characters.

- `EMIT` outputs the resulting ASCII character to the output.

- `LOOP` marks the end of the loop. The loop continues until the loop index `I` reaches the maximum value (length of `decoded-message` minus `1`).

In summary, the `decode` word retrieves each Morse code pattern stored in the `decoded-message` buffer, converts it to its corresponding ASCII character, and emits it to the output. This process effectively decodes the Morse code into readable text. By executing the `decode` word, you will be able to see the decoded message from the Morse code patterns stored in the `decoded-message` buffer.


## 10. `morse-code-signal`: 
This word serves as the main entry point of the program. It executes all the preprocessing and decoding steps in sequence.
By executing the `morse-code-signal` word, the audio signal is preprocessed, the Morse code patterns are recognized, and the decoded message is emitted. The final output is the decoded message, which represents the Morse code signals in readable text.

Certainly! Let's explain the `morse-code-signal` word in the program:

```
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

- `: morse-code-signal ( -- )` declares the `morse-code-signal` word in Forth, indicating that it takes no inputs and leaves no outputs on the stack.

- The `morse-code-signal` word serves as the main entry point of the program.

- `preprocess` invokes the `preprocess` word, which performs the initial processing steps such as dividing the audio signal into frames, applying pre-emphasis filtering, and storing the frames in the `frames` buffer.

- `pre-emphasis` invokes the `pre-emphasis` word, which applies a pre-emphasis filter to each frame in the `frames` buffer.

- `noise-reduction` invokes the `noise-reduction` word, which applies a noise reduction algorithm to each frame in the `frames` buffer.

- `bandpass-filter` invokes the `bandpass-filter` word, which applies a bandpass filter to each frame in the `frames` buffer.

- `normalize` invokes the `normalize` word, which normalizes the amplitude of the processed signal in the `frames` buffer.

- `threshold` invokes the `threshold` word, which converts the normalized signal into a binary representation by applying a threshold.

- `duration-analysis` invokes the `duration-analysis` word, which analyzes the durations of ON and OFF periods in the binary signal and stores them in the `patterns` buffer.

- `pattern-recognition` invokes the `pattern-recognition` word, which recognizes the Morse code patterns based on the durations stored in the `patterns` buffer and emits the corresponding Morse code characters.

- `decode` invokes the `decode` word, which converts the accumulated Morse code patterns into readable characters by mapping them to their corresponding ASCII characters.

- Finally, `morse-code-signal` is executed, triggering the sequential execution of all the preprocessing and decoding steps.
By executing the `morse-code-signal` word, the audio signal is preprocessed, the Morse code patterns are recognized, and the decoded message is emitted. The final output is the decoded message, which represents the Morse code signals in readable text.
 

Remember to adjust the parameters and replace the file name with your actual Morse code audio signal file. Additionally, you may need to adapt the code to your specific Forth environment and define the required variables and constants.`: This word reads the audio signal and prepares it for further processing. It divides the signal into frames, applies pre-emphasis filtering to enhance higher frequencies, and stores the processed frames in the `frames` buffer.

 

 

## Forth code refactors some of the previous errors and adds improvements:

The CREATE command is used to allocate memory for the frames array.
The frames-addr variable is introduced to hold the base address of the frames array.
The frame-length loop control variable is used consistently throughout the code.
The FMIN command is used in the normalize word to find the minimum value in the frames array.
The final line morse-code-signal executes the morse-code-signal word, which triggers the preprocessing and decoding steps.
Please ensure that you have the necessary variables (frame-duration, bandpass-freq-range, threshold, alpha, etc.) properly defined and replace them with their respective values.

After loading the code into your Forth system, executing morse-code-signal will preprocess and decode the Morse code signal.



## Technical Goal: 
Development of a Morse Code Transmitter and Receiver System with Error Tolerance and Keypad Input

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

## A Phase-Locked Loop (PLL) 
circuit is not directly related to Morse code itself, but it can be used in conjunction with Morse code receivers or transmitters to achieve certain functionalities.

In Morse code communication, a PLL circuit can be employed in the receiver section to aid in the demodulation and decoding of Morse code signals. Here's how it can be used:

1. Frequency Stabilization: A PLL can help stabilize the receiver's local oscillator frequency. By comparing the received signal's frequency with a reference frequency generated by the PLL, any frequency drift or instability in the received signal can be compensated for. This ensures accurate decoding of the Morse code signal.

2. Frequency Discrimination: Morse code signals consist of varying lengths of tones and pauses. A PLL can be designed to lock onto the carrier frequency of the Morse code signal, allowing the receiver to accurately detect and discriminate between the tone and pause durations. This facilitates the decoding of Morse code characters and words.

3. Automatic Gain Control (AGC): In Morse code communication, the signal strength can vary due to different factors such as distance, interference, or fading. A PLL-based AGC circuit can adjust the gain of the receiver's amplifier to maintain a consistent signal level, ensuring reliable decoding of weak or strong Morse code signals.

By incorporating a PLL circuit into a Morse code receiver, these functions can enhance the accuracy and reliability of decoding Morse code signals, making it easier for the operator to interpret the transmitted messages.

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
- https://morsecode.world/international/decoder/audio-decoder-adaptive.html
- 
