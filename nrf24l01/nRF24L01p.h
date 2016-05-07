
#ifndef nRF24L01p_h
	#define nRF24L01p_h

	#include <regMapCmds.h>
	#include <stdbool.h>
	#include <stdint.h>

	void initFirst();
	
	void csnHigh();
	void csnLow();
	void ceHigh();
	void ceLow();
	void cePulse();
	void testLow();
	void testHigh();
	void enableRX(uint8_t);

	void iterRX(uint8_t *, uint8_t);
	void iterTX(uint8_t *, uint8_t);
	void iterARX(uint8_t *, uint8_t, uint8_t);
	void iterATX(uint8_t *, uint8_t, uint8_t);
	void iterAARX(uint8_t *, uint8_t,uint8_t,uint8_t);
	void iterAATX(uint8_t *, uint8_t,uint8_t,uint8_t);
	uint8_t qtyChars(char *,uint8_t);
	uint8_t getStatus();
	uint8_t getFIFO();
	void dynamicPayload(uint8_t);
	void feature(uint8_t);
	void powerUp();
	void powerDown();
	//void writeReg(uint8_t, uint8_t);
	void writeRegByte(uint8_t, uint8_t);
	void writeReg(uint8_t, char*, uint8_t);
	uint8_t readRegByte(uint8_t);
	void readReg(uint8_t,uint8_t[],uint8_t);
	bool address(uint8_t, char*);
	void addrWidth(uint8_t);
	void irqFunction(uint8_t, bool);
	void primPRX();
	void primPTX();
	uint8_t payLoadWidth();
	void flushRX();
	void flushTX();

//public:
	//nRF24L01p(const uint8_t, const uint8_t);
	void init();
	void TXaddress(char*);
	void RXaddress(char*);
	void channel(uint8_t);
	void dataRate(uint8_t);

	bool available();
	uint8_t bitRead(uint8_t,uint8_t);
	bool read();
	
	int rxPL(char *);
	void iterRX(uint8_t *_adr,uint8_t _num);
	// void rxPL(uint8_t &);
	// void rxPL(char &);
	// void rxPL(int &);
	// void rxPL(unsigned int &);
	// void rxPL(long &);
	// void rxPL(unsigned long &);
	// void rxPL(float &);
	// void rxPL(String &);
	// void rxPL(uint8_t *, uint8_t);
	// void rxPL(char *&);
//	void rxPL(char *, uint8_t);
//	void rxPL(int *, uint8_t);
//	void rxPL(unsigned int *, uint8_t);
//	void rxPL(long *, uint8_t);
//	void rxPL(unsigned long *, uint8_t);
//	void rxPL(float *, uint8_t);
//	void rxPL(uint8_t *, uint8_t, uint8_t);
//	void rxPL(char *, uint8_t, uint8_t);
//	void rxPL(int *, uint8_t, uint8_t);
//	void rxPL(unsigned int *, uint8_t, uint8_t);
//	void rxPL(long *, uint8_t, uint8_t);
//	void rxPL(unsigned long *, uint8_t, uint8_t);
//	void rxPL(float *, uint8_t, uint8_t);
		
		int	txPL(char * );
	// void txPL(uint8_t &);
	// void txPL(char &);
	// void txPL(int &);
	// void txPL(unsigned int &);
	// void txPL(long &);
	// void txPL(unsigned long &);
	// void txPL(float &);
	// int txPL(String);
//	void txPL(uint8_t *, uint8_t);
//	void txPL(char *);
//	void txPL(char *, uint8_t);
//	void txPL(int *, uint8_t);
//	void txPL(unsigned int *, uint8_t);
//	void txPL(long *, uint8_t);
//	void txPL(unsigned long *, uint8_t);
//	void txPL(float *, uint8_t);
//	void txPL(uint8_t *, uint8_t, uint8_t);
//	void txPL(char *, uint8_t, uint8_t);
//	void txPL(int *, uint8_t, uint8_t);
//	void txPL(unsigned int *, uint8_t, uint8_t);
//	void txPL(long *, uint8_t, uint8_t);
//	void txPL(unsigned long *, uint8_t, uint8_t);
//	void txPL(float *, uint8_t, uint8_t);

	bool send(bool );
//};

#endif
