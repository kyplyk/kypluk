#ifndef _kypluk_BigInteger_
#define _kypluk_BigInteger_

//����� ��������� �� ����� ���������
#include <kyplukDefine.h>
#include <kyplukVector.h>
#include <kyplukList.h>
#include <kyplukPair.h>

#include <iostream>

namespace kypluk {

//ebanuy rot ska
class unlimInt {
	private:
		static const uint8_t base = 10;
    	//���� + ��� - 
    	//true - is_negative
    	//false - is_not_negative
    	bool _is_negative;
    	using container_t = List<uint8_t>;
		container_t arr;
		
		void _remove_leading_zeros() {
			while (!arr.is_empty() and arr.back() == 0)
				arr.pop_back();
			if (arr.is_empty()) {
				arr.push_back(0);
				_is_negative = false;
			}
		}
		
	public:
		
		unlimInt(llint puk = 0) {
			_is_negative = puk < 0;
			arr.push_back(puk%10);
			while ((puk/=10) != 0) {
				arr.push_back(puk%10);
		    }
		}
		
		unlimInt(const unlimInt & puk) {
			*this = puk;
		}
		
		~unlimInt() {
			arr.clear();
		}
		
		Size_t length() const {
			return arr.size();
		}
		
		unlimInt& add (const unlimInt& puk) {
			// �� ������� ���� �������� ���� ������������� �����
        	// ��������� �� �������, ��������� ����� ����� � ���������
			if (this->_is_negative) {
                if (puk._is_negative) return *this = -((-*this) + (-puk));
                else return *this = puk - (-*this);
	        }
	        else if (puk._is_negative) return this->sub(-puk);
	        
			if (arr.size() < puk.arr.size()) {
				Size_t buf = puk.arr.size()-arr.size();
				for (Size_t i = 0; i < buf; i++)
					arr.push_back(0);
			}
			
		    int desyatok = 0;
		    auto i = arr.begin();
			for (auto j = puk.arr.begin(); i != arr.end() and j != puk.arr.end(); ++i, ++j) {
		        *i = *i + *j + desyatok;
		        desyatok = 0;
		        if (*i > 9) {
		            desyatok = 1;//i / 10
		            *i -= 10;//i % 10
		        }
		    }
		    if (i != arr.end()) {
		    	for ( ; i != arr.end(); ++i) {
		    		if (desyatok != 0) {
				        *i = *i + desyatok;
				        desyatok = 0;
				        if (*i > 9) {
				            desyatok = 1;//i / 10
				            *i -= 10;//i % 10
				        }
				    }
			    }
		    }
		    if (desyatok)
				arr.push_back(1);
			
		    return *this;
		}
		
		unlimInt& sub (const unlimInt& puk) {
			
			if (puk._is_negative) return this->add(-puk);
	        else if (this->_is_negative) return *this = -(-*this + puk);
	        else if (*this < puk) return *this = -(puk - *this);
	        
			bool carry = false;
	        for (auto i = this->arr.begin(), j = puk.arr.begin(); j != puk.arr.end() || carry != 0; ++i) {
                *i -= carry + (j != puk.arr.end() ? *j : 0);
                carry = *i > base;
                if (carry) *i += base;
                if (j != puk.arr.end()) ++j;
	        }
	 
	        this->_remove_leading_zeros();
			
		    return *this;
		}
		
		unlimInt& mult10(unlimInt puk = 1) {
			if ((*this) != 0)
				for (unlimInt i = 0; i != puk; ++i) 
					arr.push_front(0);
			return *this;
		}
		
		//fix ������ �������
		unlimInt& mult0to9 (unsigned short int puk) {
			unlimInt xraniliche(*this);
			(*this) = 0;
			
			for (int i = 0; i < puk; i++)
			{
				(*this) += xraniliche;	
			}
			
			return *this;
		}
		
		unlimInt& mult (const unlimInt& puk) {
			unlimInt xraniliche;
			xraniliche = *this;
			(*this) = 0;
			Size_t j = 0;
			for (auto i = puk.arr.begin(); i != puk.arr.end(); ++i, ++j)
			{
				unlimInt xraniliche1 = xraniliche; 
				(*this) += xraniliche1.mult0to9(*i).mult10(j);
			}
			
			return *this;
		}
		
		
		Pair<unlimInt, unlimInt> divmod(const unlimInt &b1) const {
	        int8_t norm = base / (b1.arr.back() + 1);
	        unlimInt a = this->abs() * norm;
	        unlimInt b = b1.abs() * norm;
	        unlimInt q, r;
	        //q.arr.resize(arr.arr.size());
	        q.arr = container_t(a.arr.size());
	
	        for (auto i = --a.arr.end(), j = --q.arr.end(); i != a.arr.end(); --i, --j) {
	            r *= base;
	            r += *i;
	            uint8_t s1 = r.arr.size() <= b.arr.size() ? 0 : r.arr.at(b.arr.size());
	            uint8_t s2 = r.arr.size() <= b.arr.size() - 1 ? 0 : r.arr.at(b.arr.size() - 1);
	            int d = ((long long) base * s1 + s2) / b.arr.back();
	            r -= b * d;
	            while (r < 0) {
	                r += b;
					--d;
				}
	            *j = d;
	        }
	
	        q._is_negative = this->_is_negative * b1._is_negative;
	        r._is_negative = this->_is_negative;
	        
	        q._remove_leading_zeros();
        	r._remove_leading_zeros();

	        return Pair<unlimInt, unlimInt>({q, r.divbase(norm)});
	    }
	    
