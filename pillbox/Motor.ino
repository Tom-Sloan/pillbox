
void spotToOpen(byte row, byte coln){
  
}

void lockedPosition(byte row){
  
}

//Default microstep mode function
void StepForwardDefault()
{
  Serial.println("Moving forward at default step mode.");
  ioex[0].digitalWrite(dirctn, LOW); //Pull direction pin low to move "forward"
  for (int x = 0; x < 1000; x++) //Loop the forward stepping enough times for motion to be visible
  {
    ioex[0].digitalWrite(stp, HIGH); //Trigger one step forward
    delay(1);
    ioex[0].digitalWrite(stp, LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
  ioex[0].digitalWrite(dirctn, HIGH); //Pull direction pin high to move in "reverse"
  for (int x = 0; x < 1000; x++) //Loop the stepping enough times for motion to be visible
  {
    ioex[0].digitalWrite(stp, HIGH); //Trigger one step
    delay(1);
    ioex[0].digitalWrite(stp, LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}
