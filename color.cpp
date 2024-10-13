#include "color.h"
#include "algorithm"

const std::unordered_map<ColorName, std::string> ansiCodeMap = {
        {ColorName::Red, "\033[31m"},
        {ColorName::Green, "\033[32m"},
        {ColorName::Blue, "\033[34m"},
        {ColorName::Yellow, "\033[33m"},
        {ColorName::Cyan, "\033[36m"},
        {ColorName::Magenta, "\033[35m"},
        {ColorName::White, "\033[37m"},
        {ColorName::Reset, "\033[0m"}
};

Color::Color(ColorName name) : name(name) {}

std::string Color::getName() const {
    switch (name) {
        case ColorName::Red: return "Red";
        case ColorName::Green: return "Green";
        case ColorName::Blue: return "Blue";
        case ColorName::Yellow: return "Yellow";
        case ColorName::Cyan: return "Cyan";
        case ColorName::Magenta: return "Magenta";
        case ColorName::White: return "White";
        case ColorName::Reset: return "Reset";
        default: return "Unknown";
    }
}

ColorName Color::fromString(const std::string& colorStr) {
    static const std::unordered_map<std::string, ColorName> colors = {
            {"red", ColorName::Red},
            {"green", ColorName::Green},
            {"blue", ColorName::Blue},
            {"yellow", ColorName::Yellow},
            {"cyan", ColorName::Cyan},
            {"magenta", ColorName::Magenta},
            {"white", ColorName::White},
            {"reset", ColorName::Reset}
    };
    std::string lowerColorStr;
    std::transform(colorStr.begin(), colorStr.end(), std::back_inserter(lowerColorStr),
                   [](unsigned char c){ return std::tolower(c); });
    auto it = colors.find(colorStr);
    return it != colors.end() ? it->second : ColorName::Invalid;
}

std::string ColorFormatter::getAnsiCode(const Color& color) {
    return ansiCodeMap.at(color.name);
}