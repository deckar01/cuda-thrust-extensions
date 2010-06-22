
/*! \file big_integer.h
 *  \brief Implementation of arbitrary precision integer arithmetic.
 */

#pragma once

#include <iosfwd>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <limits>

typedef unsigned int uint;

namespace thrust
{
	const uint msb = std::numeric_limits<uint>::max() ^ ( std::numeric_limits<uint>::max() >> 1 );
	const uint max_uint = std::numeric_limits<uint>::max();

	template<uint value_size>
	struct big_integer
	{
	//public:
		__host__ __device__
		inline big_integer(uint value)
		{
			set_zero();
			data[0] = value;
		}
		
		__host__ __device__
		inline big_integer(int value)
		{
			set_zero();
			data[0] = (uint)value;
		}

		__host__ __device__
		inline big_integer()
		{
			set_zero();
		}
		
		__host__ __device__
		inline big_integer<value_size>& operator=(uint value)
		{
			data[0] = value;
			return *this;
		}
		
		__host__ __device__
		inline big_integer<value_size>& operator=(int value)
		{
			data[0] = (uint)value;
			return *this;
		}

		__host__ __device__
		inline operator uint()
		{
			return data[0];
		}
		
		__host__ __device__
		inline operator int()
		{
			return (int)data[0];
		}

		__host__ __device__
		inline uint size() const
		{
			return value_size;
		}
		
		__host__ __device__
		inline void set_zero()
		{
			// device don't have memset, 
			//memset(data, 0, sizeof(uint) * value_size);
			for(int i = 0; i < value_size; i++)
				data[i] = 0;
		}
		
		__host__ __device__
		static inline big_integer<value_size> zero()
		{
			return m_zero;
		}

		__host__ __device__
		static inline big_integer<value_size> max()	// return max value for a given value size
		{
			big_integer<value_size> m;
			for(int i = 0; i < value_size; i++)
				m.data[i] = max_uint;
			return m;
		}

		// Shift operators
		/*
		big_integer<value_size> operator>>(int shift);
		big_integer<value_size> operator<<(int shift);
		big_integer<value_size>& operator>>=(int shift);
		big_integer<value_size>& operator<<=(int shift);
		*/

		// Arithmetic operators
		__host__ __device__
		big_integer<value_size> operator+(const big_integer<value_size>& other) const;
		/*
		big_integer<value_size>& operator+=(const big_integer<value_size>& q);
		big_integer<value_size>& operator++();  		// Pre Increment operator -- faster than add
		big_integer<value_size>& operator++(int);  		// Post Increment operator -- faster than add
		big_integer<value_size> operator-();  			// Negates a number
		big_integer<value_size> operator-(const big_integer<value_size>& q) const;
		big_integer<value_size>& operator-=(const big_integer<value_size>& q);
		big_integer<value_size>& operator--();  		// Pre Decrement operator -- faster than add
		big_integer<value_size>& operator--(int);  		// Post Decrement operator -- faster than add
		//big_integer<value_size> operator*(big_integer<value_size> q);
		big_integer<value_size> operator*(const big_integer<value_size>& q) const;
		big_integer<value_size> Divide(big_integer<value_size> dividend, 
									   big_integer<value_size> divisor,
									   big_integer<value_size>* remainder);
		big_integer<value_size> operator/(const big_integer<value_size>& q);
		big_integer<value_size> operator%(const big_integer<value_size>& q);
		big_integer<value_size> sqrt();			// returns the square root of this
		*/

		// Comparison operators
		__host__ __device__
		int operator<(const big_integer<value_size>& q) const;
		__host__ __device__
		int operator>(const big_integer<value_size>& q) const;
		__host__ __device__
		int operator<=(const big_integer<value_size>& q) const;
		__host__ __device__
		int operator>=(const big_integer<value_size>& q) const;

		// Bitwise operators
		/*
		big_integer<value_size> operator&(const big_integer<value_size>& q);
		big_integer<value_size> operator|(const big_integer<value_size>& q);
		big_integer<value_size> operator^(const big_integer<value_size>& q);
		big_integer<value_size>& operator&=(const big_integer<value_size>& q);
		big_integer<value_size>& operator|=(const big_integer<value_size>& q);
		big_integer<value_size>& operator^=(const big_integer<value_size>& q);
		big_integer<value_size> operator~();
		*/

		// Comparison operators
		__host__ __device__
		int operator==(const big_integer<value_size>& other) const
		{
			// device don't have memcmp
			return memcmp( data, other.data, value_size * sizeof(uint) ) == 0;
		}

		/*
		__host__ __device__
		int operator!=(const big_integer<value_size>& other) const
		{
			// device don't have memcmp
			return memcmp( data, other.data, value_size * sizeof(uint) ) != 0;
		}
		*/
		
	//private:
		// Storage for values
		uint data[value_size];
		
		// zero value
		static big_integer<value_size> m_zero;
		
		//template<uint value_size>
		//friend std::ostream& operator<<(std::ostream& out, const big_integer<value_size>& number);
	};

}

template<uint value_size> thrust::big_integer<value_size> thrust::big_integer<value_size>::m_zero;

/*
template<uint value_size>
std::ostream& operator<<(std::ostream & out, thrust::big_integer<value_size>& num)
{
	thrust::big_integer<value_size> billion = 1000000000;
	thrust::big_integer<value_size> digit;
	thrust::big_integer<value_size> q;
	uint decimaldigit;
	ldiv_t divideresult;
	int i;
	//char tmp;
	char* p;
	char* decimal_array = new char[8000];	// Allocate only needed, taking into 
											// account value_size template parameter
	
	q = num;
	p = decimal_array + 3999;
	*p = 0;
	p--;

	if( q == thrust::big_integer<value_size>::zero() )
	{
		*p = '0';
		p--;
	}

	for(; q > thrust::big_integer<value_size>::zero(); )
	{
		q = q.Divide( q, billion, &digit );
		decimaldigit = digit;
		for( i = 0; i < 9; i++ )
		{
			divideresult = ldiv( decimaldigit, 10 );
			*p = (char) divideresult.rem + 0x30;
			p--;
			decimaldigit = divideresult.quot;
		}
	}

	for( p++; *p=='0'; p++ ) {} // Strip off leading zeros.

	if( *p == 0 )
		p--;

	//printf("%s", p);
	//out << p;
	out << std::string(p);
	delete decimal_array;
	return out;
}
*/

#include "big_integer.inl"
