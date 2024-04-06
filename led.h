struct Led{
  byte red;
  byte green;
  byte blue;

  Led(byte red, byte green, byte blue){
    this->red = red;
    this->blue = blue;
    this->green = green;
  }
};