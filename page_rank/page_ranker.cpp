#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <ostream>

#include "graph_utils.h"
#include "parse_utils.h"
#include "utils.h"

#include <math.h>

#define ERR_LIMIT 1e-08

/**
 * Page_Ranker
 *
 * Calculates page_rank
 * [ http://ilpubs.stanford.edu:8090/457/1/2000-37.pdf ]
 * 
 */
class Page_Ranker {
public:
    Page_Ranker(float decay=0.85) : m_decay(decay) {}
    
    /**
     * InitGraph
     * @param	fromFile file with "node1 node2" pairs in each line
     **/
    void InitGraph(const char* fromFile) 
    {
        ASSERT(fromFile);
        m_graph = ParseUtils::ParseGraphFromFile(fromFile);
    }

    GraphUtils::Graph* GetGraph() { return m_graph; }

    /*=************************************************
     * CalcNodeRank calculates the non-decay and with-decay versions of 
     * node-ranks. By default it returns the with-decay version.
     * **************************************************=*/
    float CalcNodeRank(GraphUtils::NodeIdx_t idx, bool classical=false) {
        ASSERT(m_graph);
        ASSERT(m_graph->GetNumNodes() > idx);
        float f = m_graph->CalcNodeRank1(idx);
        float g = m_decay*f + (1-m_decay)*(1.0/float(m_graph->GetNumNodes()));
        DEBUG( "Node " << idx << " classical_wt: " << f << " with_decay " << g);
        return (classical?f:g);
    }

    /*=************************************************
     * Iterate over all nodes and print their rank
     * **************************************************=*/
    void PrintAllRank(std::ostream& os = std::cout) {
        os << "**************" << std::endl;
        for (GraphUtils::NodeIdx_t i=0; i < m_graph->GetNumNodes(); i++) {
            os << "Node " << i << " rank " << m_graph->GetRank(i) << std::endl;
        }
        os << "**************" << std::endl;
    }

    
/**
 * CalcAllRank
 *
 * Calculates ranks for all nodes. 
 * Updates them once the iteration is over.
 *
 * @param	update true // should we update after calculation-iteration is over ?
 * @return	float calculated covergence-metric (l2norm of node-rank differences)
 */
    float CalcAllRank(bool update=true) {
        ASSERT(m_graph);
        std::vector<float> v_ranks;
        float l2norm = 0;
        //
        for (GraphUtils::NodeIdx_t i=0; i < m_graph->GetNumNodes(); i++) {
            float f = CalcNodeRank(i);
            v_ranks.push_back(f);
            float r = m_graph->GetRank(i);
            r -= f;
            l2norm += (r*r); // diff_square
        }
        //
        if (update) {
            for (GraphUtils::NodeIdx_t i=0; i < m_graph->GetNumNodes(); i++) {
                m_graph->SetRank(i, v_ranks[i]);
            }
        }

        return sqrt(l2norm);
    }

protected:
    float m_decay; // decay fator for handling "trasporation" effects (practical page_rank)
    GraphUtils::Graph* m_graph; // underlying graph
};


void Usage(const char* progName)
{
    printf("USAGE: %s <input_file> <num_iterations> <decay_factor>\n", progName);
}

int
main(int argc , char* argv[])
{
    // input_file num_iterations decay_factor
    if (argc < 4) {
        Usage(argv[0]);
        exit(0);
    }

    if (argc > 4) {
        int iargc = 4;
        while ( iargc < argc) {
            if (0 == strcmp("-d", argv[iargc])) {
                iargc++;
                debug = 1;
                if ((iargc < argc) && (argv[iargc][0] != '-')) {
                    debug = atoi(argv[iargc]);
                }
            }
            iargc++;
        }
    }

    // parse inputs
    try {
        /*=************************************************
         * Initialize the graph from file
         * **************************************************=*/
        Page_Ranker pgRanker(atof(argv[3] /* decay factor */));
        pgRanker.InitGraph(argv[1] /* filename */);
        INFO( "Graph NumLeaks: " << pgRanker.GetGraph()->GetNumLeaks() );

        float fconv = 0;
        pgRanker.PrintAllRank();
        int numIter = atoi(argv[2]);
        for (int i=0; i<numIter; i++) {
            /*=************************************************
             * In each iteration: 
             * * calculate the rank of all nodes
             * * test if ranks from previous iteration are close enough to declare convergence
             * **************************************************=*/
            std::cout << "**** Iteration: " << i ;
            fconv = pgRanker.CalcAllRank();
            std::cout << " rank_dist_l2=" << fconv << " ****" << std::endl;
            if (fconv < ERR_LIMIT) {
                std::cout << "Stopping iterations. " << fconv << " converged to less than " << ERR_LIMIT << " ****" << std::endl;
                break;
            }
            if (debug & 0x2) { pgRanker.PrintAllRank(std::cout); }
        }
        pgRanker.PrintAllRank(std::cout);

    } catch(std::exception *e) {
        DEBUG( "Exception from ParseGraphFromFile(): " << e->what() );
    }

    return 0;
}
