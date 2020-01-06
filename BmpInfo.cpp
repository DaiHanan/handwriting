#include "BmpInfo.h"

string BmpInfo::sourcePath = "";

BmpInfo::BmpInfo(string path) : filePath(std::move(path)) {
    if (this->sourcePath.empty()) {
        this->sourcePath = this->filePath;
    }
    this->initInfo();
    if(this->isSuccess)
        this->initVal();
}

bool BmpInfo::getDIBColor(int X, int Y, BYTE* r, BYTE* g, BYTE* b) {
    int dpixeladd;
    BYTE* ptr;
    if (X < 0 || X >= bih.biWidth || Y < 0 || Y >= bih.biHeight)
    {
        return 0;
    }
    dpixeladd = bih.biBitCount / 8;
    ptr = Buffer + X * dpixeladd + (bih.biHeight - 1 - Y) * LineByteWidth;
    *b = *ptr;
    *g = *(ptr + 1);
    *r = *(ptr + 2);
    return 1;
}

BmpInfo::~BmpInfo() {
    free(this->Buffer);
}

void BmpInfo::initVal() {
    int width = this->bih.biWidth;
    int height = this->bih.biHeight;
    //分配空间
    this->_val = new vector<vector<int>>(height, vector<int>(width));
    auto& val = *this->_val;

    BYTE r, g, b;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (this->getDIBColor(j, i, &r, &g, &b)) {
                val[i][j] = (int)r == 255 && (int)g == 255 && (int)b == 255 ? 0 : 1;
            }
            else this->isSuccess = false;
        }
    }
}

void BmpInfo::initInfo() {
    FILE* file;
    WORD bfh[7];
    long dpixeladd;
    if (NULL == (file = fopen(this->filePath.c_str(), "rb")))
    {
        this->isSuccess = false;
        return;
    }
    fread(&bfh, sizeof(WORD), 7, file);
    if (bfh[0] != (WORD)(((WORD)'B') | ('M' << 8)))
    {
        fclose(file);

        this->isSuccess = false;
        return;
    }
    fread(&bih, sizeof(myBITMAPINFOHEADER), 1, file);
    if (this->bih.biBitCount < 24)
    {
        fclose(file);

        this->isSuccess = false;
        return;
    }
    dpixeladd = this->bih.biBitCount / 8;
    this->LineByteWidth = this->bih.biWidth * (dpixeladd);
    if ((this->LineByteWidth % 4) != 0)
        this->LineByteWidth += 4 - (this->LineByteWidth % 4);
    if ((this->Buffer = (BYTE*)malloc(sizeof(BYTE) * this->LineByteWidth * bih.biHeight)) != nullptr)
    {
        fread(this->Buffer, this->LineByteWidth * this->bih.biHeight, 1, file);
        fclose(file);

        this->isSuccess = true;
        return;
    }
    fclose(file);
    this->isSuccess = false;
}

BmpInfo* BmpInfo::printVal() {
    ostringstream& ss = Log::ss;
    auto& val = *this->_val;
    for (int i = 0; i < this->getHeight(); i++) {
        for (int j = 0; j < this->getWidth(); j++) {
            ss << val[i][j] << " ";
        }
        ss << endl;
    }
    ss << endl;
    Log::write();
    return this;
}

BmpInfo* BmpInfo::printInfo() {
    ostringstream& ss = Log::ss;
    ss << "已获得图片信息" << endl
        << "长度：" << this->getHeight() << endl
        << "宽度：" << this->getWidth() << endl << endl;
    Log::write();
    return this;
}

BmpInfo* BmpInfo::getBmp(const string& path)
{
    //初始信息
    BmpInfo* bmp = new BmpInfo(path);
    if (!bmp->isSuccess)
    {
        printf("打开文件失败： %s", path.c_str());
        return nullptr;
    }
    return bmp;
}

