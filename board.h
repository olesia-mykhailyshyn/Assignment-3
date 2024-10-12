#pragma once
#include <vector>
#include <iostream>
#include "figure.h"
#include <memory>

class Board {
public:
    Board() : grid(boardHeight, std::vector<std::string>(boardWidth, " ")), shapeIDCounter(0), selectedID(-1) {}

    void print() const;
    [[nodiscard]] std::vector<std::shared_ptr<Figure>> getFigures() const;
    [[nodiscard]] bool isDuplicate(const std::shared_ptr<Figure>& figure) const;

    void draw();
    void list() const;
    static void shapes();
    void add(const std::string& shapeName, const std::string& colorStr, int x, int y, int parameter1, int parameter2, FillMode fillMode);
    void undo();
    void clear();
    void save(const std::string& filePath) const;
    void load(const std::string& filePath);
    [[nodiscard]] std::string getFilePath() const;

    void select(int ID);
    void select(int x, int y);
    void remove();
    void edit(int x, int y, int parameter1, int parameter2);
    void paint(const std::string& colorStr);
    void move(int newX, int newY);

    void updateFigureColor(int figureID, const std::string& colorStr);
    int shapeIDCounter;
    int selectedID;
    int boardWidth = 10;
    int boardHeight = 10;
    std::vector<std::vector<std::string>> grid;
    std::vector<std::pair<int, std::shared_ptr<Figure>>> figures;
    std::string filePath = R"(C:\KSE\OOP_design\Assignment_2\myFile.txt)";
};