#include <Arduino.h>

#include <SoftwareSerial.h>
#include <elapsedMillis.h>
#include <Bounce2.h>
Bounce2::Button button = Bounce2::Button();

SoftwareSerial SerialSync(10, 11); // RX | TX

elapsedMillis timeout;

struct data_structure
{
  char cmd[20];
  int senzor1;
  float senzor2;
  boolean btnState = LOW;
};
struct data_structure data;

int size_of_data = sizeof(struct data_structure);
void send(const data_structure *table);
bool receive(data_structure *table);

void setup()
{
  Serial.begin(9600);
  SerialSync.begin(9600);
  button.attach(2, INPUT);
  button.interval(5);
  button.setPressedState(LOW);
}

void loop()
{
  button.update();
  if (button.pressed())
  {
    data.btnState = !data.btnState;
    digitalWrite(3, data.btnState);
    strcpy(data.cmd, "Hello World B");
    data.senzor1 = 200;
    data.senzor2 = 40.5;
    send(&data);
  }
  if (SerialSync.available())
  {
    receive(&data);
    Serial.println(data.senzor1);
    Serial.println(data.senzor2);
    Serial.println(data.cmd);
    Serial.println(data.btnState);
    Serial.println("--------------");
  }
}

void send(const data_structure *table)
{
  SerialSync.write((const char *)table, size_of_data);
}

bool receive(data_structure *table)
{
  return (SerialSync.readBytes((char *)table, sizeof(data_structure)) == sizeof(data_structure));
}
