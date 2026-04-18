#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Slider {
public:
    Slider() = default;
    Slider(const sf::Vector2f& position, float width, float minValue, float maxValue, float initialValue,
           const sf::Font& font, const std::string& label);

    void setPosition(const sf::Vector2f& position);
    void setValue(float value);
    float getValue() const;
    void setLabel(const std::string& label);

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderTarget& target) const;

private:
    void updateKnobFromValue();
    void updateValueFromMouse(float mouseX);

    sf::RectangleShape track_;
    sf::RectangleShape fill_;
    sf::CircleShape knob_;
    sf::Text labelText_;
    sf::Text valueText_;
    float minValue_ = 0.0f;
    float maxValue_ = 1.0f;
    float value_ = 0.0f;
    bool dragging_ = false;
};
