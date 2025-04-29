#include "biginteger.h"

//-------------------------------------------------------------------------------------------
BigInteger::~BigInteger() {
	delete[]num;
	num = NULL;
	size = 0;
	
	//std::cout << '\n' << "DELETE" << '\n';
}
BigInteger::BigInteger() {
	size = 1;
	sign = false;
	delete[] num;
	num = NULL;
	num = new int64_t [size];
	num[0] = 0;
}

BigInteger::BigInteger(long long num_) {

	int64_t copy_num = (num_ < 0) ? (num_ * (-1)) : num_;
	int size_ = 1;
	sign = (num_ < 0);
	while (copy_num > 1) {
		if (copy_num / BIG > 0) {
			++size_;         
		}
		copy_num /= BIG;
	}
	
	size = size_;
	if (this->num != nullptr) {
		delete[]this;
	}
	num = new int64_t [size];
	copy_num = (num_ < 0) ? (num_ * (-1)) : num_;
	int i = 0;
	while (i < size) {
		num[i] = static_cast<unsigned int>(copy_num % BIG);
		copy_num /= BIG;
		++i;
	}
	
}

BigInteger::BigInteger(const BigInteger& value) {

		size = value.size;
		sign = value.sign;
		delete[] num;
		num = new int64_t[size];
		for (int i = 0; i < size; ++i) {
			num[i] = value.num[i];
		}
	
}
//value==(a*b)
BigInteger::BigInteger(BigInteger&& value) noexcept {

		size = (value.size);
		sign = (value.sign);
		this->num = value.num;

		value.num = nullptr;
		value.sign = false;
		value.size = 0;
	
}

BigInteger::BigInteger(const std::string& num_) {
	if (num_.length() == 0) {
		throw std::invalid_argument("empty");
	}
	get_sign(num_);
	
	int j = 0;
	if (num_.length() == 1 && sign) { throw std::invalid_argument("err"); }
	for (int i = (sign) ? 1 : 0; i < num_.length(); ++i) {
		if (num_[i] < '0' || num_[i]>'9') {
			throw std::invalid_argument("err");
		}
	}
	std::string copy_num_;
	std::string bits;
	copy_num_ = (sign) ? num_.substr(1, num_.size() - 1) : num_;
	copy_num_ = conv_binary(copy_num_);
	int tmp = ((copy_num_.size() % 32 == 0) ? 0 : 1);
	size = static_cast<int>(tmp + copy_num_.size() / 32);
	delete[]num;
	num = nullptr;
	num = new int64_t [size];
	for (int i = 0; i < size; ++i) {
		num[i] = 0;
	}
	j = 0;
	for (int i = static_cast<int>(copy_num_.length()); i > 0; i -= 32) {
		num[j] = ((i < 32) ? to_binary(copy_num_.substr(0, i)) : to_binary(copy_num_.substr(i - 32, 32)));
		if (j + 1 < size) {
			++j;
		}
	}
	if (size - 1 >= 0) {
		if (size==1 && num[0] == 0) {
			sign = false;
		}
	}
}

//-------------------------------------------------------------------------------------------

BigInteger& BigInteger::operator=(BigInteger& value) {
	if (this == &value) { return *this; }
	size = value.size;
	sign = value.sign;
	delete[]num;
	
	num = new int64_t[value.size];
	for (int i = 0; i < value.size; ++i) {
		num[i] = value.num[i];
	}
	return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& value) noexcept {
	
	if (this != &value) {
		sign = value.sign;
		delete[]num;
		num = value.num;
		size = value.size;
		value.sign = false;
		value.num = nullptr;
		value.size = 0;
	}
	return *this;
}

//-------------------------------------------------------------------------------------------
//_____BOOL(S)_______

bool operator==(const BigInteger& l_value, const BigInteger& r_value) {
	if (l_value.sign == r_value.sign&&l_value.size==r_value.size) {
		for (int i = l_value.size - 1; i >= 0; --i) {
			if (l_value.num[i] != r_value.num[i]) { return false; }
		}
		return true;
	}
	return false;
}

bool operator!=(const BigInteger& l_value, const BigInteger& r_value) {
	return !(l_value == r_value);
}

