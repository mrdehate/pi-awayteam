#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <signal.h>

#define SWITCH_PIN 21
#define ALL_OFF 0
#define SECONDS_TO_WAIT 1.0
#define TIMER_LENGTH 3600

using std::cout;
using std::endl;

void init() {
  wiringPiSetupGpio();
  pinMode(SWITCH_PIN, INPUT);
  pullUpDnControl(SWITCH_PIN, PUD_UP);
}
int i = 0;
bool isOn = false; 

auto start = std::chrono::system_clock::now();

double microsecondsToSeconds(double microseconds) {
  return microseconds / 1000.0 / 1000.0;
}

void startAlarm() {
  cout << "start alarming" << endl;
  alarm(TIMER_LENGTH);
}

void stopAlarm() {
  cout << "stop alarming" << endl;
  alarm(0);
}

void turnOffCamera() {
  cout << "start offing" << endl;
  system("/home/pi/button/stop_recording.sh");
  cout << "stop stopping" << endl;
  stopAlarm();
  isOn = !isOn;
}

void turnOnCamera() {
  cout << "shell script started" << endl;
  system("/home/pi/button/start_recording.sh");
  cout << "stop starting" << endl;
  startAlarm();
  isOn = !isOn;
}



void increment() {
  cout << "button pressed" << endl;
  auto end = std::chrono::system_clock::now();
  auto duration = end - start;
  start = end;
  if(microsecondsToSeconds(duration.count()) > SECONDS_TO_WAIT) {
    if(isOn) {
      turnOffCamera();
    } else {
      turnOnCamera();
    }
  }
}

void sigHandler(int signal) {
  cout << "HANDLE THE SIGNAL" << endl;
  turnOffCamera();
}

int main(int argc, char** argv) {
  init();
  signal(SIGALRM, sigHandler); 
  wiringPiISR(SWITCH_PIN, INT_EDGE_RISING, &increment);
  getchar();
  return 0;
}
