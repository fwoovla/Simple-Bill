#include <Servo.h>



class Wheel {
  private:
    int forward_pin;
    int reverse_pin;
    int pwm_pin;
    
  public:
    Wheel(int _f, int _r, int _pwm) {
      forward_pin = _f;
      reverse_pin = _r;
      pwm_pin = _pwm;
    }

  void set_speed(int _speed) {
    analogWrite(pwm_pin, _speed);
  }

  
  void stop_wheel() {
    digitalWrite(forward_pin, HIGH);
    digitalWrite(reverse_pin, HIGH);
  }

  void forward() {
    digitalWrite(forward_pin, HIGH);
    digitalWrite(reverse_pin, LOW);
  }

 void reverse() {
  digitalWrite(forward_pin, LOW);
  digitalWrite(reverse_pin, HIGH);
 }
  
};


class Eyes : public Servo{
  private:
    int base_pin;
    int trigger;
    int echo;

   public:
    Servo base;
    int view[5]; //0 = 0, 1 = 45, 2 = 90, 3 = 135, 4 = 180
//    int object_0;
//    int object_45;
//    int object_90;
//    int object_135;
//    int object_180;
    
    
    Eyes(int _t, int _e, int _b) {
      trigger = _t;
      echo = _e;
      base_pin = _b;
      //base.attach(base_pin);

    }

    void begin() {
      base.attach(base_pin);
      pinMode(trigger, OUTPUT);
      pinMode(echo, INPUT);
      pinMode(base_pin, OUTPUT);
      reset();
    }

    void reset() {
      base.write(90);
    }


    int find_closest() {  //returns index 0-4 of view array for closest object
      int index;
      int best = 500;
      
      for(int i = 0; i < sizeof(view); i++) {
          if (view[i] < best) {
            best = view[i];
            index = i;
          }
      }
      return index;
    }


    int find_furthest() {  //returns index 0-4 of view array for furthest object
      int index;
      int best = 0;
      
      for(int i = 0; i < sizeof(view); i++) {
          if (view[i] > best) {
            best = view[i];
            index = i;
          }
      }
      return index;
    }


    void detect_forward() {
      view[2] = 500;
      base.write(90);
      view[2] = range();
        
    }

    
    void detect_objects() {

      for(int i = 0; i < sizeof(view); i++) {
        view[i] = 5000;
      }
      
      base.write(0);
      delay(500);

      view[0] = range();

      base.write(45);
      delay(300);

      view[1] = range();
      
      base.write(90);
      delay(300);

      view[2] = range();

      base.write(135);
      delay(300);

      view[3]= range();

      base.write(180);
      delay(300);

      view[4] = range();
      
      reset();
    }


    int range() {
      int distance;
      int duration;
      // Clears the trigPin condition
      digitalWrite(trigger, LOW);
      delayMicroseconds(2);
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echo, HIGH);
      // Calculating the distance
      distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
      // Displays the distance on the Serial Monitor
  //    Serial.print("Distance: ");
  //    Serial.print(distance);
  //    Serial.println(" cm");
      delay(10);
      return distance;
    }

    //void 
//
//    void sweep() {
//      
//      for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//        // in steps of 1 degree
//        base.write(pos);              // tell servo to go to position in variable 'pos'
//        delay(15);                       // waits 15ms for the servo to reach the position
//      }
//      for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//        base.write(pos);              // tell servo to go to position in variable 'pos'
//        delay(15);                       // waits 15ms for the servo to reach the position
//      }
//    }
    
};



Wheel leftWheel(4, 2, 3);
Wheel rightWheel(7, 8, 5);
Eyes eyes(11, 12, 6);

int speed = 255;
int closest_index;

void setup() {
  eyes.begin();
  leftWheel.set_speed(speed);
  rightWheel.set_speed(speed);
  eyes.reset();
  //eyes.detect_objects();
  delay(1000);

}


void loop() {

  eyes.detect_forward();
  delay(50);
  
  if(eyes.view[2] >= 40) {
    forward();
  }
  if(eyes.view[2] < 40) {
    stop_wheels();
    
    eyes.detect_objects();

    closest_index = eyes.find_closest();

    if(closest_index == 0) { //closest is at 0 deg
      //rightWheel.forward();
      turn_left();
      delay(800);
      stop_wheels();  
    }
    
    if(closest_index == 1) { //closest is at 45 deg
      //rightWheel.reverse();
      turn_left();
      delay(400);
      stop_wheels();        
    }
    
    if(closest_index == 2) { //closest is at 90 deg
      //rightWheel.forward();
      //leftWheel.forward();
      reverse();
      delay(700);
      stop_wheels();
      delay(50);
      turn_right();
      delay(400);
      stop_wheels();
    }
    
    if(closest_index == 3) { //closest is at 135 deg
      //leftWheel.reverse();
      turn_right();
      delay(800);
      stop_wheels();         
    }
    
    if(closest_index == 4) { //closest is at 180 deg
      //leftWheel.forward();
      turn_right();
      delay(400);
      stop_wheels();         
    }
  }
  delay(100);
//  
//  if (eyes.view[2] < 50) {
//    stop_wheels();

}


void stop_wheels() {
  leftWheel.stop_wheel();
  rightWheel.stop_wheel();
}


void forward() {
  leftWheel.forward();
  rightWheel.forward();
}


void reverse() {
  leftWheel.reverse();
  rightWheel.reverse();
}

void turn_left() {
  leftWheel.reverse();
  rightWheel.forward();  
}

void turn_right() {
  leftWheel.forward();
  rightWheel.reverse();  
}


void test_wheels() {
  forward();
  delay(1000);

  stop_wheels();
  delay(1000);

  reverse();
  delay(1000);

  stop_wheels();
  delay(1000);

  turn_left();
  delay(1000);

  stop_wheels();
  delay(1000);

  turn_right();
  delay(1000);

  stop_wheels();
  delay(1000);
}
