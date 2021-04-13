
//opens a specified slot
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

//locks a row
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

//open all slots in a row
//this is also the reset function called to set the initial motor position
void openAllRow(byte row){
  if (isServo){
    myservo.write(positions_servo[NUM_SENSORS]);   
  }else{
    runStepper(row, finalPositions[NUM_SENSORS] + 100, true);
    positions_stepper[row] = finalPositions[NUM_SENSORS];
    Serial.println(row);
    Serial.print("Setting stepper to");
    Serial.println(finalPositions[NUM_SENSORS]);
  }
}

//moves the arm of a row to a desired position 
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
    }else if (positions_stepper[row] > finalPositions[NUM_SENSORS]){
      positions_stepper[row]=finalPositions[NUM_SENSORS];
    }
}
