#ifndef REGISTER_H
#define REGISTER_H
#include <cstdint>

class Register
{
    private:

        uint8_t value = 0;

    public:

        uint8_t getValue();
        void setValue(uint8_t value);
        void reset();

};

#endif
