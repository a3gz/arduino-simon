#include <Arduino.h>

#define RED 2
#define YELLOW 3
#define GREEN 4
#define BLUE 5

#define GAME_SIZE 100

int toneDuration = 400; // ms
int tones[] = {200, 350, 500, 650}; // Hz
int ledPins[] = {RED, YELLOW, GREEN, BLUE};
int btnPins[] = {RED+4, YELLOW+4, GREEN+4, BLUE+4};
int buzzerPin = btnPins[3]+1;

int gameSet[GAME_SIZE] = {};
int gameLen = 0;

void gameOver() {
  gameLen = -1;
  Serial.println("GAME OVER!");
  for (int r=0; r < 3; r++) {
    for (int i=0; i < 4; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    tone(buzzerPin, 150, 600);
    delay(600);
    for (int i=0; i < 4; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(100);
  }
  tone(buzzerPin, 100, 1000);
}

void makeNextMove() {
  int r = random(3);
  gameSet[gameLen] = r;
  gameLen++;
}

void play(int i) {
  digitalWrite(ledPins[i], HIGH);
  tone(buzzerPin, tones[i], toneDuration);
  delay(toneDuration);
  digitalWrite(ledPins[i], LOW);
  delay(100);
}

void playBegin() {
  Serial.println("Begin!");
  for (int i=0; i < 4; i++) {
    play(i);
  }
  delay(2000);
}

void playerMove() {
  int moves = 0;
  do {
    for (int b=0; b < 4; b++) {
      if (digitalRead(btnPins[b]) == HIGH) {
        play(b);
        if (gameSet[moves] != b) {
          gameOver();
          return;
        }
        moves++;
      }
    }
  } while (moves < gameLen);
  delay(200);
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  Serial.print("Game set size: ");
  Serial.println(GAME_SIZE);

  for (int i=0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  for (int i=0; i < 4; i++) {
    pinMode(btnPins[i], INPUT);
  }
  playBegin();
}

void loop() {
  if ( gameLen >= 0) {
    if ( gameLen == GAME_SIZE ) {
      Serial.println("You win!");
      Serial.println("START AGAIN!");
      gameLen = 0;
    }
    makeNextMove();
    for (int i=0; i < gameLen; i++) {
      play(gameSet[i]);
    }
    playerMove();
  }
}