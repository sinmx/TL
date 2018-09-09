#pragma once
#include "CellMath.hpp"
namespace   CELL
{
	enum    DRAWMODE
	{
		DM_POINTS = 0,// ��
		DM_LINES = 1,// ��
		DM_LINE_LOOP = 2,// ������
		DM_LINE_STRIP = 3,// �ı���
		DM_MAX,
	};

	class Raster
	{
	public:
		Raster(int w, int h, void* buffer);
		~Raster();
	public:
		Rgba * _buffer;
		int    _width;
		int    _height;
		Rgba4Byte _color;
	public:
		void    clear();
		void    drawPoint(int x, int y, Rgba color, int ptSize);
		void    drawLine(float2 pt1, float2 pt2, Rgba color);
		void    drawLine(float2 pt1, float2 pt2, Rgba color1, Rgba color2);
		void    drawArrays(DRAWMODE mode, const float2* points, int count);
		void    drawPoints(float2 pt1, Rgba4Byte color);
		inline void setPixel(unsigned x, unsigned y, Rgba color)
		{
			if (x >= _width || y >= _height)
			{
				return;
			}
			_buffer[y * _width + x] = color;
		}
	};
}
