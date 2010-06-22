
namespace thrust
{
	/*
	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator>>(int shift)
	{
		big_integer<value_size> a, t;
		int i, j;

		t = *this;

		for( i = 0; i < shift; i++ )
		{
			a.data[value_size - 1] = t.data[value_size - 1] >> 1;
			for( j = value_size - 2; j >= 0; j-- )
			{
				a.data[j] = t.data[j] >> 1;
				if( ( t.data[j+1] & 1 ) != 0 )
					a.data[j] |= msb;  				// Set most sig. bit
			}
			t = a;
		}

		return a;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator<<(int shift)
	{
		big_integer<value_size> a, t;
		int i, j;

		t = *this;

		for ( i = 0; i < shift; i++ )
		{
			a.data[0] = t.data[0] << 1;
			for ( j = 1; j < value_size; j++ )
			{
				a.data[j] = t.data[j] << 1;
				if ( ( t.data[j-1] & msb ) != 0 )
					a.data[j] |= 1;  // Set least sig. bit
			}
			t = a;
		}

		return a;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator>>=(int shift)
	{
		uint carry;
		int i, j;

		for (i=0; i<shift; i++)
		{
			carry = data[value_size-1] & 1;
			data[value_size-1] >>= 1;
			for ( j = (int)value_size - 2; j >= 0; j-- )
			{
				if (carry)
				{
					carry = data[j] & 1;
					data[j] >>= 1;
					data[j] |= msb;
				}
				else
				{
					carry = data[j] & 1;
					data[j] >>= 1;
				}
			}
		}

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator<<=(int shift)
	{
		uint carry;
		int i, j;

		for (i=0; i<shift; i++)
		{
			carry = data[0] & msb;
			data[0] <<= 1;
			for ( j = 1; j < value_size; j++ )
			{
				if (carry)
				{
					carry = data[j] & msb;
					data[j] <<= 1;
					data[j] |= 1;
				}
				else
				{
					carry = data[j] & msb;
					data[j] <<= 1;
				}
			}
		}

		return *this;
	}
	*/

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator+(const big_integer<value_size>& other) const
	{
		big_integer<value_size> result; 
		uint carry = 0;
		int i;

		for( i = 0; i < value_size; i++ )
		{
			result.data[i] = data[i] + other.data[i] + carry;
			if( carry == 0 )
			{
				if( result.data[i] < data[i] || result.data[i] < other.data[i] )
					carry = 1;
				else
					carry = 0;
			}
			else
			{
				if( result.data[i] <= data[i] || result.data[i] <= other.data[i] )
					carry = 1;
				else
					carry = 0;
			}
		}

		return result;
	}

