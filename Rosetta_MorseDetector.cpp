 

// Pin powering the mic pre-amp
// This is the setup for the audio input, the green and red LEDs, and the power for the amplifier.
#define AUDIO_IN_PIN A0
#define LED_GREEN_PIN 2
#define LED_RED_PIN 3
#define AMPLI_PWR_PIN A3


// forth 
: AUDIO-IN-PIN  A0 ;
: LED-GREEN-PIN 2  ;
: LED-RED-PIN   3  ;
: AMPLI-PWR-PIN A3 ;
 : SAMPLING_RATE 5000 ;
: DOT_LEN 1 ;
: THRESHOLD 200 ;



/////////////////////////////////////////////////////////////
// Precalculated coefficient for our goertzel filter.
float goetzelCoeff=0;

 for (int i=0;i<83;i++)
    {
    // Goertzel filter
    float sine=sin(2*3.14159*i*goetzelCoeff);
    float cosine=cos(2*3.14159*i*goetzelCoeff);
    float goertzel = sine*sine + cosine*cosine;
    
    // Output the result
    std::cout << goertzel << std::endl;
    }
//////////////////////////

// forth
 : for-goertzel ( n -- )
   BEGIN
     dup 0<> WHILE
       3.14159 2* goetzelCoeff * cosine!
       3.14159 2* goetzelCoeff * sine!
       sine@ sine@ + cosine@ cosine@ +
       sine@ cosine@ - cosine@ sine@ -
       goertzel !
       goertzel @ .
       1+
     REPEAT
     drop
   END ;




// This defines the sampling rate in samples per second, the length of one dot, and the threshold.
// Sampling rate in samples/s
#define SAMPLING_RATE 5000

// Length of one dot. Assumes constant WPM at the moment
#define DOT_LEN 1
#define THRESHOLD 200

// forth
: SAMPLING_RATE 5000 ;
: DOT_LEN 1 ;
: THRESHOLD 200 ;



// Precalculated coefficient for our goertzel filter.
float goetzelCoeff=0;

// forth
goetzelCoeff := 2.0*cos(2.0*pi*(k/N));



// Length of the goertzel filter
#define GOERTZEL_N 128

// forth
: GOERTZEL_N 128
 
// or
: goertzel 
   128 GOERTZEL_N !
;



// The samples taken from the A/D. The A/D results are 10-bits.
// The array sampledData is being declared as an integer.
int sampledData[GOERTZEL_N];  

// forth
: sampledData ( -- addr )  
  GOERTZEL_N @ sampledData + ;  


//This is an enum that defines six different statuses that a character can have. 
// none = 0, This means that the character has no status. 
// dot = 1, This means that the character is a dot. 
// dash = 2, This means that the character is a dash. 
// intersymbol = 3, This means that the character is between two symbols. 
// interchar = 4, This means that the character is between two characters. 
// interword = 5, This means that the character is between two words.
enum statuses
{
    none =0,
    dot,
    dash,
    intersymbol,
    interchar,
    interword
};

// forth
: enum-statuses ( n -- )
   case
     0 of none endof
     1 of dot endof
     2 of dash endof
     3 of intersymbol endof
     4 of interchar endof
     5 of interword endof
   endcase ;

 


// This is a function that takes in a string and outputs the corresponding decoded message. 
// The first line initializes a variable called "currentStatus" to "none".
// The second line initializes a variable called "statusCounter" to 0.
// The third line sets a pointer called "lookupString" to the string ".EISH54V.3UF....2ARL.....WP..J.1TNDB6.X..KC..Y..MGZ7.Q..O.8..90.".
// The fourth line sets a variable called "currentDecoderIndex" to 0.
// The fifth line sets a variable called "currentDashJump" to 64.
// The sixth line sets a variable called "currentAssumedChar" to '\0'.

statuses currentStatus=none;

int statusCounter=0;

char* lookupString = ".EISH54V.3UF....2ARL.....WP..J.1TNDB6.X..KC..Y..MGZ7.Q..O.8..90.";
byte currentDecoderIndex = 0;
byte currentDashJump = 64;
char currentAssumedChar='\0';

// forth
: none 0 ;
: statusCounter 0 ;
: lookupString 64 chars allot ;
: currentDecoderIndex 0 ;
: currentDashJump 64 ;
: currentAssumedChar char 0 ;


