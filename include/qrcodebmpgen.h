#pragma once
/*
 * qrencode - QR Code encoder
 * BMP 生成类
 */

#include <vector>
#include <string>
#include "qrencode.h"

struct BitmapOption
{
	struct
	{
		unsigned char red, green, blue;
	}color;

	int pixel;
	int compression;

	// default
	BitmapOption() 
	{
		color.red = 0;
		color.green = 0;
		color.blue = 255;
		pixel = 8;
		compression = 0;
	}
};

struct QRCodeOption
{
	const char* text;
	QRecLevel level;
	QRencodeMode mode;
	bool casesensitive;

	QRCodeOption()
	{
		text = NULL;
		level = QR_ECLEVEL_H;
		mode = QR_MODE_8;
		casesensitive = true;
	}
};

// 根据文本字符串生成二维码bmp图片，返回数据存放在 result 里面，成功返回 true, 失败返回 false
bool QRCodeBitmap(const QRCodeOption* qrcodeOption, BitmapOption* bitmapOption, std::vector<char>* result);
