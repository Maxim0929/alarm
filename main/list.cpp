#include "list.h"

uint8_t List::first = 0;
uint8_t List::current = 0;
uint8_t List::IDcounter = 0;

List::List(){
  this->nodeType = NONE;
  this->name = "";
  this->next = nullptr;
  this->prev = nullptr;
  this->value = 0;
  this->maxValue = 255;
  ID = IDcounter;
}

List::~List(){
  delete[] next;
}


void List::initList(const char* name){
  this->nodeType = FUNCTION;
  this->name = name;
  this->next = nullptr;
  this->prev = nullptr;
  this->value = 0;
  this->maxValue = 255;
  ID = IDcounter;
}

void List::initList(const char* name, List *prev){
  initList(name);
  this->prev = prev;
}

void List::initList(const char* name, uint8_t value, NodeType nodeType){
  initList(name);
  this->nodeType = nodeType;
  if(nodeType == LIST) next = new List[value];
  else IDcounter++;
  this->value = value;
}
void List::initList(const char* name, uint8_t value, int maxValue){
  initList(name);
  this->value = value;
  this->nodeType = VALUE;
  this->maxValue = maxValue;
  IDcounter++;
}



List& List::operator=(const List& other){
  this->name = other.name;
  this->value = other.value;
  this->next = other.next;
  this->nodeType = other.nodeType;
}

void List::update(uint8_t encState, const Display& dsp){
  if(this->nodeType != LIST) return 0;
  bool print = 0;
  bool clear = 0;
  uint8_t last = value - 1;

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

uint8_t List::getCurrent(){return current;}
uint8_t List::getFirst(){return first;}

void List::setCurrent(uint8_t current){List::current = current;}
void List::setFirst(uint8_t first){List::first = first;}

int List::getValue(bool isReal){
  if(isReal) return value; 
  else return round(value / 255.0 * maxValue);
}
void List::setValue(int value){ this->value = value;}

char* List::getName(){return this->name;}
void List::setName(char* name){this->name = name;}

List::NodeType List::getNodetype(){return nodeType;}

uint8_t List::getID(){ return ID;}

int List::getMaxValue(){ return maxValue; }




