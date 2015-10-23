#ifndef SPARSEGRAPH_H_INCLUDED
#define SPARSEGRAPH_H_INCLUDED

#include <vector>
#include <list>
#include <cstdlib>
#include <cassert>

namespace graph {

template< typename T >
struct node {
    // a valid index is >= 0
    int index;
    T   value;
    
    using value_type = T;
};

template< typename T >
struct edge {
    int from;
    int to;
    T cost;
};

template<>
struct edge< void > {
    int from;
    int to;
};

/**
* @class sparse_graph
* @brief A sparse graph container.
* In the following comments, N is going to denote the number of nodes,
* and E the number of edges a node has, on average.
*/
template< typename N, typename E > 
class sparse_graph {
        
    public:
        using node_type = N;
        using edge_type = E;
        
    private:
        using node_vector = std::vector< node_type >;
        using edge_list = std::list< edge_type >;
        using edge_list_vector = std::vector< edge_list >;
        
    public:
        
        sparse_graph( bool is_digraph );
        sparse_graph() = default;
        ~sparse_graph() = default;
        
        /**
        * @brief Get the number of nodes.
        * Has O(1) time complexity.
        */
        std::size_t number_of_nodes() const;
        /**
        * @brief Get the number of edges.
        * Has O(1) time complexity.
        */
        std::size_t number_of_edges() const;
        /**
        * @brief Check whether the graph contains any nodes.
        */
        bool is_empty() const;
        /**
        * @brief Check whether the graph is a bidirectional graph.
        * @returns true, is the graph is a bidirectional graph.
        */
        bool is_digraph() const;
        
        N& at( int index );
        const N& at( int index ) const;
        
        N& operator[]( int index );
        const N& operator[]( int index ) const;
        /**
        * @brief Add a new node with a value.
        * @param value The value to add to the node.
        * @returns The index of the node.
        * This operation has O(1) time complexity.
        */
        int add_node( typename N::value_type value );
        /**
        * @brief Remove a node at index i.
        * This operation has O(N*E) time complexity. 
        */
        void remove_node( int i );
        /**
        * @brief Add an edge uniquely to the graph.
        * @param e The edge to add.
        * If the graph is a digraph, then an edge in the opposite bidirection 
        * is added as well. Note that if the edge already exists, then nothing will * * be added.
        * 
        * Adding an edge has O( E ) time complexity, where E is the number of edges
        * between nodes, on average.
        */
        void add_edge( edge_type e );
        /**
        * @brief Check whether a node has outgoing edges.
        * @returns true if the node has no outgoing edges, false otherwise
        * Has O(E) time complexity.
        */
        bool is_leaf( int i ) const;
        
        using edge_iterator = typename edge_list::iterator;
        using const_edge_iterator = typename edge_list::const_iterator;
        
        class node_iterator {
            public:
                node_iterator( int i, sparse_graph& owner )
                :   position_{ i },
                    owner_{ owner }
                    {}
                node_iterator()                                     = delete;
                node_iterator( const node_iterator& )               = default;
                node_iterator( node_iterator&&  )                   = default;
                node_iterator& operator=( const node_iterator& )    = default;
                node_iterator& operator=( node_iterator&& )         = default;
                ~node_iterator()                                    = default;
                
                bool operator==( const node_iterator& n ) const {
                    return position_ == n.position_;
                }
                
                bool operator!=( const node_iterator& n ) const {
                    return position_ != n.position_;
                }
                
                node_iterator& operator++() {
                    position_++;
                    return *this;
                }
                
                N& operator*() {
                    return owner_.nodes_.at( position_ );
                }
                
                edge_iterator begin() {
                    return owner_.edges_[ position_ ].begin();
                }
                
                const_edge_iterator begin() const {
                    return owner_.edges[ position_ ].begin();
                }
                
                edge_iterator end() {
                    return owner_.edges_[ position_ ].end();
                }
                
                const_edge_iterator end() const {
                    return owner_.edges_[ position_ ].end();
                }
                
            private:
                int             position_;
                sparse_graph&   owner_;
        };
        
        class const_node_iterator {
            public:
                const_node_iterator( int i, const sparse_graph& owner )
                :   position_{ i },
                    owner_{ owner }
                    {}
                const_node_iterator()                                           = delete;
                const_node_iterator( const const_node_iterator& )               = default;
                const_node_iterator( const_node_iterator&&  )                   = default;
                const_node_iterator& operator=( const const_node_iterator& )    = default;
                const_node_iterator& operator=( const_node_iterator&& )         = default;
                ~const_node_iterator()                                          = default;
                
                bool operator==( const const_node_iterator& n ) const {
                    return position_ == n.position_;
                }
                
                bool operator!=( const const_node_iterator& n ) const {
                    return position_ != n.position_;
                }
                
                const_node_iterator& operator++() {
                    position_++;
                    return *this;
                }
                
                const N& operator*() const {
                    return owner_.nodes_.at( position_ );
                }
                
                const_edge_iterator begin() const {
                    return owner_.edges_[ position_ ].begin();
                }
                
                const_edge_iterator end() const {
                    return owner_.edges_[ position_ ].end();
                }
                
