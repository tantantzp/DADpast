#pragma once
#include "vector2.h"

void Triangulate_2d(std::vector<MATH::vector2d>& pts, std::vector<int>& tris);
void Triangulate_2d(QVector<QPoint>& pts, QVector<int>& tris);