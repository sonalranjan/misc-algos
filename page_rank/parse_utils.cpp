#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "parse_utils.h"
#include "graph_utils.h"

namespace ParseUtils {

GraphUtils::Graph* ParseGraphFromFile(const char* fileName) 
{
    GraphUtils::Graph* g = new GraphUtils::Graph();

    std::ifstream infile(fileName);

    std::string s1, s2;
    while (infile.good()) {
        infile >> s1 >> s2;
        g->AddLink(s1, s2);
    }

    return g;
}

}
