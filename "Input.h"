#include <SFML/Graphics.hpp>
#include <string>
 
using namespace sf;
using namespace std;
using str = string;
Font font;


int CollumnsX = 50 / 2;
int CollumnsY = 50 / 2;

float W = 3000 / 2;
float H = 3000 / 2;

float Dx = W / CollumnsX;
float Dy = H / CollumnsY;

pair<float, float> C = {727, 724};


RenderWindow window(VideoMode(W, H), "function");

class Button {
    Text title;
    int backlight_thickness = 0;

public:
    int backlight = false;
    RectangleShape field;

    void setTitle(std::string name, int size, Color color = { 0, 0, 0, 255 }) {
        title.setString(name);
        title.setCharacterSize(size);
        title.setFillColor(color);
    }

    void setTitlePosition(Vector2f pos) {
        title.setPosition(pos);
    }

    void setButtonPosition(Vector2f pos) {
        field.setPosition(pos);
    }

    void setButtonSize(Vector2f size, int size_thickness = 0) {
        field.setSize(size);
        backlight_thickness = size_thickness;
    }

    void setButtonColor(Color fill_color, Color outline_color = Color::Cyan) {
        field.setFillColor(fill_color);
        field.setOutlineColor(outline_color);
    }

    bool contains(const Vector2i& mouse) {
        bool check = (mouse.x >= field.getPosition().x &&
                      mouse.x <= (field.getPosition().x + field.getSize().x) &&
                      mouse.y >= field.getPosition().y &&
                      mouse.y <= (field.getPosition().y + field.getSize().y));
        if (backlight >= 0) {
            if (check ^ backlight) {
                backlight ^= 1;
            }
        }
        return check;
    }

    bool pressed(Vector2i& mouse, Event now) {
        return (contains(mouse) && now.type == Event::MouseButtonPressed && now.key.code == Mouse::Left);
    }

    void draw(RenderWindow& window) {
        if (backlight == 1 || backlight == -1) {
            field.setOutlineThickness(backlight_thickness);
        } else {
            field.setOutlineThickness(0);
        }
        window.draw(field);
        title.setFont(font);
        window.draw(title);
    }
};

class InputField {
    Button field;
    Text text;
    bool chosen = false;
    bool only_numbers = false;

    Clock clock;

public:
    std::string value;
    void setOnlyNumbers(bool type) {
        only_numbers = type;
    }

    void setTitle(string name, int size, Color color = Color::White) {
        field.setTitle(name, size, color);
    }

    void setTitlePosition(Vector2f pos) {
        field.setTitlePosition(pos);
    }

    void setButtonPosition(Vector2f pos) {
        field.setButtonPosition(pos);
    }

    void setTextPosition(Vector2f pos) {
        text.setPosition(pos);
    }

    void setButtonSettings(Vector2f size, int outline_size, Color fill, Color outline = {0, 0, 0, 255}) {
        field.setButtonSize(size, outline_size);
        field.setButtonColor(fill, outline);
    }

    void setTextSettings(int size, Color color) {
        text.setCharacterSize(size);
        text.setFillColor(color);
    }

    void change(Event& now, Vector2i mouse) {
        if (chosen) {
            if (now.key.code == Keyboard::Backspace && value.size() != 0) {
                Time time = clock.getElapsedTime();
                if (time > milliseconds(130)) {
                    clock.restart();
                    value.pop_back();
                }
            }
            if (now.type == Event::TextEntered) {
                if (now.text.unicode < 128 && now.text.unicode != 8 ||
                    now.text.unicode >= 1025 && now.text.unicode <= 1105) {
                    if (!only_numbers || (now.text.unicode >= (wchar_t)'0' && now.text.unicode <= (wchar_t)'9')) {
                        value += now.text.unicode;
                    }
                }
            }
            text.setString(value);
            if (text.getPosition().x + text.getLocalBounds().width >= field.field.getPosition().x + field.field.getSize().x) {
                value.pop_back();
                text.setString(value);
            }
        }
        if (field.contains(mouse) && now.type == Event::MouseButtonPressed && now.key.code == Mouse::Left) {
            chosen = true;
            field.backlight = -1;
        } else if (!field.contains(mouse) &&
                   (now.key.code == Mouse::Left && now.type == Event::MouseButtonPressed)
                   || now.key.code == Keyboard::Enter) {
            chosen = false;
            field.backlight = -2;
        } else if (field.backlight != -1) {
            field.backlight = -2;
        }
    }

    void draw(RenderWindow& window) {
        field.draw(window);
        text.setFont(font);
        window.draw(text);
    }
};

struct Map {
    const sf::Vector2i GridSize{ int(window.getSize().x) / CollumnsX, int(window.getSize().y) / CollumnsY };
    const sf::Color GridLinesColor = sf::Color::Black;

    void drawCollumns() {
        for (int i = 1; i < CollumnsX; i++) {
            float Thickness = 1.f;
            if (i == CollumnsX / 2)
                Thickness = 5.f;

            sf::RectangleShape Line({ Thickness, float(window.getSize().y) });
            Line.setFillColor(GridLinesColor);
            Line.setPosition(i * GridSize.x, 0.f);
            window.draw(Line);
        }
        for (int i = 1; i < CollumnsY; i++) {
            float Thickness = 1.f;
            if (i == CollumnsY / 2) {
                Thickness = 5.f;
            }
            sf::RectangleShape Line({ float(window.getSize().x), Thickness });
            Line.setFillColor(GridLinesColor);
            Line.setPosition(0.f, i * GridSize.y);
            window.draw(Line);
        }
    }
};
