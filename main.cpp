#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "algorithms/graph_algorithms.h"
#include "algorithms/sorting_algorithms.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "utils/constants.h"
#include "utils/helpers.h"
#include "visualizer/GraphVisualizer.h"
#include "visualizer/SortingVisualizer.h"

class Dropdown {
public:
    Dropdown() = default;
    Dropdown(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font,
             std::vector<std::string> options)
        : options_(std::move(options)) {
        box_.setPosition(position);
        box_.setSize(size);
        box_.setFillColor(sf::Color(44, 46, 62));
        box_.setOutlineThickness(1.0f);
        box_.setOutlineColor(constants::BORDER_COLOR);

        label_.setFont(font);
        label_.setCharacterSize(16);
        label_.setFillColor(constants::TEXT_COLOR);
        if (!options_.empty()) {
            label_.setString(options_[selected_]);
        }
        label_.setPosition(position.x + 10.0f, position.y + 8.0f);
    }

    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2f mouse = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            if (box_.getGlobalBounds().contains(mouse)) {
                open_ = !open_;
                return false;
            }
            if (open_) {
                for (std::size_t i = 0; i < optionRects_.size(); ++i) {
                    if (optionRects_[i].getGlobalBounds().contains(mouse)) {
                        selected_ = i;
                        label_.setString(options_[selected_]);
                        open_ = false;
                        return true;
                    }
                }
                open_ = false;
            }
        }
        return false;
    }

    void update() {
        optionRects_.clear();
        optionTexts_.clear();
        if (!open_) {
            return;
        }

        const sf::Vector2f pos = box_.getPosition();
        const sf::Vector2f size = box_.getSize();
        for (std::size_t i = 0; i < options_.size(); ++i) {
            sf::RectangleShape rect;
            rect.setPosition(pos.x, pos.y + size.y + static_cast<float>(i) * size.y);
            rect.setSize(size);
            rect.setFillColor(sf::Color(37, 39, 54));
            rect.setOutlineThickness(1.0f);
            rect.setOutlineColor(constants::BORDER_COLOR);
            optionRects_.push_back(rect);

            sf::Text txt = label_;
            txt.setString(options_[i]);
            txt.setPosition(rect.getPosition().x + 10.0f, rect.getPosition().y + 8.0f);
            optionTexts_.push_back(txt);
        }
    }

    void draw(sf::RenderTarget& target) const {
        target.draw(box_);
        target.draw(label_);

        sf::Text icon = label_;
        icon.setString(open_ ? "^" : "v");
        icon.setPosition(box_.getPosition().x + box_.getSize().x - 18.0f, box_.getPosition().y + 7.0f);
        target.draw(icon);

        if (open_) {
            for (std::size_t i = 0; i < optionRects_.size(); ++i) {
                target.draw(optionRects_[i]);
                target.draw(optionTexts_[i]);
            }
        }
    }

    std::string selected() const {
        if (options_.empty()) {
            return {};
        }
        return options_[selected_];
    }

