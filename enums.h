#pragma once
#include <string>
#include <unordered_map>

enum class ShapeType {
    Triangle,
    Rectangle,
    Circle,
    Line,
    Invalid
};

enum class CommandType {
    Add,
    Draw,
    List,
    Shapes,
    Undo,
    Clear,
    Save,
    Load,
    Exit,
    Select,
    Remove,
    Edit,
    Paint,
    Move,
    Invalid
};

extern const std::unordered_map<std::string, ShapeType> shapeTypeMap;
extern const std::unordered_map<std::string, CommandType> commandMap;