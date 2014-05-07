// 
// 
// 

#include "WirelessProtocolCrypto.h"

WirelessProtocolCrypto::WirelessProtocolCrypto(int cepin, int cspin)
{
	DBGPRINT("StartInit");

		for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	
		reciving_pipes_state[i] = false;

	radio = new RF24(cepin, cspin);
	radio->begin();

#ifdef DEBUG
	radio->printDetails();
#endif

	DBGPRINT("EndInit");
}



WirelessProtocolCrypto::~WirelessProtocolCrypto()
{
	delete radio;
}

int WirelessProtocolCrypto::addListeningPipe(uint64_t Pipe, uint8_t *AesKey)
{
	for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	{
		if (!reciving_pipes_state[i])
		{
			DBGPRINT2("Start adding pipe");
			reciving_pipes_state[i] = true;
			reciving_pipes[i] = Pipe;
			aes256_init(&(AESContext[i]), AesKey);
			DBGPRINT2("Added reciving pipe");
			return ERROR_SUCCESS;
		}
	}

	DBGPRINT2("Free pipe not found!");
	return ERROR_NO_FREE_PIPE;
}

int WirelessProtocolCrypto::removeListeningPipe(uint64_t Pipe)
{
	for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	{
		if (reciving_pipes[i] == Pipe && reciving_pipes_state[i])
		{
			reciving_pipes_state[i] = false;
			aes256_done(&(AESContext[i]));
			DBGPRINT("Removed reciving pipe");
			return ERROR_SUCCESS;
		}
	}

	DBGPRINT("Pipe not found!");
	return ERROR_PIPE_NOT_FOUND;
}



void WirelessProtocolCrypto::bin_dump(char *buf, int count)
{
#ifdef DEBUG
	for (uint16_t i = 0; i < count; i++)
	{
		Serial.print(buf[i], HEX);
	}
	Serial.println();
#endif
}

void WirelessProtocolCrypto::CryptoWrite32(uint8_t *Buf)
{
	memcpy(CryptoBuf, Buf, 32);
	aes256_encrypt_ecb(&WriteAESContext, CryptoBuf);
	radio->write(CryptoBuf, 32);
}

void WirelessProtocolCrypto::CryptoReWrite32()
{
	radio->write(CryptoBuf, 32);
}
void WirelessProtocolCrypto::CryptoRead32(uint8_t PipeInd, uint8_t *Buf)
{
	memset(CryptoBuf, 0, 32);
	radio->read(CryptoBuf, 32);
	aes256_decrypt_ecb(&(AESContext[PipeInd]), CryptoBuf);
	memset(Buf, 0, 32);
	memcpy(Buf, CryptoBuf, 32);
}


int WirelessProtocolCrypto::sendData(byte* data, uint16_t size, uint64_t Pipe, uint8_t *AesKey, uint16_t SendTimeout)
{
	ProtocolMessage PM;

	if (size % 30 == 0)
		PM.AllLength = size / 30;
	else
		PM.AllLength = size / 30 + 1;


	uint8_t AllLenTmp = PM.AllLength;
	aes256_init(&WriteAESContext,  AesKey);
	radio->stopListening();
	radio->openWritingPipe(Pipe);


	DBGPRINT("Send data size: %hu, package count: %hu", size, PM.AllLength);

	unsigned long lastSuccSend = millis();

	for (uint8_t i = 0; i < PM.AllLength; i++)
	{
		if (millis() - lastSuccSend > SendTimeout)
		{
			DBGPRINT2("All send timeout");
			return ERROR_TIMEOUT;
		}

		DBGPRINT2("Trying send package: %hu", i);
		PM.AllLength = AllLenTmp;
		PM.CurrentNum = i;
		memcpy(PM.data, &(data[i*30]), min(30, size - 30*i));
		bin_dump((char *)(PM.data), 30);
		CryptoWrite32((uint8_t*)&PM);
		long wait_start = millis();

		bool ackR = false;
		while ( !(ackR = radio->available()) && (millis() - wait_start) < PACKET_TIMEOUT)
		{
			radio->stopListening();
			CryptoReWrite32();
			radio->startListening();
			delay(30);
		}
			
	
		if (ackR)
		{
			uint8_t message_count;
			radio->read(&message_count,sizeof(message_count));
			DBGPRINT2("Reciving ack package %hu", message_count);
			if (message_count != i)
			{
				DBGPRINT2("Failed send package: %hu", i);
				i--;
			}
			else
			{
				DBGPRINT2("Packet send");
				lastSuccSend = millis();
			}
		}
		else
		{
			DBGPRINT2("Timeout sending package: %hu", i);
			i--;
		}
	}



	DBGPRINT2("Send complete");



	return ERROR_SUCCESS;
}

