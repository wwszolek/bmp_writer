#include "bmp_writer.h"

#include <fstream>
#include <bitset>
#include <cmath>

bool save_bitmap(const BITMAPFILEHEADER* bmfh, const BITMAPINFOHEADER* bmih,
	const RGBQUAD* colors, const uint8_t* pixel_data, const std::string& path) {

	std::fstream bitmap_file(path, 
		std::fstream::out | std::fstream::trunc | std::fstream::binary);

	bitmap_file.write((char*)(&bmfh->bfType), 2);
	bitmap_file.write((char*)(&bmfh->bfSize), 4);
	bitmap_file.write((char*)(&bmfh->bfReserved1), 2);
	bitmap_file.write((char*)(&bmfh->bfReserved2), 2);
	bitmap_file.write((char*)(&bmfh->bfOffBits), 4);

	bitmap_file.write((char*)(&bmih->biSize), 4);
	bitmap_file.write((char*)(&bmih->biWidth), 4);
	bitmap_file.write((char*)(&bmih->biHeight), 4);
	bitmap_file.write((char*)(&bmih->biPlanes), 2);
	bitmap_file.write((char*)(&bmih->biBitCount), 2);
	bitmap_file.write((char*)(&bmih->biCompression), 4);
	bitmap_file.write((char*)(&bmih->biSizeImage), 4);
	bitmap_file.write((char*)(&bmih->biXPelsPerMeter), 4);
	bitmap_file.write((char*)(&bmih->biYPelsPerMeter), 4);
	bitmap_file.write((char*)(&bmih->biClrUsed), 4);
	bitmap_file.write((char*)(&bmih->biClrImportant), 1);
	bitmap_file.write((char*)(&bmih->biClrRotation), 1);
	bitmap_file.write((char*)(&bmih->biReserved), 2);

	for (int i = 0; i < pow(2,bmih->biBitCount); i++) {
		bitmap_file.write((char*)(&colors[i].rgbBlue), 1);
		bitmap_file.write((char*)(&colors[i].rgbGreen), 1);
		bitmap_file.write((char*)(&colors[i].rgbRed), 1);
		bitmap_file.write((char*)(&colors[i].rgbReserved), 1);
	}

	int bit_width = bmih->biWidth*bmih->biBitCount;
	int fill = 32 - bit_width % 32;
	fill = !fill? 0 : fill;
	int row_length = (bit_width + fill) / 8;
	char* bytes = new char[row_length];
	int pix_in_byte = 8 / bmih->biBitCount;

	for (int y = bmih->biHeight-1; y > -1; y--) {
		for (int i = 0; i < row_length; i++)
			bytes[i] = 0b00000000;
		for (int x = 0; x < bmih->biWidth; x++) {
			bytes[x / pix_in_byte] += pixel_data[y*bmih->biWidth + x]
				<< (8 - bmih->biBitCount - x % pix_in_byte);
		}

		bitmap_file.write(bytes, row_length);
	}

	delete[] bytes;
	bitmap_file.close();
	return 0;
}

void fill_headers(BITMAPFILEHEADER* bmfh, BITMAPINFOHEADER* bmih, int width, int height) {
	bmih->biWidth = width;
	bmih->biHeight = height;
	
	int bit_width = bmih->biWidth*bmih->biBitCount;
	int fill = 32 - bit_width % 32;
	fill = !fill ? 0 : fill;

	bmih->biSizeImage = height * (bit_width + fill) / 8;

	bmfh->bfOffBits = 14 + 40 + 4 * uint32_t(pow(2, bmih->biBitCount));
	bmfh->bfSize = bmfh->bfOffBits + bmih->biSizeImage;
}
