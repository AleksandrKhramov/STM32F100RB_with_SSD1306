//------------------------------------------------------------------------------
#ifndef _F10X16F_H
#define _F10X16F_H

#include "main.h"


// ����� ������������, 6�8 ��������
#define f10x16_FLOAT_WIDTH         10
#define f10x16_FLOAT_HEIGHT        16

// ���-�� �������� � ������� ������
#define f10x16f_NOFCHARS           256


// ������� ���������� ��������� �� ���������� ������� Char
uint8_t *f10x16f_GetCharTable(uint8_t Char);

#endif 
