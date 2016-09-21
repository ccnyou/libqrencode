#include "qrcodebmpgen.h"
#include "qrencode.h"

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;

#pragma pack(push, 2)

typedef struct
{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

bool QRCode2Bitmap(const QRcode* qrcode, const BitmapOption* bitmapOption, std::vector<char>* result)
{
	if (!bitmapOption || !result)
	{
		return false;
	}

	int unWidth = qrcode->width;
	int unWidthAdjusted = unWidth * bitmapOption->pixel * 3;
	if (unWidthAdjusted % 4)
		unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
	int unDataBytes = unWidthAdjusted * unWidth * bitmapOption->pixel;
	unsigned char*	pRGBData = NULL, *pSourceData = NULL, *pDestData = NULL;
	// Allocate pixels buffer

	if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
	{
		printf("Out of memory");
		return false;
	}

	// Preset to white
	memset(pRGBData, 0xff, unDataBytes);

	// Prepare bmp header
	BITMAPFILEHEADER kFileHeader;
	kFileHeader.bfType = 0x4d42;  // "BM"
	kFileHeader.bfSize = sizeof(BITMAPFILEHEADER)+
		sizeof(BITMAPINFOHEADER)+
		unDataBytes;
	kFileHeader.bfReserved1 = 0;
	kFileHeader.bfReserved2 = 0;
	kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+
		sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER kInfoHeader;
	kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	kInfoHeader.biWidth = unWidth * bitmapOption->pixel;
	kInfoHeader.biHeight = -((int)unWidth * bitmapOption->pixel);
	kInfoHeader.biPlanes = 1;
	kInfoHeader.biBitCount = 24;
	kInfoHeader.biCompression = bitmapOption->compression;
	kInfoHeader.biSizeImage = 0;
	kInfoHeader.biXPelsPerMeter = 0;
	kInfoHeader.biYPelsPerMeter = 0;
	kInfoHeader.biClrUsed = 0;
	kInfoHeader.biClrImportant = 0;

	// Convert QrCode bits to bmp pixels
	pSourceData = qrcode->data;
	for (int y = 0; y < unWidth; y++)
	{
		pDestData = pRGBData + unWidthAdjusted * y * bitmapOption->pixel;
		for (int x = 0; x < unWidth; x++)
		{
			if (*pSourceData & 1)
			{
				for (int l = 0; l < bitmapOption->pixel; l++)
				{
					for (int n = 0; n < bitmapOption->pixel; n++)
					{
						*(pDestData + n * 3 + unWidthAdjusted * l) = bitmapOption->color.blue;
						*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = bitmapOption->color.green;
						*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = bitmapOption->color.red;
					}
				}
			}
			pDestData += 3 * bitmapOption->pixel;
			pSourceData++;
		}
	}

	// insert to result vector
	const char* buffer = reinterpret_cast<const char*>(&kFileHeader);
	result->insert(result->end(), buffer, buffer + sizeof(BITMAPFILEHEADER));

	buffer = reinterpret_cast<const char*>(&kInfoHeader);
	result->insert(result->end(), buffer, buffer + sizeof(BITMAPINFOHEADER));

	buffer = reinterpret_cast<const char*>(pRGBData);
	result->insert(result->end(), buffer, buffer + unDataBytes);

	return true;
}

bool String2QRCodeBitmap(const QRCodeOption* qrcodeOption, BitmapOption* bitmapOption, std::vector<char>* result)
{
	if (!qrcodeOption || !bitmapOption || !result)
	{
		return false;
	}

	bool success = false;
	QRcode* pQRC = QRcode_encodeString(qrcodeOption->text, 0, qrcodeOption->level, qrcodeOption->mode, qrcodeOption->casesensitive);
	if (pQRC)
	{
		success = QRCode2Bitmap(pQRC, bitmapOption, result);
		QRcode_free(pQRC);
	}

	return success;
}
