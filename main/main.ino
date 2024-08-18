#include "eprom.h"
#include "encoder.h"
#include "led.h"
#include "list.h"
#include "display.h"
#include "time.h"

#define MENULAST 4
#define EFFECTLAST 7
//test fillHSV
enum Func{TIME, SETVAL, SETTIME, SETALARM, NONEFUNC};

void listClick();
List& initMenu();
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

void setup(){
  Serial.begin(9600);
  dsp.setup();
  led.init();
  initMenu(menu);
  current = &menu;
  time.init();
  alarm.init(epr);
}
void loop(){
  enc.updateEncoder();
  if(currentFunc != SETTIME){
    time.update();
    if(currentFunc != SETALARM) alarm.update(time, led, enc.getEncstate());
  }
  update();
}


void update(){
  if(currentFunc == TIME){
    if(enc.getEncstate() == 4){
      currentFunc = NONEFUNC;
      dsp.printList(*current, 1);
    }else{
      dsp.printTime(time.getHour(), time.getMin());
    }
  }else if(currentFunc == NONEFUNC){
    if(enc.getEncstate() == 1){
      listClick();
    }
    current->update(enc.getEncstate(), dsp);
  }else{
    set();
  }
}
void listClick(){
  List* next;
  next = &(current->next[current->getCurrent()]);
  if(next->getNodetype() == List::NodeType::LIST){//LIST
    current = next;
    current->setCurrent(0);
    current->setFirst(0);
    dsp.printList(*current);
    if(current->getName() != "EFFECTS"){
      led.setStripState(current->getName());
      led.updateStrip(menu.next[3]);
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
      led.updateStrip(menu.next[3]);
    }
  }
}

void set(){
  if(currentFunc == SETVAL){
    if(setVal(enc.getEncstate(), current->next[current->getCurrent()])){
      currentFunc = NONEFUNC;

      if(current->next[current->getCurrent()].getName() == "DISPLAY BRIGHTNESS")
         dsp.setBr(current->next[current->getCurrent()].getValue(0));

      dsp.printList(*current);
      led.updateStrip(menu.next[3]);
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
  menu.initList("MENU", MENULAST + 1, List::NodeType::LIST);// initilize menu
  menu.next[0].initList("SETTIME");
  menu.next[1].initList("SETALARM");
  menu.next[2].initList("DISPLAY BRIGHTNESS", epr.read("DISPLAY BRIGHTNESS" + 0), 255);
  menu.next[3].initList("EFFECTS", EFFECTLAST + 1, List::NodeType::LIST);
  menu.next[4].initList("BACK");/////////////////////
  
  List *current = &menu.next[3];//initilize effects
  current->next[0].initList("OFF");
  current->next[1].initList("COLOUR", 3, List::NodeType::LIST);
  current->next[2].initList("FLOW", 4, List::NodeType::LIST);
  current->next[3].initList("COLOUR CHANGE", 5, List::NodeType::LIST);
  current->next[4].initList("RANDOM", 3, List::NodeType::LIST);
  current->next[5].initList("DRIFT", 3, List::NodeType::LIST);
  current->next[6].initList("SNAKE", 3, List::NodeType::LIST);
  current->next[7].initList("BACK", &menu);

  current = &menu.next[3].next[1];//initilize COLOUR
  current->next[0].initList("COLOUR", epr.read("COLOUR" + 1), 359);
  current->next[1].initList("AMOUNT", epr.read("AMOUNT" + 2), 600);
  current->next[2].initList("BACK", &menu.next[3]);

  current = &menu.next[3].next[2];//initilize FLOW
  current->next[0].initList("COLOUR", epr.read("COLOUR" + 3), 359);
  current->next[1].initList("AMOUNT", epr.read("AMOUNT" + 4), 600);
  current->next[2].initList("SPEED", epr.read("SPEED" + 5), List::NodeType::VALUE);
  current->next[3].initList("BACK", &menu.next[3]);

  current = &menu.next[3].next[3];//initilize COLOUR CHANGE
  current->next[0].initList("COLOUR FROM", epr.read("COLOUR FROM" + 6), 359);
  current->next[1].initList("COLOUR TO", epr.read("COLOUR TO" + 7), 359);
  current->next[2].initList("AMOUNT", epr.read("AMOUNT" + 8), 600);
  current->next[3].initList("SPEED", epr.read("SPEED" + 9), List::NodeType::VALUE);
  current->next[4].initList("BACK", &menu.next[3]);

  current = &menu.next[3].next[4];//initilize RANDOM
  current->next[0].initList("SPEED", epr.read("SPEED" + 10), List::NodeType::VALUE);
  current->next[1].initList("AMOUNT", epr.read("AMOUNT"+ 11), 600);
  current->next[2].initList("BACK", &menu.next[3]);

  current = &menu.next[3].next[5];//initilize DRIFT
  current->next[0].initList("SPEED", epr.read("SPEED" + 12), List::NodeType::VALUE);
  current->next[1].initList("AMOUNT", epr.read("AMOUNT" + 13), 600);
  current->next[2].initList("BACK", &menu.next[3]);

  current = &menu.next[3].next[6];//initilize SNAKE
  current->next[0].initList("COLOUR", epr.read("COLOUR" + 14), 359);
  current->next[1].initList("AMOUNT", epr.read("AMOUNT" + 15), 600);
  current->next[2].initList("BACK", &menu.next[3]);
  
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








