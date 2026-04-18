#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button() = default;
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, const std::string& label);

    void setLabel(const std::string& label);
    void setPosition(const sf::Vector2f& position);
    void setEnabled(bool enabled);

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderTarget& target) const;

private:
    sf::RectangleShape rect_;
    sf::Text text_;
    bool hovered_ = false;
    bool enabled_ = true;
};
