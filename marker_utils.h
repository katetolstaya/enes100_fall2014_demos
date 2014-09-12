
#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial sSerial(8,9);


char buf[50];
  
typedef struct {
  int id;
  float x;
  float y;
  float theta;
  long time;
} Marker;

//get_next_marker stores the next marker string in the
//given 'marker_string_buffer' character array. This function
//reads synchronously from the serial.  If a full marker
//string is read before the timeout is reached, the function 
//returns 1.  Otherwise, the function returns 0 if the marker is
//not fully read before the timeout.
int getNextMarkerString(char* marker_string_buffer, int timeout_ms) 
{
  long start_time = millis();
  
  //Throw away data until a starting newline character is found.
//  while((millis() - start_time) < timeout_ms)
//  {
//    if(sSerial.available())
//    {
//      if(sSerial.read()=='\n')
//        break;
//    }
//  }
  //Read data in to buffer until terminating newline character is found
  int i = 0;
  while((millis() - start_time) < timeout_ms)
  {
    if(sSerial.available())
    {
      //If the next character is not a newline character, store it in
      //the buffer.
      if(sSerial.peek()!='\n') 
      {
        marker_string_buffer[i] = sSerial.read();
        i++;
      }
      //If the next character is a newline character, terminate
      //the string, leaving the '\n' in the serial buffer (unread).
      else {
        marker_string_buffer[i] = 0;
        //Exit, marker received
        sSerial.read();
        return 1;
        
      }
    }
  }
  //Exit, marker not received, timeout reached.
  return 0;
}

void parseMarkerString(Marker& m, char* marker_string)
{
  char temp_buffer[10];
  int i=0, j=0;
  //Parse id
  while(marker_string[i] != ',')
  {
    temp_buffer[j] = marker_string[i];
    j++;
    i++;
  }
  temp_buffer[j] = 0;
  j = 0;
  i++;
  m.id = atoi(temp_buffer);
  
  //Parse X
  while(marker_string[i] != ',')
  {
    temp_buffer[j] = marker_string[i];
    j++;
    i++;
  }
  temp_buffer[j] = 0;
  j = 0;
  i++;
  m.x = atof(temp_buffer);
  
  //Parse y
  while(marker_string[i] != ',')
  {
    temp_buffer[j] = marker_string[i];
    j++;
    i++;
  }
  temp_buffer[j] = 0;
  j = 0;
  i++;
  m.y = atof(temp_buffer);
  
  //Parse theta
  while(marker_string[i] != ',')
  {
    temp_buffer[j] = marker_string[i];
    j++;
    i++;
  }
  temp_buffer[j] = 0;
  j = 0;
  i++;
  m.theta = atof(temp_buffer);
  
  //Parse time
  while(marker_string[i] != 0)
  {
    temp_buffer[j] = marker_string[i];
    j++;
    i++;
  }
  temp_buffer[j] = 0;
  m.time = atol(temp_buffer);
  
}

//getNextMarker attempts to read a marker from serial
//into the provided marker.  If the marker is fully
//received by the timeout, a value of 1 is returned.
//Otherwise, if the marker is not fully received by the timeout,
//a value of 0 is returned.
int getNextMarker(Marker& m, int timeout) {
  if(getNextMarkerString(buf,timeout))
  {
    parseMarkerString(m,buf);
    return 1;
  }
  else
    return 0;
}
 