int WirelessProtocolCrypto::reciveData(byte* data, uint16_t MaxSize, uint16_t* size, uint64_t* Pipe, uint16_t ReciveTimeout)
{
	for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	{
		if (reciving_pipes_state[i])
		{
			DBGPRINT2("Waiting pipe %d", i);
			*Pipe = reciving_pipes[i];
			
			radio->openReadingPipe(1, *Pipe);
			radio->openWritingPipe(*Pipe);
			radio->startListening();

			long startReciveTime = millis();
			bool rC = false;
			while ( !(rC = radio->available()) && (millis() - startReciveTime) < PACKET_TIMEOUT ) ;

			if (rC)
			{
				DBGPRINT2("Start reciving, pipe %d", i);
				CryptoRead32(i, buf);
				
				ProtocolMessage *PM = (ProtocolMessage*)buf;
				radio->stopListening();
				radio->write(&(PM->CurrentNum), sizeof(uint8_t));
				
				//radio->writeAckPayload(1, &(PM.CurrentNum), sizeof(uint8_t));

				DBGPRINT2("Package count %d", PM->AllLength);
				if (PM->CurrentNum != 0)
					continue;

				
				
				

				*size = PM->AllLength*30;
				if (*size > MaxSize)
					return ERROR_NO_ENAUGHT_MEMORY;

				DBGPRINT("Memory copy");
			    memcpy(data, PM->data, 30);

				DBGPRINT2("Data size %d", *size);
				long lastSuccRecv = millis();
				for (uint8_t j = 1; j < PM->AllLength; j++)
				{


					if (millis() - lastSuccRecv > ReciveTimeout)
					{
						DBGPRINT2("Package %d timeout", PM->CurrentNum);
						return ERROR_TIMEOUT;
					}

					startReciveTime = millis();
					radio->startListening();
					while ( !(rC = radio->available()) && (millis() - startReciveTime) < PACKET_TIMEOUT ) ;
			
					if (rC)
					{
						CryptoRead32(i, buf);
						radio->stopListening();
						PM = (ProtocolMessage*)buf;
						
						if (PM->CurrentNum < j)
						{
							bool ackres = radio->write(&(PM->CurrentNum), sizeof(uint8_t));
							j--;
							DBGPRINT2("Duplicate packet %d, ackres: %d", PM->CurrentNum, ackres);
							continue;
						}
						else if (PM->CurrentNum > j)
						{
							DBGPRINT2("Radio transmitt error on package %d", PM->CurrentNum);
							return ERROR_RADIO_TRANSMITT;
						}


						
						radio->write(&j, sizeof(uint8_t));
						memcpy(&(data[j*30]), PM->data, 30);
						bin_dump((char *)(PM->data), 30);
						lastSuccRecv = millis();
						DBGPRINT2("Recived packege %d", j);
					}
					else
					{
						j--;
					}

				}

				

					while ( !(rC = radio->available()) && (millis() - startReciveTime) < PACKET_TIMEOUT ) 
						radio->write(&(PM->CurrentNum), sizeof(uint8_t));
			



				DBGPRINT2("Recive complete");
				return ERROR_SUCCESS;
			}
			else
			{
				DBGPRINT2("No data");
				continue;
			}
			
		}
	}

	return ERROR_NO_ACTIVE_PIPE;
}

