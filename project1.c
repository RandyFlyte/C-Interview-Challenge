// C++ code
//
#define t_LED 2
#define t_RED 5
#define t_D13 13
#define t_EOL 255
#define t_ON 7
#define MAX_BUF_SIZE 64 // maximum buffer size
#define MAX_INPUT_SIZE 64 // maximum input string size
#define MAX_TOKENS 16 // maximum number of tokens
#define MAX_TOKEN_SIZE 16 // maximum token size

char inputBuffer[MAX_INPUT_SIZE]; // input string buffer
int inputIndex = 0; // input string buffer index

uint8_t tokenBuffer[MAX_TOKENS]; // token buffer
int tokenIndex = 0; // token buffer index
const uint8_t token_lookup[] = {
  'l', 'e', 3, t_LED,
  'r', 'e', 3, t_RED,
  'e', 'o', 3, t_EOL,
  'd', 1, 5, t_D13,
  'o', 'n', 2, t_ON
};

void runCommand() {
  switch (tokenBuffer[0]) {
    case t_LED:
      switch (tokenBuffer[1]) {
        case t_RED:
       //   turnLedRed(); // example function to turn the LED red
          break;
        default:
          // handle unrecognized command
          break;
      }
      break;
    case t_D13:
      switch (tokenBuffer[1]) {
        case t_ON:
          digitalWrite(LED_BUILTIN, HIGH);
          break;
        default:
          break;
      }
    default:
      // handle unrecognized command
      break;
  }
}

void parseInput(char *input) {
  char *token;
  token = strtok(input, " "); // get the first token
  while (token != NULL && tokenIndex < MAX_TOKENS) {
    // search for the token in the lookup table
    for (int i = 0; i < sizeof(token_lookup); i += 4) {
      if (strncmp(token, (char *)&token_lookup[i], 2) == 0 &&
          strlen(token) == token_lookup[i+2]) {
        tokenBuffer[tokenIndex++] = token_lookup[i+3]; // add the token to the buffer
        break;
      }
    }
    token = strtok(NULL, " "); // get the next token
  }
  tokenBuffer[tokenIndex++] = t_EOL; // add the EOL token to the end of the buffer
}

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) { // check if there is data available to read
    char c = Serial.read(); // read in one character
    if (c == '\r') { // check if the enter key was pressed (ASCII 13 or '\r')
      inputBuffer[inputIndex] = '\0'; // terminate the input string with a null character
      Serial.println("Input received:"); // print message to serial monitor
      Serial.println(inputBuffer); // print the input string to serial monitor
      parseInput(inputBuffer); // parse the input string and add tokens to buffer
      // send the token buffer off for processing here
      runCommand();
      inputIndex = 0; // reset the input buffer index for next input
      tokenIndex = 0; // reset the token buffer index for next input
    }
    else if (inputIndex < MAX_INPUT_SIZE-1) { // check if buffer is not full
      inputBuffer[inputIndex++] = c; // add the character to the buffer and increment index
    }
  }
}
