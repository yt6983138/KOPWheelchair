// MatrixPS2Controller.h

#ifndef _MATRIXPS2CONTROLLER_h
#define _MATRIXPS2CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

class MatrixPS2Controller {
private:
	uint8_t _rx[6];
	uint8_t _sck_mask, _mosi_mask, _miso_mask, _css_mask;
	volatile uint8_t* _sck_port, * _mosi_port, * _miso_port, * _css_port;
	void SWSPI_BEGIN();
	void SWSPI_END();
	bool init_PS2();
	uint8_t SWSPI_TXRX(uint8_t command);
	uint8_t _ver = 1;

public:
	bool L1, R1, L2, R2, L3, R3, SELECT, START;
	bool UP, RIGHT, DOWN, LEFT, TRIANGLE, CIRCLE, CROSS, SQUARE;
	uint8_t RX, RY, LX, LY;

	MatrixPS2Controller();

	void polling();
	bool begin();
};

#endif

