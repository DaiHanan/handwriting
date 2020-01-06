#pragma once

typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef struct {
    DWORD biSize;
    long biWidth;
    long biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} myBITMAPINFOHEADER;

typedef struct {
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} myBITMAPFILEHEADER;