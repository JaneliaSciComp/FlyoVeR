
#include <iostream>
#include <ctime>

#include <conio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    unsigned char INBUFFER[2410];
    unsigned char OUTBUFFER[50];
    DWORD        bytes_read    = 0;    // Number of bytes read from port
    DWORD        bytes_written = 0;    // Number of bytes written to the port
    HANDLE       comport      = NULL;  // Handle COM port
	int   bStatus;
    DCB          comSettings;          // Contains various port settings
    COMMTIMEOUTS CommTimeouts;

	// Open COM port
    if ((comport = 
         CreateFile(TEXT("\\\\.\\COM3"),                // open com5:
                    GENERIC_READ | GENERIC_WRITE, // for reading and writing
                    0,                            // exclusive access
                    NULL,                         // no security attributes
                    OPEN_EXISTING,              
                    FILE_ATTRIBUTE_NORMAL,
                    NULL)) == INVALID_HANDLE_VALUE)
    {
        // error processing code goes here
    }
    // Set timeouts in milliseconds
    CommTimeouts.ReadIntervalTimeout         = 2000; 
    CommTimeouts.ReadTotalTimeoutMultiplier  = 0; 
    CommTimeouts.ReadTotalTimeoutConstant    = 100;
    CommTimeouts.WriteTotalTimeoutMultiplier = 2000;
    CommTimeouts.WriteTotalTimeoutConstant   = 100;
    bStatus = SetCommTimeouts(comport,&CommTimeouts);
    if (bStatus != 0)
    {
        // error processing code goes here
    }
    // Set Port parameters.
    // Make a call to GetCommState() first in order to fill
    // the comSettings structure with all the necessary values.
    // Then change the ones you want and call SetCommState().
    GetCommState(comport, &comSettings);
    comSettings.BaudRate = 1250000;
    comSettings.StopBits = ONESTOPBIT;
    comSettings.ByteSize = 8;
    comSettings.Parity   = NOPARITY;
    comSettings.fParity  = FALSE;
    bStatus = SetCommState(comport, &comSettings);
    if (bStatus == 0)
    {
        // error processing code goes here
    }
	OUTBUFFER[0] = 254;
	OUTBUFFER[1] = 0;
	bStatus = WriteFile(comport,              // Handle
		&OUTBUFFER,      // Outgoing data
		2,              // Number of bytes to write
		&bytes_written,  // Number of bytes written
		NULL);
	if (bStatus != 0)
	{
		// error processing code here
	}
	Sleep( 200 );

	OUTBUFFER[0] = 252;
	OUTBUFFER[1] = 0;
	bStatus = WriteFile(comport,              // Handle
		&OUTBUFFER,      // Outgoing data
		2,              // Number of bytes to write
		&bytes_written,  // Number of bytes written
		NULL);
	if (bStatus != 0)
	{
		// error processing code here
	}
	Sleep( 200 );

	bStatus = ReadFile(comport,   // Handle
		&INBUFFER,            // Incoming data
		50,                  // Number of bytes to read
		&bytes_read,          // Number of bytes read
		NULL);
	if (bStatus != 0)
	{
		// error processing code goes here
	}
	// code to do something with the data goes here

	OUTBUFFER[0] = 246;
	OUTBUFFER[1] = 0;
	bStatus = WriteFile(comport,              // Handle
		&OUTBUFFER,      // Outgoing data
		2,              // Number of bytes to write
		&bytes_written,  // Number of bytes written
		NULL);
	if (bStatus != 0)
	{
		// error processing code here
	}
	Sleep( 200 );

	OUTBUFFER[0] = 255;
	OUTBUFFER[1] = 0;
	bStatus = WriteFile(comport,              // Handle
		&OUTBUFFER,      // Outgoing data
		2,              // Number of bytes to write
		&bytes_written,  // Number of bytes written
		NULL);
	if (bStatus != 0)
	{
		// error processing code here
	}
	Sleep( 200 );

	int dx[2],dy[2],squal[2];
	float shutter[2];
	long packet_count = 0;
	long malformed = 0;
	//poll device at 20Hz until canceled by user (ctrl+c)
	while( !kbhit() )
	{
		bool valid = true;
		int index;

		std::cout << "S: " << clock() << std::endl;

		//Read 200 packets of data (12 bytes per packet)
		bStatus = ReadFile(comport,   // Handle
			&INBUFFER,            // Incoming data
			12*20,                  // Number of bytes to read
			&bytes_read,          // Number of bytes read
			NULL);
		if (bStatus == 0 || bytes_read!=240){
			printf("Bad Read. %d bytes read\n", bytes_read);
			Sleep(1);
			continue;
		}

		packet_count++;
		for (index = 0; valid && index < 240; index += 12 )
		{
			valid &= ((int)INBUFFER[index] == 0);
		}

		if ( valid )
		{
			//Accumulate Motion Data for this 20Hz chunk
			dx[0]=0; dx[1]=0; dy[0]=0; dy[1]=0;
			for (int i = 0; i<240; i+=12){
				dx[0] += ((int)INBUFFER[i+2])-128;
				dy[0] += ((int)INBUFFER[i+3])-128;
				dx[1] += ((int)INBUFFER[i+4])-128;
				dy[1] += ((int)INBUFFER[i+5])-128;
			}
			if ( packet_count == 0 )
			{
				squal[0] = (int)INBUFFER[6];
				squal[1] = (int)INBUFFER[7];
				shutter[0] = (((float)(INBUFFER[8]-1))*256.0 + (float)INBUFFER[9])/24.0;
				shutter[1] = (((float)(INBUFFER[10]-1))*256.0 + (float)INBUFFER[11])/24.0;

				printf("%d bytes received\n",bytes_read);
				printf("\tdx0 = %+3d",dx[0]);
				printf("\tdy0 = %+3d\n",dy[0]);
				printf("\tdx1 = %+3d",dx[1]);
				printf("\tdy1 = %+3d\n",dy[1]);
				printf("\tsqual0 = %d",squal[0]);
				printf("\tsqual1 = %d\n",squal[1]);
				printf("\tShutter0 = %.2fus",shutter[0]);
				printf("\tShutter1 = %.2fus\n",shutter[1]);
				Sleep(1);
			}
		}
		else
		{
			printf( "Malformed packet #%d\n", ++malformed );
			OUTBUFFER[0] = 254;
			OUTBUFFER[1] = 0;
			bStatus = WriteFile(comport,              // Handle
				&OUTBUFFER,      // Outgoing data
				2,              // Number of bytes to write
				&bytes_written,  // Number of bytes written
				NULL);
			if (bStatus != 0)
			{
				// error processing code here
			}
			Sleep( 200 );

			bStatus = ReadFile(comport,   // Handle
				&INBUFFER,            // Incoming data
				12*200,                  // Number of bytes to read
				&bytes_read,          // Number of bytes read
				NULL);

			OUTBUFFER[0] = 255;
			OUTBUFFER[1] = 0;
			bStatus = WriteFile(comport,              // Handle
				&OUTBUFFER,      // Outgoing data
				2,              // Number of bytes to write
				&bytes_written,  // Number of bytes written
				NULL);
			if (bStatus != 0)
			{
				// error processing code here
			}
			Sleep( 200 );
		}
	}

	OUTBUFFER[0] = 254;
	OUTBUFFER[1] = 0;
	bStatus = WriteFile(comport,              // Handle
		&OUTBUFFER,      // Outgoing data
		2,              // Number of bytes to write
		&bytes_written,  // Number of bytes written
		NULL);
	if (bStatus != 0)
	{
		// error processing code here
	}
	Sleep( 20 );


	CloseHandle(comport);
	printf( "%d out of %d packets malformed\n", malformed, packet_count );
 return 0;
}