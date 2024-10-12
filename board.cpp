#include "board.h"
#include "fstream"
#include <iostream>
#include <iomanip>
#include "algorithm"
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
void Board::select(int ID) {
    if (ID < 0 || ID >= figures.size()) {
        std::cout << "Shape was not found" << std::endl;
    }
    else {
        std::shared_ptr<Figure> selectedFigure = figures[ID].second;
        if (selectedFigure != nullptr) {
            selectedID = ID;
            std::cout << "< " << selectedFigure->getInfo() << " >" << std::endl;
        }
        else {
            std::cout << "Shape was not found" << std::endl;
        }
    }
}

void Board::select(const std::vector<int>& coordinates) const {
    if (coordinates.size() != 2) {
        std::cout << "Invalid coordinates. Please provide exactly two coordinates." << std::endl;
        return;
    }

    int x = coordinates[0];
    int y = coordinates[1];
    bool found = false;

    for (const auto& figurePair : figures) {
        std::shared_ptr<Figure> figure = figurePair.second;
        if (figure != nullptr) {
            if (!figure->isOutOfBounds(boardWidth, boardHeight) && figure->x == x && figure->y == y) {
                std::cout << "< " << figure->getInfo() << " >" << std::endl;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        std::cout << "< shape was not found >" << std::endl;
    }
}

void Board::remove() {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    auto it = std::find_if(figures.begin(), figures.end(), [this](const std::pair<int, std::shared_ptr<Figure>>& pair) {
        return pair.first == selectedID;
    });

    if (it != figures.end()) {
        std::cout << "< [" << it->first << "] " << it->second->getInfo() << " removed >" << std::endl;
        figures.erase(it);
        selectedID = -1;
    }
    else {
        std::cout << "Error: Selected shape not found in the list." << std::endl;
    }
}

void Board::edit(int ID, const std::string& shapeName, int x, int y, int parameter1, int parameter2) {
    if (ID < 0 || ID >= figures.size()) {
        std::cout << "< error: invalid shape ID >" << std::endl;
        return;
    }

    auto it = std::find_if(figures.begin(), figures.end(), [this, ID](const std::pair<int, std::shared_ptr<Figure>>& pair) {
        return pair.first == ID;
    });

    if (it == figures.end() || !it->second) {
        std::cout << "< error: no shape found with ID " << ID << " >" << std::endl;
        return;
    }

    std::shared_ptr<Figure> &figure = it->second;

    if (shapeName == "triangle") {
        auto triangle = dynamic_cast<Triangle*>(figure.get());
        if (triangle && parameter1 > 0) {
            triangle->height = parameter1;
            triangle->x = x;
            triangle->y = y;
        }
        else {
            std::cout << "< error: invalid arguments for triangle >" << std::endl;
            return;
        }
    }
    else if (shapeName == "rectangle") {
        auto rectangle = dynamic_cast<Rectangle*>(figure.get());
        if (rectangle && parameter1 > 0 && parameter2 > 0) {
            rectangle->width = parameter1;
            rectangle->height = parameter2;
            rectangle->x = x;
            rectangle->y = y;
        }
        else {
            std::cout << "< error: invalid arguments for rectangle >" << std::endl;
            return;
        }
    }
    else if (shapeName == "circle") {
        auto circle = dynamic_cast<Circle*>(figure.get());
        if (circle && parameter1 > 0) {
            circle->radius = parameter1;
            circle->x = x;
            circle->y = y;
        }
        else {
            std::cout << "< error: invalid arguments for circle >" << std::endl;
            return;
        }
    }
    else if (shapeName == "line") {
        auto line = dynamic_cast<Line*>(figure.get());
        if (line) {
            line->x = x;
            line->y = y;
            line->x2 = parameter1;
            line->y2 = parameter2;
        }
        else {
            std::cout << "< error: invalid arguments for line >" << std::endl;
            return;
        }
    }
    else {
        std::cout << "< error: shape type does not match any editable type >" << std::endl;
        return;
    }

    if (figure->isOutOfBounds(boardWidth, boardHeight)) {
        std::cout << "< error: shape will go out of the board >" << std::endl;
        return;
    }

    std::cout << "< [" << ID << "] " << shapeName << " modified successfully >" << std::endl;
}


void Board::paint(const std::string& colorStr) {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    Color newColor;
    if (colorStr == "red")        newColor = Color(ColorName::Red);
    else if (colorStr == "green") newColor = Color(ColorName::Green);
    else if (colorStr == "blue")  newColor = Color(ColorName::Blue);
    else if (colorStr == "yellow")newColor = Color(ColorName::Yellow);
    else if (colorStr == "cyan")  newColor = Color(ColorName::Cyan);
    else if (colorStr == "magenta")newColor = Color(ColorName::Magenta);
    else                          newColor = Color(ColorName::Reset);

    auto it = std::find_if(figures.begin(), figures.end(), [this](const std::pair<int, std::shared_ptr<Figure>>& pair) {
        return pair.first == selectedID;
    });

    if (it != figures.end()) {
        it->second->color = newColor;
        std::cout << "< [" << it->first << "] " << it->second->getInfo() << " " << newColor.getName() << " >" << std::endl;
    }
    else {
        std::cout << "Error: Selected shape not found in the list." << std::endl;
    }
}

void Board::move(int newX, int newY) {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    auto it = std::find_if(figures.begin(), figures.end(), [this](const std::pair<int, std::shared_ptr<Figure>>& pair) {
        return pair.first == selectedID;
    });

    if (it != figures.end()) {
        if (Figure::isPositionOutOfBounds(newX, newY, boardWidth, boardHeight)) {
            std::cout << "< error: shape cannot be moved outside the board >" << std::endl;
            return;
        }

        it->second->x = newX;
        it->second->y = newY;

        std::cout << "< [" << it->first << "] " << it->second->getInfo() << " moved >" << std::endl;
    }
    else {
        std::cout << "Error: Selected shape not found in the list." << std::endl;
    }
}