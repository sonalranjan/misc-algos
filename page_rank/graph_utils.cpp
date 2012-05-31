#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <algorithm>

#include "graph_utils.h"
#include "utils.h"

namespace GraphUtils {

void
Graph::PreProcess()
{
    float f = 1.0/std::max(size_t(1), m_nodeList.size());
    NodeList_t::iterator it = m_nodeList.begin();
    for (; it != m_nodeList.end(); it++) {
        if ((*it)->GetNumLinkedTo() == 0) { 
            m_numLeaks++;
        }
        (*it)->SetRank(f);
    }
    m_pre_processed = true;
}

float 
Graph::CalcNodeRank1(NodeIdx_t idx) 
{
    ASSERT(idx < m_nodeList.size());

    float f = 0.0;

    // 1. for all predecessors p : sum ( PageRankOf(p) / OutLinksOf(p) )
    LinkingNodeList_t& linkedBy = m_nodeList[idx]->m_linkedBy;
    LinkingNodeList_t::iterator it = linkedBy.begin();
    for (; it != linkedBy.end(); it++) {
        NodeIdx_t jIdx = (*it);
        Node* n = m_nodeList[jIdx];
        f += (n->GetRank()/float(n->GetNumLinkedTo()));
    }
    
    // 2. sum for all rank_leaks k : k/n
    f += (GetNumLeaks()/float(GetNumNodes()));

    return f;
}

NodeIdx_t
Graph::MkNode(std::string& s)
{
    NodeMap_t::iterator it = m_nodeMap.find(s);
    if (it != m_nodeMap.end()) { return (*it).second; }
    
    // we have insert a new Node here
    NodeIdx_t nIdx = m_nodeMap.size();
    m_nodeMap[s] = nIdx;
    m_nodeList.push_back(new Node(nIdx));
    assert(m_nodeList[nIdx]->GetIdx() == nIdx);
    DEBUG( "MkNode: " << s << " -- " << nIdx );

    return nIdx;
}

void 
Graph::AddLink(std::string& s1, std::string& s2) 
{
    NodeIdx_t idx1 = MkNode(s1);
    NodeIdx_t idx2 = MkNode(s2);
    assert(idx2 < m_nodeList.size());
    
    /*=************************************************
     * 1. Skip any self-loops
     * **************************************************=*/
    if (idx2 == idx1) return;

    // idx1 -> links_to -> idx2 
    m_nodeList[idx2]->AddLinkingNodeIdx( idx1);
    m_nodeList[idx1]->AddOutLinkNodeIdx( idx2);
}

void
Node::AddOutLinkNodeIdx(NodeIdx_t idx) 
{
    LinkingNodeList_t::iterator it = find_if(m_linkedTo.begin(), m_linkedTo.end(), std::bind1st( std::equal_to<NodeIdx_t>(), idx));
    if (it == m_linkedTo.end()) {
        m_linkedTo.push_back(idx);
        DEBUG( GetIdx() << " links_to " << idx );
    } else {
        // std::cout << " Already in here " << (*it) << std::endl;
    }
}

void
Node::AddLinkingNodeIdx(NodeIdx_t idx) 
{
    LinkingNodeList_t::iterator it = find_if(m_linkedBy.begin(), m_linkedBy.end(), std::bind1st( std::equal_to<NodeIdx_t>(), idx));
    if (it == m_linkedBy.end()) {
        m_linkedBy.push_back(idx);
        DEBUG( GetIdx() << " linked_by " << idx );
    } else {
        // std::cout << " Already in here " << (*it) << std::endl;
    }
}

}

