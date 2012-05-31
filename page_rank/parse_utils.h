#ifndef _PARSE_UTILS_H_
#define _PARSE_UTILS_H_

namespace GraphUtils {
class Graph;
}

namespace ParseUtils {
/*=************************************************
 * Parses lines from files - and feeds into graph structure
 * **************************************************=*/
GraphUtils::Graph* ParseGraphFromFile(const char* filename);
}

#endif