void setup()
{
  //This function is called once when the program starts. It opens the serial port and sets the data rate to 115200 bits per second.
    Serial.begin(115200); 
// forth 
 ???
   
  
  // Set ADC prescaler to 16
  //_SFR_BYTE(ADCSRA) is the ADC control and status register A
  //|= is setting a bit
  //_BV(ADPS2) is the clock prescaler select bits. This sets the bit to a value of 1.
  //The clock prescaler is used to divide the system clock down to a suitable frequency for the ADC. A division factor of 2-128 can be selected.
  
  _SFR_BYTE(ADCSRA) |=  _BV(ADPS2); // Set ADPS2
   
 // forth
 ADCSRA |=  8 ;
  
  
  
  // This line clears the ADPS1 bit in the ADCSRA register. This register controls the clock prescaling for the ADC.  
  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS1); // Clear ADPS1
 
 //forth 
 ??
  
  
  //The ADCSRA register controls the ADC. This line clears the ADPS0 bit, which sets the ADC clock prescaler.
  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS0); // Clear ADPS0
   
 //forth 
 ??

  
    
  //This code sets the pin modes for the green and red LEDs as output, and sets their initial value to LOW.
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  
// forth
 : LED_GREEN_PIN OUTPUT !
: LED_RED_PIN OUTPUT !
: LOW 0
LED_GREEN_PIN LOW !
LED_RED_PIN LOW !
 
  
  
  
// Power up the amplifier.
// The goertzel filter needs two samples to "warm up" before it will begin giving accurate results.  
// So we throw away the first two samples.
// for (int i=0; i<2; i++)
//   updateGoertzel(analogRead(AUDIO_IN_PIN));
//  This code powers up the amplifier and initializes the goertzel filter.

pinMode(AMPLI_PWR_PIN, OUTPUT);
  digitalWrite(AMPLI_PWR_PIN,HIGH);
  
  float omega = (2.0 * PI * 700) / SAMPLING_RATE;
  goetzelCoeff = 2.0 * cos(omega);
}

