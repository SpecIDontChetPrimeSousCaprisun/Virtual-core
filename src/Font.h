#pragma once

#include "../include/glad/glad.h"

#include <string>
#include <vector>
#include <map>

#include <stb/stb_truetype.h>

struct FontInfo {
public:
  FontInfo(std::string path, float pixelHeight);

  bool operator<(const FontInfo& other) const;

  std::string path;
  float pixelHeight;
};

class Font {
public: 
  static Font* getFont(const std::string& path, float pixelHeight);

  unsigned int texture;
  int ascent, descent, lineGap;
  float scale, height;
  stbtt_fontinfo fontInfo;

  stbtt_bakedchar cdata[96];
private:
  Font(const std::string& path, float pixelHeight);

  static std::map<FontInfo, Font*> fonts;

  std::vector<unsigned char> ttfBuffer;
};
