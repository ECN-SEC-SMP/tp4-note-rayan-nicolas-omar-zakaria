#pragma once

#include "parcelle.hpp"

class Constructible
{
    virtual float surfaceConstructible() = 0;
};

// Zone urbaine
template <typename T>
class ZU : public Parcelle<T>, public Constructible
{
public:
    ZU(const Parcelle<T> &parcelle, float constructedArea) : Parcelle<T>(parcelle), _constructedArea(constructedArea)
    {
        Parcelle<T>::setType("ZU");
    }

    float surfaceConstructible() override
    {
        return Parcelle<T>::constructableAreaPercentage_ * Parcelle<T>::getArea() - _constructedArea;
    }

    float getConstructedArea() const { return _constructedArea; }

    friend std::ostream &operator<<(std::ostream &os, const ZU &zone)
    {
        os << static_cast<const Parcelle<T> &>(zone);

        os << " Constructuble : " << zone.constructableAreaPercentage_ << '%' << std::endl
           << " Surface Construite : " << zone._constructedArea << "m2" << std::endl
           << " Surface à construire restante : " << zone.surfaceConstructible() << "m2" << std::endl;

        return os;
    }

private:
    float _constructedArea;
};

// Zone à urbaniser
template <typename T>
class ZAU : public Parcelle<T>, public Constructible
{
public:
    ZAU(const Parcelle<T> &parcelle) : Parcelle<T>(parcelle)
    {
        Parcelle<T>::setType("ZAU");
    }

    float surfaceConstructible() override
    {
        return Parcelle<T>::constructableAreaPercentage_ * Parcelle<T>::getArea();
    }

    friend std::ostream &operator<<(std::ostream &os, const ZAU &zone)
    {
        os << static_cast<const Parcelle<T> &>(zone);

        os << " Constructuble : " << zone.constructableAreaPercentage_ << '%' << std::endl
           << " Surface à construire restante : " << zone.surfaceConstructible() << "m2" << std::endl;

        return os;
    }
};

// Zone naturelle
template <typename T>
class ZN : public Parcelle<T>
{
public:
    ZN(const Parcelle<T> &parcelle) : ZN(parcelle, "") {}
    ZN(const Parcelle<T> &parcelle, const std::string &cultureCultivee) : Parcelle<T>(parcelle), _cultureCultivee(cultureCultivee)
    {
        Parcelle<T>::setType("ZN");
    }

    std::string getCultureCultivee() const { return _cultureCultivee; }
    void setCultureCultivee(const std::string &n) { _cultureCultivee = n; }

    friend std::ostream &operator<<(std::ostream &os, const ZN &zone)
    {
        os << static_cast<const Parcelle<T> &>(zone);

        os << " Type culture : " << zone._cultureCultivee << std::endl;

        return os;
    }

private:
    std::string _cultureCultivee;
};

// Zone agricole
template <typename T>
class ZA : public ZN<T>
{
public:
    ZA(const Parcelle<T> &parcelle) : ZA(parcelle, "") {}
    ZA(const Parcelle<T> &parcelle, const std::string &cultureCultivee) : ZN<T>(parcelle, cultureCultivee)
    {
        Parcelle<T>::setType("ZA");
    }

    bool construct(float area)
    {
        const float total = area + _constructedArea;

        if (total > 200)
            throw std::exception("Cant construct more than 200m2 in total");

        if (total > 0.1 * Parcelle<T>::getArea())
            throw std::exception("Cant construct more than 10\% of the total area");

        _constructedArea = total;

        return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const ZA &zone)
    {
        os << static_cast<const ZA<T> &>(zone);

        return os;
    }

private:
    float _constructedArea = 0;
};
