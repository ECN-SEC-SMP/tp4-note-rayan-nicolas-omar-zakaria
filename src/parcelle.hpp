#pragma once

#include <iostream>
#include <string>

#include "polygon.hpp"

template <typename T>
class Parcelle
{

public:
    Parcelle(int number, std::string owner, Polygon<T> shape) : number_(number),
                                                                owner_(owner),
                                                                shape_(shape)
    {
    }

    Parcelle(const Parcelle<T> &other) : type_(other.type_),
                                         owner_(other.owner_),
                                         number_(other.number_),
                                         constructableAreaPercentage_(other.constructableAreaPercentage_),
                                         shape_(other.shape_) {}

    int getNumber() { return number_; }
    void setNumber(int number) { number_ = number; }

    std::string geOwner() { return owner_; }
    void setOwner(const std::string &owner) { owner_ = owner; }

    Polygon<T> getShape() { return shape_; }
    void setShape(Polygon<T> shape) { shape_ = shape; }

    std::string getType() { return type_; }
    void setType(const std::string &type) { type_ = type; }

    float getArea() { return shape_._area; }

    friend std::ostream &operator<<(std::ostream &os, const Parcelle &p)
    {
        os << "Parcelle nº" << p.number_ << std::endl
           << " Type : " << p.type_ << std::endl
           << " Polygone : " << p.shape_ << std::endl
           << " Propriétaire : " << p.owner_ << std::endl
           << " Surface : " << p.getArea() << std::endl;

        return os;
    }

protected:
    int number_,
        constructableAreaPercentage_;
    std::string type_, owner_;
    Polygon<T> shape_;
};