#ifndef _GRAPH_UTILS_H_
#define _GRAPH_UTILS_H_

#include <iostream>
#include <vector>
#include <map>
#include "utils.h"

namespace GraphUtils {

class Node;

typedef uint32_t NodeIdx_t; // NodeIdx_t is used for sizes_of and for indices_into data structures
typedef std::vector<Node*> NodeList_t; //
typedef std::map<std::string, NodeIdx_t> NodeMap_t; //
typedef std::vector<NodeIdx_t> LinkingNodeList_t; //


/**
 * Node
 *
 * Keeper of adjacency-information, node-rank and identity
 */
class Node {
    friend class Graph;

public:
    Node(NodeIdx_t idx) : m_idx(idx) {}
    void AddLinkingNodeIdx(NodeIdx_t idx);
    void AddOutLinkNodeIdx(NodeIdx_t idx);

    NodeIdx_t GetNumLinkedBy() { return m_linkedBy.size(); }
    NodeIdx_t GetNumLinkedTo() { return m_linkedTo.size(); }
    NodeIdx_t GetIdx() const { return m_idx; }

    void SetRank(float f) { m_rank = f; }
    float GetRank() { return m_rank; }

protected:
    float m_rank;
    NodeIdx_t m_idx;
    LinkingNodeList_t m_linkedBy;
    LinkingNodeList_t m_linkedTo;
};


/**
 * Graph
 * 
 * Keeper of node information, top-level preprocessed data
 * Also a gateway to visitor-functions on node-list (Rank-retrieval, calculation
 * and setting) 
 *
 */
class Graph {
public:
    
/**
 * AddLink
 *
 * @param	s1	nodeName1 from a line in the input file
 * @param	s2	nodeName2 from a line in the input file
 * @warning	This will internally make calls to make name-to-index hash. 
 *          Any filtering of the file etc happens inside AddLink()
 */
    void AddLink(std::string& s1, std::string& s2);

    
/**
 * CalcNodeRank1
 * 
 * @param	NodeIdx_t idx 
 * @return  float rank of the node
 *
 */
    float CalcNodeRank1(NodeIdx_t );
    
    /*=************************************************
     * Retrieve node's rank. Do not calculate.
     * **************************************************=*/
    float GetRank(NodeIdx_t idx) {
        ASSERT(idx < m_nodeList.size());
        return m_nodeList[idx]->GetRank();
    }

    /*=************************************************
     * Set a node's rank
     * **************************************************=*/
    void SetRank(NodeIdx_t idx, float f) {
        if (idx >= m_nodeList.size()) return;
        m_nodeList[idx]->SetRank(f);
    }

public:
    // total number of nodes in the graph
    NodeIdx_t GetNumNodes() { return m_nodeList.size(); }

    /*=************************************************
     * NumLeaks : Gives the num of rank-leaks (and hanging-nodes)
     * **************************************************=*/
    NodeIdx_t GetNumLeaks() {
        if (!m_pre_processed) {
            PreProcess();
        }
        return m_numLeaks; 
    }

    // not implemented
    std::string& GetNodeName(NodeIdx_t idx1);

    /*=************************************************
     * Assign default values to node-ranks.
     * PreProcess graph to count hanging/0-out-degree nodes
     * **************************************************=*/
    void PreProcess();

protected:
    
/**
 *
 * MkNode
 * If needed, creates a Node for a given name. Assigns a contiguous index. Inserts into node-lists and maps.
 *
 * @param	nodeName node to be (potentially) inserted
 * @return	node index for the nodeName
 * @warning	assigns consecutive node indices to each new name encountered
 *
 */
    NodeIdx_t MkNode(std::string& nodeName);

protected:
    // state info
    bool m_pre_processed;

protected:
    NodeIdx_t m_numLeaks; // 
    NodeList_t m_nodeList; // vector of Node* 
    NodeMap_t m_nodeMap; // map : nodeName -> nodeIdx
};

}

#endif

