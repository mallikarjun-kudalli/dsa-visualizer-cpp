#include "helpers.h"

#include <algorithm>
#include <array>

namespace utils {
float clamp(float value, float minValue, float maxValue) {
    return std::max(minValue, std::min(value, maxValue));
}

bool pointInRect(float x, float y, const sf::FloatRect& rect) {
    return rect.contains(x, y);
}

float mapRange(float value, float inMin, float inMax, float outMin, float outMax) {
    if (inMax - inMin == 0.0f) {
        return outMin;
    }
    const float normalized = (value - inMin) / (inMax - inMin);
    return outMin + normalized * (outMax - outMin);
}

sf::Text makeText(const sf::Font& font, const std::string& content, unsigned size, float x, float y) {
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setPosition(x, y);
    return text;
}

bool loadDefaultFont(sf::Font& font) {
    static const std::array<const char*, 4> candidates = {
        "assets/Roboto-Regular.ttf",
        "assets/Inter-Regular.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
    };
    for (const char* path : candidates) {
        if (font.loadFromFile(path)) {
            return true;
        }
    }
    return false;
}
}  // namespace utils
