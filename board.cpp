#include "board.h"
#include "fstream"
#include <iostream>
#include <iomanip>
#include "enums.h"

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
    std::vector<std::shared_ptr<Figure>> result;
    for (const auto& figurePair : figures) {
        result.push_back(figurePair.second);
    }
    return result;
}

void Board::add(const std::string& shapeName, const std::string& colorStr, int x, int y, int parameter1, int parameter2, FillMode fillMode) {
    ColorName colorName = Color::fromString(colorStr);
    if (colorName == ColorName::Invalid) {
        std::cout << "Invalid color." << std::endl;
        return;
    }
    Color color(colorName);
    std::shared_ptr<Figure> newFigure = nullptr;

    if (shapeName == "triangle") {
        newFigure = std::make_shared<Triangle>(x, y, parameter1, color, fillMode);
    }
    else if (shapeName == "rectangle") {
        newFigure = std::make_shared<Rectangle>(x, y, parameter1, parameter2, color, fillMode);
    }
    else if (shapeName == "circle") {
        newFigure = std::make_shared<Circle>(x, y, parameter1, color, fillMode);
    }
    else if (shapeName == "line") {
        newFigure = std::make_shared<Line>(x, y, parameter1, parameter2, color, fillMode);
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
        figures.emplace_back(shapeIDCounter, newFigure);
        std::cout << "< [" << shapeIDCounter << "] " << shapeName << " " << color.getName()
                  << " " << x << " " << y << " " << parameter1;

        if (shapeName == "rectangle" || shapeName == "line") {
            std::cout << " " << parameter2;
        }

        std::cout << " >" << std::endl;

        ++shapeIDCounter;
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
    grid.assign(boardHeight, std::vector<char>(boardWidth, ' '));

    std::string shapeName;
    bool loadSuccessful = true;
    int id;
    std::string shapeType, colorStr, fillModeStr;
    int x, y, param1, param2;

    while (input >> id >> fillModeStr >> colorStr >> shapeType >> x >> y >> param1) {
        FillMode fillMode = (fillModeStr == "fill") ? FillMode::Fill : FillMode::Frame;
        Color color;
        if (colorStr == "red")  {
            color = Color(ColorName::Red);
        }
        else if (colorStr == "green") {
            color = Color(ColorName::Green);
        }
        else if (colorStr == "blue") {
            color = Color(ColorName::Blue);
        }
        else if (colorStr == "yellow") {
            color = Color(ColorName::Yellow);
        }
        else if (colorStr == "cyan")  {
            color = Color(ColorName::Cyan);
        }
        else if (colorStr == "magenta") {
            color = Color(ColorName::Magenta);
        }
        else {
            color = Color(ColorName::Reset);
        }

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
            newFigure = std::make_shared<Triangle>(x, y, param1, color, fillMode);
        }
        else if (shapeName == "Rectangle") {
            newFigure = std::make_shared<Rectangle>(x, y, param1, param2, color, fillMode);
        }
        else if (shapeName == "Circle") {
            newFigure = std::make_shared<Circle>(x, y, param1, color, fillMode);
        }
        else if (shapeName == "Line") {
            newFigure = std::make_shared<Line>(x, y, param1, param2, color, fillMode);
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
            figures.emplace_back(shapeIDCounter++, newFigure);
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
    for (const std::shared_ptr<Figure>& existingFigure : getFigures()) {
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
        for (const auto& figurePair : figures) {
            int id = figurePair.first;
            std::shared_ptr<Figure> figure = figurePair.second;
            if (figure != nullptr) {
                std::cout << "[" << id << "] " << figure->getInfo()
                          << " Color: " << figure->color.getName()
                          << " FillMode: " << (figure->fillMode == FillMode::Fill ? "Fill" : "Frame")
                          << std::endl;
            }
        }
    }
}

void Board::shapes() {
    std::cout << "List of available shapes and their parameters for the 'add' command:" << std::endl;
    std::cout << "> circle [x, y, radius]" << std::endl;
    std::cout << "> rectangle [x, y, width, height]" << std::endl;
    std::cout << "> triangle [x, y, height]" << std::endl;
    std::cout << "> line [x1, y1, x2, y2]" << std::endl;
    std::cout << "You can specify a color for any shape by adding the color name before the shape parameters." << std::endl;
    std::cout << "Available colors: Red, Green, Blue, Yellow, Cyan, Magenta, White, Reset (default)." << std::endl;
    std::cout << "Usage Example: add fill red circle 5 5 3 - This command creates a filled red circle at position (5, 5) with a radius of 3." << std::endl;
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

//Assignment-3
void Board::select(int ID)  {
    if (ID >= 0 && ID < figures.size()) {
        selectedID = ID;
        std::shared_ptr<Figure> selectedFigure = figures[selectedID].second;
        std::cout << "Shape [" << selectedID << "] selected: " << selectedFigure->getInfo() << std::endl;
    } else {
        std::cout << "Shape with ID " << ID << " not found." << std::endl;
        selectedID = -1;
    }
}

void Board::select(int x, int y)  {
    bool found = false;
    for (int i = 0; i < figures.size(); ++i) {
        auto& figure = figures[i].second;
        if (figure != nullptr && figure->x == x && figure->y == y) {
            selectedID = i;
            std::cout << "Shape [" << selectedID << "] at (" << x << ", " << y << ") selected: " << figure->getInfo() << std::endl;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "No shape found at (" << x << ", " << y << ")." << std::endl;
        selectedID = -1;
    }
}


void Board::remove() {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    figures.erase(figures.begin() + selectedID);
    std::cout << "Shape [" << selectedID << "] removed." << std::endl;
    selectedID = -1;
}


void Board::edit(int x, int y, int param1, int param2) {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    auto& figure = figures[selectedID].second;
    figure->x = x;
    figure->y = y;
    std::cout << "Shape [" << selectedID << "] edited." << std::endl;
}



void Board::paint(const std::string& colorStr) {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    ColorName colorName = Color::fromString(colorStr);
    if (colorName == ColorName::Invalid) {
        std::cout << "Invalid color specified." << std::endl;
        return;
    }
    Color newColor(colorName);

    figures[selectedID].second->color = newColor;
    std::cout << "Shape [" << selectedID << "] painted " << newColor.getName() << "." << std::endl;
}

void Board::move(int newX, int newY) {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    auto& figure = figures[selectedID].second;
    figure->x = newX;
    figure->y = newY;
    std::cout << "Shape [" << selectedID << "] moved to (" << newX << ", " << newY << ")." << std::endl;
}