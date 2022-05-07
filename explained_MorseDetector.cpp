 

// Pin powering the mic pre-amp
// This is the setup for the audio input, the green and red LEDs, and the power for the amplifier.
#define AUDIO_IN_PIN A0
#define LED_GREEN_PIN 2
#define LED_RED_PIN 3
#define AMPLI_PWR_PIN A3



// This defines the sampling rate in samples per second, the length of one dot, and the threshold.
// Sampling rate in samples/s
#define SAMPLING_RATE 5000

// Length of one dot. Assumes constant WPM at the moment
#define DOT_LEN 1
#define THRESHOLD 200



// Precalculated coefficient for our goertzel filter.
float goetzelCoeff=0;

// Length of the goertzel filter
#define GOERTZEL_N 128

// The samples taken from the A/D. The A/D results are 10-bits.
// The array sampledData is being declared as an integer.
int sampledData[GOERTZEL_N];  


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


void setup()
{
  //This function is called once when the program starts. It opens the serial port and sets the data rate to 115200 bits per second.
    Serial.begin(115200); 
  
   
  
  // Set ADC prescaler to 16
  //_SFR_BYTE(ADCSRA) is the ADC control and status register A
  //|= is setting a bit
  //_BV(ADPS2) is the clock prescaler select bits. This sets the bit to a value of 1.
  //The clock prescaler is used to divide the system clock down to a suitable frequency for the ADC. A division factor of 2-128 can be selected.
  
  _SFR_BYTE(ADCSRA) |=  _BV(ADPS2); // Set ADPS2
   
  
  
  
  // This line clears the ADPS1 bit in the ADCSRA register. This register controls the clock prescaling for the ADC.  
  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS1); // Clear ADPS1
  
  
  //The ADCSRA register controls the ADC. This line clears the ADPS0 bit, which sets the ADC clock prescaler.
  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS0); // Clear ADPS0
  

  
    
  //This code sets the pin modes for the green and red LEDs as output, and sets their initial value to LOW.
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  
  
  
  
  
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
  for (int ix = 0; ix < GOERTZEL_N; ix++)
  {
    sampledData[ix] = analogRead(AUDIO_IN_PIN); // 17uS
    delayMicroseconds(233); // total 250uS -> 4KHz
    dcOffset+=sampledData[ix];
  } 
  dcOffset=dcOffset/GOERTZEL_N;
 
 
 
 
 
  // Remove DC offset (center signal around zero).
  // Calculate RMS signal level.
  // rmsLevel is the root mean square level of the signal
  // dcOffset is the mean value of the signal
  // sampledData is an array containing the signal
  // GOERTZEL_N is the number of samples in the signal
  
  int32_t rmsLevel=0;
  for (int ix = 0; ix < GOERTZEL_N; ix++)
  {
    sampledData[ix] -= dcOffset;
    rmsLevel += abs(sampledData[ix]);
  }
  rmsLevel= rmsLevel/GOERTZEL_N;
  
  
  
  
  
  
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
  
  
  
   
  
  // Apply goertzel filter and get amplitude of
  // This is the Goertzel algorithm. Q2 and Q1 represent previous samples, 
  // while Q0 represents the current sample. 
  // The algorithm is used to find the magnitude of a signal.
  
  
  float Q2 = 0;
  float Q1 = 0;
  for (int ix = 0; ix < GOERTZEL_N; ix++)
  {
      float Q0 = goetzelCoeff * Q1 - Q2 + (float) (sampledData[ix]);
      Q2 = Q1;
      Q1 = Q0;  
  }
  
  int magnitude = sqrt(Q1*Q1 + Q2*Q2 - goetzelCoeff*Q1*Q2);
  
  
  
  
  
  //Serial.println(magnitude);


  // This code checks to see if the current status is "none" and 
  // if the magnitude is greater than the THRESHOLD. 
  // If both of these conditions are true, then something is done.
  if(currentStatus==none && magnitude>THRESHOLD)

  // This code sets the value of currentStatus to dot, and sets the value of statusCounter to 0.
  {
    currentStatus=dot;
    statusCounter=0;  
  }
  
  
  // If the current status is a dot and the magnitude is greater than the threshold, 
  // and the status counter is greater than twice the length of a dot, the current status is changed to a dash.
  else if(currentStatus==dot && magnitude>THRESHOLD && statusCounter>DOT_LEN*2)
  {
    currentStatus=dash;
  }
  
  // This code checks if the current status is intersymbol and the magnitude is less than the threshold. 
  // If so, the current status is set to interchar.
  else if(currentStatus==intersymbol && magnitude<THRESHOLD && statusCounter>DOT_LEN*3)
  {
    currentStatus=interchar;
  }
  
  
// The code is checking if the current status is interchar and the magnitude is less than the threshold. 
// If both of those conditions are true, the current status is set to interword.  
  else if(currentStatus==interchar && magnitude<THRESHOLD && statusCounter>DOT_LEN*8)
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
  
  
  
  
  // This is an else if statement. It is saying that if the current status is greater than a dash, 
  // and the magnitude is greater than the THRESHOLD, or the current status is equal to interword, 
  // then do the following. 
  else if(currentStatus>dash && (magnitude>THRESHOLD || currentStatus==interword))
  
  //
  {
    // This line is printing a space if the current status is not equal to intersymbol. 
    //Serial.print((currentStatus==intersymbol)?"":" ");
    
    // This line is setting the LED green pin to low, and the LED red pin to low. 
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    
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
    {
        // This is an increment operator which will add one to the current value of currentDecoderIndex.
        currentDecoderIndex++;
    }
    // This is an else if statement. It means that if the currentMark is equal to '-', then do something.
    else if (currentMark == '-')
    {
        // This is an increment operator that is adding the value of currentDashJump to currentDecoderIndex. 
        currentDecoderIndex += currentDashJump;
    }
    // The else if statement is used to test for multiple conditions. 
    // In this example, the else if statement is testing to see if the currentMark variable is equal to '\0'. 
    // If it is, then the statement will be executed.
    else if (currentMark == '\0')
    {
        // This is a function that returns a null character ('\0'). 
        // The currentDecoderIndex and currentDashJump variables are both set to 64.
        currentDecoderIndex = 0;
        currentDashJump = 64;
        return '\0';
    }
    
    
    return lookupString[currentDecoderIndex];
}
