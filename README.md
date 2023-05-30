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

using this logic we derive ... `cw-1`



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

=================


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

need to experiment, iterate, and fine-tune algorithms and settings to achieve the best results based on your specific audio signals and requirements.

 


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
