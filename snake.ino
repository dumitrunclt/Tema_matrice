/*
 * SNAKE
 * Sarpele se misca constant iar jucatorul trebuie sa
 * evite sa se intersecteze cu marginile sau cu "corpul"
 * sarpelui.
 * Cand sarpele mananca mancarea primeste cate 5 puncte
 * pentru fiecare "bucata" iar viteza de miscare creste
 * cate putin.
 */

#include <LiquidCrystal.h>
#include <LedControl.h>

// Joystick PINs
#define VRX A5
#define VRY A4
#define SW 3

// LCD PIN
#define V0_PIN 9

#define SIZE 8
#define ADVANCE_DELAY 30

int snake[SIZE*SIZE][2];
int length;
int food[2], v[2];
bool is_game_over = false;
long current_time;
long previous_advance;
int blink_count;
const short message_speed = 9;
int score = 0;
int level = 0;
long long previous_time = 0;
int interval = 1000;

LiquidCrystal lcd(2, 5, 4, 6, 7, 8);
LedControl lc = LedControl(12, 11, 10, 1);

const PROGMEM bool start_game_message[8][69] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0},
};

const PROGMEM bool game_over_message[8][90] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}
}; 

void show_start_message() {
  for (int i = 0; i < sizeof(start_game_message[0]) - 7; i++) {
    for (int col = 0; col < 8; col++) {
      delay(message_speed); // afiseaza mesajul de inceput - nu are importanta ca blocheaza totul pana afiseaza mesajul
      for (int row = 0; row < 8; row++){
        lc.setLed(0, row, col, pgm_read_byte(&(start_game_message[row][col + i])));
        // citeste bit-ul din PROGMEM si il afiseaza pe ecran
      }
    }
  }
}

void show_game_over_message() {
  for (int i = 0; i < sizeof(game_over_message[0]) - 7; i++) {
    for (int col = 0; col < 8; col++) {
      delay(message_speed); // afiseaza mesajul de sfarsit - nu are importanta ca blocheaza totul pana afiseaza mesajul
      for (int row = 0; row < 8; row++){
        lc.setLed(0, row, col, pgm_read_byte(&(game_over_message[row][col + i])));
        // citeste bit-ul din PROGMEM si il afiseaza pe ecran
      }
    }
  }
}

void initialization_game() {
  previous_advance = current_time = 0;
  blink_count = 3; // numarul de "blink"-uri dupa sfarsitul jocului - arata pozitia finala
  int half = SIZE / 2;
  length = SIZE / 3; 

  for (int i = 0; i < length; i++) {
    snake[i][0] = half - 1;
    snake[i][1] = half + i;
  }

  food[0] = half + 1;
  food[1] = half - 1;

  v[0] = 0;
  v[1] = -1;
}

void restart_game() {
  initialization_game();
  is_game_over = false;
  score = 0;
  level = 0;
}

void render () {
  for (int i = 0; i < length; i++)  {
    lc.setLed(0, snake[i][0], snake[i][1], 1);
  }
  lc.setLed(0, food[0], food[1], 1);
}

void clear_screen() {
  for (int x = 0; x < SIZE; x++) {
    for(int y = 0; y < SIZE; y++) {
      lc.setLed(0, x, y, 0);
    }
  }
}

/*
 * muta sarpele inainte
 * returneaza true daca jocul s-a terminat
 */
 
bool advance() {
  int head[2] = {snake[0][0] + v[0], snake[0][1] + v[1]};

  if (head[0] < 0 || head[0] >= SIZE) {
    if (millis() - previous_time > interval) {
      previous_time = millis();
      show_game_over_message();
      return true;
    }
  }

  if (head[1] < 0 || head[1] >= SIZE) {
    if (millis() - previous_time > interval) {
      previous_time = millis();
      show_game_over_message();
      return true;
    }
  }

  for (int i = 0; i < length; i++) {
    if (snake[i][0] == head[0] && snake[i][1] == head[1]) {
      if (millis() - previous_time > interval) {
        previous_time = millis();
        show_game_over_message();
        return true;
       }
     }
  }

  bool grow = (head[0] == food[0] && head[1] == food[1]);
  if (grow) {
    length = length + 1;
    score = score + 5;
    lcd.setCursor(0, 7);
    lcd.print(score);
    level = level + 1;
    lcd.setCursor(1, 7);
    lcd.print(level);
    randomSeed(current_time);    
    food[0] = random(SIZE);
    food[1] = random(SIZE);
  }

  for (int i = length - 1; i >= 0; i--) {
    snake[i + 1][0] = snake[i][0];
    snake[i + 1][1] = snake[i][1];
  }
  
  snake[0][0] += v[0];
  snake[0][1] += v[1];
  return false;
}

void read_controls() {
  int dx = map(analogRead(VRX), 0, 906, 2, -2);
  int dy = map(analogRead(VRY), 0, 906, -2, 2);

  if (dx != 0) {
    dx = dx / abs(dx);
  }
  if (dy != 0) {
    dy = dy / abs(dy);
  }

  if (dy != 0 && v[0] != 0) {
    v[0] = 0;
    v[1] = dy;
  }

  if (dx != 0 && v[1] != 0) {
    v[0] = dx;
    v[1] = 0;
  }
}

void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  attachInterrupt(digitalPinToInterrupt(SW), restart_game, RISING);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Press on Joystick");
  lcd.setCursor(0, 1);
  lcd.print("To play");
  delay(2000);

  pinMode(V0_PIN, OUTPUT);
  analogWrite(V0_PIN, 90);

  show_start_message();
  initialization_game();
  render();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Level: ");
  lcd.print(level);

  if (!is_game_over) {
    clear_screen();
    render();

    if (current_time - previous_advance > ADVANCE_DELAY) {
      is_game_over = advance();
      previous_advance = current_time;
    }
  } else {
    while (blink_count > 0) {
      clear_screen();
      delay(300);
      render();
      delay(300);
      blink_count = blink_count - 1;
    }
  }

  read_controls();
  current_time = current_time + 1;
}
