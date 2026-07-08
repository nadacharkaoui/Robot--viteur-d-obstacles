#include <Servo.h>

Servo servo;

const int trigPin = 13;
const int echoPin = 12;

const int servoPin = 11;

const int enAPin = 6;
const int in1Pin = 7;
const int in2Pin = 5;
const int in3Pin = 4;
const int in4Pin = 2;
const int enBPin = 3;

const int leftTrim = 0;  // ajuste entre -30 et +30 si le robot dérive

enum Motor { LEFT, RIGHT };

void go(enum Motor m, int speed)
{
    if (m == RIGHT)
        speed = -speed;  // roue droite câblée en sens inverse
    digitalWrite(m == LEFT ? in1Pin : in3Pin, speed > 0 ? HIGH : LOW);
    digitalWrite(m == LEFT ? in2Pin : in4Pin, speed <= 0 ? HIGH : LOW);
    analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

void testMotors()
{
    static int speed[8] = {128, 255, 128, 0, -128, -255, -128, 0};
    go(RIGHT, 0);
    for (unsigned char i = 0; i < 8; i++)
        go(LEFT, speed[i]), delay(200);
    for (unsigned char i = 0; i < 8; i++)
        go(RIGHT, speed[i]), delay(200);
}

void moveServoSlowly(int targetAngle)
{
    static int currentAngle = 90;
    int step = (targetAngle > currentAngle) ? 1 : -1;
    while (currentAngle != targetAngle)
    {
        currentAngle += step;
        servo.write(currentAngle);
        delay(15);
    }
}

unsigned int readDistance()
{
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    unsigned long period = pulseIn(echoPin, HIGH, 25000);
    if (period == 0)
        return 9999;
    return period * 343 / 2000;
}

#define NUM_ANGLES 7
unsigned char sensorAngle[NUM_ANGLES] = {60, 70, 80, 90, 100, 110, 120};
unsigned int distance[NUM_ANGLES];

void readNextDistance()
{
    static unsigned char angleIndex = 0;
    static signed char step = 1;
    distance[angleIndex] = readDistance();
    angleIndex += step;
    if (angleIndex == NUM_ANGLES - 1)
        step = -1;
    else if (angleIndex == 0)
        step = 1;
    moveServoSlowly(sensorAngle[angleIndex]);
}

void setup()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);

    pinMode(enAPin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);
    pinMode(enBPin, OUTPUT);

    servo.attach(servoPin);
    servo.write(90);

    go(LEFT, 0);
    go(RIGHT, 0);
    testMotors();

    moveServoSlowly(sensorAngle[0]);
    delay(200);
    for (unsigned char i = 0; i < NUM_ANGLES; i++)
        readNextDistance(), delay(200);
}

void loop()
{
    bool tooClose = false;
    readNextDistance();
    for (unsigned char i = 0; i < NUM_ANGLES; i++)
    {
        unsigned int threshold = (i >= 2 && i <= 4) ? 300 : 150;
        if (distance[i] < threshold)
            tooClose = true;
    }

    if (tooClose)
    {
        go(LEFT, -200);
        go(RIGHT, -200);
        delay(500);

        go(LEFT, -180);
        go(RIGHT, 180);
        delay(400);

        go(LEFT, 0);
        go(RIGHT, 0);
        delay(100);
    }
    else
    {
        go(LEFT, 255 + leftTrim);
        go(RIGHT, 255 - leftTrim);
    }
    delay(50);
}
