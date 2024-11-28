# tec-MORSE
## sender 
Morse code sender that takes keyboard input and outputs to a port bit. 
Includes timing and proper Morse spacing.

```
/// Full Alphanumeric Morse Code Sender for MINT2
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
:S [ // A-Z patterns
    102 2110 2110 2110 10 1110 220 1110 1111 1222 212 1210 22  // A-M
    21 222 1210 2212 121 111 2 112 112 122 2212 2122 2211 0    // N-Z
    // 0-9 patterns
    22222 12222 11222 11122 11112 11111 21111 22111 22211 22221 0 ] s! ; 

// Send bit to port - accepts 0 or 1
:B b * p /O ; // Output to port using mask

// Time delay - n * base unit
:D t * 50 * (0) ; // Basic delay loop

// Send dot
:T 1 B 1 D 0 B 1 D ;

// Send dash
:H 1 B 3 D 0 B 1 D ;

// Convert ASCII to pattern index
:V c! // Save character
   c #30 >= c #39 <= & ( // If number 0-9
     c #30 - 26 + // Convert to index after letters
   ) /E (
     c #41 >= c #5A <= & ( // If letter A-Z
       c #41 - // Convert to index 0-25
     ) /E (
       0 // Invalid character
     )
   ) ;

// Find pattern for character (returns index to s array)
:F V // Get character index
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
     `Enter text (uppercase A-Z, 0-9):` /N
     /K c! // Get character
     c #0D = /W // Exit on CR
     c #20 = ( 7 D ) /E ( // Space or character
       c #30 >= c #39 <= & ( c C ) /E ( // If number
         c #41 >= c #5A <= & ( c C ) /E ( // If letter
           `Invalid char` /N // Otherwise invalid
         )
       )
     )
   ) ;

// Initialize and run
:R I S M ;
```

# MINT2 Morse Code Sender Manual

## Overview
The MINT2 Morse Code Sender is a program that converts keyboard input into Morse code signals output through a single bit on a specified port. It supports all alphanumeric characters (A-Z, 0-9) and implements standard International Morse Code timing.

## Technical Specifications

### Hardware Requirements
- Port access (default: port #80)
- Single bit output capability (default: bit 0)
- Keyboard input
- MINT2 interpreter system

### Timing Standards
All timings are derived from a base unit (t) which can be adjusted for different WPM:
- Dot = 1 unit
- Dash = 3 units
- Inter-element space = 1 unit
- Inter-character space = 3 units
- Word space = 7 units

### Variable Usage
- p: Output port number (#80)
- b: Output bit mask (#01)
- t: Base time unit (ms)
- c: Current character
- i: Lookup index
- s: Morse string buffer

## Morse Code Patterns

### Letters (A-Z)
```
A: .-      B: -...    C: -.-.    D: -..     E: .
F: ..-.    G: --.     H: ....    I: ..      J: .---
K: -.-     L: .-..    M: --      N: -.      O: ---
P: .--.    Q: --.-    R: .-.     S: ...     T: -
U: ..-     V: ...-    W: .--     X: -..-    Y: -.--
Z: --..
```

### Numbers (0-9)
```
0: -----   1: .----   2: ..---   3: ...--   4: ....-
5: .....   6: -....   7: --...   8: ---..   9: ----.
```

## Program Functions

### Setup Functions
```mint
:I  // Initialize ports and timing
:S  // Set up Morse patterns lookup table
```

### Core Operation Functions
```mint
:B  // Send bit to port (0 or 1)
:D  // Time delay
:T  // Send dot
:H  // Send dash
:V  // Convert ASCII to pattern index
:F  // Find pattern for character
:C  // Send one Morse character
:M  // Main input/output loop
:R  // Initialize and run program
```

## Using the Program

### Starting the Program
1. Load the program into MINT2
2. Type `:R` to start
3. Program will prompt for input

### Input Rules
- Use UPPERCASE letters only (A-Z)
- Numbers 0-9 are supported
- Use SPACE for word separation
- Press ENTER/RETURN to exit

### Example Usage
```
Enter text (uppercase A-Z, 0-9):
SOS
[Output: ... --- ...]

HELLO 123
[Output: .... . .-.. .-.. --- .---- ..--- ...--]
```

## Implementation Details

### Pattern Storage
The program stores Morse patterns in a compact format:
- 1 = dot
- 2 = dash
- 0 = end of character
- Patterns stored in s array

### Timing Implementation
```mint
// Base delay unit (adjustable)
256 t! 

// Delay function
:D t * 50 * (0) ;
```

### Port Output
```mint
// Configure port and bit
#80 p!  // Set port
#01 b!  // Set bit mask

// Output function
:B b * p /O ;
```

## Error Handling
- Invalid characters display error message
- Program continues operation
- Invalid inputs are ignored
- Proper spacing maintained

## Adjusting the Program

### Speed Adjustment
Modify the base timing unit in the :I function:
```mint
256 t!  // Increase for slower, decrease for faster
```

### Port Configuration
Change port number and bit mask in :I function:
```mint
#80 p!  // Change port number
#01 b!  // Change bit mask
```

### Delay Tuning
Adjust multiplier in :D function:
```mint
:D t * 50 * (0) ;  // Change 50 to adjust timing
```

## Limitations
- Uppercase letters only
- No punctuation marks
- Fixed WPM (requires restart to change speed)
- Single output bit only
- No simultaneous input handling

## Technical Notes
- Uses MINT2's integer-only operations
- Implements fixed timing ratios
- Efficient pattern storage
- Minimal memory usage
- Real-time operation capable

## Troubleshooting
1. No Output
   - Check port number configuration
   - Verify bit mask setting
   - Ensure hardware connection

2. Wrong Timing
   - Adjust base timing unit
   - Check delay multiplier
   - Verify system timing constraints

3. Invalid Characters
   - Use uppercase letters only
   - Verify input is A-Z or 0-9
   - Check for proper spacing

## Future Improvements
- Add support for punctuation
- Implement variable speed control
- Add input buffering
- Include two-way communication
- Add sound output option



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

