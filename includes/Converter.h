#ifndef CONVERTER_H
#define CONVERTER_H

#define SAFE_RGB_BORDERS(value) \
  (value) > 255 ? 255 : (value) < 0 ? 0 : (value)

#define SAFE_RGBF_BORDERS(value) \
  (value) > 1.0f ? 1.0f : (value) < 0.0f ? 0.0f : (value)

#define SAFE_RGB_MAX_BORDER(value) \
  (value) > 255 ? 255 : (value)

#define SAFE_RGBF_MAX_BORDER(value) \
  (value) > 1.0f ? 1.0f : (value)

#define SAFE_RGB_MIN_BORDER(value) \
  (value) < 0 ? 0 : (value)

#define SAFE_RGBF_MIN_BORDER(value) \
  (value) < 0.0f ? 0.0f : (value)

#define SAFE_HSVF_BORDERS(value) \
  (value) > 1.0f ? 1.0f : (value) < 0.0f ? 0.0f : (value)

class Converter
{
public:

  static void rgbToHsv(float r, float g, float b, float * h, float * s, float * v);
  static void hsvToRgb(float * r, float * g, float * b, float h, float s, float v);
};

#endif // CONVERTER_H