bool operator>(const BigInteger& l_value, const BigInteger& r_value) {
	if (l_value == r_value||
	(l_value.size > r_value.size && l_value.sign)||
	(l_value.size < r_value.size&& !l_value.sign && r_value.sign == l_value.sign)||
	(l_value.size == r_value.size && l_value.sign != r_value.sign && l_value.sign)) { return false; }


	if ((l_value.size == r_value.size && l_value.sign != r_value.sign && !l_value.sign)|| 
	(l_value.size > r_value.size && !l_value.sign&& r_value.sign == l_value.sign)){ return true; }
	if (l_value.sign != r_value.sign) { return static_cast<int>(l_value.sign) > static_cast<int>(r_value.sign); }

	for (int i = l_value.size-1; i >=0; --i) {
		if (l_value.num[i] != r_value.num[i]) {
			return (!l_value.sign && l_value.num[i] > r_value.num[i] ||
				(l_value.sign && l_value.num[i] < r_value.num[i]));
			return false;
		}
	}
	return false;
}

bool operator<(const BigInteger& l_value, const BigInteger& r_value) {
	return !(l_value > r_value);
}

bool operator>=(const BigInteger& l_value, const BigInteger& r_value) {
	return (l_value > r_value || l_value == r_value);
}

bool operator<=(const BigInteger& l_value, const BigInteger& r_value) {
	return (l_value < r_value || l_value == r_value);
}

BigInteger BigInteger::operator+() const {
	return (*this);
}


BigInteger BigInteger::operator-() const {
	BigInteger tmp(*this);
	if (*this==0) {return *this;}
	tmp.sign = !tmp.sign;
	return tmp;
}

//-------------------------------------------------------------------------------------------
//______OPERATOR +_______
BigInteger operator+(const BigInteger& l_value, const BigInteger& r_value) {
	BigInteger result;
	int next = 0;
	int size = 0;

	if (l_value.sign == r_value.sign) {
		size=(l_value.size > r_value.size) ? l_value.size + 1 : r_value.size + 1;
	}
	else {
		size=(l_value.size > r_value.size) ? l_value.size : r_value.size ;
	}
	int64_t num1 = 0;
	int64_t num2 = 0;

	BigInteger a(l_value);
	a.sign = false;
	BigInteger c(r_value);
	c.sign = false;

	if (a == c&&l_value.sign!=r_value.sign) { return BigInteger(0); }
	int min = (l_value.size < r_value.size) ? l_value.size : r_value.size;
	int max = (l_value.size < r_value.size) ? r_value.size : l_value.size;
	int i = 0;
	int k = 0;
	bool zero = false;

	//4294967296 4294967295 + 1 < BIG    [0][3]-[1][2]==[2^32][2]-[1][2]==[2^32-1][0]  [0][0][2]-[1][1][1]
	if (l_value.sign == r_value.sign) {
		delete[]result.num;
		result.num = nullptr;
		result.num = new int64_t[(l_value.size > r_value.size) ? l_value.size + 1 : r_value.size + 1];
		for (int i = 0; i < size; ++i) {
			result.num[i] = 0;
		}
		do{
			num1 = (i >=l_value.size) ? 0 : l_value.num[i];
			num2 = (i >= r_value.size) ? 0 : r_value.num[i];
			if (num1 + num2 + next >= r_value.BIG) {
				result.num[i] = num1 + num2 - r_value.BIG + next;
				next = 1;
			}
			else {
				result.num[i] = num1 + num2 + next;
				next = 0;
			}
			++i;
			--min;

		} while (i!=size);
	}
	else {
		if (r_value == l_value) { return BigInteger(0); }
		delete[]result.num;
		result.num = nullptr;
		result.num = new int64_t[max];
		result.size = (a > c) ? a.size : c.size;
		for (int i = 0; i < max; ++i) {result.num[i] = (a > c) ? a.num[i] : c.num[i];}

		BigInteger b;
		delete[]b.num;
		b.num = nullptr;
		b.num = new int64_t[min];
		for (int i = 0; i < min; ++i) {
			b.num[i] = (a < c) ? a.num[i] : c.num[i];
		}
		b.size = (a < c) ? a.size : c.size;
		
		i = 0;
		int min_ = min;
		do {
			
			num2 = (i < min_) ? b.num[i] : 0;
			if (result.num[i] - num2 < 0) {
				result.num[i + 1] -= 1;
				result.num[i] = result.num[i] + r_value.BIG - num2;
			}
			else {
				result.num[i] = result.num[i] - num2;
			}
			++i;
			--min;
		} while (i!=size);
		delete[]b.num;
		b.num = nullptr;
	}
	i = size-1;
	int size_ = size;

	while (result.num[i] == 0) {
		if (i - 1 != -1) {
			--i;
		}
		else {
			break;
		}
		--size_;
	}

	BigInteger res;
	res.size = size_;
	delete[]res.num;
	res.num = nullptr;
	res.num = new int64_t[size_];
	
	k = 0;
	for (i = 0; i < size_; ++i,++k) {
		res.num[i] = result.num[k];
	}
	if (l_value.sign == r_value.sign) {
		res.sign = l_value.sign;
	}
	else {
		res.sign = (a > c) ? l_value.sign : r_value.sign;
	}
	delete[]result.num;
	result.num = nullptr;
	delete[]a.num;
	a.num = nullptr;
	delete[]c.num;
	c.num = nullptr;
	return BigInteger(res);
}