	    unlimInt divbase(int8_t v) const {
	    	unlimInt copy = *this;
	    	if (v < 0) {
            	copy._is_negative = true;
				v = -v;
            }
            int rem = 0;
	        for (auto i = --copy.arr.end(); i != copy.arr.end(); --i) {
	            uint8_t cur = *i + rem * base;
	            *i = (int) (cur / v);
	            rem = (int) (cur % v);
	        }
	        copy._remove_leading_zeros();
	        return copy;
		}
		
		//--------
		//dispensable methods
		//--------
		
		int sign() const {
			if ((*this) == 0) return 0; 
			return this->_is_negative ? -1 : 1;
		}
		
		unlimInt abs() const {
			unlimInt puk(*this);
			puk._is_negative = false;
			return puk;
		}
		
		bool odd() const {
        	return this->arr.front() & 1;
		}
		
		bool even() const {
		    return !this->odd();
		}
		
		//---------
		//static function
		//---------
		
		static Vector<char> to_vstring(const unlimInt& number) {
			Size_t j = number.length()-1+number._is_negative;
			Vector<char> buf(number.length()+1+number._is_negative);
			if (number._is_negative) buf.front() = '-';
			for (auto i = number.arr.begin(); j >= 0+number._is_negative and i != number.arr.end(); --j, ++i) {
				buf[j] = *i+'0';
			}
			
			buf.back() = '\0';
			return buf;
		}
		
		static unlimInt from_string(const char * value) {
			unlimInt res;
			bool is_ngt = false;
			if (*value) {
				if (*value == '-') {
					is_ngt = true;
					value++;
				}
				res = *value - '0';
				value++;
			}
			while (*value) {
				res.mult10(1).add(*value - '0');
				value++;
			}
			res._is_negative = is_ngt;
			return res;
		}
		
		static int compare(const unlimInt& raz, const unlimInt& dva) {
			if (raz._is_negative != dva._is_negative) 
				return dva._is_negative - raz._is_negative;
			
			if (raz.length() != dva.length())
				return raz.length() > dva.length() ? 1 : -1;
			
			for (auto i = --raz.arr.end(), j = --dva.arr.end(); i != raz.arr.end() and j != dva.arr.end(); --i, --j)
			{
				if (*i != *j)
					return (*i - *j) * (raz._is_negative ? -1 : 1);
			}
			
			return 0;
		}
		
		//---------
		//operators
		//---------
		
		unlimInt& operator ++ () {
			if (++(*arr.begin()) > 9) {
				*arr.begin() -= 10;
				add(unlimInt(10));
			}
			
			return *this;
		}
		
		unlimInt& operator = (const unlimInt& puk) {
			arr = puk.arr;
			_is_negative = puk._is_negative;
			return *this;
		}
		
		unlimInt& operator += (const unlimInt &puk) {
			return add(puk);
		}

		unlimInt& operator -= (const unlimInt &puk) {
			return sub(puk);
		}
		
		unlimInt& operator *= (const unlimInt& puk) {
			return mult(puk);
		}
		
		friend const unlimInt operator + (const unlimInt& raz, const unlimInt& dvas) {
			unlimInt puk = raz;
			return puk.add(dvas);
		}
		
		friend const unlimInt operator - (const unlimInt& raz, const unlimInt& dvas) {
			unlimInt puk = raz;
			return puk.sub(dvas);
		}

		friend const unlimInt operator * (const unlimInt& raz, const unlimInt& dvas) {
			unlimInt puk = raz;
			return puk.mult(dvas);
		}
		
		friend const unlimInt operator / (const unlimInt& raz, const unlimInt& dvas) {
			return raz.divmod(dvas).first;
		}
		
		friend const unlimInt operator % (const unlimInt& raz, const unlimInt& dvas) {
			return raz.divmod(dvas).second;
		}
		
		const unlimInt operator +() const {
		    return unlimInt(*this);
		}
		 
		const unlimInt operator -() const {
			unlimInt temp(*this);
			temp._is_negative = !temp._is_negative;
		    return temp;
		}
		
		bool operator != (const unlimInt& puk) const{
			return compare(*this, puk) != 0;
		}
		
		bool operator >= (const unlimInt& puk) const {
			return compare(*this, puk) >= 0;
		}
		
		bool operator <= (const unlimInt& puk) const {
			return compare(*this, puk) <= 0;
		}
		
		bool operator > (const unlimInt& puk) const {
			return compare(*this, puk) > 0;
		}
		
		bool operator < (const unlimInt& puk) const {
			return compare(*this, puk) < 0;
		}
		
		bool operator == (const unlimInt& puk) const{
			return compare(*this, puk) == 0;
		}
};

} // end namespace kypluk
#endif
