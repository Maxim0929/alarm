#include "eprom.h"
#include "encoder.h"
#include "led.h"
#include "list.h"
#include "display.h"
#include "time.h"


#define MENULAST 5
#define EFFECTLAST 7
enum Func{TIME, SETVAL, SETTIME, SETALARM, NONEFUNC};
//speed in random to delay
void listClick();
void initMenu(List & menu);
bool setVal(uint8_t encState, const List& list);
void update();


Led led;

MyEncoder enc;

Eprom epr;

List menu;
List* current;

Display dsp;

Time time;
Alarm alarm;

Func currentFunc = TIME;

Metro backTimer = Metro(5000);

void setup(){
  Serial.begin(9600);
  initMenu(menu);
  dsp.setup(menu);
  led.init(&menu);
  current = &menu;
  time.init();
  alarm.init(epr);
  dsp.printTime(time.getHour(), time.getMin());
}
void loop(){
  if(enc.updateEncoder())
    update();
  if(alarm.getisAlarm())
    alarm.update(time, led, enc.getEncstate());

  if(currentFunc != SETTIME){
    if(time.update()){
      if(currentFunc == TIME)
        dsp.printTime(time.getHour(), time.getMin());
      if(currentFunc != SETALARM) alarm.update(time, led, enc.getEncstate());
    }
  }
  if(currentFunc == TIME)
    led.updateStrip(*menu.next.getPtr("EFFECTS"), 0);

  if(backTimer.check() && currentFunc != TIME){
    currentFunc = TIME;
    dsp.clear();
    dsp.printTime(time.getHour(), time.getMin());
  }
}


void update(){
  backTimer.reset();
  if(currentFunc == TIME){
    if(enc.getEncstate() == 4){
      currentFunc = NONEFUNC;
      dsp.printList(*current, 1);
    }
  }else if(currentFunc == NONEFUNC){
    if(enc.getEncstate() == 1)
      listClick();
    else
      current->update(enc.getEncstate(), dsp);
  }else{
    set();
  }
}
void listClick(){
  List* next;
  next = current->next.getPtr(current->getCurrent());
  if(next->getNodetype() == List::NodeType::LIST){//LIST
    current = next;
    current->setCurrent(0);
    current->setFirst(0);
    dsp.printList(*current);
    if(current->getName() != "EFFECTS"){
      led.setStripState(current->getName());
      led.updateStrip(*menu.next.getPtr("EFFECTS"), 1);
    }
  }else if(next->getNodetype() == List::NodeType::VALUE){ // VALUE
    currentFunc = SETVAL;
    dsp.clear();
    dsp.printNum(next->getValue(1));


  }else if(next->getNodetype() == List::NodeType::FUNCTION){
    if(next->getName() == "SETTIME"){
      currentFunc = SETTIME;
      dsp.clear();
      dsp.printTime(time.getHour(), time.getMin());

    }else if(next->getName() == "SETALARM"){
      currentFunc = SETALARM;
      dsp.clear();
      dsp.printTime(alarm.getHour(), alarm.getMin());

    }else if(next->getName() == "BACK"){
      if(current->getName() == "MENU"){
        dsp.clear();
        dsp.printTime(time.getHour(), time.getMin());
        currentFunc = TIME;
      } 
      else {
        currentFunc = NONEFUNC;
        current = next->prev;
        current->setCurrent(0);
        current->setFirst(0);
        dsp.printList(*current);
      }
    }else if(next->getName() == "OFF"){      
      led.setStripState("OFF");
      led.updateStrip(*menu.next.getPtr("EFFECTS"), 1);
    }
  }
}

void set(){
  if(currentFunc == SETVAL){
    if(setVal(enc.getEncstate(), *current->next.getPtr(current->getCurrent()))){
      currentFunc = NONEFUNC;

      if(current->next.getPtr(current->getCurrent())->getName() == "DISPLAY BRIGHTNESS")
        dsp.setBr(current->next.getPtr(current->getCurrent())->getValue(0));
      else if(current->next.getPtr(current->getCurrent())->getName() == "LED BRIGHTNESS")
        led.setBr(current->next.getPtr(current->getCurrent())->getValue(0));

      dsp.printList(*current);
      led.updateStrip(*menu.next.getPtr("EFFECTS"), 1);
    }
  }else if(currentFunc == SETTIME){
    if(time.set(enc.getEncstate())){
      currentFunc = NONEFUNC;
      dsp.printList(*current);
    }else if(enc.getEncstate() != 0) dsp.printTime(time.getHour(), time.getMin());
  }else if(currentFunc == SETALARM){
      if(alarm.set(enc.getEncstate(), epr)){
        currentFunc = NONEFUNC;
        dsp.printList(*current);
      }else if(enc.getEncstate() != 0) dsp.printTime(alarm.getHour(), alarm.getMin());
  }
}

