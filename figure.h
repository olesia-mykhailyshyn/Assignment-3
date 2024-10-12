#pragma once
#include <string>
#include <memory>
#include "color.h"

class Board;

enum class FillMode {
    Frame,
    Fill
};

class Figure {
public:
    Figure(int x, int y, const Color& color = Color(ColorName::Reset), FillMode fillMode = FillMode::Frame)
            : x(x), y(y), color(color), fillMode(fillMode) {}
    virtual void draw(Board& board) = 0;
    [[nodiscard]] virtual std::string getInfo() const = 0;
    [[nodiscard]] virtual std::string getSaveFormat() const = 0;
    [[nodiscard]] virtual bool isOutOfBounds(int boardWidth, int boardHeight) const = 0;
    static bool isPositionOutOfBounds(int x, int y, int boardWidth, int boardHeight);

    void setColor(ColorName newColor) { color = Color(newColor); }


    int x;
    int y;
    Color color;
    FillMode fillMode;
};

class Triangle : public Figure {
public:
    Triangle(int x, int y, int height, const Color& color = Color(ColorName::Reset), FillMode fillMode = FillMode::Frame)
            : Figure(x, y, color, fillMode), height(height) {}

    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;

    int height;
};

class Rectangle : public Figure {
public:
    Rectangle(int x, int y, int width, int height, const Color& color = Color(ColorName::Reset), FillMode fillMode = FillMode::Frame)
            : Figure(x, y, color, fillMode), width(width), height(height) {}

    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;

    int width, height;
};

class Circle : public Figure {
public:
    Circle(int x, int y, int radius, const Color& color = Color(ColorName::Reset), FillMode fillMode = FillMode::Frame)
            : Figure(x, y, color, fillMode), radius(radius) {}

    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;

    int radius;
};

class Line : public Figure {
public:
    Line(int x1, int y1, int x2, int y2, const Color& color = Color(ColorName::Reset), FillMode fillMode = FillMode::Frame)
            : Figure(x1, y1, color, fillMode), x2(x2), y2(y2) {}

    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;

    int x2, y2;
};