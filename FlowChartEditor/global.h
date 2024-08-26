#pragma once

#include <Qt>

// 圆周率 PI
const qreal PI = 3.14159265358979323;

// 从角度转换到弧度
qreal degToRad(qreal deg);

// 从弧度转换到角度
qreal radToDeg(qreal rad);

// 将角度规范到 [0, 360]
qreal degMod(qreal deg);
