// WirelssProtocol.h

#ifndef _WIRELSSPROTOCOLCRYPTO_h
#define _WIRELSSPROTOCOLCRYPTO_h

#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>

#include "aes256.h"


#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//#define DEBUG

#ifdef DEBUG
#define DBGPRINT(data, ...) printf("DBGPINT: " data " at line %d\n\r", ##__VA_ARGS__, __LINE__); Serial.flush();
#else
#define DBGPRINT(data, ...)
#endif

//#define DEBUG2

#ifdef DEBUG2
#define DBGPRINT2(data, ...) printf("DBGPINT: " data " at line %d\n\r", ##__VA_ARGS__, __LINE__); Serial.flush();
#else
#define DBGPRINT2(data, ...)
#endif

#define RECIVING_PIPES_MAXCOUNT 5

#define ERROR_SUCCESS 1
#define ERROR_NO_FREE_PIPE -1
#define ERROR_PIPE_NOT_FOUND -2
#define ERROR_TIMEOUT -3
#define ERROR_NO_ACTIVE_PIPE -4
#define ERROR_NO_DATA -5 
#define ERROR_RADIO_TRANSMITT -6
#define ERROR_NO_ENAUGHT_MEMORY -7
#define DEFAULT_TIMEOUT 4000
#define PACKET_TIMEOUT 1000


class WirelessProtocolCrypto
{

private:
	// Пайпы для прослушки
	uint64_t reciving_pipes[RECIVING_PIPES_MAXCOUNT];
	// Статус пайпов для прослушки
	bool reciving_pipes_state[RECIVING_PIPES_MAXCOUNT];
	RF24* radio;

	uint8_t buf[32];


	struct ProtocolMessage
	{
		// Текущий номер сообщения
		uint8_t CurrentNum;
		// Полная длинна
		uint8_t AllLength;
		// Данные
		uint8_t data[30];
	};
	
	void bin_dump(char *buf, int count);
	void CryptoWrite32(uint8_t *Buf);
	void CryptoReWrite32();
	void CryptoRead32(uint8_t PipeInd, uint8_t *Buf);

	aes256_context AESContext[RECIVING_PIPES_MAXCOUNT];
	aes256_context WriteAESContext;
	uint8_t CryptoBuf[32];
public:
	// Иницализация
	WirelessProtocolCrypto(int cepin, int cspin);
	~WirelessProtocolCrypto();


	
	// Добавление пайпа для прослушивания
	int addListeningPipe(uint64_t Pipe, uint8_t *AesKey);
	// Удаление пайпа для прослушивания
	int removeListeningPipe(uint64_t Pipe);

	// Отправка данных
	int sendData(byte* data, uint16_t size, uint64_t Pipe, uint8_t *AesKey, uint16_t SendTimeout = DEFAULT_TIMEOUT);
	// Приём данных. Возвращает начало данных, длинну, номер пайпа
	int reciveData(byte* data, uint16_t MaxSize, uint16_t* size, uint64_t* Pipe, uint16_t ReciveTimeout = DEFAULT_TIMEOUT);
};



#endif

