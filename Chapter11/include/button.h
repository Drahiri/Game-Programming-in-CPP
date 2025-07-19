#ifndef BUTTON_H
#define BUTTON_H

#include "math.h"

#include <functional>
#include <string>

class Font;
class Texture;

class Button {
public:
    // Constructor takes in a name, font, callback function
    // and position/dimensions of button
    Button(const std::string& name,
          Font* font,
          std::function<void()> onClick,
          const Vector2& pos,
          const Vector2& dims);
    ~Button();
    // Set the name of the button, and generate name texture
    void setName(const std::string& name);
    const std::string& getName() const;

    // Returns true if the point is withing the button's bounds
    bool containsPoint(const Vector2& pt) const;
    // Called when button is clicked
    void onClick();

    Texture* getNameTex() const;

    void setHighlighted(bool value);
    bool getHighlighted() const;

    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;

private:
    std::function<void()> onClickFunc;
    std::string name;
    Texture* nameTex;
    Font* font;
    Vector2 position;
    Vector2 dimensions;
    bool highlighted;
};

#endif