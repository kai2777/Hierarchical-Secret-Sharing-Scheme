#include "MathFunction.cpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
    // Initiallize
    vector<long long> P = {110321006, 110321014, 110321015,
                           110321018, 110321065, 110321066,
                           110321070, 110321072, 110352028,
                           111214004, 111321002, 111321008,
                           111321028, 111321030, 111321063,
                           111321067, 111321073, 111321074,
                           112321541, 113321550};

    long long Q = 6567629137;

    long long K = 107321049;

    vector<long long> R = {4787, 1439, 2693, 863, 3593, 1019, 2281, 3323, 4373, 4783, 2477};

    vector<vector<long long>> A = {{110321065, 110321066, 110321070, 110321072, 110352028, 111214004},
                                   {111321008, 111321028, 111321030, 111321063, 111321067}};

    // Distribution
    vector<long long> V;

    int count = 0;
    for (int i = 0; i < A.size(); i++)
    {
        // Calculate f_Ai(0)
        vector<pair<long long, long long>> point;

        for (int j = 0; j < A[i].size(); j++)
        {
            point.push_back(pair<long long, long long>{A[i][j], R[count]});
            count++;
        }

        // Calculate V_Ai, V_Ai = K - f_Ai(0)
        long long answer = (K - LagrangeInterpolation(point, Q)) % Q;
        V.push_back(answer < 0 ? answer + Q : answer);
    }

    // Reconstruction
    count = 0;
    for (int i = 0; i < A.size(); i++)
    {
        // Calculate f_Ai(0)
        vector<pair<long long, long long>> point;

        for (int j = 0; j < A[i].size(); j++)
        {
            point.push_back(pair<long long, long long>{A[i][j], R[count]});
            count++;
        }

        // Calculate K, K = V_Ai + f_Ai(0)
        long long answer = LagrangeInterpolation(point, Q);
        answer = (answer + V[i]) % Q;

        cout << answer << '\n';
    }

    return 0;
}
