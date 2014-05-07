// 
// 
// 

#include "WirelessProtocol.h"



WirelessProtocol::WirelessProtocol(int cepin, int cspin)
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

WirelessProtocol::~WirelessProtocol()
{

	delete radio;
}

int WirelessProtocol::addListeningPipe(uint64_t Pipe)
{
	for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	{
		if (!reciving_pipes_state[i])
		{
			reciving_pipes_state[i] = true;
			reciving_pipes[i] = Pipe;
			DBGPRINT("Added reciving pipe");
			return ERROR_SUCCESS;
		}
	}

	DBGPRINT("Free pipe not found!");
	return ERROR_NO_FREE_PIPE;
}

int WirelessProtocol::removeListeningPipe(uint64_t Pipe)
{
	for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	{
		if (reciving_pipes[i] == Pipe && reciving_pipes_state[i])
		{
			reciving_pipes_state[i] = false;
			DBGPRINT("Removed reciving pipe");
			return ERROR_SUCCESS;
		}
	}

	DBGPRINT("Pipe not found!");
	return ERROR_PIPE_NOT_FOUND;
}
void WirelessProtocol::bin_dump(char *buf, int count)
{
#ifdef DEBUG
	for (uint16_t i = 0; i < count; i++)
	{
		Serial.print(buf[i], HEX);
	}
	Serial.println();
#endif
}


int WirelessProtocol::sendData(byte* data, uint16_t size, uint64_t Pipe, uint16_t SendTimeout)
{
	ProtocolMessage PM;

	if (size % 30 == 0)
		PM.AllLength = size / 30;
	else
		PM.AllLength = size / 30 + 1;

	radio->stopListening();
	radio->openWritingPipe(Pipe);


	DBGPRINT("Send data size: %hu, package count: %hu", size, PM.AllLength);

	unsigned long lastSuccSend = millis();

	for (uint8_t i = 0; i < PM.AllLength; i++)
	{
		if (millis() - lastSuccSend > SendTimeout)
		{
			DBGPRINT("All send timeout");
			return ERROR_TIMEOUT;
		}

		DBGPRINT("Trying send package: %hu", i);
		PM.CurrentNum = i;
		memcpy(PM.data, &(data[i*30]), min(30, size - 30*i));
		bin_dump((char *)(PM.data), 30);
		radio->write(&PM, sizeof(ProtocolMessage));
		long wait_start = millis();

		bool ackR = false;
		while ( !(ackR = radio->available()) && (millis() - wait_start) < PACKET_TIMEOUT)
		{
			radio->stopListening();
			radio->write(&PM, sizeof(ProtocolMessage));
			radio->startListening();
			delay(30);
		}
			
	
		if (ackR)
		{
			uint8_t message_count;
			radio->read(&message_count,sizeof(message_count));
			DBGPRINT("Reciving ack package %hu", message_count);
			if (message_count != i)
			{
				DBGPRINT("Failed send package: %hu", i);
				i--;
			}
			else
			{
				DBGPRINT("Packet send");
				lastSuccSend = millis();
			}
		}
		else
		{
			DBGPRINT("Timeout sending package: %hu", i);
			i--;
		}
	}



	DBGPRINT("Send complete");



	return ERROR_SUCCESS;
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

int WirelessProtocol::reciveData(byte* data, uint16_t MaxSize, uint16_t* size, uint64_t* Pipe, uint16_t ReciveTimeout)
{
	for (uint8_t i = 0; i < RECIVING_PIPES_MAXCOUNT; i++)
	{
		if (reciving_pipes_state[i])
		{
			DBGPRINT("Waiting pipe %d", i);
			*Pipe = reciving_pipes[i];
			
			radio->openReadingPipe(1, *Pipe);
			radio->openWritingPipe(*Pipe);
			radio->startListening();

			long startReciveTime = millis();
			bool rC = false;
			while ( !(rC = radio->available()) && (millis() - startReciveTime) < PACKET_TIMEOUT ) ;

			if (rC)
			{
				DBGPRINT("Start reciving, pipe %d", i);
				radio->read(buf, 32);
				
				ProtocolMessage *PM = (ProtocolMessage*)buf;
				radio->stopListening();
				radio->write(&(PM->CurrentNum), sizeof(uint8_t));
				
				//radio->writeAckPayload(1, &(PM.CurrentNum), sizeof(uint8_t));

				if (PM->CurrentNum != 0)
					continue;

				
				DBGPRINT("Package count %d", PM->AllLength);
				

				*size = PM->AllLength*30;
				if (*size > MaxSize)
					return ERROR_NO_ENAUGHT_MEMORY;

				DBGPRINT("Memory copy");
			    memcpy(data, PM->data, 30);

				DBGPRINT("Data size %d", *size);
				long lastSuccRecv = millis();
				for (uint8_t j = 1; j < PM->AllLength; j++)
				{


					if (millis() - lastSuccRecv > ReciveTimeout)
					{
						DBGPRINT("Package %d timeout", PM->CurrentNum);
						return ERROR_TIMEOUT;
					}

					startReciveTime = millis();
					radio->startListening();
					while ( !(rC = radio->available()) && (millis() - startReciveTime) < PACKET_TIMEOUT ) ;
			
					if (rC)
					{
						radio->read(buf, 32);
						radio->stopListening();
						PM = (ProtocolMessage*)buf;
						
						if (PM->CurrentNum < j)
						{
							bool ackres = radio->write(&(PM->CurrentNum), sizeof(uint8_t));
							j--;
							DBGPRINT("Duplicate packet %d, ackres: %d", PM->CurrentNum, ackres);
							continue;
						}
						else if (PM->CurrentNum > j)
						{
							DBGPRINT("Radio transmitt error on package %d", PM->CurrentNum);
							return ERROR_RADIO_TRANSMITT;
						}


						
						radio->write(&j, sizeof(uint8_t));
						memcpy(&(data[j*30]), PM->data, 30);
						bin_dump((char *)(PM->data), 30);
						lastSuccRecv = millis();
						DBGPRINT("Recived packege %d", j);
					}

				}

				

					while ( !(rC = radio->available()) && (millis() - startReciveTime) < PACKET_TIMEOUT ) 

						radio->write(&(PM->CurrentNum), sizeof(uint8_t));
			



				DBGPRINT("Recive complete");
				return ERROR_SUCCESS;
			}
			else
			{
				DBGPRINT("No data");
				continue;
			}
			
		}
	}

	return ERROR_NO_ACTIVE_PIPE;
}

