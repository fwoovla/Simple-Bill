//scooter bot
//

#include <Adafruit_NeoPixel.h>
#define LIGHT_PIN 6
#define NUMPIXELS 1


class Wheel {
  private:
    int forward_pin;
    int reverse_pin;
    int pwm_pin;
    int wheel_state = 0; //-1,0,1
    
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
    if(wheel_state ==1) { //we're going forwd
    reverse();     
    }
    else {                //were going bkwd
      forward();
    }
    wheel_state = 0;
  }

  void coast() {
    digitalWrite(forward_pin, LOW);
    digitalWrite(reverse_pin, LOW);
    wheel_state = 0;
  }

  void forward() {
    digitalWrite(forward_pin, HIGH);
    digitalWrite(reverse_pin, LOW);
    wheel_state = 1;
  }

 void reverse() {
  digitalWrite(forward_pin, LOW);
  digitalWrite(reverse_pin, HIGH);
  wheel_state = -1;
 }
  
};

Adafruit_NeoPixel light(NUMPIXELS, LIGHT_PIN, NEO_RGB);

Wheel rightWheel(6, 5, 3); //a,b,pwm
Wheel leftWheel(8, 7, 11);

int speed = 100;

void setup() {
  Serial.begin(9600);
  light.begin();
  light.clear();
  light.setBrightness(100);
  light.setPixelColor(0, light.Color(255,0,0));
  light.show();
  leftWheel.set_speed(speed);
  rightWheel.set_speed(speed);
  delay(1000);

}

void loop() {
  //leftWheel.forward();
  //rightWheel.forward();
  test_wheels();
  //speed_test(0,100);
  //speed_test_2(150, 250);
  //step_forward();
}



void stop_wheels() {
  leftWheel.stop_wheel();
  rightWheel.stop_wheel();
  delay(200);
  leftWheel.coast();
  rightWheel.coast();
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

void speed_test(int _start, int _stop) {
  //int s = 100;
  leftWheel.set_speed(_start);
  rightWheel.set_speed(_start);
  forward();
  for(int i = _start; i < _stop; i++) {
    Serial.println(i);
  leftWheel.set_speed(i);
  rightWheel.set_speed(i);
  delay(100);
  }
}

void speed_test_2(int _low, int _high) {

  forward();
  for(int loop = 0; loop < 10; loop++) {
    
    Serial.println(_low);
    leftWheel.set_speed(_low);
    rightWheel.set_speed(_low);

    delay(500);
  
    Serial.println(_high);
    leftWheel.set_speed(_high);
    rightWheel.set_speed(_high);

    delay(500);
  }
}


void step_forward() {
  forward();
  delay(100);
  stop_wheels();
  delay(300);  
}


void test_wheels() {
  forward();
  delay(500);

  stop_wheels();
  delay(1000);

  reverse();
  delay(500);

  stop_wheels();
  delay(1000);

  turn_left();
  delay(500);

  stop_wheels();
  delay(1000);

  turn_right();
  delay(500);

  stop_wheels();
  delay(1000
  );
}
