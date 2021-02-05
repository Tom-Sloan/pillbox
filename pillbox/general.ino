//Adds each number of val into data at a specific point i.e -12322 -> [1,1,2,3,2,2], removes the negative by adding a one
int* convert(int data[], int val, int startLoc) {

  if (val < 0) {
    val = val * -1;
    data[startLoc] = 1;
  } else {
    data[startLoc] = 0;
  }
  //Pulls out numbers in reverse order (may decrease by two digits as /100)
  int i = startLoc + 3;
  while (val > 0) {
    int digit = val % 100;
    data[i] = digit;
    val /= 100;
    i--;
  }

  for (i; i > startLoc; i--) {
    data[i] = 0;
  }

  return data;
}


void output(String txt) {
  if (canOutput) {
    Serial.print(txt);
  }
}
void output(int txt) {
  if (canOutput) {
    Serial.print(txt);
  }
}
void outputln(String txt) {
  if (canOutput) {
    Serial.println(txt);
  }
}
