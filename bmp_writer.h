#ifndef BMP_WRITER_H
#define BMP_WRITER_H

#include <cstdint>
#include <string>

struct BITMAPFILEHEADER {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};

struct BITMAPINFOHEADER {
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint8_t biClrImportant;
	uint8_t biClrRotation;
	uint16_t biReserved;
};

struct RGBQUAD {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
};

//aColors is an array of size 2^bmih.biBitCount
//pixel_data is an two dimensional array of uint8*bmih.biBitCount_t values
//of size [bmih.biWidth*bmih.biHeight]
bool save_bitmap(const BITMAPFILEHEADER* bmfh, const BITMAPINFOHEADER* bmih,
	const RGBQUAD* colors, const uint8_t* pixel_data, const std::string& path);

//fills parameters in headers created from default headers
void fill_headers(BITMAPFILEHEADER*, BITMAPINFOHEADER*, int width, int height);


//fill bfSize, bfOffBits
const BITMAPFILEHEADER default_fileheader = {
	19778,
	0,
	0,
	0,
	0
};

//fill biWidth,biHeight,biSizeImage
const BITMAPINFOHEADER default_mono_infoheader = {
	40,
	0,
	0,
	1,
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

//0 - white
//1 - black
const RGBQUAD mono_color_table[2] = {
	{255,255,255,0},
	{0,0,0,0}
};

#endif