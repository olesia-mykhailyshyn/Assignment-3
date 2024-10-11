#include "board.h"
#include "fstream"
#include <iostream>
#include <iomanip>

void Board::print() const {
    std::cout << "   ";
    for (int col = 0; col < boardWidth; ++col) {
        std::cout << std::setw(2) << col << " ";
    }
    std::cout << "\n";

    std::cout << "  +";
    for (int col = 0; col < boardWidth; ++col) {
        std::cout << "---";
    }
    std::cout << "+\n";

    for (int row = 0; row < boardHeight; ++row) {
        std::cout << std::setw(2) << row << "|";
        for (char c : grid[row]) {
            std::cout << " " << c << " ";
        }
        std::cout << "|\n";
    }

    std::cout << "  +";
    for (int col = 0; col < boardWidth; ++col) {
        std::cout << "---";
    }
    std::cout << "+\n";
}

std::vector<std::shared_ptr<Figure>> Board::getFigures() const {
    return figures;
}

void Board::add(const std::string& shapeName, int x, int y, int parameter1, int parameter2) {
    std::shared_ptr<Figure> newFigure = nullptr;

    if (shapeName == "triangle") {
        newFigure = std::make_shared<Triangle>(x, y, parameter1);
    }
    else if (shapeName == "rectangle") {
        newFigure = std::make_shared<Rectangle>(x, y, parameter1, parameter2);
    }
    else if (shapeName == "circle") {
        newFigure = std::make_shared<Circle>(x, y, parameter1);
    }
    else if (shapeName == "line") {
        newFigure = std::make_shared<Line>(x, y, parameter1, parameter2);
    }
    else {
        std::cout << "Invalid shape name." << std::endl;
        return;
    }

    if (isDuplicate(newFigure)) {
        std::cout << "Error: Figure with the same parameters already exists at the same position!" << std::endl;
        return;
    }
    else if (newFigure->isOutOfBounds(boardWidth, boardHeight)) {
        std::cout << "Error: Figure is too large to fit on the board and cannot be added." << std::endl;
        return;
    }
    else {
        figures.push_back(newFigure);
        std::cout << "Figure added successfully!" << std::endl;
    }
}

void Board::load(const std::string& filePath) {
    std::ifstream input(filePath, std::ios::in);
    if (!input.is_open()) {
        std::cout << "Could not open file " << filePath << " for reading." << std::endl;
        return;
    }

    if (input.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "The file " << filePath << " is empty. Nothing to load." << std::endl;
        input.close();
        return;
    }

    figures.clear();
    std::string shapeName;
    bool loadSuccessful = true;

    while (input >> shapeName) {
        int x, y, param1, param2 = 0;

        if (!(input >> x >> y >> param1)) {
            loadSuccessful = false;
            break;
        }

        if (shapeName == "Rectangle" || shapeName == "Line") {
            if (!(input >> param2)) {
                loadSuccessful = false;
                break;
            }
        }

        std::shared_ptr<Figure> newFigure = nullptr;

        if (shapeName == "Triangle") {
            newFigure = std::make_shared<Triangle>(x, y, param1);
        }
        else if (shapeName == "Rectangle") {
            newFigure = std::make_shared<Rectangle>(x, y, param1, param2);
        }
        else if (shapeName == "Circle") {
            newFigure = std::make_shared<Circle>(x, y, param1);
        }
        else if (shapeName == "Line") {
            newFigure = std::make_shared<Line>(x, y, param1, param2);
        }
        else {
            std::cout << "Error: Invalid shape type found in file. Aborting load." << std::endl;
            loadSuccessful = false;
            break;
        }

        if (newFigure->isOutOfBounds(boardWidth, boardHeight)) {
            std::cout << "Error: Figure " << newFigure->getInfo() << " is too large to fit on the board." << std::endl;
            loadSuccessful = false;
            break;
        }

        if (!isDuplicate(newFigure)) {
            figures.push_back(newFigure);
        }
        else {
            std::cout << "Error: Duplicate figure found in file. Aborting load." << std::endl;
            loadSuccessful = false;
            break;
        }
    }

    input.close();

    if (loadSuccessful) {
        std::cout << "Figures loaded successfully from " << filePath << std::endl;
    }
    else {
        std::cout << "Invalid data found in file: " << filePath << ". Aborting load." << std::endl;
    }
}

bool Board::isDuplicate(const std::shared_ptr<Figure>& figure) const {
    for (const std::shared_ptr<Figure>& existingFigure : figures) {
        if (figure->getSaveFormat() == existingFigure->getSaveFormat()) {
            return true;
        }
    }
    return false;
}

void Board::draw() {
    grid.assign(boardHeight, std::vector<char>(boardWidth, ' '));
    for (const std::shared_ptr<Figure>& figure : getFigures()) {
        figure->draw(*this);
    }
    print();
}

void Board::list() const {
    if (figures.empty()) {
        std::cout << "There are no figures on the board." << std::endl;
    }
    else {
        std::cout << "Figures on the board:" << std::endl;
        for (const std::shared_ptr<Figure>& figure : getFigures()) {
            if (figure != nullptr) {
                std::cout << figure->getInfo() << std::endl;
            }
        }
    }
}

void Board::shapes() {
    std::cout << "List of available shapes and their parameters:" << std::endl;
    std::cout << "1. Circle: x, y, radius" << std::endl;
    std::cout << "2. Rectangle: x, y, width, height" << std::endl;
    std::cout << "3. Triangle: x, y, height" << std::endl;
    std::cout << "4. Line: x1, y1, x2, y2 (start and end points)" << std::endl;
}


void Board::undo() {
    if (figures.empty()) {
        std::cout << "There are no figures. Undo command cannot be performed." << std::endl;
    }
    else {
        figures.pop_back();
        std::cout << "The last added figure was deleted." << std::endl;
    }
}

void Board::save(const std::string& filePath) const {
    std::ofstream myFile(filePath, std::ios::out);
    if (myFile.is_open()) {
        if (figures.empty()) {
            std::cout << "There are no figures. An empty file will be saved." << std::endl;
        }
        else {
            for (const std::shared_ptr<Figure>& figure : getFigures()) {
                if (figure != nullptr) {
                    myFile << figure->getSaveFormat() << std::endl;
                }
            }
            std::cout << "Figures saved to " << filePath << std::endl;
        }
        myFile.close();
    }
    else {
        std::cout << "Could not open file " << filePath << " for writing." << std::endl;
    }
}

void Board::clear(const std::string& filePath) {
    if (figures.empty()) {
        std::cout << "There are no figures. Clear command cannot be performed." << std::endl;
    }
    else {
        figures.clear();
        std::ofstream ofs;
        ofs.open(filePath, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
        std::cout << "All shapes are removed from the board. File is empty as well." << std::endl;
    }
}

std::string Board::getFilePath() const {
    return filePath;
}