#pragma once
#include <vector>
#include <iostream>
#include "figure.h"
#include <memory>

class Board {
public:
    Board() : grid(boardHeight, std::vector<char>(boardWidth, ' ')) {}

    void print() const;
    [[nodiscard]] std::vector<std::shared_ptr<Figure>> getFigures() const;
    [[nodiscard]] bool isDuplicate(const std::shared_ptr<Figure>& figure) const;

    void draw();
    void list() const;
    static void shapes();
    void add(const std::string& shapeName, int x, int y, int parameter1, int parameter2 = 0);
    void undo();
    void clear(const std::string& filePath);
    void save(const std::string& filePath) const;
    void load(const std::string& filePath);
    [[nodiscard]] std::string getFilePath() const;

    int boardWidth = 10;
    int boardHeight = 10;
    std::vector<std::vector<char>> grid;
    std::vector<std::shared_ptr<Figure>> figures;
    std::string filePath = R"(C:\KSE\OOP_design\Assignment_2\myFile.txt)";
};