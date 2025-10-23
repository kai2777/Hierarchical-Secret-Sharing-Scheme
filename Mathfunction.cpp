#include "Math.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

using namespace std;

// Randomly generate big odd number n, with n < 2^b
long long GenerateBigOddNumber(int b)
{
    long long n = 0;

    for (int i = 0; i < b / 8 - 1; i++)
    {
        n += rand() % 256;
        n = n << 8;
    }

    n = n << (b % 8 - 1);
    n += rand() % (int)pow(2, 7 + b % 8);

    n = (n << 1) + 1;

    return n;
}

// MultiplicationModular with square multiplication
// Calculate a * b mod n
long long MultiplicationModular(long long a, long long d, long long n)
{
    bool negative = false;
    if (d < 0)
    {
        negative = true;
        d *= -1;
    }

    long long x = a % n, y = 0, temp = 0;

    while (d != 0)
    {
        if (d % 2 == 1)
            y = (y + x) % n;

        d /= 2;

        x = (x + x) % n;
    }

    if (negative)
        y *= -1;

    return y < 0 ? y + n : y;
}

// ExponentiationModular with square multiplication
// Calculate a ^ b mod n
long long ExponentiationModular(long long a, long long d, long long n)
{
    long long x = a % n, y = 1, temp = 0;

    while (d != 0)
    {
        if (d % 2 == 1)
            y = MultiplicationModular(y, x, n);

        d /= 2;

        x = MultiplicationModular(x, x, n);
    }

    return y;
}

// Test if n is prime
bool PrimalityTest(long long n)
{
    // Calculate r, and d, with n = (2^r)*d + 1
    long long r = 0, d = n - 1;
    while (d % 2 == 0)
    {
        d /= 2;
        r++;
    }

    // Test n with a = 2, 325, 9375, 28178, 450775, 9780504
    vector<long long> TestNumber = {2, 325, 9375, 28178, 450775, 9780504};
    for (int i = 0; i < TestNumber.size(); i++)
    {
        // Test if x = a^d = 1 (mod n)
        long long a = TestNumber[i], x = ExponentiationModular(a, d, n);
        if (x != 1 && x != n - 1)
        {
            // Test if x = a^(2^s*d)) = n - 1 (mod n), with 0 <= s <= r - 1
            long long count = 0;
            while (x != n - 1 && count != r - 1)
            {
                x = ExponentiationModular(x, 2, n);
                count++;
            }

            if (count == r - 1 && x != n - 1)
                return false;
        }
    }

    return true;
}

// Randomly generate big prime n, with n < 2^b
long long GeneratePrime(int b)
{
    long long n = 0;

    do
    {
        n = GenerateBigOddNumber(b);
    } while (!PrimalityTest(n));

    return n;
}

// Calculate x, and y, with a * x + b * y = 1, and gcd(a, b)
vector<long long> ExtentionEuclid(long long a, long long b)
{
    if (b == 0)
        return {1, 0, a};

    vector<long long> buffer = ExtentionEuclid(b, a % b);
    long long x = buffer[0], y = buffer[1], gcd = buffer[2];

    long long x_prime = y, y_prime = x - (a / b) * y;

    while (x_prime < 0)
    {
        x_prime += b;
        y_prime -= a;
    }

    return {x_prime, y_prime, gcd};
}

// Using LagrangeInterpolation’s formula to form f(x), and calculate f(0)
long long LagrangeInterpolation(vector<pair<long long, long long>> point, long long Q)
{
    long long answer = 0;

    // Calculate answer of each term
    for (int i = 0; i < point.size(); i++)
    {
        // Calculate numerator, and denominator
        long long temp = 1, numerator = 1, denominator = 1;
        for (int j = 0; j < point.size(); j++)
        {
            if (i == j)
                continue;

            numerator = MultiplicationModular(numerator, point[j].first, Q);
            denominator = MultiplicationModular(denominator, point[i].first - point[j].first, Q);
        }
        denominator = ExtentionEuclid(denominator, Q)[0];

        // Calculate answer
        temp = MultiplicationModular(temp, point[i].second, Q);
        temp = MultiplicationModular(temp, numerator, Q);
        temp = MultiplicationModular(temp, denominator, Q);

        answer = (answer + temp) % Q;
    }

    return answer;
}