BigInteger& BigInteger::operator++() {
	return (*this+=1);
}

BigInteger BigInteger::operator++(int) {
	BigInteger tmp=*this;
	++(*this);
	return tmp;
}

BigInteger& BigInteger::operator+=(const BigInteger& value) {
	BigInteger tmp(*this + value);
	return *this = tmp;
}

//-------------------------------------------------------------------------------------------
//________OPERATOR -________
BigInteger& BigInteger::operator--() {
	return (*this+=(-1));
}

BigInteger BigInteger::operator--(int) {
	BigInteger tmp(*this);
	--(*this);
	return tmp;
}

BigInteger& BigInteger::operator-=(const BigInteger&value) {
	BigInteger tmp(*this - value);
	return *this = tmp;
}

BigInteger operator-(const BigInteger& l_value, const BigInteger& r_value) {
	const BigInteger& smaller(l_value);
	BigInteger result(r_value);
	//5 - (-6) __5+6
	result.sign = !r_value.sign;
	return BigInteger(result+smaller);
}

//-------------------------------------------------------------------------------------------
//__________OPERAROR *___________


BigInteger operator*(const BigInteger& l_value, const BigInteger& r_value) {
	BigInteger res;
	delete[]res.num;
	res.num = nullptr;
	res.num = new int64_t[l_value.size + r_value.size];
	res.size = l_value.size + r_value.size;
	for (int i = 0; i < l_value.size + r_value.size; ++i) { res.num[i] = 0; }

	BigInteger result;
	delete[]result.num;
	result.num = nullptr;
	result.num = new int64_t[l_value.size + r_value.size];
	result.size = l_value.size + r_value.size;
	for (int i = 0; i < result.size; ++i) { result.num[i] = 0; }

	BigInteger c(l_value);
	BigInteger b(r_value);

	if (c < b) { std::swap(c, b); }

	int k = 0;
	int64_t next = 0;
	int64_t a_ = 0;
	//a[i+j] += c[i]*b[j] (10)
	//[10] int 0 9 (1000000000) 120002000000 = [2000000][12]
	for (int i = 0; i < b.size; ++i) {
		k = i;
		for (int j = 0; j < l_value.size + r_value.size; ++j) {
			a_ = (j < c.size) ? c.num[j] : 0;
			//(unsigned long long)a
			if ((static_cast<unsigned long long>(a_) * b.num[i] + next) >= c.BIG) {
				res.num[k] = static_cast<int64_t>((static_cast<unsigned long long>(a_) * b.num[i]) % c.BIG + next % c.BIG );
				//(static_cast<unsigned long long>(a_) * b.num[i]+next)%2^32
				next = static_cast<int64_t>((static_cast<unsigned long long>(a_) * b.num[i]) / c.BIG);
			
			}
			else {
				res.num[k] = static_cast<int64_t>(a_ * b.num[i] + next);
				next = 0;
			}
			if (k + 1 == l_value.size + r_value.size) {
				break;
			}
			++k;
		}
		
		BigInteger tmp_;
		tmp_ = (result + res);
		for (int i = 0; i < result.size; ++i) { result.num[i] = (i < tmp_.size) ? tmp_.num[i] : 0; }
		delete[]tmp_.num;
		tmp_.num = nullptr;
		
		for (int f = 0; f < res.size; ++f) { res.num[f] = 0; }
	}

	int size_ = l_value.size + r_value.size;
	int i = size_ - 1;

	while (result.num[i] == 0) {
		--size_;
		--i;
	}

	BigInteger RESULT;
	RESULT.size = size_;
	delete[]RESULT.num;
	RESULT.num = nullptr;
	RESULT.num = new int64_t[size_];

	for (int i = 0; i < size_; ++i) {
		RESULT.num[i] = result.num[i];
	}
	if (l_value.sign != r_value.sign) { RESULT.sign = true; }
	delete[]result.num;
	result.num = nullptr;
	return BigInteger(RESULT);
}

