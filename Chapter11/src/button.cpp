#include "button.h"

#include "font.h"
#include "texture.h"

Button::Button(const std::string& name,
      Font* font,
      std::function<void()> onClick,
      const Vector2& pos,
      const Vector2& dims) :
    onClickFunc(onClick),
    nameTex(nullptr),
    font(font),
    position(pos),
    dimensions(dims),
    highlighted(false) {
    setName(name);
}

Button::~Button() {
    if(nameTex) {
        nameTex->unload();
        delete nameTex;
    }
    nameTex = font->renderText(name);
}

void Button::setName(const std::string& name) {
    this->name = name;

    if(this->nameTex) {
        nameTex->unload();
        delete nameTex;
        nameTex = nullptr;
    }
    nameTex = font->renderText(this->name);
}

bool Button::containsPoint(const Vector2& pt) const {
    bool no = pt.x < (position.x - dimensions.x / 2.0f) || pt.x > (position.x + dimensions.x / 2.0f)
              || pt.y < (position.y - dimensions.y / 2.0f)
              || pt.y > (position.y + dimensions.y / 2.0f);

    return !no;
}

void Button::onClick() {
    if(onClickFunc) {
        onClickFunc();
    }
}

const std::string& Button::getName() const {
    return name;
}

Texture* Button::getNameTex() const {
    return nameTex;
}

void Button::setHighlighted(bool value) {
    highlighted = value;
}

bool Button::getHighlighted() const {
    return highlighted;
}

void Button::setPosition(const Vector2& pos) {
    position = pos;
}

const Vector2& Button::getPosition() const {
    return position;
}