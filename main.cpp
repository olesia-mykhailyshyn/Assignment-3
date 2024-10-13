#include "board.h"
#include <iostream>
#include <sstream>
#include "enums.h"

int main() {
    Board board;
    std::string input;

    while (true) {
        std::cout << "\nEnter command (draw/list/shapes/add/select/remove/edit/paint/move/clear/save/load/exit): " << std::endl;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        auto cmd = commandMap.find(command);
        if (cmd != commandMap.end()) {
            CommandType commandType = cmd->second;
            switch (commandType) {
                case CommandType::Draw: {
                    board.draw();
                    break;
                }
                case CommandType::List: {
                    board.list();
                    break;
                }
                case CommandType::Shapes: {
                    Board::shapes();
                    break;
                }
                case CommandType::Add: {
                    std::string fillModeStr, color, shapeName;
                    int x, y, param1, param2 = 0;
                    iss >> fillModeStr >> color >> shapeName >> x >> y >> param1;

                    auto shapeTypeIt = shapeTypeMap.find(shapeName);
                    if (shapeTypeIt == shapeTypeMap.end()) {
                        std::cout << "Invalid shape type." << std::endl;
                        continue;
                    }
                    ShapeType shapeType = shapeTypeIt->second;
                    if (shapeType == ShapeType::Rectangle || shapeType == ShapeType::Line) {
                        if (!(iss >> param2)) {
                            std::cout << "Invalid parameters for " << shapeName << ". Needs an additional parameter." << std::endl;
                            continue;
                        }
                    }
                    FillMode fillMode = (fillModeStr == "fill") ? FillMode::Fill : FillMode::Frame;
                    board.add(shapeName, color, x, y, param1, param2, fillMode);
                    break;
                }
                case CommandType::Clear: {
                    board.clear(board.getFilePath());
                    break;
                }
                case CommandType::Save: {
                    board.save(board.getFilePath());
                    break;
                }
                case CommandType::Load: {
                    board.load(board.getFilePath());
                    break;
                }
                case CommandType::Select: {
                    int firstParam;
                    if (iss >> firstParam) {
                        if (iss.peek() == ',' || iss.peek() == ' ') {
                            int x = firstParam, y;
                            iss >> y;
                            board.select(x, y);
                        }
                        else {
                            board.select(firstParam);
                        }
                    }
                    else {
                        std::cout << "Invalid select command. Please provide either an ID or coordinates." << std::endl;
                    }
                    break;
                }
                case CommandType::Remove: {
                    board.remove();
                    break;
                }
                case CommandType::Edit: {
                    int x, y, param1, param2 = 0;
                    std::string color, fillModeStr;
                    if (!(iss >> x >> y >> param1 >> param2 >> color >> fillModeStr)) {
                        std::cout << "Invalid parameters for edit command. Expected format: edit x y param1 param2 color fillMode" << std::endl;
                        continue;
                    }
                    board.edit(x, y, param1, param2, color, fillModeStr);
                    break;
                }
                case CommandType::Paint: {
                    std::string color;
                    iss >> color;
                    board.paint(color);
                    break;
                }
                case CommandType::Move: {
                    int x, y;
                    iss >> x >> y;
                    board.move(x, y);
                    break;
                }
                case CommandType::Exit: {
                    std::cout << "Exiting the program." << std::endl;
                    return 0;
                }
                case CommandType::Invalid:
                    break;
            }
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }
    //return 0;
}