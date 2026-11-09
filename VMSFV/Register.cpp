#include "Register.h"


    uint8_t Register::getValue(){
    return value;
    }

    void Register::setValue(uint8_t value){
    this->value = value;
    }

    void Register::reset(){
    value=0;
    }


