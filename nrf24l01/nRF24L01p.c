
#include <regMapCmds.h>
#include <nRF24L01p.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include <string.h>

#define CSN_PORT GPIOD
#define CE_PORT GPIOD
#define CSN_PIN GPIO_PIN_9
#define CE_PIN GPIO_PIN_8

#define TIMEOUT 100
	
	extern SPI_HandleTypeDef hspi2;
	extern UART_HandleTypeDef huart2;
	
	bool _prim_rx;
	bool _prim_tx;
	bool _init;
	uint8_t _status;
	uint8_t _fifo;
//	uint8_t _csn_pin,_ce_pin;
//	uint8_t _csn_port,_ce_port;
	uint8_t _rxIndex;
	uint8_t _txIndex;
	uint8_t _rxPayLoad[32];
	uint8_t _txPayLoad[32];
	
//void initFirst(const uint8_t csn_port,const uint8_t csn_pin,const uint8_t ce_port,const uint8_t ce_pin){
void initFirst(){	
//	_csn_port = csn_port;
//	_csn_pin = csn_pin;

//	_ce_port = ce_port;
//	_ce_pin = ce_pin;

	// pinMode(_csn,OUTPUT); //not require because it already set in STMCube
	// pinMode(_ce,OUTPUT);
	csnHigh();
	ceLow();
	_txIndex=0;
	_rxIndex=0;
	_prim_rx=false;
	_prim_tx=false;
	_init=false;
}

void channel(uint8_t _RFCh){
	testLow();
	if(_RFCh>127){
		_RFCh=127;
	}
	writeRegByte(RF_CH, _RFCh);
	testHigh();
	
}

		void testLow(){
			if(_init==true && _prim_rx==true)ceLow();
		}
		void testHigh(){
			if(_init==true && _prim_rx==true)ceHigh();
		}
		
		void MY_SPI_Tranmit_Byte(uint8_t data){
	
				HAL_SPI_Transmit(&hspi2, &data, 1, TIMEOUT);
				while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
		}
		
		void writeRegByte(uint8_t _reg, uint8_t _data){
			csnLow();
			//_status=SPI.transfer(W_REGISTER | (_reg & REGISTERS));
			//HAL_SPI_MspInit(&hspi2);
			
//			uint8_t temp = W_REGISTER | (_reg & REGISTERS) ;
//			HAL_SPI_Transmit(&hspi2, (uint8_t *) &temp, 1, TIMEOUT);
//			while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX)
//			{
//			}
//			
//			HAL_SPI_Transmit(&hspi2, (uint8_t *) &_data, 1, TIMEOUT);
//			while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX)
//			{
//			}
			
			MY_SPI_Tranmit_Byte( W_REGISTER | (_reg & REGISTERS));
			MY_SPI_Tranmit_Byte(_data);
			
			csnHigh();
		}
		
				void csnHigh(){
					HAL_GPIO_WritePin( CSN_PORT , CSN_PIN , GPIO_PIN_SET);
				}
				void csnLow(){
					HAL_GPIO_WritePin( CSN_PORT , CSN_PIN , GPIO_PIN_RESET);
				}
				void ceHigh(){
					HAL_GPIO_WritePin( CE_PORT , CE_PIN , GPIO_PIN_SET);
				}
				void ceLow(){
					HAL_GPIO_WritePin( CE_PORT , CE_PIN , GPIO_PIN_RESET);
				}
				
		uint8_t readRegByte(uint8_t _reg){
			
			csnLow();
			
			uint8_t readValue[32];
			uint8_t temp = _reg & REGISTERS ;
			
		//	HAL_SPI_MspInit(&hspi2);
			
			HAL_SPI_TransmitReceive(&hspi2, &temp, readValue, 1, TIMEOUT);
			while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
			
			temp = 0;
			HAL_SPI_TransmitReceive(&hspi2, &temp, readValue, 1, TIMEOUT);
			while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
			
			
			while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET);
			HAL_UART_Transmit(&huart2, (uint8_t*) readValue, 1, 500);
			HAL_Delay(300); 

			
			csnHigh();
			return readValue[0];
			
		}
		
		void readReg(uint8_t _reg,uint8_t bufferData[],uint8_t sizeRead){
					csnLow();
			
					uint8_t readValue[32];
					uint8_t temp = _reg & REGISTERS ;
					
				//	HAL_SPI_MspInit(&hspi2);
					
					HAL_SPI_TransmitReceive(&hspi2, &temp, readValue, 1, TIMEOUT);
					while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX)
					{
					}
					
					HAL_SPI_Receive(&hspi2, readValue, sizeRead , TIMEOUT);
					while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
				
