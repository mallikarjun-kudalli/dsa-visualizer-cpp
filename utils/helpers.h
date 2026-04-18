#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

namespace utils {
float clamp(float value, float minValue, float maxValue);
bool pointInRect(float x, float y, const sf::FloatRect& rect);
float mapRange(float value, float inMin, float inMax, float outMin, float outMax);
sf::Text makeText(const sf::Font& font, const std::string& content, unsigned size, float x, float y);
bool loadDefaultFont(sf::Font& font);
}  // namespace utils
