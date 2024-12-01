#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <regex>

#include "constructible.hpp"

template <typename T>

class Carte
{
public:
    Carte(std::vector<std::shared_ptr<Parcelle<T>>> parcelles) : _parcelles(parcelles) {}

    static Carte FromFile(const std::string &filename)
    {
        std::fstream file;
        file.open(filename);

        if (!file.is_open())
            throw std::invalid_argument("Error opening file");

        std::vector<std::shared_ptr<Parcelle<T>>> parcelles;

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream lineStream(line);

            // Parse the first line
            std::string type, num, owner;
            lineStream >> type >> num >> owner;

            // Parse the second line (list of points)
            std::getline(file, line);
            Polygon shape = Polygon(Carte::ParsePoint(line));

            Parcelle p(std::stoi(num), owner, shape);

            //         — Une Parcelle est définie par deux lignes :
            // — La première contient au moins : typeParcelle numéro propriétaire
            // — La deuxième contient la liste des points définissant la forme de la Parcelle
            // — Pour une ZU : typeParcelle numéro propriétaire pConstructible surfaceConstruite
            // — Pour une ZAU : typeParcelle numéro propriétaire pConstructible
            // — Pour une ZA : typeParcelle numéro propriétaire typeCulture
            // — Pour une ZN : typeParcelle numéro propriétaire

            std::shared_ptr<Parcelle<T>> zone;
            if (type == "ZU")
            {
                std::string pConstructible, surfaceConstruite;
                lineStream >> pConstructible >> surfaceConstruite;
                p.setConstructableAreaPercentage(std::stoi(pConstructible));
                zone = std::make_shared<ZU<T>>(p, std::stoi(surfaceConstruite));
            }
            else if (type == "ZAU")
            {
                std::string pConstructible;
                lineStream >> pConstructible;
                p.setConstructableAreaPercentage(std::stoi(pConstructible));
                zone = std::make_shared<ZAU<T>>(p);
            }
            else if (type == "ZA")
            {
                std::string typeCulture;
                lineStream >> typeCulture;
                zone = std::make_shared<ZA<T>>(p, typeCulture);
            }
            else if (type == "ZN")
            {
                zone = std::make_shared<ZN<T>>(p);
            }

            parcelles.push_back(zone);
        }

        file.close();

        return Carte(parcelles);
    }

    // template <typename T>
    static std::vector<Point2D<T>> ParsePoint(const std::string &line)
    {
        std::vector<Point2D<T>> points;
        std::regex pointRegex(R"(\[(-?\d+);(-?\d+)\])");
        std::string::const_iterator searchStart(line.cbegin());
        std::smatch match;

        while (std::regex_search(searchStart, line.cend(), match, pointRegex))
        {
            int x = std::stoi(match[1]);
            int y = std::stoi(match[2]);
            // std::cout << match[1] << ' ' << x << ' ' << match[2] << ' ' << y << std::endl;

            points.emplace_back(x, y);
            searchStart = match.suffix().first;
        }

        return points;
    }

    void ToFile(const std::string &filename) const
    {
        std::ofstream file;
        file.open(filename);

        if (!file.is_open())
            throw std::invalid_argument("Error opening file");

        //         — Une Parcelle est définie par deux lignes :
        // — La première contient au moins : typeParcelle numéro propriétaire
        // — La deuxième contient la liste des points définissant la forme de la Parcelle
        // — Pour une ZU : typeParcelle numéro propriétaire pConstructible surfaceConstruite
        // — Pour une ZAU : typeParcelle numéro propriétaire pConstructible
        // — Pour une ZA : typeParcelle numéro propriétaire typeCulture
        // — Pour une ZN : typeParcelle numéro propriétaire

        for (size_t i = 0; i < _parcelles.size(); ++i)
        {
            auto parcelle = _parcelles[i];
            file << parcelle->getType() << ' ' << parcelle->getNumber() << ' ' << parcelle->getOwner() << ' ';

            auto type = parcelle->getType();

            if (type == "ZU")
            {
                auto zone = std::static_pointer_cast<ZU<T>>(parcelle);
                file << parcelle->getConstructableAreaPercentage() << ' ' << zone->getConstructedArea();
            }
            else if (type == "ZAU")
            {
                file << parcelle->getConstructableAreaPercentage();
            }
            else if (type == "ZA")
            {
                auto zone = std::static_pointer_cast<ZA<T>>(parcelle);
                file << zone->getCultureCultivee();
            } // ZN got no extra fields

            file << std::endl;

            // print coordinates
            const auto summits = parcelle->getShape().getSummits();
            for (size_t i = 0; i < summits.size(); ++i)
                file << '[' << summits[i].getX() << ';' << summits[i].getY() << "] ";

            file << std::endl;
        }

        file.close();
    }

private:
    std::vector<std::shared_ptr<Parcelle<T>>> _parcelles;
};