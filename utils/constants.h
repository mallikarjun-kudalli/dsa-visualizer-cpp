#pragma once

#include <SFML/Graphics/Color.hpp>

namespace constants {
constexpr unsigned WINDOW_WIDTH = 1000;
constexpr unsigned WINDOW_HEIGHT = 700;

constexpr float TOP_PANEL_HEIGHT = 88.0f;
constexpr float CONTENT_PADDING = 16.0f;

constexpr float DEFAULT_SPEED = 100.0f;
constexpr float MIN_SPEED = 5.0f;
constexpr float MAX_SPEED = 500.0f;

constexpr int DEFAULT_ARRAY_SIZE = 95;

static const sf::Color BG_COLOR(18, 18, 24);
static const sf::Color PANEL_COLOR(28, 28, 38);
static const sf::Color BORDER_COLOR(56, 56, 74);
static const sf::Color TEXT_COLOR(230, 230, 240);
static const sf::Color MUTED_TEXT_COLOR(170, 170, 185);

static const sf::Color BAR_DEFAULT(245, 245, 245);
static const sf::Color BAR_COMPARING(243, 208, 84);
static const sf::Color BAR_SWAPPING(233, 88, 74);
static const sf::Color BAR_SORTED(104, 207, 129);

static const sf::Color NODE_DEFAULT(214, 214, 220);
static const sf::Color NODE_VISITED(104, 207, 129);
static const sf::Color NODE_ACTIVE(233, 88, 74);
static const sf::Color EDGE_DEFAULT(90, 92, 112);
static const sf::Color EDGE_ACTIVE(243, 208, 84);
}  // namespace constants
