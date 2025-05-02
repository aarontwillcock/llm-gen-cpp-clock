#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[9];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return std::string(buffer);
}

struct Theme {
    sf::Color background;
    sf::Color foreground;
    std::string name;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Current Time", sf::Style::Resize | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // VS Code-inspired themes
    std::vector<Theme> themes = {
        {sf::Color(30, 30, 30), sf::Color::White, "Dark+"},
        {sf::Color::White, sf::Color::Black, "Light+"},
        {sf::Color(12, 12, 12), sf::Color(198, 120, 221), "Monokai"},
        {sf::Color(39, 40, 34), sf::Color(249, 38, 114), "Dracula"},
        {sf::Color(43, 48, 59), sf::Color(97, 175, 239), "One Dark"},
        {sf::Color(250, 250, 250), sf::Color(0, 0, 0), "Quiet Light"},
        {sf::Color(245, 245, 245), sf::Color(51, 51, 51), "Solarized Light"},
        {sf::Color(255, 255, 255), sf::Color(60, 60, 60), "Light Visual Studio"},
        {sf::Color(20, 20, 20), sf::Color(102, 217, 239), "Night Owl"},
        {sf::Color(250, 250, 250), sf::Color(88, 110, 117), "GitHub Light"}
    };
    int currentTheme = 0;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(100);
    text.setFillColor(themes[currentTheme].foreground);

    // Dropdown menu basics
    bool dropdownOpen = false;
    sf::RectangleShape dropdownBox(sf::Vector2f(150, 35));
    dropdownBox.setFillColor(sf::Color(100, 100, 100));
    dropdownBox.setPosition(20, 20);

    sf::Text dropdownLabel;
    dropdownLabel.setFont(font);
    dropdownLabel.setCharacterSize(24);
    dropdownLabel.setFillColor(themes[currentTheme].foreground);
    dropdownLabel.setPosition(25, 25);
    dropdownLabel.setString(themes[currentTheme].name);

    std::vector<sf::Text> themeOptions;
    for (size_t i = 0; i < themes.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(themes[i].name);
        option.setCharacterSize(24);
        option.setPosition(25, 60 + static_cast<int>(i) * 35);
        option.setFillColor(themes[currentTheme].foreground);
        themeOptions.push_back(option);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (dropdownBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        dropdownOpen = !dropdownOpen;
                    } else if (dropdownOpen) {
                        for (size_t i = 0; i < themeOptions.size(); ++i) {
                            if (themeOptions[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                currentTheme = static_cast<int>(i);
                                text.setFillColor(themes[currentTheme].foreground);
                                dropdownLabel.setString(themes[currentTheme].name);
                                dropdownLabel.setFillColor(themes[currentTheme].foreground);
                                for (auto& option : themeOptions) {
                                    option.setFillColor(themes[currentTheme].foreground);
                                }
                                dropdownOpen = false;
                            }
                        }
                    } else {
                        dropdownOpen = false;
                    }
                }
            }
        }

        sf::Vector2u windowSize = window.getSize();
        text.setCharacterSize(windowSize.y / 6);
        text.setPosition(windowSize.x / 2 - text.getGlobalBounds().width / 2, windowSize.y / 2 - text.getGlobalBounds().height / 2);

        text.setString(getCurrentTime());

        window.clear(themes[currentTheme].background);
        window.draw(text);
        window.draw(dropdownBox);
        window.draw(dropdownLabel);
        if (dropdownOpen) {
            for (const auto& option : themeOptions)
                window.draw(option);
        }
        window.display();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

