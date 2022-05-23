//when using wifi/2.4GHz function, 
//ADC2 will be disable for analog read/write
//digital read/write is fine

//avoid to use 36(VP), 39(VN), 12(VDD_Flash), 6~11(flash memory),
//0(boot), 15(LOG), 5(SDIO)

#define Ljoystick_x_pin 32
#define Ljoystick_y_pin 33
#define L1_pin  2
#define L2_pin  4

#define up_pin 25
#define left_pin 26
#define down_pin 27 
#define right_pin 14

#define Rjoystick_x_pin 34
#define Rjoystick_y_pin 35
#define R1_pin  23
#define R2_pin  22

#define triangle_pin 19
#define circle_pin 21
#define square_pin 17
#define cross_pin 18

#define question_pin 13
#define estop_pin 16
