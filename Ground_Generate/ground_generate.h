#include <fstream>
#include <string>
#define REALCOLORBITMAP 24
#define REALCOLORBITMAP_32 32

#pragma once

const int VERTEX_COUNT = 250, GRASS_SIZE = 100;
std::string high_path = "3.bmp";
std::string obj_path = "ground.obj";
void get_height(float* height, std::string path, const int vex_count);
void get_normal(float* vertices, unsigned int* indices, float* normal);
void create_obj(std::string obj_path);

struct COLORTABLE {
    //3����ɫ��/��ɫ�壨color table��
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved; //��������Ϊ0
};

class bitmap_image
{
protected:
    unsigned int bfSize;    //��С
    unsigned int biWidth;   //���
    int biHeight;           //�߶�
    unsigned short biBitCount;//λ��
    COLORTABLE* pRgbQuad;   //��ɫ������
    char* pImageData;       //ͼ������
    int RowLine() const;
public:
    int width() const;	//����ͼƬ�Ŀ��
    int height() const; //����ͼƬ�ĸ߶�
    unsigned int get_pixel(int row, int col) const; //����ָ�����RGB��ɫ
    int get_grey(int row, int col) const;

    /* ������Ҫ�����Լ��Ķ��� */
    bitmap_image(const char*filename);
    ~bitmap_image();
};