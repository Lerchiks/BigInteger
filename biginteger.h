#ifndef BigInt
#define BigInt
#include <iostream>
#include <string>
#include <fstream>
#include "sources.h"

class BigInteger {
    
    bool sign; //sign = false means num>=0 && sign == true means num<0 
    int size;//size without sigh. only digits
    int64_t* num = nullptr;
    const int64_t BIG = 4294967296;//4294967296;//(2^32)

public:
    //int64_t 
    ~BigInteger();
    BigInteger(); //construct default
    BigInteger(long long num_); //construct 
    explicit BigInteger(const std::string& num_); //construct string
    BigInteger(const BigInteger& value); //copy construct
    BigInteger(BigInteger&& value) noexcept; //move construct
    friend std::string to_string(const BigInteger& value);
    //incr decr
    BigInteger& operator++();//+
    BigInteger operator++(int);//+
    BigInteger& operator--();//+
    BigInteger operator--(int);//+

    //unr
    BigInteger operator-() const;//+
    BigInteger operator+() const;//+


    BigInteger& operator=(BigInteger&& value)noexcept; //+
    BigInteger& operator=(BigInteger& value); //+

    //operators+-*/%
    friend BigInteger operator+(const BigInteger& l_value, const BigInteger& r_value);//+
    friend BigInteger operator-(const BigInteger& l_value, const BigInteger& r_value);//+
    friend BigInteger operator*(const BigInteger& l_value, const BigInteger& r_value);//+
    friend BigInteger operator/(const BigInteger& l_value, const BigInteger& r_value);//+
    friend BigInteger operator%(const BigInteger& l_value, const BigInteger& r_value);//+
    friend std::ostream& operator<<(std::ostream& out, const BigInteger& value);

    //operators +=//-=//*=// /=/%=/
    BigInteger& operator+=(const BigInteger& r_value);//+
    BigInteger& operator-=(const BigInteger& r_value);//+
    BigInteger& operator*=(const BigInteger& r_value);//+
    BigInteger& operator/=(const BigInteger& r_value);//+
    BigInteger& operator%=(const BigInteger& r_value);//+

    //bool func
    friend bool operator==(const BigInteger& l_value, const BigInteger& r_value); //+
    friend bool operator!=(const BigInteger& l_value, const BigInteger& r_value); //+
    friend bool operator>(const BigInteger& l_value, const BigInteger& r_value); //+
    friend bool operator<(const BigInteger& l_value, const BigInteger& r_value); //+
    friend bool operator>=(const BigInteger& l_value, const BigInteger& r_value); //+
    friend bool operator<=(const BigInteger& l_value, const BigInteger& r_value); //+
    //friend BigInteger karatsuba(const BigInteger& a, const BigInteger& b);
    
private:
    void get_sign(const std::string& num_);

};

std::string conv_binary(std::string num_);
int64_t to_binary(std::string num_);
std::string division_by_(std::string num_, int by_);
std::string bin(int64_t num_);
std::string add(std::string A, std::string M);
std::string compliment(std::string m);
std::string BinaryDIVMOD(const std::string& tmp);
std::string operator^(const std::string& value, int x);
std::string sum(const std::string& s1, const std::string& s2);
#endif
