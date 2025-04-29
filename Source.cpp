#include "biginteger.h"

std::string sum(const std::string &s1,const std::string &s2) {
	std::string a = (s1.length() < s2.length()) ? s2 : s1;
	std::string b = (s1.length() > s2.length()) ? s2 : s1;
	std::string tmp;
	a = (s1.length() == s2.length()) ? s1 : a;
	b = (s1.length() == s2.length()) ? s2 : b;
	while (b.length() != a.length()) { tmp = b;  b = "0"+tmp; }
	std::string result;
	int next = 0;

	int i = static_cast<int>(a.length()-1);
	while (next!=0 || i >= 0) {
		
		if (next!=0 && i < 0) {
			tmp = static_cast<char>(next + '0') + result;
			result = tmp;
			next = 0;
		}
		else if (a[i]-'0' + b[i] -'0' + next >= 10) {
			tmp = static_cast<char>((a[i] - '0' + b[i] - '0' + next) % 10 + '0') + result;
			result =tmp;
			next = (a[i] - '0' + b[i] - '0' + next) / 10;
		}
		else {
			tmp = static_cast<char>((a[i] - '0' + b[i] - '0' + next) + '0') + result;
			result = tmp;
			next = 0;
		}
		--i;
	}

	return result;
}

std::string operator^(const std::string& value, int x) {
	std::string result;
	std::string tmp;
	int next = 0;
	int i = static_cast<int>(value.length() - 1);
	while (next != 0 ||i>=0) {
		if (next != 0 && i < 0) {
			tmp = static_cast<char>((next)+'0') + result;
			result = tmp;
			next = 0;
		}
		else if (value[i] * x + next >= 10) {
			tmp = static_cast<char>(((value[i] - '0') * x + next) % 10 + '0') + result;
			result = tmp;
			next = ((value[i] - '0') * x + next) / 10;
		}
		else {
			tmp = static_cast<char>(((value[i] - '0') * x + next) + '0') + result;
			result = tmp;
			next = 0;
		}
		--i;
	}
	return result;
}

std::string add(std::string A, std::string M)
{
	int carry = 0;
	std::string Sum;
	for (int i = static_cast<int>(A.size() - 1); i >= 0; --i) {
		int temp = (A[i] - '0') + (M[i] - '0') + carry;

		if (temp > 1) {
			Sum += std::to_string(temp % 2);
			carry = 1;
		}
		else {
			Sum += std::to_string(temp);
			carry = 0;
		}
	}

	return std::string(Sum.rbegin(), Sum.rend());
}

std::string compliment(std::string m)
{
	std::string M; // Iterating through the number
	for (int i = 0; i < m.length(); i++) {M += std::to_string((m[i] - '0' + 1) % 2);}

	std::string tmp = std::string(M.size()-1, '0') + '1';

	M = add(M, tmp);
	return std::string(M);
}

std::string BinaryDIVMOD(const std::string& tmp)
{
	std::string M;
	std::string Q;
	std::string A;
	int i = 0;
	while (tmp[i] != ' ') { M += tmp[i]; ++i; }
	++i;
	while (tmp[i] != ' ') { Q += tmp[i]; ++i; }
	++i;
	while (i < static_cast<int>(tmp.size())) { A += tmp[i]; ++i; }


	int count = static_cast<int>(M.length());
	std::string comp_M = compliment(M);
	bool flag = true;
	while (count!=0) {
		A = A.substr(1) + Q[0];
		if (flag) {
			A = add(A, comp_M);
		}
		else {
			A = add(A, M);
		}

		if (A[0] == '1') {
			Q = Q.substr(1) + "0";
			flag =false;
		}
		else {
			Q = Q.substr(1) + "1";
			flag = true;
		}
		count--;
	}

	return Q;
}


void BigInteger::get_sign(const std::string& num) {
	int i = 0;
	while (num[i] == ' ') { ++i; }
	(num[i] == '-') ? this->sign = true : this->sign = false;
	if (num[i] == '0') { this->sign = false; }
}


std::string bin(int64_t num_) {
	std::string res;
	std::string tmp;
	while (num_ != 0) {
		tmp = std::to_string(num_ % 2) + res;
		res = tmp;
		num_ /= 2;
	}

	while (res.size() != 32) { tmp = "0" + res;  res = tmp; }
	return std::string(res);
}

std::string conv_binary(std::string num_) {
	std::string copy_num_ = std::move(num_);
	std::string result;

	do {
		result =static_cast<char>((copy_num_[copy_num_.length() - 1] - '0') % 2 + '0') + result;
		copy_num_ = division_by_(copy_num_,2);
	} while (copy_num_ != "0");
	return std::string(result);
}

int64_t to_binary(std::string num_) {
	unsigned int result = 0;
	int degree = 1;

	for (int i = static_cast<int>(num_.length() - 1); i >= 0; --i) {
		result += (num_[i] - '0') * degree;
		degree *= 2;
	}
	return result;
}

std::string division_by_(std::string num_,int by_) {
	std::string result;
	int tmp = (num_[0]-'0');
	for (int i = 0; i < num_.length(); ++i) {
		if (tmp < 2 && i != num_.length()-1) {
			tmp = (num_[i + 1] != '0')?(num_[i + 1] - '0') + tmp * 10:tmp*10;
			result += '0';
		}
		else {
			result += static_cast<char>(tmp / 2 + '0');
			tmp =tmp % 2;
			tmp=(i != num_.length() - 1)?num_[i + 1] - '0' + tmp * 10: tmp;
		}
	}
	int i = 0;
	while (result[i] == '0' && i < result.length() - 1) {
		++i;
	}
	return (result == "0" || result.empty()) ? "0" : result.substr(i);

}
