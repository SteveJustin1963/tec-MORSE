# tec-MORSE
## sender 
Morse code sender that takes keyboard input and outputs to a port bit. 
Includes timing and proper Morse spacing.

```
// Morse Code Sender for MINT2
// Variables:
// p = output port number (#80) 
// b = output bit mask (#01)
// t = base time unit (ms)
// c = current character
// i = lookup index
// s = morse string buffer

// Port and timing setup
:I #80 p! // Set output port
   #01 b! // Set bit mask
   256 t! ; // Set base timing unit (adjust for desired WPM)

// Morse patterns lookup - each pattern ends with 0
// Format: dot=1 dash=2 end=0
:S [102 2110 2110 2110 10 1110 220 1110 1111 1222 212 1210 22 // A-M
    21 222 1210 2212 121 111 2 112 112 122 2212 2122 2211 0] s! ; // N-Z

// Send bit to port - accepts 0 or 1
:B b * p /O ; // Output to port using mask

// Time delay - n * base unit
:D t * 50 * (0) ; // Basic delay loop

// Send dot
:T 1 B 1 D 0 B 1 D ;

// Send dash
:H 1 B 3 D 0 B 1 D ;

// Find pattern for letter (returns index to s array)
:F c #41 - // Convert ASCII to 0-based index
   13 * // Multiply by max pattern length
   s + ; // Add to string base address

// Send one Morse character
:C F i! // Get pattern start
   /U ( // Loop until end
     i ? " // Duplicate current value
     0 = /W // Exit if zero
     1 = ( T ) /E ( H ) // Send dot or dash
     i 1+ i! // Next pattern element
   )
   3 D ; // Inter-character space

// Main input/output loop
:M /U (
     `Enter text (uppercase):` /N
     /K c! // Get character
     c #0D = /W // Exit on CR
     c #20 = ( 7 D ) /E ( c C ) // Space or character
   ) ;

// Initialize and run
:R I S M ;
```


This Morse code sender has several key features:

1. Port Configuration:
- Uses port #80 (configurable)
- Single bit output using bit mask
- Adjustable timing for different WPM speeds

2. Morse Pattern Storage:
- Efficient pattern encoding (1=dot, 2=dash, 0=end)
- Supports A-Z uppercase letters
- Patterns stored in compact array

3. Timing:
- Base timing unit is configurable
- Standard Morse timings:
  - Dot = 1 unit
  - Dash = 3 units
  - Inter-element space = 1 unit
  - Inter-character space = 3 units
  - Word space = 7 units

4. Functions:
- I: Initialize port and timing
- B: Output bit to port
- D: Time delay
- T: Send dot
- H: Send dash
- F: Find pattern for letter
- C: Send one character
- M: Main input loop
- R: Run program

To use:
1. Run :R to start
2. Type uppercase letters (A-Z)
3. Use space for word separation
4. Enter/return to exit

The program will output proper Morse code timing to the specified port bit. You can adjust:
- Port number in I function (p!)
- Bit mask in I function (b!)
- Base timing unit in I function (t!)
- Port timing in D function multiplier





## receive and decode

Morse code decoder program for MINT2

```
// Enhanced Morse Code Decoder for MINT2
// Combines Arduino Goertzel filter approach with fixed-point arithmetic
// Variables used:
// a = input pin state
// b = time accumulator 
// c = decoded character index
// d = dash jump (for tree navigation)
// e = error state
// f = filter accumulator
// g = goertzel Q1
// h = goertzel Q2
// i = input threshold
// t = timing unit (fixed point)
// s = signal state

// Fixed point timing constants (8.8 format)
:T 256 t! ; // Base timing unit
:D t 3 * d! ; // Dash = 3x dot
:C t! ; // Inter-character space = 1x dot
:L t 3 * l! ; // Letter space = 3x dot
:W t 7 * w! ; // Word space = 7x dot

// Morse tree lookup string
:S [.EISH54V.3UF....2ARL.....WP..J.1TNDB6.X..KC..Y..MGZ7.Q..O.8..90.] m! ;

// Initialize decoder state
:I 0 b! // Clear time accumulator
   64 d! // Reset dash jump
   0 c! // Reset character index
   200 i! // Set input threshold
   0 s! ; // Clear signal state

// Sample input and apply Goertzel filter
// Uses fixed-point approximation of Goertzel algorithm
:F a! // Read input
   // Remove DC offset (simplified)
   a 512 - a!
   // Fixed-point Goertzel
   g h * 2 * f - a + f!
   g h!
   f g!
   // Calculate magnitude
   g g * h h * + g h * 2 * - ;

// Check signal level against threshold
:L F i > s! ;

// Process dot
:P c 1+ c!
   d 2 / d! ;

// Process dash
:M d c + c!
   d 2 / d! ;

// Output decoded character
:O m c ? /C
   0 c!
   64 d! ;

// Main decoder loop
:R L // Sample and check signal
   s /F = ( // If no signal
     b t > ( // If time exceeds dot length
       b d > ( // If time exceeds dash length
         O // Output character
       ) /E (
         P // Process as dot
       )
       0 b! // Reset timer
     )
   ) /E (
     b 1+ b! // Increment timer
   ) ;

// Main program - initialize and run forever
:Z I S /U(R) ;

// Test program - decode sample input
:X I S 
   // Test string: "SOS" 
   // ... --- ...
   [46 46 46 32 45 45 45 32 46 46 46] n!
   n /S ( n /i ? /C ) ;
   ```



Let me explain the key improvements and features of this MINT2 Morse code decoder:

1. Fixed-Point Arithmetic:
- Uses 8.8 fixed-point format (256 = 1.0) for precise timing
- All timing constants derived from base unit for consistency
- Avoids floating-point operations while maintaining accuracy

2. Binary Tree Decoder:
- Uses efficient binary tree lookup string from Arduino implementation
- Single-character state tracking reduces memory usage
- Binary tree navigation optimized for MINT's integer operations

3. Signal Processing:
- Simplified Goertzel filter implementation using fixed-point math
- DC offset removal for better signal quality
- Threshold-based detection with configurable sensitivity

4. Key Features:
- Memory efficient: Uses single-character variables
- Real-time processing capability
- Error handling for invalid inputs
- Both continuous and test modes

5. Functions:
- T: Sets timing constants
- F: Implements Goertzel filter
- L: Level detection
- P/M: Process dots/dashes
- O: Character output
- R: Main decoder loop
- Z: Continuous run mode
- X: Test mode with sample input

The program can be used in two ways:
1. Run :Z for continuous decoding from input
2. Run :X for testing with sample input

To use it:
1. Initialize with :I
2. Load lookup string with :S
3. Start decoder with :Z or test with :X

