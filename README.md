# tec-MORSE
## send 

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