void initMenu(List & menu){
  uint8_t eprID = 0;

  static const char* names1[] = {"EFFECTS", "SETALARM", "SETTIME", "DISPLAY BRIGHTNESS", "LED BRIGHTNESS", "BACK", nullptr};// initilize menu
  menu.initList("MENU", MENULAST + 1, names1);

  static const char* names2[] = {"OFF", "COLOUR", "FLOW", "COLOUR CHANGE", "RANDOM", "DRIFT", "SNAKE", "BACK", nullptr};
  menu.next.getPtr("EFFECTS")->initList("EFFECTS", EFFECTLAST + 1, names2);
  menu.next.getPtr("SETALARM")->initList("SETALARM");
  menu.next.getPtr("SETTIME")->initList("SETTIME");
  menu.next.getPtr("DISPLAY BRIGHTNESS")->initList("DISPLAY BRIGHTNESS", epr.read("DISPLAY BRIGHTNESS" + eprID++), 255);
  menu.next.getPtr("LED BRIGHTNESS")->initList("LED BRIGHTNESS", epr.read("LED BRIGHTNESS" + eprID++), 255);//epr.read("LED BRIGHTNESS" + eprID++)
  menu.next.getPtr("BACK")->initList("BACK");


  List *current = menu.next.getPtr("EFFECTS");//initilize effects
  current->next.getPtr("OFF")->initList("OFF");

  static const char* names3[] = {"COLOUR", "AMOUNT", "BACK", nullptr};
  current->next.getPtr("COLOUR")->initList("COLOUR", 3, names3);

  static const char* names4[] = {"COLOUR", "AMOUNT", "SPEED", "BACK", nullptr};
  current->next.getPtr("FLOW")->initList("FLOW", 4, names4);

  static const char* names5[] = {"COLOUR FROM", "COLOUR TO", "AMOUNT", "SPEED", "BACK", nullptr};
  current->next.getPtr("COLOUR CHANGE")->initList("COLOUR CHANGE", 5, names5);

  static const char* names6[] = {"DELAY", "AMOUNT", "BACK", nullptr};
  current->next.getPtr("RANDOM")->initList("RANDOM", 3, names6);

  static const char* names7[] = {"SPEED", "AMOUNT", "BACK", nullptr};
  current->next.getPtr("DRIFT")->initList("DRIFT", 3, names7);

  static const char* names8[] = {"COLOUR", "AMOUNT", "BACK", nullptr};
  current->next.getPtr("SNAKE")->initList("SNAKE", 3, names8);

  current->next.getPtr("BACK")->initList("BACK", &menu);


  current = current->next.getPtr("COLOUR");//initilize COLOUR
  current->next.getPtr("COLOUR")->initList("COLOUR", epr.read("COLOUR" + eprID++), 359);
  current->next.getPtr("AMOUNT")->initList("AMOUNT", epr.read("AMOUNT" + eprID++), 600);
  current->next.getPtr("BACK")->initList("BACK", menu.next.getPtr("EFFECTS"));

  current = menu.next.getPtr("EFFECTS")->next.getPtr("FLOW");//initilize FLOW
  current->next.getPtr("COLOUR")->initList("COLOUR", epr.read("COLOUR" + eprID++), 359);
  current->next.getPtr("AMOUNT")->initList("AMOUNT", epr.read("AMOUNT" + eprID++), 600);
  current->next.getPtr("SPEED")->initList("SPEED", epr.read("SPEED" + eprID++), 255);
  current->next.getPtr("BACK")->initList("BACK", menu.next.getPtr("EFFECTS"));

  current = menu.next.getPtr("EFFECTS")->next.getPtr("COLOUR CHANGE");//initilize COLOUR CHANGE
  current->next.getPtr("COLOUR FROM")->initList("COLOUR FROM", epr.read("COLOUR FROM" + eprID++), 359);
  current->next.getPtr("COLOUR TO")->initList("COLOUR TO", epr.read("COLOUR TO" + eprID++), 359);
  current->next.getPtr("AMOUNT")->initList("AMOUNT", epr.read("AMOUNT" + eprID++), 600);
  current->next.getPtr("SPEED")->initList("SPEED", epr.read("SPEED" + eprID++), 200);
  current->next.getPtr("BACK")->initList("BACK", menu.next.getPtr("EFFECTS"));

  current = menu.next.getPtr("EFFECTS")->next.getPtr("RANDOM");//initilize RANDOM
  current->next.getPtr("DELAY")->initList("DELAY", epr.read("DELAY" + eprID++), 5000);// DELAY = 500; every 500 ms 10% of pixels change color amd position
  current->next.getPtr("AMOUNT")->initList("AMOUNT", epr.read("AMOUNT"+ eprID++), 600);
  current->next.getPtr("BACK")->initList("BACK", menu.next.getPtr("EFFECTS"));

  current = menu.next.getPtr("EFFECTS")->next.getPtr("DRIFT");//initilize DRIFT
  current->next.getPtr("SPEED")->initList("SPEED", epr.read("SPEED" + eprID++), 255);
  current->next.getPtr("AMOUNT")->initList("AMOUNT", epr.read("AMOUNT" + eprID++), 600);
  current->next.getPtr("BACK")->initList("BACK", menu.next.getPtr("EFFECTS"));

  current = menu.next.getPtr("EFFECTS")->next.getPtr("SNAKE");//initilize SNAKE
  current->next.getPtr("COLOUR")->initList("COLOUR", epr.read("COLOUR" + eprID++), 359);
  current->next.getPtr("AMOUNT")->initList("AMOUNT", epr.read("AMOUNT" + eprID++), 600);
  current->next.getPtr("BACK")->initList("BACK", menu.next.getPtr("EFFECTS"));

  
  return menu;
}

bool setVal(uint8_t encState, const List& list){
  uint8_t val = list.getValue(1);
  switch(encState){
    case 1:
      epr.write(list.getName() + list.getID(), val);
      return 1;
    break;
    case 2:
      if(val == 0) list.setValue(255);
      else list.setValue(val - 1);
      dsp.printNum(list.getValue(1));
    break;
    case 3:
      if(val == 255) list.setValue(0);
      else list.setValue(val + 1);
      dsp.printNum(list.getValue(1));
    break;
    case 5:
      if(val > 9) list.setValue(val - 10);
      dsp.printNum(list.getValue(1));
    break;
    case 6:
      if(val < 255 - 9) list.setValue(val + 10);
      dsp.printNum(list.getValue(1));
    break;
  }
  return 0;
}








