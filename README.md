# libqrencode
QR Code encoding library

基于 fukuchi/libqrencode 修改的一个易于在Windows上C++开发使用的二维码工具库

## Usage:
```c++
std::string responseString("https://github.com/ccnyou/libqrencode");
std::vector<char> bitmapVector;
QRCodeOption qrcodeOption;
BitmapOption bitmapOption;

qrcodeOption.text = responseString.c_str();
bitmapOption.color.red = 0;
bitmapOption.color.green = 0;
bitmapOption.color.blue = 0;
bool bitmapSuccess = QRCodeBitmap(&qrcodeOption, &bitmapOption, &bitmapVector);
```