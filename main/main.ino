//#include "eprom.h"
//#include "encoder.h"
// #include "led.h"
//#include "list.h"
//#include "display.h"
#include "menu.h"
// #include "time.h"



// #define MENULAST 5
// #define EFFECTLAST 7
// enum Func{TIME, SETTIME, SETALARM, NONEFUNC};
// bool setVal(uint8_t encState, const List& list);
// struct ListFunctions;


// Led led;


// Eprom epr;
Menu menu;
MyEncoder enc;

// Time time;
// Alarm alarm;

// Func currentFunc = TIME;
// #include "listFunctions.h"

// ListFunctions listFunctions;

// Value* settingValue = nullptr;
// Metro backTimer = Metro(5000);


void setup() {
  Serial.begin(9600);
  delay(3000);
  menu.init();
}
  //epr.write("c", 9);
  //Serial.println(epr.read("e"));


  //menu.init();

 // dsp.setup(menu.getValue("DISPLAY BRIGHT.")->getValue(0));
  
  //led.init(menu.getValue("LED BRIGHTNESS")->getValue(0), menu.getList("EFFECTS"));

  // time.init();
  // alarm.init(epr);
  // dsp.printTime(time.getHour(), time.getMin());


void loop(){
  if(enc.update()) menu.update(enc.state());
}
// void loop(){
//   if(listFunctions.getFuncName() != "SETTIME"){//update time
//     if(time.update()){
//       if(current == nullptr)
//         dsp.printTime(time.getHour(), time.getMin());
//       if(listFunctions.getFuncName() != "SETALARM")// check alarm when time changed
//         alarm.update(time, led, enc.getEncstate());
//     }
//   }

//    if(alarm.getisAlarm())// update alarm when isAlarm = true
//       alarm.update(time, led, enc.getEncstate());


//   if(enc.update()){
//     backTimer.reset();
//     if(!listFunctions.update()){ // if no function is called

//       if(settingValue == nullptr){ // if no value is setting
//         update();
//       }else{// set Value
//         if(setVal(enc.getEncstate(), *settingValue, current->getCurrent() - current->getFirst())){
//           if(settingValue->getName() == "DISPLAY BRIGHT.") dsp.setBrightness(settingValue->getValue(1)); // update display brightness
//           if(settingValue->getName() == "LED BRIGHTNESS") led.setBrightness(settingValue->getValue(1)); // update display brightness
//           settingValue = nullptr;
//           led.updateStrip(*menu.getList("EFFECTS"), 1);
//         }
//       }
//     }
//   }


//   if(current == nullptr)//when time screen
//     led.updateStrip(*menu.getList("EFFECTS"), 0);

//   if(backTimer.check() && current != nullptr){//go to time screen when nothing happend 5 sec
//     current = nullptr;
//     listFunctions.off();
//     if(settingValue != nullptr){
//       epr.write(settingValue->getName() + settingValue->getID(), settingValue->getValue(1));
//       if(settingValue->getName() == "DISPLAY BRIGHT.") dsp.setBrightness(settingValue->getValue(1));// seperate func
//       if(settingValue->getName() == "LED BRIGHTNESS") led.setBrightness(settingValue->getValue(1));
//       led.updateStrip(*menu.getList("EFFECTS"), 1);
//       settingValue = nullptr;
//     }
//     dsp.clear();
//     dsp.printTime(time.getHour(), time.getMin());
//   }
// }


void update(){

}














// // void ListFunctions::setVal(){

// //       dsp.printList(*current);
// //       led.updateStrip(*menu.next.getPtr("EFFECTS"), 1);
// // }




// bool setVal(uint8_t encState, const Value& value, uint8_t pos){
//   int val = value.getValue(0);
//   int maxValue = value.getMaxValue();
//   switch(encState){
//     case 1:
//       epr.write(value.getName() + value.getID(), value.getValue(1));
//       return 1;
//     break;
//     case 2:
//       if(val == 0) value.setValue(maxValue, 0);
//       else value.setValue(val - 1, 0);
//       dsp.printVal(value.getValue(0), pos);
//     break;
//     case 3:
//       if(val == maxValue) value.setValue(0, 0);
//       else value.setValue(val + 1, 0);
//       dsp.printVal(value.getValue(0), pos);
//     break;
//     case 5:
//       if(val > 9) value.setValue(val - 10, 0);
//       dsp.printVal(value.getValue(0), pos);
//     break;
//     case 6:
//       if(val < maxValue - 9) value.setValue(val + 10, 0);
//       dsp.printVal(value.getValue(0), pos);
//     break;
//   }
//   return 0;
// }