//					while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET);
//					HAL_UART_Transmit(&huart2, (uint8_t*) readValue, sizeRead, 500);
//					HAL_Delay(300); 
				
					
					csnHigh();
		}
		
void RXaddress(char* _address){
		testLow();
		address(RX_ADDR_P1,_address);
		testHigh();
		_prim_rx=true;
}

void TXaddress(char* _address){
	address(RX_ADDR_P0,_address);
	address(TX_ADDR, _address);
	_prim_tx=true;
}

		bool address(uint8_t _reg,char* _addr){
				uint8_t _aw=0;
			
				for(char *it=_addr;*it;it++){  // find width of address string
					_aw++;
				}
				
				if(_aw>=3 && _aw<=5){ // maximum 5 character
					addrWidth(_aw); 
					writeReg(_reg,_addr,_aw);
					return true;
				}else{
					return false;
				}
		}
		
					void addrWidth(uint8_t _addrw){
							_addrw-=2;   
							if(_addrw>=1 && _addrw<=3){ // change in 2 bit
								writeRegByte(SETUP_AW,_addrw);
							}
					}
					
					void writeReg(uint8_t _reg, char* _data, uint8_t _numBytes){
								csnLow();
						
								//_status=SPI.transfer(W_REGISTER | (_reg & REGISTERS));
						
								uint8_t temp = W_REGISTER | (_reg & REGISTERS);
								HAL_SPI_TransmitReceive(&hspi2, &temp, &_status, 1, TIMEOUT);
								while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
							
								for(uint8_t it=0; it<_numBytes; it++){
										//SPI.transfer(_data[it]);
									  temp = _data[it];
										HAL_SPI_Transmit(&hspi2, &temp, 1, TIMEOUT);
										while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
								}
								
								csnHigh();
					}


void init(){
		flushRX();
		flushTX();
		dynamicPayload(P0|P1);
		feature(DPL|DYN_ACK);
  	writeRegByte(SETUP_RETR,0x2F);
		writeRegByte(STATUS,0x70);
		powerUp();
		if(_prim_rx && _prim_tx){
			enableRX(P1|P0);
			primPRX();
			ceHigh();
		}else if(_prim_rx && !_prim_tx){
			enableRX(P1);
			primPRX();
			ceHigh();
		}else if(!_prim_rx && _prim_tx){
			enableRX(P0);
			primPTX();
		}
		_init=true;
}

			void flushRX(){
					csnLow();
					//SPI.transfer(FLUSH_RX);
					MY_SPI_Tranmit_Byte(FLUSH_RX);
					csnHigh();
			}
			void flushTX(){
					csnLow();
					//SPI.transfer(FLUSH_TX);
					MY_SPI_Tranmit_Byte(FLUSH_TX);
					csnHigh();
			}
			void dynamicPayload(uint8_t _dyn){
					_dyn &= 0x3F;
					writeRegByte(DYNPD, _dyn);
			}
			void feature(uint8_t _fea1){
					_fea1 &= 0x07;
					writeRegByte(FEATURE, _fea1);
			}
			void powerUp(){
					uint8_t buf=readRegByte(CONFIG);
					writeRegByte(CONFIG, buf | 0x02);
					//delayMicroseconds(4500);
					HAL_Delay(45);
			}
			void enableRX(uint8_t _pipes){
					writeRegByte(EN_RXADDR, _pipes);
			}
			void primPRX(){
					uint8_t buf=readRegByte(CONFIG);
					writeRegByte(CONFIG,buf | 0x01);
			}
			void primPTX(){
				uint8_t buf=readRegByte(CONFIG);
				writeRegByte(CONFIG,buf & 0xFE);
			}

bool available(){
	if(_prim_rx==false){
		_rxIndex=32;
		return false;
	}
	getFIFO();
	while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET);
	HAL_UART_Transmit(&huart2, (uint8_t*)&_fifo , 1 , 500);
	HAL_Delay(300);
	if(bitRead(_fifo,0)==0){
		if(payLoadWidth()>32){
			flushRX();
			getFIFO();
			if(bitRead(_fifo,0)==0){


				writeRegByte(STATUS,0x40);
				return true;
			}else{
				return false;
			}
		}else{
			return true;
		}
	}else{
		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET);
		HAL_UART_Transmit(&huart2, (uint8_t*)"DDDD" , 4 , 500);
		HAL_Delay(300);
		return false;
	}	
}
			uint8_t bitRead(uint8_t number , uint8_t numBitRead){
						if(( number & (1<<numBitRead) ) == (1<<numBitRead) ){
							  return 1;
						}else{
								return 0;
						}
			}
			uint8_t getFIFO(){
				_fifo=readRegByte(FIFO_STATUS);
				return _fifo;
			}
		
			uint8_t payLoadWidth(){
				csnLow();
				//SPI.transfer(R_RX_PL_WID);
			//	uint8_t _width=SPI.transfer(0x00);
				
				MY_SPI_Tranmit_Byte(R_RX_PL_WID);
				
				uint8_t _width , temp = 0x00;
				HAL_SPI_TransmitReceive(&hspi2, &temp, &_width, 1, TIMEOUT);
				while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
				
				csnHigh();
				return _width;
			}