private:
    sf::RectangleShape box_;
    sf::Text label_;
    std::vector<std::string> options_;
    std::size_t selected_ = 0;
    bool open_ = false;
    std::vector<sf::RectangleShape> optionRects_;
    std::vector<sf::Text> optionTexts_;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "DSA Visualizer",
                            sf::Style::Close);
    window.setFramerateLimit(120);

    sf::Font font;
    if (!utils::loadDefaultFont(font)) {
        std::cerr << "Failed to load font. Add a font file to assets/ and try again.\n";
        return 1;
    }

    const std::vector<std::string> algorithms = {
        "Sort: Bubble Sort", "Sort: Selection Sort", "Sort: Insertion Sort", "Sort: Merge Sort",
        "Sort: Quick Sort",   "Graph: BFS",          "Graph: DFS",            "Graph: Dijkstra"};

    Dropdown dropdown({18.0f, 24.0f}, {235.0f, 36.0f}, font, algorithms);
    Button startButton({270.0f, 24.0f}, {110.0f, 36.0f}, font, "Start");
    Button pauseButton({390.0f, 24.0f}, {110.0f, 36.0f}, font, "Pause");
    Button resetButton({510.0f, 24.0f}, {110.0f, 36.0f}, font, "Reset");
    Slider speedSlider({640.0f, 44.0f}, 250.0f, constants::MIN_SPEED, constants::MAX_SPEED, constants::DEFAULT_SPEED,
                       font, "Speed");

    SortingVisualizer sorting;
    GraphVisualizer graph;

    enum class ViewMode { Sorting, Graph };
    ViewMode mode = ViewMode::Sorting;
    std::string algorithmName = "Bubble Sort";

    sf::Clock frameClock;
    while (window.isOpen()) {
        const float dt = frameClock.restart().asSeconds();
        const sf::FloatRect contentArea(constants::CONTENT_PADDING, constants::TOP_PANEL_HEIGHT + constants::CONTENT_PADDING,
                                        constants::WINDOW_WIDTH - 2.0f * constants::CONTENT_PADDING,
                                        constants::WINDOW_HEIGHT - constants::TOP_PANEL_HEIGHT - 2.0f * constants::CONTENT_PADDING);
        graph.ensureLayout(contentArea);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            dropdown.handleEvent(event, window);

            speedSlider.handleEvent(event, window);

            if (startButton.handleEvent(event, window)) {
                if (mode == ViewMode::Sorting) {
                    sorting.start();
                } else {
                    graph.start();
                }
            }
            if (pauseButton.handleEvent(event, window)) {
                if (mode == ViewMode::Sorting) {
                    if (sorting.isRunning()) {
                        sorting.pause();
                    } else {
                        sorting.resume();
                    }
                } else {
                    if (graph.isRunning()) {
                        graph.pause();
                    } else {
                        graph.resume();
                    }
                }
            }
            if (resetButton.handleEvent(event, window)) {
                if (mode == ViewMode::Sorting) {
                    sorting.resetRandom(constants::DEFAULT_ARRAY_SIZE);
                } else {
                    graph.reset();
                }
            }
        }
        dropdown.update();

        const std::string selected = dropdown.selected();
        if (selected.rfind("Sort:", 0) == 0) {
            mode = ViewMode::Sorting;
            algorithmName = selected.substr(6);
            sorting.setAlgorithm(algorithmName);
        } else {
            mode = ViewMode::Graph;
            algorithmName = selected.substr(7);
            graph.setAlgorithm(algorithmName);
        }

        const float speed = speedSlider.getValue();
        sorting.setSpeed(speed);
        graph.setSpeed(speed);
        sorting.update(dt);
        graph.update(dt);

        if (mode == ViewMode::Sorting) {
            pauseButton.setLabel(sorting.isRunning() ? "Pause" : "Resume");
        } else {
            pauseButton.setLabel(graph.isRunning() ? "Pause" : "Resume");
        }

        startButton.update(window);
        pauseButton.update(window);
        resetButton.update(window);
        speedSlider.update(window);

        window.clear(constants::BG_COLOR);

        sf::RectangleShape panel;
        panel.setPosition(0.0f, 0.0f);
        panel.setSize({static_cast<float>(constants::WINDOW_WIDTH), constants::TOP_PANEL_HEIGHT});
        panel.setFillColor(constants::PANEL_COLOR);
        panel.setOutlineThickness(1.0f);
        panel.setOutlineColor(constants::BORDER_COLOR);
        window.draw(panel);

        dropdown.draw(window);
        startButton.draw(window);
        pauseButton.draw(window);
        resetButton.draw(window);
        speedSlider.draw(window);

        if (mode == ViewMode::Sorting) {
            sorting.draw(window, contentArea);
        } else {
            graph.draw(window, contentArea, font);
        }

        sf::Text title = utils::makeText(font, selected, 18, 18.0f, constants::TOP_PANEL_HEIGHT + 2.0f);
        title.setFillColor(constants::TEXT_COLOR);
        window.draw(title);

        sf::Text complexity;
        complexity.setFont(font);
        complexity.setCharacterSize(15);
        complexity.setFillColor(constants::MUTED_TEXT_COLOR);
        complexity.setPosition(18.0f, constants::TOP_PANEL_HEIGHT + 26.0f);
        if (mode == ViewMode::Sorting) {
            complexity.setString(sortingComplexity(algorithmName));
        } else {
            complexity.setString(graphComplexity(algorithmName));
        }
        window.draw(complexity);

        sf::Text stats;
        stats.setFont(font);
        stats.setCharacterSize(15);
        stats.setFillColor(constants::TEXT_COLOR);
        stats.setPosition(18.0f, constants::WINDOW_HEIGHT - 26.0f);
        if (mode == ViewMode::Sorting) {
            stats.setString("Comparisons: " + std::to_string(sorting.comparisonCount()) + "   Swaps/Writes: " +
                            std::to_string(sorting.swapCount()) + "   Steps: " + std::to_string(sorting.stepCount()));
        } else {
            stats.setString("Visited Nodes: " + std::to_string(graph.visitedCount()) + "   Steps: " +
                            std::to_string(graph.stepCount()));
        }
        window.draw(stats);

        window.display();
    }

    return 0;
}
