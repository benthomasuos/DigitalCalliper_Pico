#define clkpin 10
#define datpin 9


int now = 0; // time now in microseconds
int start = 0; // time for start of pulse
int previous_read_t = 0; // time of the last reading

int delta_clk_t = 0; // time between clock interrupts
int delta_read_t = 0; // time between position reads
int delta_position = 0; // Change in position between reads
int position = 0; // position in 10 microns resolution, native for the digital scale
int position_um = 0; // position in micron resolution
int previous_position_um = 0; // Previously read position
byte pulse_idx = 0; // track how many pulses have occurred

void setup() {
  
  pinMode(clkpin, INPUT);
  pinMode(datpin, INPUT);



  attachInterrupt(digitalPinToInterrupt(clkpin), pulseISR, RISING); // Detect rising edges on the clock pin

  Serial.begin(115200);
}


void loop() {
  // Nothing needs to run here as the system runs on the interrupts from the clock pin

}


void pulseISR(){

  int now = micros(); // update time NOW
  
  int data_val = digitalRead(datpin); // Read the current value of the data pin (0-1)

  delta_clk_t = now - start; // time between this clock pulse and now
  Serial.println(delta_clk_t);
  start = now; // Make sure to save this time in start as the previous clock pulse time 
  
  if(delta_clk_t > 5000){
    // If the time between clock pulses is larger than 5000 us then we are
    // in between position reads so we need to reinitialise the reading
    position = 0; // Reinitialise this
    position_um = 0; // Reinitialise this
    pulse_idx = 0; // Reinitialise this
  }



  if(pulse_idx < 20){ // Fill up all the position bits as we move through the first 20 clock pulses
      if(data_val == 1){
        bitSet(position, pulse_idx); // Sets the bit at index = pulse_idx to be 1
      }
  }
  else if(pulse_idx == 20){
    // Deal with the negative bit
    position_um = position * 10; // convert the position into a micron value
    if(data_val == 1){ // If this bit is HIGH then we have a negative number
      position_um *= -1; // Use compound multiplication to negate the position_um value
    }
  }
  else if(pulse_idx == 23){
    // This is the last pulse so we can write it out
    delta_read_t = now - previous_read_t; // time between the last reading and now in microseconds
    previous_read_t = now; // save this value for the next read
    delta_position = position_um - previous_position_um; // Calculate the change in position between now and the last read
    previous_position_um = position_um; // Save the current value for the next read
    
    long speed = delta_position / delta_read_t; // calculate the movement speed between the previous two readings
    
    // Print out the results
    Serial.print(position_um); // Print out the value
    Serial.print(" ");
    Serial.print(speed); 
    Serial.println(); 

    position = 0; // Reinitialise this
    position_um = 0; // Reinitialise this
    pulse_idx = 0; // Reinitialise this

  }


}