bool read(){
			if(_prim_rx==false){
				_rxIndex=32;
				return false;
			}
			_rxIndex=0;
			for(int it=0;it<32;it++){
				_rxPayLoad[it]=0;
			}
			uint8_t _plw=payLoadWidth();
			csnLow();
			//SPI.transfer(R_RX_PAYLOAD);
			MY_SPI_Tranmit_Byte(R_RX_PAYLOAD);
			
			for(int it=0;it<_plw;it++){
					//_rxPayLoad[it]=SPI.transfer(0x00);
					uint8_t temp = 0x00;
					HAL_SPI_TransmitReceive(&hspi2, &temp, &_rxPayLoad[it], 1, TIMEOUT);
					while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
			}
			csnHigh();
			writeRegByte(STATUS,1<<6);
			
//			while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET);
//			HAL_UART_Transmit(&huart2, (uint8_t*) _rxPayLoad , payLoadWidth() , 500);
//			HAL_Delay(300); 
			
			return true;
}

int rxPL(char *_sen){
	uint8_t _pos=_rxIndex;
	int it;
	for(it=0;(it<32) && (_rxIndex<32);it++){		
			if(_rxPayLoad[it+_pos]=='\0'){
				_rxIndex++;
				break;
			}else{
				_sen[it] = (char)_rxPayLoad[it+_pos];
				_rxIndex++;
			}
	}
	//_sen[it] = 0;
	
	char tempStr[30];
	
	sprintf(tempStr,"[ %d %d]",it,_pos);
		while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)==RESET);
		HAL_UART_Transmit(&huart2, (uint8_t*) tempStr , strlen(tempStr) , 500);
		HAL_Delay(300); 
	
	  return _rxIndex;
}

int txPL(char* _payLoad){
		int _sizeS= strlen(_payLoad);
		int _qty=0;
		int i;
		uint8_t _pos=_txIndex;
		for(i=0;(i<_sizeS) && (_txIndex<32);i++){
			_txPayLoad[_pos + i]=_payLoad[i];
			_txIndex++;
			_qty++;
		}
		if(_txIndex<32 && (_sizeS==i)){
			_txPayLoad[_pos + _sizeS]='\0';
			_txIndex++;
		}
		return _qty;
}

bool send(bool _modeSend){
			if(_prim_tx==false){
				_txIndex=32;
				return false;
			}
			if(_prim_rx==true){
				ceLow();
				primPTX();
			}
			csnLow();
			if(_modeSend==false || (_prim_rx && _prim_tx)){
						//SPI.transfer(W_TX_PAYLOAD);
						MY_SPI_Tranmit_Byte(W_TX_PAYLOAD);
			}else{
						//SPI.transfer(W_TX_PAYLOAD_NOACK);
						MY_SPI_Tranmit_Byte(W_TX_PAYLOAD_NOACK);
			}
			for(int i=0;i<_txIndex;i++){
						//SPI.transfer(_txPayLoad[i]);
						MY_SPI_Tranmit_Byte(_txPayLoad[i]);
			}
			csnHigh();
			cePulse();
			
				
			//unsigned long tmC1=micros();
			while(1){
				if(bitRead(getStatus(),5)){
					//writeReg(STATUS,_status|(1<<6));
					writeRegByte(STATUS,_status|(1<<5));
					writeRegByte(STATUS,_status|(1<<4));
					break;
				}
				if(bitRead(getStatus(),4)){
					writeRegByte(STATUS,_status|(1<<4));
					cePulse();
				}
		//		unsigned long tmC2=micros();
				
					//don't check error because Hard to get time in stm32		
				
//				if(tmC2-tmC1>2500000L){
//					//Serial.println("send(SLOW) returned false");
//					//Serial.println("connection lost ");
//					return false;
//				}
			}
			_txIndex=0;
			if(_prim_rx==true){
				primPRX();
				ceHigh();
			}
			return true;
}
			
			void cePulse(){
				ceHigh();
				HAL_Delay(1); // true value is 10 microseconds
				ceLow();
			}
			
			uint8_t getStatus(){
				return readRegByte(STATUS);
			}