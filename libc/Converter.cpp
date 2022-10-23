#include "Converter.h"
#include "../drivers/screen.h"

string Converter::Convert (int value) {
    string toReturn = "";
    int_to_ascii(value, toReturn);
    return toReturn;
}