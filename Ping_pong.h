

#define LED_PIN 2     // пин ленты
#define LED_NUM 90   // кол-во светодиодов
#define LED_BR 250     // яркость ленты

#define ZONE_SIZE 10  // размер зоны
#define MIN_SPEED 5   // минимальная скорость
#define MAX_SPEED 20  // максимальная скорость
#define WIN_SCORE 5   // победный счёт
struct Ping_pong{
  public:
    byte speed;
    int pos;
    //CRGB leds[LED_NUM];
    byte score1, score2;
    bool b1;
    bool b2;
    unsigned int tmr;
    Ping_pong(){
      b1 = b2 = false;
      speed = random(0, 2) ? MIN_SPEED : -MIN_SPEED;  // случайное направление
      pos = (LED_NUM * 10) / 2;     // в центр ленты
      score1 = score2 = 0;
      tmr = millis();
      //FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
      //FastLED.setBrightness(LED_BR);
    }

    void newGame(){
      blink(CRGB::Red, CRGB::Red, 300);
      blink(CRGB::Yellow, CRGB::Yellow, 300);
      blink(CRGB::Green, CRGB::Green, 300);
      fillZones(CRGB::Green, CRGB::Green);
      FastLED.show();
      randomSeed(millis()); // делаем случайные числа более случайными
      newRound();
    }
    void poolButtons(){
      if (b1) {                             // произошёл клик игрока 1
        if (pos >= ZONE_SIZE * 10) gameOver(0);     // мячик вне зоны 1 игрока - проиграл
        else                                     // мячик в зоне - отбил
          speed = map(pos, ZONE_SIZE * 10, 0, MIN_SPEED, MAX_SPEED); // меняем скорость
      }

      // аналогично для игрока 2
      if (b2) {
        if (pos < (LED_NUM - ZONE_SIZE) * 10) gameOver(1);
        else
          speed = map(pos, (LED_NUM - ZONE_SIZE) * 10, LED_NUM * 10, -MIN_SPEED, -MAX_SPEED);
      }
    }
    void gameRoutine(){
      if (millis() - tmr >= 10) {   // каждые 10 мс
        tmr = millis();
        pos += speed;     // двигаем мячик
        if (pos < 0) {  // вылетел слева
          gameOver(0);  // игрок 1 проиграл
          return;       // выходим
        }
        if (pos >= LED_NUM * 10) {  // вылетел справа
          gameOver(1);              // игрок 2 проиграл
          return;                   // выходим
        }
        FastLED.clear();
        fillZones(CRGB::Green, CRGB::Green);  // показываем зоны
        leds[pos / 10] = CRGB::Cyan;          // рисуем мячик
        FastLED.show();
      }
    }
  private:
    void blink(CRGB color1, CRGB color2, int del) {
      fillZones(color1, color2);    // залить зоны
      FastLED.show();               // показать
      delay(del);                   // ждём
      fillZones(0, 0);              // выключить зоны
      FastLED.show();               // показать
      delay(del);                   // ждать
    }
    void fillZones(CRGB color1, CRGB color2) {
      // заливаем концы ленты переданными цветами
      for (int i = 0; i < ZONE_SIZE; i++) {
        leds[i] = color1;
        leds[LED_NUM - i - 1] = color2;
      }
    }
    void newRound() {
      speed = random(0, 2) ? MIN_SPEED : -MIN_SPEED;  // случайное направление
      pos = (LED_NUM * 10) / 2;     // в центр ленты
    }
    void gameOver(byte player) {
      newRound();    // новый раунд

      if (player == 0) {
        score2++;
        if (score2 == WIN_SCORE) {  // победил игрок 2
          score1 = score2 = 0;      // обнуляем счёт
          // победный бип бип бип игрока 2
          blink(CRGB::Black, CRGB::Green, 600);
          blink(CRGB::Black, CRGB::Green, 600);
          blink(CRGB::Black, CRGB::Green, 600);
          delay(500);
          newGame();  // новая игра
        } else blink(CRGB::Red, CRGB::Green, 200);   // красный бииип игрока 1
      } else {
        score1++;
        if (score1 == WIN_SCORE) {  // победил игрок 1
          score1 = score2 = 0;
          blink(CRGB::Green, CRGB::Black, 600);
          blink(CRGB::Green, CRGB::Black, 600);
          blink(CRGB::Green, CRGB::Black, 600);
          delay(500);
          newGame();  // новая игра
        } else blink(CRGB::Green, CRGB::Red, 200);
      }
    }

};