            private:
                int                 position_;
                const sparse_graph& owner_;
        };
        
        friend class node_iterator;
        friend class const_node_iterator;
        
        node_iterator iterator_at( int index );
        const_node_iterator iterator_at( int index ) const;
        node_iterator begin();
        const_node_iterator begin() const;
        node_iterator end();
        const_node_iterator end() const;
        
    private:
        // METHODS
        // Check if there is only one of the given edge
        bool is_unique_( int, int );
        // remove all edges pointing to invalid nodes
        // this operation has O( N*E ) time complexity
        void cull_invalid_edges_();
        
        // FIELDS
        node_vector      nodes_{};
        edge_list_vector edges_{};
        std::size_t      edge_count_{ 0u };
        bool             is_digraph_{ false };
        const int        invalid_node_index_{ -1 };
};

template< typename N, typename E >
sparse_graph< N, E >::sparse_graph( bool is_digraph )
:   nodes_(),
    edges_(),
    edge_count_( 0u ),
    is_digraph_( is_digraph ),
    invalid_node_index_( -1 )
    {}
    
template< typename N, typename E >
bool sparse_graph< N, E >::is_unique_( int from, int to ) {
    for ( const edge_type& e: iterator_at( from ) ) {
        if ( e.to == to ) {
            return false;
        }
    }
    return true;
}

template< typename N, typename E >
void sparse_graph< N, E >::cull_invalid_edges_() {
    // it points to list of edges
    for ( auto it = edges_.begin(); it != edges_.end(); it++ ) {
        // e points to edge_type
        for ( auto e = it->begin(); e != it->end(); e++ ) {
            if ( nodes_[ e->from ] == invalid_node_index_ || nodes_[ e->to ] == invalid_node_index_ ) {
                e = it->erase( e );
                edge_count_--;
            }
        }
    }
}

template< typename N, typename E >
bool sparse_graph< N, E >::is_empty() const {
    return nodes_.empty();
}
   
template< typename N, typename E >
bool sparse_graph< N, E >::is_digraph() const {
    return is_digraph_;
}
  
template< typename N, typename E >
std::size_t sparse_graph< N, E >::number_of_nodes() const {
    return nodes_.size();
}

template< typename N, typename E >
std::size_t sparse_graph< N, E >::number_of_edges() const {
    return edge_count_;
}

template< typename N, typename E >
N& sparse_graph< N, E >::operator[]( int i ) {
    return nodes_[i];
}

template< typename N, typename E >
const N& sparse_graph< N, E >::operator[]( int i ) const {
    return nodes_[i];
}

template< typename N, typename E >
int sparse_graph< N, E >::add_node( typename N::value_type val ) {
    nodes_.push_back( node_type{ int (nodes_.size()), val } );
    edges_.emplace_back();
    return nodes_.size() - 1u;
}

template< typename N, typename E >
void sparse_graph< N, E >::remove_node( int index ) {
    nodes_[ index ] = invalid_node_index_;
    cull_invalid_edges_();
}

template< typename N, typename E >
void sparse_graph< N, E >::add_edge( edge_type e ) {
    assert( e.from >= 0 && e.to >= 0 );
    assert( e.from < int (nodes_.size()) && e.to < int (nodes_.size()) );
    if ( is_unique_( e.from, e.to ) ) {
        edges_[ e.from ].push_back( e );
        edge_count_++;
    }
    
    if ( is_digraph_ ) {
        if ( is_unique_( e.to, e.from ) ) {
            edges_[ e.to ].push_back( e );
            edge_count_++;
        }
    }
}

template< typename N, typename E >
bool sparse_graph< N, E >::is_leaf( int i ) const {
    if ( edges_[i].size() == 0u ) {
        return true;
    }
    return false;
}

template< typename N, typename E >
typename sparse_graph< N, E >::node_iterator sparse_graph< N, E >::begin() {
    return node_iterator( 0, *this );
}

template< typename N, typename E >
typename sparse_graph< N, E >::const_node_iterator sparse_graph< N, E >::begin() const {
    return const_node_iterator( 0, *this );
}

template< typename N, typename E >
typename sparse_graph< N, E >::node_iterator sparse_graph< N, E >::end() {
    return node_iterator( -1, *this );
}

template< typename N, typename E >
typename sparse_graph< N, E >::const_node_iterator sparse_graph< N, E >::end() const {
    return const_node_iterator( -1, *this );
}

template< typename N, typename E >
N& sparse_graph< N, E >::at( int i ) {
    return nodes_[i];
}

template< typename N, typename E >
const N& sparse_graph< N, E >::at( int i ) const {
    return nodes_[i];
}

template< typename N, typename E >
typename sparse_graph< N, E >::node_iterator sparse_graph< N, E >::iterator_at( int i ) {
    assert( i < int( nodes_.size() ) );
    return node_iterator( i, *this );
}

template< typename N, typename E >
typename sparse_graph< N, E >::const_node_iterator sparse_graph< N, E >::iterator_at( int i ) const {
    assert( i < int( nodes_.size() ) );
    return const_node_iterator( i, *this );
}

}   // namespace graph

#endif // SPARSEGRAPH_H_INCLUDED