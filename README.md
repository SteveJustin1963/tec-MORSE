# tec-MORSE
TEC-1 morse code send and receive and decode

Here's how the improved code works:

Morse code timing units are represented using fixed-point integer arithmetic. For instance, a dot is represented by 256 instead of 1.

Similarly, dash, char-space, letter-space, and word-space are also represented using integer fixed-point arithmetic.

An error state (error) is introduced, represented by 9999.

The init word initializes a time accumulator to zero.

The add-time word adds a unit of time to the accumulator.

decode-char takes a string address and a length, iterating through each character to decode and accumulate the timing. This includes basic fault tolerance by setting the stack to error if an unrecognized character is encountered.

Finally, decode is the main loop that iterates through each Morse code character in a string and decodes it using decode-char.

This is a simplified, yet somewhat fault-tolerant, version. It is not complete in terms of translating Morse to text but it does provide a structured way to accumulate the Morse code timing based on the characters read. A full-fledged system would require additional functionality for error correction, translating the timing to actual letters, and handling various edge cases.

