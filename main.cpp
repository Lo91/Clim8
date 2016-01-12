/* ===================================================
C code : test.cpp
* ===================================================
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "RCSwitch.h"
#include "RcOok.h"
#include "Sensor.h"
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

const char* Channels[] =
{
"Channel1",
"Channel2",
"Channel3"
};


int main(int argc, char *argv[])
{
	int RXPIN = 2;
	int TXPIN = 0;

	if(wiringPiSetup() == -1)
		return 0;

	bool LogMode = false;
	FILE* LogOut = stdout;
	char oldName[80] = {0};
	if( argv[1] && !strcmp(argv[1],"-log") )
	{
		LogMode = true;
		++argv;
		--argc;
		LogOut = NULL;
	}

	if( argc==4 )
	{
		Channels[0] = argv[1];
		Channels[1] = argv[2];
		Channels[2] = argv[3];
	}
	RCSwitch *rc = new RCSwitch(RXPIN,TXPIN);


	while (1)
	{
		if (rc->OokAvailable())
		{
			char message[100];

			rc->getOokCode(message);
			//printf("%s\n",message);

			Sensor *s = Sensor::getRightSensor(message);
			if (s!= NULL)
			{
				time_t rawtime;
				time (&rawtime);
				struct tm* timeinfo = localtime (&rawtime);

				char timeString[80];
				strftime (timeString,sizeof(timeString),"%F %T",timeinfo);
				unsigned iChannel = s->getChannel() - 1;
				if( iChannel >= 3 )
					continue;

				const char* label = Channels[iChannel];
				if( LogMode )
				{
					char fileName[80];
					strftime(fileName, sizeof(fileName),"%F.csv", timeinfo);
					if( strcmp(fileName,oldName) )
					{
						if( LogOut )
						{
							fclose(LogOut);
						}
						LogOut = fopen( fileName, "w+" );
						system("rm current.csv");
						char cmd[40];
						sprintf(cmd,"ln -s %s current.csv", fileName );
						system(cmd);
						strcpy(oldName,fileName);
					}
				}
				fprintf(LogOut,"%s,%s,%.1f,%.0f\n", timeString,label,s->getTemperature(),s->getHumidity());
			}
			delete s;
		}
		delay(1000);
		if( fopen("stop","r") )
			break;
	}
}
