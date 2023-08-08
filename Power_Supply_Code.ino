//Power supply code
double kp = 2;                          //PID constants
double ki = 5;
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output;
double setPoint = 100;
double cumError, rateError;
int out1 = 9;
int out2 = 10;
int weldTime = 7;
int startWeld = 4;
int Vcomp = A0;                          //Initializing pins
void setup() {                           //Declaring output and input
pinMode(out1, OUTPUT);
pinMode(out2, OUTPUT);
pinMode(startWeld, INPUT);
pinMode(weldTime, INPUT);
pinMode(Vcomp, INPUT);                  //Define Variables we'll be connecting to
Serial.begin(9600);                    // opens serial port, sets data rate to 9600 bps
}

double computePI(double inp)
{     
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from     previous computation
        
        error = setPoint - inp;                             // determine error
        cumError += error * elapsedTime;                   // compute integral
        double out = kp*error + ki*cumError;              //PI output               
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
        return out;                                        //have function return the PI output
}

void invertAnalogWrite(int pin, int value)
{
   analogWrite(pin, value);
  TCCR1A = TCCR1A & ~B00110000;             //switch off output B
  TCCR1A |= B00110000;                      //switch on the B output with inverted output
}

void loop() {

int startWeld = 0;
int weldTime = 0;
Serial.println("Enter a weld time");
while(!Serial.available()){}
weldTime = Serial.parseInt();
Serial.println("Enter 1 to start welding");
while(!Serial.available()){}
startWeld = Serial.parseInt();

 while (startWeld == 1) {       //while the start value is found, start weld
	int period = millis();
	while(millis() - period <= weldTime)
		{                //while the timer is less than the weld time

                	Serial.println("lol");
                	analogWrite(out1, 25.5);               //make out1 pulse with a 10% Duty
                	invertAnalogWrite(out2,25.5); //make out2 invert the signal being sent with 90% duty cycle (1-D)
                	input = analogRead(A0);                //read from rotary encoder connected to A0
                	output = computePI(input); 
                	analogWrite(out1, output);               //make out1 pulse with a 10% Duty
                	invertAnalogWrite(out2,output); //make out2 invert the signal being sent with 90% duty cycle (1-D)
                	startWeld = 0;
                } 
          
    	weldTime = 0;
	analogWrite(out1, 0);               //make out1 pulse with a 10% Duty
   	invertAnalogWrite(out2,0);
             
	}
}
