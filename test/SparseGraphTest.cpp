#include "utils/sparse_graph.h"
#include <UnitTest++/UnitTest++.h>

using graph::sparse_graph;
using Node = graph::node< float >;
using Edge = graph::edge< void >;
using Graph = sparse_graph< Node, Edge >;

SUITE(SparseGraphTest) {
    
    struct GraphFixture {
        Graph graph;
    };
    
    struct DigraphFixture {
        Graph digraph{ true };
    };
    
    TEST( AfterInitializationGraphContainsZeroNodes ) {
        Graph g{};
        CHECK_EQUAL( 0u, g.number_of_nodes() );
    }
    
    TEST_FIXTURE( GraphFixture, AfterAddingOneNodeGraphSizeIsOne ) {
        graph.add_node( 0.0f );
        CHECK_EQUAL( 1u, graph.number_of_nodes() );
    }
    
    TEST_FIXTURE( GraphFixture, AfterAddingOneNodeNumberOfEdgesIsZero ) {
        graph.add_node( 0.0f );
        CHECK_EQUAL( 0u, graph.number_of_edges() );
    }
    
    TEST_FIXTURE( GraphFixture, AfterAddingEdgeNumberOfEdgesIsOne ) {
        graph.add_node( 0.0f );
        graph.add_node( 0.0f );
        graph.add_edge( Edge{ 0, 1 } );
        CHECK_EQUAL( 1u, graph.number_of_edges() );
    }
    
    TEST_FIXTURE( DigraphFixture, AfterAddingEdgeNumberOfEdgesIsTwo ) {
        digraph.add_node( 0.0f );
        digraph.add_node( 0.0f );
        digraph.add_edge( Edge{ 0,1 } );
        CHECK_EQUAL( 2u, digraph.number_of_edges() );
    }
    
}