#pragma once
#include <string>
#include <memory>

class Board;

class Figure {
public:
    Figure(int x, int y) : x(x), y(y) {}
    virtual void draw(Board& board) = 0;
    [[nodiscard]] virtual std::string getInfo() const = 0;
    [[nodiscard]] virtual std::string getSaveFormat() const = 0;
    [[nodiscard]] virtual bool isOutOfBounds(int boardWidth, int boardHeight) const = 0;
    static bool isPositionOutOfBounds(int x, int y, int boardWidth, int boardHeight);

protected:
    int x, y;
};

class Triangle : public Figure {
public:
    Triangle(int x, int y, int height) : Figure(x, y), height(height) {}
    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;

private:
    int height;
};

class Rectangle : public Figure {
public:
    Rectangle(int x, int y, int width, int height) : Figure(x, y), width(width), height(height) {}
    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;
private:
    int width, height;
};

class Circle : public Figure {
public:
    Circle(int x, int y, int radius) : Figure(x, y), radius(radius) {}
    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;

private:
    int radius;
};

class Line : public Figure {
public:
    Line(int x1, int y1, int x2, int y2) : Figure(x1, y1), x2(x2), y2(y2) {}
    void draw(Board& board) override;
    [[nodiscard]] std::string getInfo() const override;
    [[nodiscard]] std::string getSaveFormat() const override;
    [[nodiscard]] bool isOutOfBounds(int boardWidth, int boardHeight) const override;
private:
    int x2, y2;
};