/*
BigInteger operator*(const BigInteger& l_value, const BigInteger& r_value) {
	return BigInteger(karatsuba(l_value,r_value));
}
*/

BigInteger& BigInteger::operator*=(const BigInteger& r_value) {
	BigInteger ymp(*this * r_value);
	return *this = ymp;
}

//-------------------------------------------------------------------------------------------
//__________OPERAROR /____&&_____OPERAROR %_________

BigInteger operator/(const BigInteger& l_value, const BigInteger& r_value) {

	BigInteger a(l_value);
	a.sign = false;
	BigInteger b(r_value);
	b.sign = false;
	if (a < b) { return BigInteger(0); }
	std::string a_;
	std::string b_;
	std::string tmp;
	for (int i = 0; i < a.size; ++i) { tmp = a_; a_ = bin(a.num[i]) + tmp; }
	for (int i = 0; i < b.size; ++i) { tmp = b_;  b_ = bin(b.num[i]) + tmp; }
	while (b_.size() != a_.size()) { tmp = b_; b_ = "0" + tmp; }

	std::string accumulator = std::string(a_.size(), '0');
	tmp= b_ + ' ' + a_+' ' + accumulator;
	std::string res = BinaryDIVMOD(tmp);

	BigInteger result;
	result.size =static_cast<int>( res.size() / 32);
	delete[]result.num;
	result.num = nullptr;
	result.num = new int64_t[result.size];

	for (int i = 0; i < result.size; ++i) {
		result.num[i] = 0;
	}
	int j = 0;
	for (int i = static_cast<int>(res.size()); i > 0; i -= 32) {
		result.num[j] = ((i < 32) ? to_binary(res.substr(0, i)) : to_binary(res.substr(i - 32, 32)));
		if (j + 1 < result.size) {
			++j;
		}
	}
	
	int newsize = (result.size);
	int i = (newsize==1)?0:newsize-1;

	if (i != 0) {
		while (i - 1 >= 0 && result.num[i] == 0) {
			--i;
			--newsize;
		}
	}

	BigInteger RESULT;
	RESULT.size = newsize;
	delete[]RESULT.num;
	RESULT.num = nullptr;
	RESULT.num = new int64_t[newsize];
	if (newsize > 0) {
		for (int i = 0; i < newsize; ++i) {
			RESULT.num[i] = 0;
		}
	}

	if (newsize == 1) { RESULT.num[newsize-1] = result.num[newsize-1]; }
	else {
		for (i = 0; i < newsize; ++i) {
			RESULT.num[i] = result.num[i];
		}
	}
	if (l_value.sign != r_value.sign) { RESULT.sign = true; }
	
	return BigInteger(RESULT);
}

BigInteger& BigInteger::operator/=(const BigInteger& r_value) {
	BigInteger ymp(*this / r_value);
	return *this = ymp;
}

//-------------------------------------------------------------------------------------------

BigInteger operator%(const BigInteger& l_value, const BigInteger& r_value) {
	BigInteger res;
	res=l_value - l_value / r_value * r_value;
	return BigInteger(res);
}

BigInteger& BigInteger::operator%=(const BigInteger& r_value) {
	BigInteger ymp;
	ymp=(*this)% r_value;
	return *this = ymp;
}

std::string to_string(const BigInteger& value) {
	std::string value_bin;
	std::string digit = "1";
	std::string tmp;
	for (int i = 0; i < value.size; ++i) {
		tmp = value_bin;
		value_bin = bin(value.num[i]) + tmp;
	}
	std::string res="0";
	int next = 0;
	for (int i = static_cast<int>(value_bin.length() - 1); i >= 0; --i) {
		if (value_bin[i] == '1') {
			res = sum(res, digit);
		}
		digit = digit ^ 2;
	}
	if (value.sign) {
		res = '-' + res;
	}
	return res;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& value) {
	for (int i = 0; i < value.size; ++i) {
		out << " [" << value.num[i] << "] ";
	}
	
	if (value.sign) {
		out << '-';
	}
	else {
		out << '+';
	}
	out << " [size: " << value.size<<"] ";
	return out;
}
