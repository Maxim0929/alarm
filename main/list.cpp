#include "list.h"
#include "eprom.h"

// value struct
uint8_t Value::IDcounter = 0;

Value::Value(){
  this->name = "";
  this->value = 0;
  this->maxValue = 255;
  ID = IDcounter;
  IDcounter++;
}

Value::Value(String name, uint8_t value){
  this->name = name;
  this->value = value;
  this->maxValue = 255;
  ID = IDcounter;
  IDcounter++;
}

String Value::getName(){
  return name;
}
void Value::setName(String name){
  this->name = name;
}


void Value::setValue(int value, bool oneByte){
  if(oneByte) this->value = round(value / 255.0 * maxValue);
  else this->value = value;
}

int Value::getValue(bool oneByte){
  if(oneByte) return round(value * 255.0 / maxValue);
  else return value;
}

void Value::setMaxValue(int maxValue){
  this->maxValue = maxValue;
}
int Value::getMaxValue(){ return maxValue; }

uint8_t Value::getID(){
  return ID;
}

//list struct
uint8_t List::first = 0;
uint8_t List::current = 0;

List::List(){
  values = nullptr;
  lists = nullptr;
  names = nullptr;

  valuesNumber = 0;
  listsNumber = 0;
  namesNumber = 0;

  prev = nullptr;

  name = "";
}

List::~List(){
  Serial.println("List destroctor was called");
  delete[] names;
  delete[] values;
  delete[] lists;
}

List& List::operator=(List& other){
  this->lists = other.lists;
  this->values = other.values;
  this->names = other.names;

  this->listsNumber = listsNumber;
  this->valuesNumber = valuesNumber;
  this->namesNumber = namesNumber;

  this->name = other.name;

  other.lists = nullptr;
  other.values = nullptr;
  other.names= nullptr;
  Serial.println("be carefull operator = was called!!!");
  return *this;

}



void List::initName(String name, String* names, uint8_t namesNumber){
  this->names = names;
  this->namesNumber = namesNumber;
  this->name = name;
}

void List::initList(uint8_t listsNumber){
  if(listsNumber + valuesNumber <= namesNumber){
    this->listsNumber = listsNumber;
    this->lists = new List[listsNumber];
  }else{
    Serial.println("In function List::initList: valueNumber + listNumber can not be bigger than namesNumber");
  }
}

void List::initVal(uint8_t valuesNumber, Eprom& epr, int maxValue[]){
  if(valuesNumber + listsNumber <= namesNumber){
    this->valuesNumber = valuesNumber;

    values = new Value[valuesNumber];

    for(int i = 0; i < valuesNumber; i++){
      values[i].setName(names[i + listsNumber]);
      values[i].setMaxValue(maxValue[i]);
      values[i].setValue(epr.read(names[i + listsNumber] + String(values[i].getID())), 1);
    }

  }else{
    Serial.println("In function List::initVal: valueNumber + listNumber can not be bigger than namesNumber");
  }
}


void List::initAll(String name, String* names, uint8_t namesNumber, uint8_t listsNumber, uint8_t valuesNumber, Eprom& epr, int maxValue[]){
  this->initName(name, names, namesNumber);
  
  if(listsNumber > 0) this->initList(listsNumber);
  if(valuesNumber > 0) this->initVal(valuesNumber, epr, maxValue);
  
}


void List::update(uint8_t encState, const Display& dsp){
  bool print = 0;
  bool clear = 0;
  uint8_t last = namesNumber - 1;

  if(encState == 2){
    if(current == 0){
      current = last;
      first = last - 3;
      clear = 1;
    }else if(current != first){
      current--;
    }else{
      current--;
      first--;
      clear = 1;
    }
    print = 1;
  }else if(encState == 3){
    if(current == last){
      current = 0;
      first = 0;
      clear = 1;
    }else if(current != first + 3){
      current++;
    }else{
      current++;
      first++;
      clear = 1;
    }
    print = 1;
  }
  
  if(print){
    if(last < 4) first = 0; 
    dsp.printList(*this, clear);
  }
}



void List::setPrev(List *prev){this->prev = prev; }
List* List::getPrev(){return prev;Serial.println(prev->getName());}


List* List::getList(String name){
  for(int i = 0; i < listsNumber; i++){
    if(names[i] == name) return &lists[i];
  }
  Serial.println(name);
  Serial.println("getList() returned nullptr");
  delay(200);
  return nullptr;
}

Value* List::getValue(String name){
  for(int i = 0; i < valuesNumber; i++){
    if(names[i + listsNumber] == name) return &values[i];
  }
  Serial.println("getValue() returned nullptr");
  delay(200);
  return nullptr;
}


List* List::getLists(){
  if(listsNumber > 0) return lists;
  else return nullptr;
}


Value* List::getValues(){
  if(valuesNumber > 0) return values;
  else return nullptr;
}

String* List::getNames(){return names;}

uint8_t List::getNamesNumber(){return namesNumber;}

String List::getName(){return name;}

uint8_t List::getListsNumber(){return listsNumber;}

uint8_t List::getValuesNumber(){return valuesNumber;}


uint8_t List::getCurrent(){return current;}
uint8_t List::getFirst(){return first;}

void List::setCurrent(uint8_t current){List::current = current;}
void List::setFirst(uint8_t first){List::first = first;}











