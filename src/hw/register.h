#ifndef hw_register_h
#define hw_register_h

typedef volatile unsigned char hw_register_value_type;
typedef hw_register_value_type* hw_register;

#define HW_REGISTER(address) ((hw_register) address)

#endif // ifndef hw_register_h