	/*
	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator+=(const big_integer<value_size>& q)
	{
		uint carry = 0, prevdigit;
		int i;

		for( i = 0; i < value_size; i++ )
		{
			prevdigit = data[i];
			data[i] = data[i] + q.data[i] + carry;
			if (carry == 0)
			{
				if (data[i] < prevdigit || data[i] < q.data[i])
					carry=1;
				else
					carry=0;
			}
			else
			{
				if (data[i] <= prevdigit || data[i] <= q.data[i])
					carry=1;
				else
					carry=0;
			}
		}

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator++()  // Pre Increment operator -- faster than add 
	{
		int i;

		data[0]++;
		for( i = 1; i < value_size; i++ )
			if (data[i-1] == 0)
				data[i]++;
			else
				break;

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator++(int)  // Post Increment operator -- faster than add
	{
		int i;

		data[0]++;
		for( i = 1; i < value_size; i++)
			if (data[i-1] == 0)
				data[i]++;
			else
				break;

		return *this;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator-()  // Negates a number
	{
		big_integer<value_size> result;
		int i;

		for( i = 0; i < value_size; i++)
			result.data[i] = ~data[i];

		result = result + one;

		return result;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator-(const big_integer<value_size>& q) const
	{
		big_integer<value_size> result;
		uint borrow = 0;
		int i;
		
		for( i = 0; i < value_size; i++ )
		{
			result.data[i] = data[i] - q.data[i] - borrow;
    
			if (borrow == 0)
			{
				if (data[i] < q.data[i])
					borrow=1;
				else
					borrow=0;
			}
			else
			{
				if (data[i] <= q.data[i])
					borrow=1;
				else
					borrow=0;
			}
		}

		return result;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator-=(const big_integer<value_size>& q)
	{
		uint borrow = 0, prevdigit;
		int i;

		for( i = 0; i < value_size; i++ )
		{
			prevdigit = data[i];
			data[i] = data[i] - q.data[i] - borrow;
			if (borrow == 0)
			{
				if (prevdigit < q.data[i])
					borrow=1;
				else
					borrow=0;
			}
			else
			{
				if (prevdigit <= q.data[i])
					borrow=1;
				else
					borrow=0;
			}
		}

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator--()  // Pre Decrement operator -- faster than add
	{
		int i;
		data[0]--;
		
		for( i = 1; i < value_size; i++ )
			if( data[i-1] == std::numeric_limits<uint>::max() )
				data[i]--;
			else
				break;

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator--(int)  // Post Decrement operator -- faster than add
	{
		int i;

		data[0]--;
		for( i = 1; i < value_size; i++)
			if (data[i-1] == std::numeric_limits<uint>::max() )
				data[i]--;
			else
				break;

		return *this;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator*(const big_integer<value_size>& q) const
	{
		big_integer<value_size> t = *this;
		big_integer<value_size> p;
		big_integer<value_size> qq = q;

		do
		{
			if ( ( qq.data[0] & 1 ) != 0 )
				p += t;
			qq >>= 1;
			t <<= 1;
		} while (qq != zero());
		
		return p;	
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::Divide(big_integer<value_size> a, 
									big_integer<value_size> b,
									big_integer<value_size>* remainder)
	{
		big_integer<value_size> c, d;
		int i, shiftcnt=0;

		// Check for attempt to divide by zero
		if( b == zero() )
			shiftcnt = 1 / shiftcnt;  	// Force a divide by zero exception. (shiftcnt=0)

		c=zero();
		d=b;		     				// Store the divisor in D

		// Left shift B until it is greater than or equal to A
		while ( b < a && ( ( b.data[value_size-1] & msb ) == 0 ) )
		{
			b <<= 1;
			shiftcnt++;
		}

		if ( b > a )      // If B is greater than A, right shift B
		{
			b >>= 1;
			shiftcnt--;
		}

		for( i = 0; i <= shiftcnt; i++ )
		{
			if ( b <= a )    	// If B is greater than A, then the bit is a 1
			{
				a -= b;     	// Subtract B from A
				b >>= 1;  		// Right shift B
				c <<= 1;  		// Left shift quotient
				c++;       		// Increment quotient
			}
			else
			{
				b >>= 1;  		// Bit is 0, right shift B, left shift quotient
				c <<= 1;
			}
		}

		if ( remainder != NULL )
			*remainder = a;

		return c;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator/(const big_integer<value_size>& q)
	{
		return Divide(*this, q, NULL);
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator%(const big_integer<value_size>& q)
	{
		big_integer<value_size> Result;

		Divide(*this, q, &Result);

		return Result;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::sqrt()		// returns the square root of this
	{
		big_integer<value_size> x,y,dx;
		big_integer<value_size> mask = (big_integer<value_size>)2;
		int i,j;

		mask = -mask;

		y = *this;

		for( i = 0, x = y; x != zero(); x >>= 1, i++ );
		for( j = 0, x = y; j < (unsigned long)( i >> 1); x >>= 1, j++ );
		do
		{
			//	We are really performing the fuction:
			//	dx = (y/x - x) / 2;
			//	below, but since these are unsigned numbers,
			//	we MUST do the subtraction last in order for
			//	the x = x + dx;  equation to work properly.
			dx = (y>>1)/x - (x>>1);
			x = x + dx;
		} while ( ( dx &= mask ) != zero() );

		return x;
	}
	*/

	template<uint value_size>
	int big_integer<value_size>::operator<(const big_integer<value_size>& q) const
	{
		int i;

		for ( i = ( value_size - 1 ); i >= 0; i-- )
		{
			if (data[i] < q.data[i])
				return 1;
			if (data[i] > q.data[i])
				return 0;
		}
		return 0;
	}

	template<uint value_size>
	int big_integer<value_size>::operator>(const big_integer<value_size>& q) const
	{
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
		{
			if (data[i] < q.data[i])
				return 0;
			if (data[i] > q.data[i])
				return 1;
		}
		return 0;
	}

	template<uint value_size>
	int big_integer<value_size>::operator<=(const big_integer<value_size>& q) const
	{
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
		{
			if (data[i] < q.data[i])
				return 1;
			if (data[i] > q.data[i])
				return 0;
		}
		return 1;
	}

	template<uint value_size>
	int big_integer<value_size>::operator>=(const big_integer<value_size>& q) const
	{
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
		{
			if (data[i] < q.data[i])
				return 0;
			if (data[i] > q.data[i])
				return 1;
		}
		return 1;
	}

	/*
	// Bitwise operators
	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator&(const big_integer<value_size>& q)
	{
		big_integer<value_size> result;
		int i;

		for( i = ( value_size - 1); i >= 0; i-- )
			result.data[i] = data[i] & q.data[i];

		return result;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator|(const big_integer<value_size>& q)
	{
		big_integer<value_size> result;
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
			result.data[i] = data[i] | q.data[i];

		return result;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator^(const big_integer<value_size>& q)
	{
		big_integer<value_size> result;
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
			result.data[i] = data[i] ^ q.data[i];

		return result;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator&=(const big_integer<value_size>& q)
	{
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
			data[i] = data[i] & q.data[i];

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator|=(const big_integer<value_size>& q)
	{
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
			data[i] = data[i] | q.data[i];

		return *this;
	}

	template<uint value_size>
	big_integer<value_size>& big_integer<value_size>::operator^=(const big_integer<value_size>& q)
	{
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
			data[i] = data[i] ^ q.data[i];

		return *this;
	}

	template<uint value_size>
	big_integer<value_size> big_integer<value_size>::operator~()
	{
		big_integer<value_size> result;
		int i;

		for( i = ( value_size - 1 ); i >= 0; i-- )
			result.data[i] = ~data[i];

		return result;
	}
	*/
}