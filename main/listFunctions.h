#include<Arduino.h>


struct ListFunctions{
  public:
  ListFunctions();
  void callFunction(String name);
  bool update(bool first = 0);
  String getFuncName();
  void off();
  
  private:
  String funcName;
  void setTime();
  void setAlarm();
  void back();
};

ListFunctions::ListFunctions(){
  funcName = "";
}


bool ListFunctions::update(bool first = 0){
  if(funcName != ""){
    if(funcName == "SETTIME"){
      if(first) dsp.printTime(time.getHour(), time.getMin());
      else setTime();
    }else if(funcName == "SETALARM"){
      if(first) dsp.printTime(alarm.getHour(), alarm.getMin());
      else setAlarm();
    }else if(funcName == "BACK"){
      back();
    }else if(funcName == "OFF"){
      off();
    }else{
      funcName = "";
      return 0;
    }
    return 1;
  }
  return 0;
}



void ListFunctions::callFunction(String name){
  funcName = name;
  if(name != "OFF") dsp.clear();
  update(1);
}

void ListFunctions::setTime(){
  if(time.set(enc.getEncstate())){
    dsp.printList(*current);
    funcName = "";
  }else if(enc.getEncstate() != 0) 
    dsp.printTime(time.getHour(), time.getMin());
}

void ListFunctions::setAlarm(){
  if(alarm.set(enc.getEncstate(), epr)){
    dsp.printList(*current);
    funcName = "";
  }else if(enc.getEncstate() != 0) 
    dsp.printTime(alarm.getHour(), alarm.getMin());
}

void ListFunctions::back(){
  current = current->getPrev();
  current->setCurrent(0);
  current->setFirst(0);
  if(current != nullptr)
    dsp.printList(*current);
  else
    dsp.printTime(time.getHour(), time.getMin());
  funcName = "";
}

void ListFunctions::off(){funcName = "";}

String ListFunctions::getFuncName(){ return funcName;}















