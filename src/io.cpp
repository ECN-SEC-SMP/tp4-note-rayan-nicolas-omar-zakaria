#include "io.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <regex>

#include "constructible.hpp"

// int main()
// {
//     std::ifstream inputFile("parcelles.txt"); // Replace with your file name
//     if (!inputFile)
//     {
//         std::cerr << "Error: Unable to open file." << std::endl;
//         return 1;
//     }

//     // Print parsed data for verification
//     for (const auto &parcelle : parcelles)
//     {
//         std::cout << "Type: " << parcelle.typeParcelle
//                   << ", Numero: " << parcelle.numero
//                   << ", Proprietaire: " << parcelle.proprietaire;

//         if (parcelle.typeParcelle == "ZU" || parcelle.typeParcelle == "ZAU" || parcelle.typeParcelle == "ZA")
//         {
//             std::cout << ", SpecificField: " << parcelle.specificField;
//         }
//         if (parcelle.typeParcelle == "ZU")
//         {
//             std::cout << ", SurfaceConstruite: " << parcelle.surfaceConstruite;
//         }

//         std::cout << "\nPoints: ";
//         for (const auto &point : parcelle.points)
//         {
//             std::cout << "[" << point.first << ";" << point.second << "] ";
//         }
//         std::cout << "\n\n";
//     }

//     return 0;
// }

template <typename T>
void IO::parseFile(const std::string &filename)
{
    std::fstream file;
    file.open(filename);

    if (!file.is_open())
        throw std::invalid_argument("Error opening file");

    std::vector<std::unique_ptr<Parcelle<T>>> parcelles;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);

        // Parse the first line
        std::string type, num, owner;
        lineStream >> type >> num >> owner;

        // Parse the second line (list of points)
        std::getline(file, line);
        Polygon shape = Polygon(parsePoints<T>(line));

        Parcelle p(num, owner, shape);

        //         — Une Parcelle est définie par deux lignes :
        // — La première contient au moins : typeParcelle numéro propriétaire
        // — La deuxième contient la liste des points définissant la forme de la Parcelle
        // — Pour une ZU : typeParcelle numéro propriétaire pConstructible surfaceConstruite
        // — Pour une ZAU : typeParcelle numéro propriétaire pConstructible
        // — Pour une ZA : typeParcelle numéro propriétaire typeCulture
        // — Pour une ZN : typeParcelle numéro propriétaire

        std::unique_ptr<Parcelle<T>> zone;
        switch (type)
        {
        case "ZU":
            std::string pConstructible, surfaceConstruite;
            lineStream >> pConstructible >> surfaceConstruite;
            p.setConstructableAreaPercentage(std::stoi(pConstructible));
            zone = std::make_unique<ZU<T>>(p, std::stof(surfaceConstruite));
            break;

        case "ZAU":
            // std::string pConstructible;
            lineStream >> pConstructible;
            p.setConstructableAreaPercentage(std::stoi(pConstructible));
            zone = std::make_unique<ZAU<T>>(p);
            break;

        case "ZA":
            std::string typeCulture;
            lineStream >> typeCulture;
            zone = std::make_unique<ZA<T>>(p, typeCulture);
            break;

        case "ZN":
            zone = std::make_unique<ZN<T>>(p);
            break;
        }

        parcelles.push_back(zone);
    }

    file.close();
}

template <typename T>
std::vector<Point2D<T>> parsePoints(const std::string &line)
{
    std::vector<Point2D<T>> points;
    std::regex pointRegex(R"(\[(-?\d+);(-?\d+)\])");
    std::string::const_iterator searchStart(line.cbegin());
    std::smatch match;

    while (std::regex_search(searchStart, line.cend(), match, pointRegex))
    {
        int x = std::stoi(match[1]);
        int y = std::stoi(match[2]);
        points.emplace_back(x, y);
        searchStart = match.suffix().first;
    }

    return points;
}