#include <cmath>
#include "figure.h"
#include "board.h"
#include "color.h"

bool Figure::isPositionOutOfBounds(int x, int y, int boardWidth, int boardHeight) {
    return (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight);
}

void Triangle::draw(Board& board) {
    std::string colorCode = ColorFormatter::getAnsiCode(color);
    std::string resetCode = ColorFormatter::getAnsiCode(Color(ColorName::Reset));
    std::string colorChar = std::string(1, color.getName()[0]);

    for (int i = 0; i < height; ++i) {
        int leftMost = x - i;
        int rightMost = x + i;
        int posY = y + i;

        for (int posX = leftMost; posX <= rightMost; ++posX) {
            if (posY >= 0 && posY < board.boardHeight && posX >= 0 && posX < board.boardWidth) {
                std::string content = colorCode + colorChar + resetCode;
                if (fillMode == FillMode::Frame && (posX == leftMost || posX == rightMost || i == height - 1)) {
                    board.grid[posY][posX] = content;
                }
                else if (fillMode == FillMode::Fill) {
                    board.grid[posY][posX] = content;
                }
            }
        }
    }
}

bool Triangle::isOutOfBounds(int boardWidth, int boardHeight) const {
    return height <= 0 || (y + height - 1 < 0) || (y >= boardHeight) || (x - height + 1 >= boardWidth) || (x + height - 1 < 0);
}

std::string Triangle::getInfo() const {
    return "Triangle at (" + std::to_string(x) + ", " + std::to_string(y) + "), height: " + std::to_string(height);
}

std::string Triangle::getSaveFormat() const {
    return "Triangle " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(height) + " 0";
}

void Rectangle::draw(Board& board) {
    std::string colorCode = ColorFormatter::getAnsiCode(color);
    std::string resetCode = ColorFormatter::getAnsiCode(Color(ColorName::Reset));

    char colorChar = color.getName()[0];
    std::string filledCell = colorCode + colorChar + resetCode;

    for (int row = y; row < y + height; ++row) {
        for (int col = x; col < x + width; ++col) {
            if (row >= 0 && row < board.boardHeight && col >= 0 && col < board.boardWidth) {
                if (fillMode == FillMode::Frame && (row == y || row == y + height - 1 || col == x || col == x + width - 1)) {
                    board.grid[row][col] = filledCell;
                }
                else if (fillMode == FillMode::Fill) {
                    board.grid[row][col] = filledCell;
                }
            }
        }
    }
}

bool Rectangle::isOutOfBounds(int boardWidth, int boardHeight) const {
    return width <= 0 || height <= 0 || (x + width - 1 < 0) || (y + height - 1 < 0) || x >= boardWidth || y >= boardHeight;
}

std::string Rectangle::getInfo() const {
    return "Rectangle at (" + std::to_string(x) + ", " + std::to_string(y) + "), width: " + std::to_string(width) + ", height: " + std::to_string(height);
}

std::string Rectangle::getSaveFormat() const {
    return "Rectangle " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(width) + " " + std::to_string(height);
}

void Circle::draw(Board& board) {
    std::string colorCode = ColorFormatter::getAnsiCode(color);
    std::string resetCode = ColorFormatter::getAnsiCode(Color(ColorName::Reset));
    char colorChar = color.getName()[0];
    std::string filledCell = colorCode + colorChar + resetCode;

    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            int distanceSquared = i * i + j * j;
            if ((fillMode == FillMode::Frame && distanceSquared >= radius * radius - radius && distanceSquared <= radius * radius) ||
                (fillMode == FillMode::Fill && distanceSquared <= radius * radius)) {
                int drawX = x + j;
                int drawY = y + i;
                if (drawX >= 0 && drawX < board.boardWidth && drawY >= 0 && drawY < board.boardHeight) {
                    board.grid[drawY][drawX] = filledCell;
                }
            }
        }
    }
}

bool Circle::isOutOfBounds(int boardWidth, int boardHeight) const {
    return radius <= 0 || (x + radius < 0) || (x - radius >= boardWidth) || (y + radius < 0) || (y - radius >= boardHeight);
}

std::string Circle::getInfo() const {
    return "Circle at (" + std::to_string(x) + ", " + std::to_string(y) + "), radius: " + std::to_string(radius);
}

std::string Circle::getSaveFormat() const {
    return "Circle " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(radius) + " 0";
}

void Line::draw(Board& board) {
    std::string colorCode = ColorFormatter::getAnsiCode(color);
    std::string resetCode = ColorFormatter::getAnsiCode(Color(ColorName::Reset));
    char colorChar = color.getName()[0];
    std::string lineCell = colorCode + colorChar + resetCode;

    int x1 = x;
    int y1 = y;
    int x2 = this->x2;
    int y2 = this->y2;

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < board.boardWidth && y1 >= 0 && y1 < board.boardHeight) {
            board.grid[y1][x1] = lineCell;
        }

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

bool Line::isOutOfBounds(int boardWidth, int boardHeight) const {
    return (x < 0 && x2 < 0) || (y < 0 && y2 < 0) || (x >= boardWidth && x2 >= boardWidth) || (y >= boardHeight && y2 >= boardHeight);
}

std::string Line::getInfo() const {
    return "Line from (" + std::to_string(x) + ", " + std::to_string(y) + ") to (" + std::to_string(x2) + ", " + std::to_string(y2) + ")";
}

std::string Line::getSaveFormat() const {
    return "Line " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(x2) + " " + std::to_string(y2);
}