// forth
:pinMode     pinMode(     ;
:digitalWrite     digitalWrite(     ;
:float     float     ;
:omega     =     ;
:goetzelCoeff     =     ;


void loop()
{
 
 
 // Sample at 4KHz
 // This code is creating a Goertzel filter to sample at 4KHz. 
 // The dcOffset variable is used to store the DC offset of the signal. 
 // The for loop reads in GOERTZEL_N samples from the audio input pin, 
 // with a delay of 233 microseconds between each sample. 
 // The total delay between samples is 250 microseconds, which gives a sampling rate of 4KHz. 
 // After the samples are read, the dcOffset variable is divided by GOERTZEL_N to get the average DC offset of the signal.
 
  int32_t dcOffset=0;
 // forth :dcOffset int32_t 0 ;
 
  for (int ix = 0; ix < GOERTZEL_N; ix++)
  // forth
   : for-loop ( -- )
   GOERTZEL_N 0 ?do
      ix @ GOERTZEL_N <
   while ;
   
  {
    sampledData[ix] = analogRead(AUDIO_IN_PIN); // 17uS
    delayMicroseconds(233); // total 250uS -> 4KHz
    dcOffset+=sampledData[ix];
 // forth
   : read-adc-4khz ( addr -- )
    AUDIO_IN_PIN @ analog-read
    sampled-data I !
    233 microseconds-delay
    250 microseconds
;
  
  
  } 
  dcOffset=dcOffset/GOERTZEL_N;
 
 // forth ??
 
 
 
  // Remove DC offset (center signal around zero).
  // Calculate RMS signal level.
  // rmsLevel is the root mean square level of the signal
  // dcOffset is the mean value of the signal
  // sampledData is an array containing the signal
  // GOERTZEL_N is the number of samples in the signal
  
  int32_t rmsLevel=0;
 // forth 
 rmsLevel 0 int32_t !
 
  for (int ix = 0; ix < GOERTZEL_N; ix++)
  // forth
   
: for-loop ( ix -- )
   GOERTZEL_N < if
      ix GOERTZEL_N < if
         ix ++
         recursive-call  ( tail-recursion optimization )
      else
         exit-for-loop
      then
   then
;
 
   
   
  {
    sampledData[ix] -= dcOffset;
    rmsLevel += abs(sampledData[ix]);
 //forth   
  
: sampledData[ix] - dcOffset sampledData[ix] ! ;
: rmsLevel +abs sampledData[ix] @ ;
   
   
  }
  rmsLevel= rmsLevel/GOERTZEL_N;
 //forth
    : rmsLevel GOERTZEL_N / ;
  
  
  
  
  
  // AGC scale to have RMS at about 100
  // This code is looping through the data and scaling it by a factor of 100/rmsLevel. 
  // If the rmsLevel is less than 5, the data is set to 0.
  
  for (int ix = 0; ix < GOERTZEL_N; ix++)
  {
    if(rmsLevel>5)
    {
      sampledData[ix] = sampledData[ix] * (100.0f/rmsLevel);
    }
    else
    {
      sampledData[ix] = 0;
    }
  }
  
 // forth
 : for-loop  GOERTZEL_N  sampledData  rmsLevel  5  >  if  100.0f  rmsLevel  /  sampledData  *  else  0  then  next  ;
 
  
   
  
  // Apply goertzel filter and get amplitude of
  // This is the Goertzel algorithm. Q2 and Q1 represent previous samples, 
  // while Q0 represents the current sample. 
  // The algorithm is used to find the magnitude of a signal.
  
  
  float Q2 = 0;
  float Q1 = 0;
 
 // forth
 : Q2   0 ;
: Q1   0 ;
 
 
  for (int ix = 0; ix < GOERTZEL_N; ix++)
 // forth
   :for ( ix -- ) GOERTZEL_N 0 > over ix < and if ix GOERTZEL_N + loop ;
  
  {
      float Q0 = goetzelCoeff * Q1 - Q2 + (float) (sampledData[ix]);
      Q2 = Q1;
      Q1 = Q0;  
 // forth
   
: goetzelCoeff 

    Q1 @ - Q2 + sampledData ix@ + float f! 
    Q2 ! 
    Q1 ! 
;
   
   
  }
  
  int magnitude = sqrt(Q1*Q1 + Q2*Q2 - goetzelCoeff*Q1*Q2);
  
  
// forth
: magnitude ( -- ) 
    Q1 Q2 * goetzelCoeff * - 
    Q1 Q1 * Q2 Q2 * + 
    sqrt 
;
  
  
  
  //Serial.println(magnitude);


  // This code checks to see if the current status is "none" and 
  // if the magnitude is greater than the THRESHOLD. 
  // If both of these conditions are true, then something is done.
  if(currentStatus==none && magnitude>THRESHOLD)
  // forth
     : check-if-something-was-done
  currentStatus @ none = if
    magnitude @ THRESHOLD @ > if
      read-word
      execute-command
    then
  then
;

  

  // This code sets the value of currentStatus to dot, and sets the value of statusCounter to 0.
  {
    currentStatus=dot;
    statusCounter=0;  
  }
  
  
  // If the current status is a dot and the magnitude is greater than the threshold, 
  // and the status counter is greater than twice the length of a dot, the current status is changed to a dash.
  else if(currentStatus==dot && magnitude>THRESHOLD && statusCounter>DOT_LEN*2)
  // forth 
   : currentStatus  @  dot = if
  : magnitude      @  THRESHOLD > if
    : statusCounter @  DOT_LEN * 2 > if
      ...
    then 
  then
then
   
   
  {
    currentStatus=dash;
  }
  
  // This code checks if the current status is intersymbol and the magnitude is less than the threshold. 
  // If so, the current status is set to interchar.
  else if(currentStatus==intersymbol && magnitude<THRESHOLD && statusCounter>DOT_LEN*3)
   
  // forth
        : main
     THRESHOLD 3       DOT_LEN 3
     currentStatus 0   magnitude 0
     statusCounter 0
     currentStatus 0 magnitude THRESHOLD > if
          ." do something" cr
     else currentStatus 1 magnitude THRESHOLD > and statusCounter DOT_LEN <= if
          ." do something" cr
     else currentStatus 1 magnitude THRESHOLD < and statusCounter DOT_LEN > if
          ." do something" cr
     else currentStatus 2 magnitude THRESHOLD > and statusCounter DOT_LEN * 3 <= if
          ." do something" cr
     else currentStatus 2 magnitude THRESHOLD < and statusCounter DOT_LEN * 3 > if
          ." do something" cr
     else currentStatus 3 magnitude THRESHOLD > and statusCounter DOT_LEN <= if
          ." do something" cr
     else currentStatus 3 magnitude THRESHOLD < and statusCounter DOT_LEN > if
          ." do something" cr
     then then then then then then ;
 
 
   
  {
    currentStatus=interchar;
  }
  
  
// The code is checking if the current status is interchar and the magnitude is less than the threshold. 
// If both of those conditions are true, the current status is set to interword.
 //   this is the inter-character space.  We wait here until we see 8 more character periods 
 // before we start looking for the next character.  This should give us plenty of time to get a good reading on the next character.
  else if(currentStatus==interchar && magnitude<THRESHOLD && statusCounter>DOT_LEN*8)
  
// forth
   : inter-character-space ( -- )
     status @ interchar < threshold @ and if
        status @ interword !
     then
;
   
   
   
   
  {
    currentStatus=interword;
  }
  
  
  
// This section of code is checking to see if the currentStatus is not equal to "none" 
// and if it is less than "intersymbol". 
// If those conditions are met, it will execute the code inside of the curly braces. 
// This code will turn on either the green or red LED depending on if the currentStatus is equal to "dot" or not. 
// It will also set the value of currentAssumedChar to the character that is looked up from the table.  
  else if(currentStatus!=none && currentStatus<intersymbol && magnitude<THRESHOLD)
  {
    //Serial.print((currentStatus==dot)?".":"-");
    digitalWrite((currentStatus==dot)?LED_GREEN_PIN:LED_RED_PIN,HIGH);
    currentAssumedChar=lookup((currentStatus==dot)?'.':'-');
    currentStatus=intersymbol;
    statusCounter=0;
    //currentStatus=none;
  }
 
 // forh
 : loop    ; start of loop
  begin    ; loop until key pressed
    key    ; get a key
    if zero= then  ; if no key pressed
      continue   ;   go back to start of loop
    then
    if currentStatus!=none && currentStatus<intersymbol && magnitude<THRESHOLD  ; else if(currentStatus!=none && currentStatus<intersymbol && magnitude<THRESHOLD)
    {
      Serial.print((currentStatus==dot)?".":"-");
      digitalWrite((currentStatus==dot)?LED_GREEN_PIN:LED_RED_PIN,HIGH);
      currentAssumedChar=lookup((currentStatus==dot)?'.':'-');
      currentStatus=intersymbol;
      statusCounter=0;
      //currentStatus=none;
    }
  again ; go back to start of loop
  ;
: lookup  ( -- c )     ; lookup function
  0= if '0' exit then
  1= if '1' exit then
  2= if '2' exit then
  3= if '3' exit then

 
  
  
  
  // This is an else if statement. It is saying that if the current status is greater than a dash, 
  // and the magnitude is greater than the THRESHOLD, or the current status is equal to interword, 
  // then do the following. 
  else if(currentStatus>dash && (magnitude>THRESHOLD || currentStatus==interword))
 
 // foth
   
: main
    start
    if currentStatus>dash and (magnitude>THRESHOLD or currentStatus=interword) then
        outputStatus
    then
;
   
  
  //
  {
    // This line is printing a space if the current status is not equal to intersymbol. 
    //Serial.print((currentStatus==intersymbol)?"":" ");
    
    // This line is setting the LED green pin to low, and the LED red pin to low. 
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
   
  // forth
   
: BEGIN
   LED_GREEN_PIN low
   LED_RED_PIN low
;
   
    
    // This is saying that if the current status is equal to interchar, 
    // then print the current assumed character. Else if the current status is equal to interword, 
    // then print the current assumed character and a space. Set the current status to none. 
    if(currentStatus==interchar)
     
    {
      Serial.print(currentAssumedChar);
      lookup('\0');
    }
    else if(currentStatus==interword)
    {
      Serial.print(currentAssumedChar);
      Serial.print(" ");
      lookup('\0');
    }
    
   
    currentStatus=none;
   // forth
   
   : lookup( address -- c-addr u | xt )
    
    Tries to find the word at the given address in the dictionary. If found,
the execution token of the word is returned along with the length of the
found word. Otherwise, zero is returned.
   
   
   
  }
// This line is incrementing statusCounter by 1.  
  statusCounter++;
}




// This is a function that takes in a character input and outputs a character. 
// The function uses a lookup table to find the corresponding character for the input character. 
// The lookup table is stored in an array called lookupString.
char lookup(char currentMark)
{
    // The code is dividing the currentDashJump value by 2.0f and then converting it to an integer value (floor). 
    // If the currentMark value is a '.', then the code does nothing.
    currentDashJump = floor(currentDashJump / 2.0f);
    if (currentMark == '.')
   // forth
     : currentDashJump 2/ floor ; : currentMark . # $ % . ;
     
     
    {
        // This is an increment operator which will add one to the current value of currentDecoderIndex.
        currentDecoderIndex++;
    }
    // This is an else if statement. It means that if the currentMark is equal to '-', then do something.
    else if (currentMark == '-')
    // forth
     : mark-if-empty   \ mark current position if empty
       dup "-" = if
       "X"
       then
     ;
     
    {
        // This is an increment operator that is adding the value of currentDashJump to currentDecoderIndex. 
        currentDecoderIndex += currentDashJump;
    // forth
     //This code increments the current decoder index by the value of the current dash jump.
     // a+= b;
 
 
     
     
     
     
    }
    // The else if statement is used to test for multiple conditions. 
    // In this example, the else if statement is testing to see if the currentMark variable is equal to '\0'. 
    // If it is, then the statement will be executed.
    else if (currentMark == '\0')
     // forth
     cm 0=()
     
    {
        // This is a function that returns a null character ('\0'). 
        // The currentDecoderIndex and currentDashJump variables are both set to 64.
        currentDecoderIndex = 0;
        currentDashJump = 64;
        return '\0';
    }
    
    
    return lookupString[currentDecoderIndex];
}
