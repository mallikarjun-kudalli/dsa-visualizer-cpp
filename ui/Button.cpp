#include "Button.h"

#include "../utils/constants.h"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, const std::string& label) {
    rect_.setPosition(position);
    rect_.setSize(size);
    rect_.setFillColor(sf::Color(44, 46, 62));
    rect_.setOutlineThickness(1.0f);
    rect_.setOutlineColor(constants::BORDER_COLOR);

    text_.setFont(font);
    text_.setCharacterSize(16);
    text_.setFillColor(constants::TEXT_COLOR);
    setLabel(label);
}

void Button::setLabel(const std::string& label) {
    text_.setString(label);
    const sf::FloatRect bounds = text_.getLocalBounds();
    text_.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text_.setPosition(rect_.getPosition().x + rect_.getSize().x / 2.0f, rect_.getPosition().y + rect_.getSize().y / 2.0f);
}

void Button::setPosition(const sf::Vector2f& position) {
    rect_.setPosition(position);
    setLabel(text_.getString());
}

void Button::setEnabled(bool enabled) {
    enabled_ = enabled;
}

bool Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!enabled_) {
        return false;
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f world = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        return rect_.getGlobalBounds().contains(world);
    }
    return false;
}

void Button::update(const sf::RenderWindow& window) {
    const sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    hovered_ = enabled_ && rect_.getGlobalBounds().contains(mouse);

    if (!enabled_) {
        rect_.setFillColor(sf::Color(38, 38, 50));
        text_.setFillColor(constants::MUTED_TEXT_COLOR);
    } else if (hovered_) {
        rect_.setFillColor(sf::Color(66, 70, 94));
        text_.setFillColor(constants::TEXT_COLOR);
    } else {
        rect_.setFillColor(sf::Color(44, 46, 62));
        text_.setFillColor(constants::TEXT_COLOR);
    }
}

void Button::draw(sf::RenderTarget& target) const {
    target.draw(rect_);
    target.draw(text_);
}
