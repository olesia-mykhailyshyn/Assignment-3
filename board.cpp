#include "board.h"
#include "fstream"
#include <iostream>
#include <iomanip>
#include "enums.h"
#include "algorithm"

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
        for (const std::string& c : grid[row]) {
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
    result.reserve(figures.size());
for (const auto& figurePair : figures) {
        result.push_back(figurePair.second);
    }
    return result;
}

void Board::add(ShapeType shapeType, ColorName colorName, int x, int y, int param1, int param2, FillMode fillMode) {
    if (colorName == ColorName::Invalid) {
        std::cout << "Invalid color." << std::endl;
        return;
    }
    Color color(colorName);
    std::shared_ptr<Figure> newFigure = nullptr;

    switch (shapeType) {
        case ShapeType::Triangle:
            newFigure = std::make_shared<Triangle>(x, y, param1, color, fillMode);
            break;
        case ShapeType::Rectangle:
            newFigure = std::make_shared<Rectangle>(x, y, param1, param2, color, fillMode);
            break;
        case ShapeType::Circle:
            newFigure = std::make_shared<Circle>(x, y, param1, color, fillMode);
            break;
        case ShapeType::Line:
            newFigure = std::make_shared<Line>(x, y, param1, param2, color, fillMode);
            break;
        default:
            std::cout << "Invalid shape type." << std::endl;
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
        std::cout << "[" << shapeIDCounter << "] " << newFigure->getShapeType() << " " << color.getName()
                  << " " << x << " " << y << " " << param1;

        if (shapeType == ShapeType::Rectangle || shapeType == ShapeType::Line) {
            std::cout << " " << param2;
        }
        std::cout << std::endl;
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

    std::vector<std::pair<int, std::shared_ptr<Figure>>> tempFigures;
    std::vector<std::vector<std::string>> tempGrid = grid;
    grid.assign(boardHeight, std::vector<std::string>(boardWidth, " "));

    int id, x, y, param1, param2;
    std::string fillModeStr, colorStr, shapeTypeStr;
    bool loadSuccessful = true;

    while (input >> id >> fillModeStr >> colorStr >> shapeTypeStr >> x >> y >> param1) {
        FillMode fillMode = (fillModeStr == "fill") ? FillMode::Fill : FillMode::Frame;

        ColorName colorName = Color::fromString(colorStr);
        if (colorName == ColorName::Invalid) {
            std::cout << "Invalid color specified: " << colorStr << std::endl;
            loadSuccessful = false;
            break;
        }
        Color color(colorName);

        auto shapeTypeIt = shapeTypeMap.find(shapeTypeStr);
        if (shapeTypeIt == shapeTypeMap.end()) {
            std::cout << "Error: Invalid shape type " << shapeTypeStr << " found in file. Aborting load." << std::endl;
            loadSuccessful = false;
            break;
        }
        ShapeType shapeType = shapeTypeIt->second;

        std::shared_ptr<Figure> newFigure = nullptr;

        switch (shapeType) {
            case ShapeType::Triangle:
                newFigure = std::make_shared<Triangle>(x, y, param1, color, fillMode);
                break;
            case ShapeType::Rectangle:
                if (!(input >> param2)) {
                    std::cout << "Missing parameters for shape " << shapeTypeStr << std::endl;
                    loadSuccessful = false;
                    break;
                }
                newFigure = std::make_shared<Rectangle>(x, y, param1, param2, color, fillMode);
                break;
            case ShapeType::Circle:
                if (param1 <= 0) {
                    std::cout << "Invalid radius for circle." << std::endl;
                    loadSuccessful = false;
                    break;
                }
                newFigure = std::make_shared<Circle>(x, y, param1, color, fillMode);
                break;
            case ShapeType::Line:
                if (!(input >> param2)) {
                    std::cout << "Missing parameters for shape " << shapeTypeStr << std::endl;
                    loadSuccessful = false;
                    break;
                }
                newFigure = std::make_shared<Line>(x, y, param1, param2, color, fillMode);
                break;
            default:
                std::cout << "Invalid shape type in file." << std::endl;
                loadSuccessful = false;
                break;
        }

        if (!loadSuccessful) {
            break;
        }

        if (newFigure->isOutOfBounds(boardWidth, boardHeight)) {
            std::cout << "Error: Figure is out of bounds." << std::endl;
            loadSuccessful = false;
            break;
        }

        if (!isDuplicate(newFigure)) {
            tempFigures.emplace_back(id, newFigure);
        }
        else {
            std::cout << "Error: Duplicate figure found." << std::endl;
            loadSuccessful = false;
            break;
        }

        shapeIDCounter = std::max(shapeIDCounter, id + 1);
    }

    input.close();

    if (loadSuccessful) {
        figures.swap(tempFigures);
        std::cout << "Figures loaded successfully from " << filePath << std::endl;
    }
    else {
        std::cout << "Failed to load file. Board was not modified." << std::endl;
    }
}


bool Board::isDuplicate(const std::shared_ptr<Figure>& figure) const {
    for (const auto& existingFigure : figures) {
        if (figure->getShapeType() == existingFigure.second->getShapeType() &&
            figure->x == existingFigure.second->x &&
            figure->y == existingFigure.second->y &&
            figure->getParam1() == existingFigure.second->getParam1() &&
            figure->getParam2() == existingFigure.second->getParam2() &&
            figure->color.getName() == existingFigure.second->color.getName() &&
            figure->fillMode == existingFigure.second->fillMode) {
            return true;
        }
    }
    return false;
}

void Board::draw() {
    grid.assign(boardHeight, std::vector<std::string>(boardWidth, " "));

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
    std::cout << "Available colors: Red, Green, Blue, Yellow, Cyan, Magenta, White, Reset (default)." << std::endl;
    std::cout << "Usage Example: add fill red circle 5 5 3 - This command creates a filled red circle at position (5, 5) with a radius of 3." << std::endl;
}

//void Board::undo() {
//    if (figures.empty()) {
//        std::cout << "There are no figures. Undo command cannot be performed." << std::endl;
//    }
//    else {
//        figures.pop_back();
//        std::cout << "The last added figure was deleted." << std::endl;
//    }
//}

void Board::save(const std::string& filePath) const {
    std::ofstream myFile(filePath, std::ios::out);
    if (myFile.is_open()) {
        if (figures.empty()) {
            std::cout << "There are no figures. An empty file will be saved." << std::endl;
        } else {
            for (const auto& figurePair : figures) {
                const auto& figure = figurePair.second;
                std::string colorName = figure->color.getName();
                std::transform(colorName.begin(), colorName.end(), colorName.begin(), ::tolower);
                myFile << figurePair.first << " "
                       << (figure->fillMode == FillMode::Fill ? "fill" : "frame") << " "
                       << colorName << " "
                       << figure->getShapeType() << " "
                       << figure->x << " " << figure->y << " "
                       << figure->getParam1();

                if (figure->getShapeType() == "rectangle" || figure->getShapeType() == "line") {
                    myFile << " " << figure->getParam2();
                }

                myFile << std::endl;
            }
            std::cout << "Figures saved to " << filePath << std::endl;
        }
        myFile.close();
    } else {
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

    for (int i = selectedID; i < figures.size(); ++i) {
        figures[i].first = i;
    }

    selectedID = -1;
}

void Board::edit(int x, int y, int parameter1, int parameter2, const std::string& colorStr, const std::string& fillModeStr) {
    if (selectedID == -1) {
        std::cout << "No shape is currently selected. Please select a shape first." << std::endl;
        return;
    }

    std::shared_ptr<Figure>& figure = figures[selectedID].second;

    figure->x = x;
    figure->y = y;

    if (auto rect = dynamic_cast<Rectangle*>(figure.get())) {
        rect->width = parameter1;
        rect->height = parameter2;
    }
    else if (auto circ = dynamic_cast<Circle*>(figure.get())) {
        circ->radius = parameter1;
    }
    else if (auto tri = dynamic_cast<Triangle*>(figure.get())) {
        tri->height = parameter1;
    }
    else if (auto line = dynamic_cast<Line*>(figure.get())) {
        line->x2 = parameter1;
        line->y2 = parameter2;
    }
    else {
        std::cout << "Unknown figure type." << std::endl;
        return;
    }

    ColorName colorName = Color::fromString(colorStr);
    if (colorName != ColorName::Invalid) {
        figure->color = Color(colorName);
    }

    figure->fillMode = (fillModeStr == "fill") ? FillMode::Fill : FillMode::Frame;

    std::cout << "Shape [" << selectedID << "] edited: New properties set." << std::endl;
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