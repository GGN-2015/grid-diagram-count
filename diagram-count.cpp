#include <algorithm>
#include <cstdio>
#include <cassert>
using namespace std;

const int maxn = 20;
const int maxk = 10 + 1;

// 砖瓦的种类

// 空白：0, 0 -> 0, 0

// 左上：1, 1 -> 0, 0
// 右上：0, 1 -> 0, 1
// 左下：1, 0 -> 1, 0
// 右下：0, 0 -> 1, 1

// 水平：1, 0 -> 0, 1
// 竖直：0, 1 -> 1, 0
// 交叉：1, 1 -> 1, 1 （交叉点数量 + 1）

typedef long long i64;
typedef unsigned int u32;

int DEBUG = 0;
i64 dp[maxn + 1][maxn + 1][1<<maxn][maxk + 1]; // 第 i 行第 j 列填写前，轮廓线状态为 S，交叉点个数为 k 的方案数

int main() {
    int n, m; scanf("%d%d", &n, &m); // 计算 n 行 m 列的 grid diagram 总共有多少种
    assert(1 <= n && n <= maxn - 2);
    assert(1 <= m && m <= maxn - 2);
    dp[0][0][0][0] = 1; // 要求左上角的状态必须是“左上”而不能是空白
    for(int i = 0; i < n; i += 1) {
        // 行内转移
        for(int j = 0; j < m; j += 1) { // 刷表法
            for(int S = 0; S < (1 << (m + 1)); S += 1) {
                for(int k = 0; k < maxk; k += 1) { // 总交叉点个数
                    if(dp[i][j][S][k]) {
                        if(DEBUG) printf("dp[%d][%d][%d][%d] = %llu\n", i, j, S, k, dp[i][j][S][k]);
                    }else {
                        continue;
                    }
                    int status_two_bits = (S >> j) & 3; // 最低两位
                    switch (status_two_bits)
                    {
                    case 0: // 0, 0：可以使用空白以及右下
                        dp[i][j+1][S][k]            += dp[i][j][S][k]; // 使用空白：0, 0 -> 0, 0
                        dp[i][j+1][S ^ (3 << j)][k] += dp[i][j][S][k]; // 使用右下：0, 0 -> 1, 1
                        break;
                    
                    case 1: // 1, 0：可以使用左下以及水平
                        dp[i][j+1][S][k]            += dp[i][j][S][k]; // 使用左下：1, 0 -> 1, 0
                        dp[i][j+1][S ^ (3 << j)][k] += dp[i][j][S][k]; // 使用水平：1, 0 -> 0, 1
                        break;

                    case 2: // 0, 1：可以使用右上以及竖直
                        dp[i][j+1][S][k]            += dp[i][j][S][k]; // 使用右上：0, 1 -> 0, 1
                        dp[i][j+1][S ^ (3 << j)][k] += dp[i][j][S][k]; // 使用竖直：0, 1 -> 1, 0
                        break;

                    case 3: // 1, 1：可以使用左上或者交叉
                        dp[i][j+1][S ^ (3 << j)][k] += dp[i][j][S][k]; // 左上：1, 1 -> 0, 0
                        dp[i][j+1][S][k+1]          += dp[i][j][S][k]; // 交叉：1, 1 -> 1, 1 增加一个交叉点
                        break;

                    default:
                        printf("status_two_bits = %d\n", status_two_bits);
                        assert(false);
                        break;
                    }
                }
            }
        }
        // 行间转移
        if(i <= n-2) { // 说明 i + 1 至少还有一行
            int j = m;
            for(int S = 0; S < (1 << m); S += 1) { // 这里我们要求 S < (1 << m)也就是最高位一定为 0
                for(int k = 0; k < maxk; k += 1) {
                    if(dp[i][j][S][k]) {
                        if(DEBUG) printf("dp[%d][%d][%d][%d] = %llu\n", i, j, S, k, dp[i][j][S][k]);
                    }else {
                        continue;
                    }
                    int nxtS = S << 1; // 在开头填充一个 0
                    dp[i+1][0][nxtS][k] = dp[i][j][S][k];
                }
            }
        }
    }
    for(int k = 0; k <= std::min(maxk, n * m); k += 1) {
        printf("dp[%d][%d][%d][%d] = %llu\n", n-1, m, 0, k, dp[n-1][m][0][k]);
    }
    return 0;
}