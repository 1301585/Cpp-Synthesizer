#pragma warning(disable:4996)
#define _USE_MATH_DEFINES
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <math.h>



struct SMinimalWaveFileHeader {

    //main chunk
    unsigned char m_szChunkID[4];
    uint32_t m_nChunkSize;
    unsigned char m_szFormat[4];

    //sub chumk 1 fmt
    unsigned char m_szSubChunk1ID[4];
    uint32_t m_nSubChunk1Size;
    uint16_t m_nAudioFormat;
    uint16_t m_nNumChannels;
    uint32_t m_nSampleRate;
    uint32_t m_nByteRate;
    uint16_t m_nBlockAlign;
    uint16_t m_nBitsPerSample;

    //sub chunk 2 data
    unsigned char m_szSubChunk2ID[4];
    uint32_t m_nSubChunk2Size;

};


bool WriteWaveFile(const char *szFileName, void *pData, int32_t nDataSize, int16_t nNumChannels, int32_t nSampleRate, int32_t nBitsPerSample) {
    //open file

    FILE *FILE = fopen(szFileName, "w+b");
    if (!FILE) {
        return false;
    }

    SMinimalWaveFileHeader waveHeader;

    //fill out main chunk
    memcpy(waveHeader.m_szChunkID, "RIFF", 4);
    waveHeader.m_nChunkSize = nDataSize + 36;
    memcpy(waveHeader.m_szFormat, "WAVE", 4);

    //fill out sub chunk 1 "fmt"
    memcpy(waveHeader.m_szSubChunk1ID, "fmt", 4);
    waveHeader.m_nSubChunk1Size = 16;
    waveHeader.m_nAudioFormat = 1;
    waveHeader.m_nNumChannels = nNumChannels;
    waveHeader.m_nSampleRate = nSampleRate;
    waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
    waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
    waveHeader.m_nBitsPerSample = nBitsPerSample;

    //fill out sub chunk 2 "data"

    memcpy(waveHeader.m_szSubChunk2ID, "data", 4);
    waveHeader.m_nSubChunk2Size = nDataSize;

    //write the header
    fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, FILE);

    //write the wave data
    fwrite(pData, nDataSize, 1, FILE);

    //close file
    fclose(FILE);
    return true;

}









int main()
{
    int nSampleRate = 44100;
    int nNumSeconds = 4;
    int nNumChannels = 1;

    int nNumSamples = nSampleRate * nNumChannels * nNumSeconds;
    int32_t *pData = new int32_t[nNumSamples];

    int32_t nValue1 = 0;
    int32_t nValue2 = 0;
    for (int nIndex = 0; nIndex < nNumSamples; nIndex += 1) {
        //data wraps around once it gets high enough, making a sawtooth wave
        pData[nIndex] = sin((float)nIndex * 2 * (float)M_PI * fFrequency / (float)nSampleRate);
    }

    WriteWaveFile("outStereo.wav", pData, nNumSamples * sizeof(pData[0]), nNumChannels, nSampleRate, sizeof(pData[0]) * 8);

    delete[]pData;



}