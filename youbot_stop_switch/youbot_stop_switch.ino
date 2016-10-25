int buttonPin = 2;
int ledPin = 4;
int counterMax = 0;
int counter;
int stopDelay = 100;
int resetTime = 2000;

bool buttonPressed = false;
bool ledLit = false;

enum states { standBy, triggered, reset };
states state;

void setup() 
{
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  counterMax = resetTime/stopDelay;
  counter = 0;
  state = standBy;
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
}
  
void loop ()
{
  // Just read the button
  if (digitalRead(buttonPin))
  {
    buttonPressed = true;
  }
  else
  {
    buttonPressed = false;
  }

  // Check the state machine
  switch(state)
  {
    // Case, when the button is not pressed and all is good
    case standBy:
    {
      if (buttonPressed)
      {
        state = triggered;
        break;
      }
      delay(5);
    }
    break;

    // Case, when button WAS pressed
    case triggered:
    {
      // Send out the stopping signal
      Serial.println("stop");

      // Turn on the indicator LED
      if (ledLit == false)
        digitalWrite(ledPin, HIGH);

      // If button is held down for 5 seconds while the state is "triggered", the system is reset
      if (buttonPressed)
      {
        if (counter >= counterMax)
        {
          state = reset;
          counter = 0;
          digitalWrite(ledPin, LOW);
          break;
        }
        else
        {
          counter++;
        }

        // Dim the LED when the button is held down
        digitalWrite(ledPin, LOW);
        delay(stopDelay/2);
        digitalWrite(ledPin, HIGH);
        delay(stopDelay/2);
      }
      else
      {
        counter = 0;
        delay(stopDelay);
      }
    }
    break;

    // Case, when button was triggered, then held 5 sec down
    case reset:
    {
      if (buttonPressed == false)
        state = standBy;
        delay(500);
    }
    break;
  }
}
