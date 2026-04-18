#include "Slider.h"

#include <iomanip>
#include <sstream>

#include "../utils/constants.h"
#include "../utils/helpers.h"

Slider::Slider(const sf::Vector2f& position, float width, float minValue, float maxValue, float initialValue,
               const sf::Font& font, const std::string& label)
    : minValue_(minValue), maxValue_(maxValue), value_(initialValue) {
    track_.setPosition(position);
    track_.setSize({width, 6.0f});
    track_.setFillColor(sf::Color(56, 59, 80));

    fill_.setPosition(position);
    fill_.setSize({1.0f, 6.0f});
    fill_.setFillColor(sf::Color(120, 190, 255));

    knob_.setRadius(8.0f);
    knob_.setOrigin(8.0f, 8.0f);
    knob_.setFillColor(sf::Color(235, 235, 245));

    labelText_.setFont(font);
    labelText_.setCharacterSize(14);
    labelText_.setFillColor(constants::MUTED_TEXT_COLOR);
    labelText_.setString(label);
    labelText_.setPosition(position.x, position.y - 22.0f);

    valueText_.setFont(font);
    valueText_.setCharacterSize(14);
    valueText_.setFillColor(constants::TEXT_COLOR);

    updateKnobFromValue();
}

void Slider::setPosition(const sf::Vector2f& position) {
    track_.setPosition(position);
    fill_.setPosition(position);
    labelText_.setPosition(position.x, position.y - 22.0f);
    updateKnobFromValue();
}

void Slider::setValue(float value) {
    value_ = utils::clamp(value, minValue_, maxValue_);
    updateKnobFromValue();
}

float Slider::getValue() const {
    return value_;
}

void Slider::setLabel(const std::string& label) {
    labelText_.setString(label);
}

bool Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f mouse = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        const bool hitKnob = knob_.getGlobalBounds().contains(mouse);
        const bool hitTrack = track_.getGlobalBounds().contains(mouse.x, mouse.y + 3.0f);
        if (hitKnob || hitTrack) {
            dragging_ = true;
            updateValueFromMouse(mouse.x);
            return true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        dragging_ = false;
    }

    if (event.type == sf::Event::MouseMoved && dragging_) {
        const sf::Vector2f mouse = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        updateValueFromMouse(mouse.x);
        return true;
    }
    return false;
}

void Slider::update(const sf::RenderWindow&) {
    updateKnobFromValue();
}

void Slider::draw(sf::RenderTarget& target) const {
    target.draw(track_);
    target.draw(fill_);
    target.draw(knob_);
    target.draw(labelText_);
    target.draw(valueText_);
}

void Slider::updateKnobFromValue() {
    const float x = utils::mapRange(value_, minValue_, maxValue_, track_.getPosition().x,
                                    track_.getPosition().x + track_.getSize().x);
    knob_.setPosition(x, track_.getPosition().y + track_.getSize().y / 2.0f);

    fill_.setSize({x - track_.getPosition().x, fill_.getSize().y});

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << value_;
    valueText_.setString(oss.str());
    valueText_.setPosition(track_.getPosition().x + track_.getSize().x + 8.0f, track_.getPosition().y - 11.0f);
}

void Slider::updateValueFromMouse(float mouseX) {
    const float left = track_.getPosition().x;
    const float right = left + track_.getSize().x;
    const float clampedX = utils::clamp(mouseX, left, right);
    value_ = utils::mapRange(clampedX, left, right, minValue_, maxValue_);
    updateKnobFromValue();
}
