
void spotToOpen(byte row, byte coln){
  if (isServo){
    myservo.write(positions_servo[coln]);  
  }else{
    int current_post = positions_stepper[row];
    int destination = finalPositions[coln];

    if (current_post > destination){
      runStepper(row, current_post - destination, false);
    }else{
      runStepper(row, destination - current_post, true);
    }
  }
}

void lockedPosition(byte row){
  Serial.println("Setting Locked row");
  
  if (isServo){
    myservo.write((positions_servo[1]+positions_servo[2])/2);   
  }else{
    int current_post = positions_stepper[row];
    Serial.println(current_post);
    int destination = (finalPositions[1] + finalPositions[2])/2;
    Serial.println(destination);
    if (current_post > destination){
      runStepper(row, current_post - destination, false);
    }else{
      runStepper(row, destination - current_post, true);
    }
  }
}

//open all
void openAllRow(byte row){
  if (isServo){
    myservo.write(positions_servo[7]);   
  }else{
    runStepper(row, finalPositions[7] + 100, true);
    positions_stepper[row] = finalPositions[7];
    Serial.println(row);
    Serial.print("Setting stepper to");
    Serial.println(finalPositions[7]);
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
   ioex[row].digitalWrite(slp, HIGH);
   ioex[row].digitalWrite(EN, LOW);
   for (int x = 0; x < numSteps; x++) //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp, HIGH); 
      delay(1);
      digitalWrite(stp, LOW); 
      delay(1);
    }
    ioex[row].digitalWrite(EN, HIGH);
    ioex[row].digitalWrite(slp, LOW);
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
  if(isServo){
    myservo.write(180);  
  }else{
    runStepper(0, 1000, true);
  }

  Serial.println("Enter new option");
  Serial.println();
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
  if(isServo){
    myservo.write(0);  
  }else{
    runStepper(0, 1000, false);
  }
  Serial.println("Enter new option");
  Serial.println();
}
