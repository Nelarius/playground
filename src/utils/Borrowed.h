
#pragma once

namespace ce {

/**
 * @class Borrowed
 * @author Nelarius
 * @date 05/11/15
 * @file Borrowed.h
 * @brief A thinly wrapped pointer, which is managed elsewhere.
 * The pointer member-access operator can be used on an object of this class.
 */
template<typename T>
class Borrowed {
	public:
		Borrowed() = default;
		Borrowed( T* t )
		:	value_( t )
			{}
		~Borrowed() = default;
		
		T* operator->() {
			return value_;
		}
        
        bool operator<( T* rhs ) {
            return value_ < rhs;
        }
        
        bool operator>( T* rhs ) {
            return value_ > rhs;
        }
        
        bool operator==( const Borrowed& rhs ) {
            return value_ == rhs.value_;
        }
		
		void set( T* t ) {
			value_ = t;
		}
	
	private:
		T*	value_{ nullptr };
};

}

