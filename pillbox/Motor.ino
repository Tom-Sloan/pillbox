
void spotToOpen(byte row, byte coln){
  if (isServo){
    myservo.write(positions_servo[coln]);  
  }else{
    int current_post = positions_stepper[row];
    int destination = finalPositions[coln];

    if (current_post > destination){
      runStepper(row, current_post - destination, false);
    }else{
      runStepper(row, current_post - destination, true);
    }
  }
}

void lockedPosition(byte row){
  if (isServo){
    myservo.write((positions_servo[1]+positions_servo[2])/2);   
  }else{
    int current_post = positions_stepper[row];
    int destination = (finalPositions[1] + finalPositions[2])/2;
    
    if (current_post > destination){
      runStepper(row, current_post - destination, false);
    }else{
      runStepper(row, current_post - destination, true);
    }
  }
}

//open all
void openAllRow(byte row){
  if (isServo){
    myservo.write(positions_servo[7]);   
  }else{
    runStepper(row, finalPositions[7] + 100, true);
  }
}

void runStepper(byte row, int numSteps, bool forward){
   if (forward){
    ioex[row].digitalWrite(dirctn, LOW); 
    positions_stepper[row] += numSteps;
   }else{
    ioex[row].digitalWrite(dirctn, HIGH); 
    positions_stepper[row] -= numSteps;
   }
   ioex[row].digitalWrite(EN, LOW);
   for (int x = 0; x < numSteps; x++) //Loop the stepping enough times for motion to be visible
    {
      ioex[row].digitalWrite(stp2, HIGH); //Trigger one step
      digitalWrite(stp, HIGH); 
      delay(1);
      ioex[row].digitalWrite(stp2, LOW); //Pull step pin low so it can be triggered again
      digitalWrite(stp, LOW); 
      delay(1);
    }
    ioex[row].digitalWrite(EN, HIGH);
    
    if (positions_stepper[row] < finalPositions[0]){
      positions_stepper[row]=finalPositions[0];
    }else if (positions_stepper[row] > finalPositions[7]){
      positions_stepper[row]=finalPositions[7];
    }
}
//Default microstep mode function
void StepForwardDefault()
{
  
  Serial.println("Moving forward at default step mode.");
  runStepper(0, 1000, true);
  Serial.println("Enter new option");
  Serial.println();
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
  runStepper(0, 1000, false);
  Serial.println("Enter new option");
  Serial.println();
}
