#include "Converter.h"
#include <algorithm>
#include <cmath>
#include <QtGlobal>

void Converter::rgbToHsv(float r, float g, float b, float * h, float * s, float * v)
{
  static const float modifier = 1.0f / 255.0f;
  r *= modifier;
  g *= modifier;
  b *= modifier;

  const std::initializer_list<float> list{r, g, b};
  const float max = std::max(list);
  const float min = std::min(list);

  const float d = max - min;
  *v = max;
  *s = (qFuzzyIsNull(max) ? 0.0f : d / max);

  if (qFuzzyCompare(max, min))
  {
    *h = 0.0f;
  } else
  {
    static const float divider = 1.0f / 6.0f;
    *h = divider * (qFuzzyCompare(max, r) ? (g - b) / d        :
                      qFuzzyCompare(max, g) ? (b - r) / d + 2.0f :
                      qFuzzyCompare(max, b) ? (r - g) / d + 4.0f : 0.0f);
  }
}

void Converter::hsvToRgb(float * r, float * g, float * b, float h, float s, float v)
{
  qint32 i = static_cast<qint32>(std::floor(h * 6.0f));
  float f = h * 6.0f - i;
  float p = v * (1.0f - s);
  float q = v * (1.0f - f * s);
  float t = v * (1.0f - (1.0f - f) * s);

  switch (i % 6)
  {
  case 0: {*r = v; *g = t; *b = p;} break;
  case 1: {*r = q; *g = v; *b = p;} break;
  case 2: {*r = p; *g = v; *b = t;} break;
  case 3: {*r = p; *g = q; *b = v;} break;
  case 4: {*r = t; *g = p; *b = v;} break;
  case 5: {*r = v; *g = p; *b = q;} break;
  }

  static const float modifier = 255.0f;
  *r *= modifier;
  *g *= modifier;
  *b *= modifier;
}
