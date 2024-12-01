#include <iostream>

#include "parcelle.hpp"
#include "constructible.hpp"
#include "carte.hpp"

int main()
{
    Carte<int> c = Carte<int>::FromFile("parcelles.txt");

    c.ToFile("out.txt");
}