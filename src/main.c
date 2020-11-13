#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/utils.h"
#include <errno.h>
#include <time.h>

#define BUFFER_SIZE 128


const char header[] = {
    'M', 'T', 'h', 'd', // MHtd string
    0, 0, 0, 6, // header size = 6 bytes
    0, 1, // type field
    0, 2, // track count
    0, 96 // 96 Pulsations Per Quarter note
};
char trackHeader[] = {
    'M', 'T', 'r', 'k',
    0, 0, 0, 0
};

const char endOfTrackEvent[] = {
    0, 0xff, 0x2f, 0
};
int m; // the modulus = how many notes we get to choose from
char notes[128] = {0};

void writeTempoTrack(FILE* fp, int tempo)
{
    int ms = 60000000 / tempo;
    char tempoEvent[] = {
        0, 0xff, 0x53, 4, 0, 0, 0, 0, // tempo event
    };
    intToBytes(ms, tempoEvent + 4);
    intToBytes(12, trackHeader + 4);
    // write everything
    writeBytes(fp, trackHeader, 8);
    writeBytes(fp, tempoEvent, 8);
    writeBytes(fp, endOfTrackEvent, 4);
}
int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        printf("Usage: randMidi tempo barCount outName\n");
        exit(1);
    }
    int length = strlen(argv[3]);
    char outFileName[BUFFER_SIZE]= {0};
    strcpy(outFileName, argv[3]);
    strcpy(outFileName + length, ".mid");

    printf("Notes (submit one by one):\n");
    
    while(!m)
    {
        printf("Please provide a valid list of notes!\n");
        m = scanNotes(notes);
    }

    const int noteCount = atoi(argv[2]) * 16;
    int tempo = atoi(argv[1]);

    FILE* fp = fopen(outFileName, "w+");
    if(fp == NULL)
    {
        printf("Error opening file %s: %s\n", outFileName, strerror(errno));
        exit(1);
    }
    writeBytes(fp, header, 14);
    writeTempoTrack(fp, tempo);
    
    char trackNameEvent[BUFFER_SIZE] = {0};
    trackNameEvent[1] = 0xff; 
    trackNameEvent[2] = 3; 
    trackNameEvent[3] = (char)length;
    strcpy(trackNameEvent + 4, argv[3]);

    size_t totalLength = length + (noteCount + 1) * 8;
    intToBytes(totalLength, trackHeader + 4);
    writeBytes(fp, trackHeader, 8);
    writeBytes(fp, trackNameEvent, length + 4);

    int previous = -1; // the previous note
    srand(time(NULL));
    for(int i = 0; i < noteCount; i++)
    {
        char noteEvent[] = {
            0, 0x90, 0, 0x64 // create noteOn event
        };
        // generate a random note
        int r = (char)(rand() % m); 
        while(r == previous)
            r = (char)(rand() % m);
        previous = r;
        noteEvent[2] = notes[r];
        // write the noteOn and noteOff events
        writeBytes(fp, noteEvent, 4);
        noteEvent[0] = 24;
        noteEvent[1] = 0x80;
        writeBytes(fp, noteEvent, 4);
    }
    // end of track
    writeBytes(fp, endOfTrackEvent, 4);

    fclose(fp);
    return 0;
}