#include "fft.h"

cFFT::cFFT(unsigned int N, bool I) : N(N), invers_FFT(I), reversed(0), T(0), pi2(2 * M_PI) {
    _c = 0;

    log_2_N = unsigned int(log(N) / log(2));

    reversed = new unsigned int[N];     // 预反转
    for (unsigned int i = 0; i < N; i++) reversed[i] = reverse(i);

    int pow2 = 1;
    T = new std::complex<float> *[log_2_N];      // 预处理T（所有W值）
    for (unsigned int i = 0; i < log_2_N; i++) {
        T[i] = new std::complex<float>[pow2];
        for (int j = 0; j < pow2; j++)
            if (invers_FFT)
                T[i][j] = conj(t(j, pow2 * 2));//复数的模为1时倒数等于共轭
            else
                T[i][j] = t(j, pow2 * 2);
        pow2 *= 2;
    }

    _c = new std::complex<float>[N];
}

cFFT::~cFFT() {
    //if (c) delete[] c;
    if (_c) delete[] _c;
    if (T) {
        for (unsigned int i = 0; i < log_2_N; i++) 
            if (T[i]) 
                delete[] T[i];
        delete[] T;
    }
    if (reversed) delete[] reversed;
}

unsigned int cFFT::reverse(unsigned int i) {
    unsigned int res = 0;
    for (unsigned int j = 0; j < log_2_N; j++) {
        //反序
        if ((i >> j) & 1) res |= (1 << (log_2_N - j - 1));
    }
    return res;
}

std::complex<float> cFFT::t(unsigned int x, unsigned int N) {
    return std::complex<float>(cos(-1 * pi2 * x / N), sin(-1 * pi2 * x / N));
}

void cFFT::fft(std::complex<float>* input, int stride, int offset) {
    //获取反转后的数据放入c数组
    for (unsigned int i = 0; i < N; i++) _c[i] = input[reversed[i] * stride + offset];

    int w_ = 0;//执行第一次循环次数

    for (unsigned int l = 2; l <= N; l *= 2) {
        int m = l / 2;
        for (std::complex<float>* p = _c; p != _c + N; p += l) {
            for (int i = 0; i < m; i++) {
                //蝴蝶变换（W在构造函数中计算过保存在T中）
                std::complex<float> tW = T[w_][i] * p[i + m];
                p[i + m] = p[i] - tW;
                p[i] = p[i] + tW;
            }
        }
        w_++;
    }
    for (unsigned int i = 0; i < N; i++) {
        input[i * stride + offset] = _c[i];
    }
}