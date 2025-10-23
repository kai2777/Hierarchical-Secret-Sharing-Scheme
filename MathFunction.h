#include <iostream>
#include <vector>

using namespace std;

// Randomly generate big number
long long GenerateBigOddNumber(int b);

// MultiplicationModular with square multiplication
// Calculate a * b mod n
long long MultiplicationModular(long long a, long long d, long long n);

// ExponentiationModular with square multiplication
// Calculate a ^ b mod n
long long ExponentiationModular(long long a, long long d, long long n);

// Test if n is prime
bool PrimalityTest(long long n);

// Randomly generate big prime
long long GeneratePrime(int b);

// Get gcd and inverse of a, and b
long long ExtentionEuclid(long long a, long long b);

// Using LagrangeInterpolation to form f(x), and calculate f(0)
long long LagrangeInterpolation(vector<pair<long long, long long>> point, long long Q);
