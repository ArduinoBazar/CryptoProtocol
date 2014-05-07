


#include <SPI.h>
#include "WirelessProtocolCrypto.h"
#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "printf.h"
#include <avr/wdt.h>


#define SCMD_SEND_DATA 0xf1
#define SCMD_RECIVE_DATA 0xf2
#define SCMD_ADD_LISTENING_PIPE 0xf3
#define SCMD_REMOVE_LISTENING_PIPE 0xf4

#define DATA_BUFER_SIZE 256

struct SerialCommand
{
	uint8_t cmd;
	uint64_t Pipe;
	uint16_t DataSize;
	long Timeout;
	uint8_t CryptoKey[32];
};

struct SerialResult
{
	uint8_t cmd;
	uint8_t result;
	uint64_t Pipe;
	uint16_t DataSize;
};

WirelessProtocolCrypto *WP;
void setup()
{

	Serial.begin(57600);
	printf_begin();
	WP = new WirelessProtocolCrypto(9, 10);
}

#define DEBUG3
void bin_dump(char *buf, int count)
{
#ifdef DEBUG3
	for (uint16_t i = 0; i < count; i++)
	{
		Serial.print(buf[i], HEX);
	}
	Serial.println();
#endif
}

void loop()
{
	SerialCommand scmd;
	SerialResult sr;

	uint8_t tmmm;

	uint8_t DataBuffer[DATA_BUFER_SIZE];

	while (Serial.available() > 0)
		Serial.readBytes(&tmmm, 1);
	delay(200);
	DBGPRINT("Iter! %d", sizeof(SerialCommand));
	while (Serial.available() < sizeof(SerialCommand)) ;

	if (Serial.available() >= sizeof(SerialCommand))
	{
		memset(DataBuffer, 0, DATA_BUFER_SIZE);
		size_t Readed = Serial.readBytes((char*)&scmd, sizeof(SerialCommand));
		DBGPRINT("Readed: %d", Readed);
		
		DBGPRINT("CMD: %hhu, Pipe: %lx%lx, DataSize: %hu, Timeout: %lu, AESKey: ", scmd.cmd, ((long*)(&(scmd.Pipe)))[0], ((long*)(&(scmd.Pipe)))[1], scmd.DataSize, scmd.Timeout);
		//bin_dump((char*)(scmd.CryptoKey), 32);
		if (scmd.cmd == SCMD_SEND_DATA)
		{
			size_t readed = 0;
			while (readed < scmd.DataSize)
			{
				readed += Serial.readBytes(&(DataBuffer[readed]), min(Serial.available(), 64));
			}
			uint8_t res = (uint8_t)WP->sendData(DataBuffer, scmd.DataSize, scmd.Pipe, &(scmd.CryptoKey[0]), scmd.Timeout);
			sr.cmd = SCMD_SEND_DATA;
			sr.result = res;
			sr.Pipe = scmd.Pipe;
			sr.DataSize = 0;
			Serial.write((uint8_t*)&sr, sizeof(SerialResult));
		}
		else if (scmd.cmd == SCMD_RECIVE_DATA)
		{
			uint16_t len;
			uint64_t Pipe;
			uint8_t res = (uint8_t)WP->reciveData(DataBuffer, DATA_BUFER_SIZE, &len, &Pipe, scmd.Timeout);
			sr.cmd = SCMD_RECIVE_DATA;
			sr.result = res;
			sr.Pipe = Pipe;
			sr.DataSize = len;
		//	bin_dump((char*)DataBuffer, len);
			Serial.write((uint8_t*)&sr, sizeof(SerialResult));
			Serial.flush();
			if (res == ERROR_SUCCESS)
				for (int i = 0; i < len; i++)
				{
					Serial.write(DataBuffer[i]);
					Serial.flush();
				}
		}
		else if (scmd.cmd == SCMD_ADD_LISTENING_PIPE)
		{
			uint8_t res = (uint8_t)WP->addListeningPipe(scmd.Pipe, &(scmd.CryptoKey[0]));
			sr.cmd = SCMD_ADD_LISTENING_PIPE;
			sr.result = res;
			Serial.write((uint8_t*)&sr, sizeof(SerialResult));
		}
		else if (scmd.cmd == SCMD_REMOVE_LISTENING_PIPE)
		{
			uint8_t res = (uint8_t)WP->removeListeningPipe(scmd.Pipe);
			sr.cmd = SCMD_REMOVE_LISTENING_PIPE;
			sr.result = res;
			Serial.write((uint8_t*)&sr, sizeof(SerialResult));
		}
	
	}
}
