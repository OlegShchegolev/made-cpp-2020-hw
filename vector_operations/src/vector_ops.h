#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <cstdint>

namespace task {

std::vector<double> operator-(const std::vector<double>& v) {
    std::vector<double> result(v);
    for (double& elem : result)
        elem = -elem;
    return result;
}

std::vector<double> operator+(const std::vector<double>& v) {
    std::vector<double> result(v);
    return result;
}

std::vector<double> operator-(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    std::vector<double> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] - rhs[i];
    return result;
}

std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    std::vector<double> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] + rhs[i];
    return result;
}

double operator*(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    double result(0);
    for (size_t i = 0; i < lhs.size(); ++i)
        result += lhs[i] * rhs[i];
    return result;
}

std::vector<double> operator%(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    std::vector<double> result(3);
    result[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    result[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    result[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
    return result;
}

bool operator||(const std::vector<double>& lhs, const std::vector<double>& rhs){
    const double EPS = 1e-8;
    double zero = abs(lhs * rhs) - sqrt(rhs * rhs) * sqrt(lhs * lhs);
    if (zero > -EPS && zero < EPS)
        return true;
    return false;
}

bool operator&&(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    if ((lhs || rhs) && (lhs * rhs) >= 0)
        return true;
    return false;
}

std::istream& operator>>(std::istream& is, std::vector<double>& v) {
    size_t size(0);
    is >> size;
    v.resize(size);
    for (size_t i = 0; i < size; ++i)
        is >> v[i];
    return is;
}

std::ostream& operator<<(std::ostream& os, const std::vector<double>& v) {
    for (size_t i = 0; i < v.size() - 1; ++i)
        os << v[i] << " ";
    os << v[v.size() - 1] << std::endl;
    return os;
}

void reverse(std::vector<double>& v) {
    for (size_t i = 0; i < v.size() / 2; ++i)
        std::swap(v[i], v[v.size() - i - 1]);
}

std::vector<int> operator|(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    std::vector<int> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] | rhs[i];
    return result;
}

std::vector<int> operator&(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    std::vector<int> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i)
        result[i] = lhs[i] & rhs[i];
    return result;
}

}  // namespace task
