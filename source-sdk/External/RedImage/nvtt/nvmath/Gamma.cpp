//
// Fast implementations of powf(x,5/11) and powf(x,11/5) for gamma conversion
// Copyright 2017 Ken Cooke <ken@highfidelity.io>
// 
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Gamma.h"
#include <stdint.h>
#include <limits>

#define INFINITE_RESULT std::numeric_limits<float>::infinity()

//
// pow(2.0, e * 5/11.0) over e=[-127,128]
//
static const float pow_5_11_table[512] = {
    // sign bit = 0
    0.00000000e+00f, 5.74369237e-18f, 7.87087416e-18f, 1.07858603e-17f, 
    1.47804139e-17f, 2.02543544e-17f, 2.77555756e-17f, 3.80348796e-17f, 
    5.21211368e-17f, 7.14242467e-17f, 9.78762916e-17f, 1.34124875e-16f, 
    1.83798156e-16f, 2.51867973e-16f, 3.45147530e-16f, 4.72973245e-16f, 
    6.48139341e-16f, 8.88178420e-16f, 1.21711615e-15f, 1.66787638e-15f, 
    2.28557589e-15f, 3.13204133e-15f, 4.29199599e-15f, 5.88154098e-15f, 
    8.05977514e-15f, 1.10447209e-14f, 1.51351438e-14f, 2.07404589e-14f, 
    2.84217094e-14f, 3.89477167e-14f, 5.33720441e-14f, 7.31384286e-14f, 
    1.00225323e-13f, 1.37343872e-13f, 1.88209311e-13f, 2.57912805e-13f, 
    3.53431070e-13f, 4.84324603e-13f, 6.63694685e-13f, 9.09494702e-13f, 
    1.24632693e-12f, 1.70790541e-12f, 2.34042972e-12f, 3.20721032e-12f, 
    4.39500389e-12f, 6.02269797e-12f, 8.25320975e-12f, 1.13097942e-11f, 
    1.54983873e-11f, 2.12382299e-11f, 2.91038305e-11f, 3.98824619e-11f, 
    5.46529731e-11f, 7.48937509e-11f, 1.02630730e-10f, 1.40640125e-10f, 
    1.92726335e-10f, 2.64102712e-10f, 3.61913416e-10f, 4.95948393e-10f, 
    6.79623358e-10f, 9.31322575e-10f, 1.27623878e-09f, 1.74889514e-09f, 
    2.39660003e-09f, 3.28418337e-09f, 4.50048399e-09f, 6.16724272e-09f, 
    8.45128678e-09f, 1.15812293e-08f, 1.58703486e-08f, 2.17479474e-08f, 
    2.98023224e-08f, 4.08396410e-08f, 5.59646445e-08f, 7.66912009e-08f, 
    1.05093868e-07f, 1.44015488e-07f, 1.97351767e-07f, 2.70441177e-07f, 
    3.70599338e-07f, 5.07851155e-07f, 6.95934318e-07f, 9.53674316e-07f, 
    1.30686851e-06f, 1.79086862e-06f, 2.45411843e-06f, 3.36300377e-06f, 
    4.60849560e-06f, 6.31525654e-06f, 8.65411766e-06f, 1.18591788e-05f, 
    1.62512370e-05f, 2.22698982e-05f, 3.05175781e-05f, 4.18197924e-05f, 
    5.73077959e-05f, 7.85317898e-05f, 1.07616121e-04f, 1.47471859e-04f, 
    2.02088209e-04f, 2.76931765e-04f, 3.79493722e-04f, 5.20039583e-04f, 
    7.12636742e-04f, 9.76562500e-04f, 1.33823336e-03f, 1.83384947e-03f, 
    2.51301727e-03f, 3.44371586e-03f, 4.71909950e-03f, 6.46682270e-03f, 
    8.86181649e-03f, 1.21437991e-02f, 1.66412666e-02f, 2.28043757e-02f, 
    3.12500000e-02f, 4.28234674e-02f, 5.86831830e-02f, 8.04165527e-02f, 
    1.10198908e-01f, 1.51011184e-01f, 2.06938326e-01f, 2.83578128e-01f, 
    3.88601571e-01f, 5.32520533e-01f, 7.29740024e-01f, 1.00000000e+00f, 
    1.37035096e+00f, 1.87786186e+00f, 2.57332969e+00f, 3.52636504e+00f, 
    4.83235788e+00f, 6.62202644e+00f, 9.07450008e+00f, 1.24352503e+01f, 
    1.70406570e+01f, 2.33516808e+01f, 3.20000000e+01f, 4.38512306e+01f, 
    6.00915794e+01f, 8.23465500e+01f, 1.12843681e+02f, 1.54635452e+02f, 
    2.11904846e+02f, 2.90384003e+02f, 3.97928009e+02f, 5.45301025e+02f, 
    7.47253784e+02f, 1.02400000e+03f, 1.40323938e+03f, 1.92293054e+03f, 
    2.63508960e+03f, 3.61099780e+03f, 4.94833447e+03f, 6.78095508e+03f, 
    9.29228809e+03f, 1.27336963e+04f, 1.74496328e+04f, 2.39121211e+04f, 
    3.27680000e+04f, 4.49036602e+04f, 6.15337773e+04f, 8.43228672e+04f, 
    1.15551930e+05f, 1.58346703e+05f, 2.16990563e+05f, 2.97353219e+05f, 
    4.07478281e+05f, 5.58388250e+05f, 7.65187875e+05f, 1.04857600e+06f, 
    1.43691713e+06f, 1.96908088e+06f, 2.69833175e+06f, 3.69766175e+06f, 
    5.06709450e+06f, 6.94369800e+06f, 9.51530300e+06f, 1.30393050e+07f, 
    1.78684240e+07f, 2.44860120e+07f, 3.35544320e+07f, 4.59813480e+07f, 
    6.30105880e+07f, 8.63466160e+07f, 1.18325176e+08f, 1.62147024e+08f, 
    2.22198336e+08f, 3.04489696e+08f, 4.17257760e+08f, 5.71789568e+08f, 
    7.83552384e+08f, 1.07374182e+09f, 1.47140314e+09f, 2.01633882e+09f, 
    2.76309171e+09f, 3.78640563e+09f, 5.18870477e+09f, 7.11034675e+09f, 
    9.74367027e+09f, 1.33522483e+10f, 1.82972662e+10f, 2.50736763e+10f, 
    3.43597384e+10f, 4.70849004e+10f, 6.45228421e+10f, 8.84189348e+10f, 
    1.21164980e+11f, 1.66038553e+11f, 2.27531096e+11f, 3.11797449e+11f, 
    4.27271946e+11f, 5.85512518e+11f, 8.02357641e+11f, 1.09951163e+12f, 
    1.50671681e+12f, 2.06473095e+12f, 2.82940591e+12f, 3.87727937e+12f, 
    5.31323368e+12f, 7.28099507e+12f, 9.97751836e+12f, 1.36727023e+13f, 
    1.87364006e+13f, 2.56754445e+13f, 3.51843721e+13f, 4.82149380e+13f, 
    6.60713903e+13f, 9.05409892e+13f, 1.24072940e+14f, 1.70023478e+14f, 
    2.32991842e+14f, 3.19280587e+14f, 4.37526473e+14f, 5.99564818e+14f, 
    8.21614225e+14f, 1.12589991e+15f, 1.54287801e+15f, 2.11428449e+15f, 
    2.89731166e+15f, 3.97033407e+15f, 5.44075129e+15f, 7.45573896e+15f, 
    1.02169788e+16f, 1.40008471e+16f, 1.91860742e+16f, 2.62916552e+16f, 
    3.60287970e+16f, 4.93720965e+16f, 6.76571037e+16f, 9.27139730e+16f, 
    1.27050690e+17f, 1.74104041e+17f, 2.38583647e+17f, INFINITE_RESULT, 
    // sign bit = 1
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
};

//
// pow(2.0, e * 11/5.0) over e=[-127,128]
//
static const float pow_11_5_table[512] = {
    // sign bit = 0
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 1.40129846e-45f, 
    4.20389539e-45f, 1.96181785e-44f, 8.96831017e-44f, 4.11981749e-43f, 
    1.89315423e-42f, 8.69926087e-42f, 3.99734400e-41f, 1.83670992e-40f, 
    8.43930599e-40f, 3.87768572e-39f, 1.78171625e-38f, 8.18661824e-38f, 
    3.76158192e-37f, 1.72836915e-36f, 7.94149964e-36f, 3.64895487e-35f, 
    1.67661942e-34f, 7.70371978e-34f, 3.53970002e-33f, 1.62641913e-32f, 
    7.47305957e-32f, 3.43371656e-31f, 1.57772181e-30f, 7.24930563e-30f, 
    3.33090637e-29f, 1.53048260e-28f, 7.03225152e-28f, 3.23117427e-27f, 
    1.48465779e-26f, 6.82169625e-26f, 3.13442837e-25f, 1.44020511e-24f, 
    6.61744490e-24f, 3.04057916e-23f, 1.39708339e-22f, 6.41930929e-22f, 
    2.94954007e-21f, 1.35525272e-20f, 6.22710612e-20f, 2.86122679e-19f, 
    1.31467454e-18f, 6.04065806e-18f, 2.77555756e-17f, 1.27531133e-16f, 
    5.85979246e-16f, 2.69245347e-15f, 1.23712677e-14f, 5.68434189e-14f, 
    2.61183761e-13f, 1.20008550e-12f, 5.51414470e-12f, 2.53363563e-11f, 
    1.16415322e-10f, 5.34904343e-10f, 2.45777509e-09f, 1.12929683e-08f, 
    5.18888577e-08f, 2.38418579e-07f, 1.09548409e-06f, 5.03352339e-06f, 
    2.31279992e-05f, 1.06268380e-04f, 4.88281250e-04f, 2.24355143e-03f, 
    1.03086559e-02f, 4.73661423e-02f, 2.17637643e-01f, 1.00000000e+00f, 
    4.59479332e+00f, 2.11121273e+01f, 9.70058594e+01f, 4.45721893e+02f, 
    2.04800000e+03f, 9.41013672e+03f, 4.32376367e+04f, 1.98668000e+05f, 
    9.12838438e+05f, 4.19430400e+06f, 1.92719600e+07f, 8.85506800e+07f, 
    4.06872064e+08f, 1.86949312e+09f, 8.58993459e+09f, 3.94689741e+10f, 
    1.81351793e+11f, 8.33273987e+11f, 3.82872191e+12f, 1.75921860e+13f, 
    8.08324589e+13f, 3.71408471e+14f, 1.70654513e+15f, 7.84122247e+15f, 
    3.60287970e+16f, 1.65544876e+17f, 7.60644549e+17f, 3.49500442e+18f, 
    1.60588236e+19f, 7.37869763e+19f, 3.39035906e+20f, 1.55780004e+21f, 
    7.15776905e+21f, 3.28884708e+22f, 1.51115727e+23f, 6.94345535e+23f, 
    3.19037448e+24f, 1.46591110e+25f, 6.73555881e+25f, 3.09485010e+26f, 
    1.42201966e+27f, 6.53388693e+27f, 3.00218593e+28f, 1.37944245e+29f, 
    6.33825300e+29f, 2.91229625e+30f, 1.33814004e+31f, 6.14847679e+31f, 
    2.82509813e+32f, 1.29807421e+33f, 5.96438273e+33f, 2.74051081e+34f, 
    1.25920805e+35f, 5.78580097e+35f, 2.65845599e+36f, 1.22150558e+37f, 
    5.61256613e+37f, 2.57885808e+38f, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, INFINITE_RESULT, 
    // sign bit = 1
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
    0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 0.00000000e+00f, 
};

//
// powf(x, 5/11.0f)
//
// for x = +inf or NaN, returns +inf
// for x = subnormal or 0.0f, returns 0.0f
// for x < 0.0f, returns 0.0f
//
// rel |error| < 1.2e-5, smooth
//
static inline float _powf_5_11(float x) {

    union { float f; uint32_t u; } m = { x };

    // split into mantissa and exponent
    int k = m.u >> 23;                              // [sign|exponent] bits
    m.u = (m.u & ((1 << 23) - 1)) | (127 << 23);    // mantissa with zero exponent

    // pow(2, e * 5/11) from table
    float pow_e = pow_5_11_table[k];

    // polynomial for pow(m, 5/11) over m=[1,2)
    float pow_m = (((-0.0110083047f * m.f + 0.0905038750f) * m.f - 0.324697506f) * m.f + 0.876040946f) * m.f + 0.369160989f;

    // recontruct the result
    return pow_e * pow_m;
}

//
// powf(x, 11/5.0f)
//
// for x = +inf or NaN, returns +inf
// for x = subnormal or 0.0f, returns 0.0f
// for x < 0.0f, returns 0.0f
//
// rel |error| < 2.9e-6, smooth
//
static inline float _powf_11_5(float x) {

    union { float f; uint32_t u; } m = { x };

    // split into mantissa and exponent
    int k = m.u >> 23;                              // [sign|exponent] bits
    m.u = (m.u & ((1 << 23) - 1)) | (127 << 23);    // mantissa with zero exponent

    // pow(2, e * 11/5) from table
    float pow_e = pow_11_5_table[k];

    // polynomial for pow(m, 11/5) over m=[1,2)
    float pow_m = (((-0.00916587552f * m.f + 0.119315466f) * m.f + 1.01847068f) * m.f - 0.158338739f) * m.f + 0.0297184721f;

    // recontruct the result
    return pow_e * pow_m;
}

#if (NV_USE_SSE > 1)
#include <emmintrin.h> // SSE2

void nv::powf_5_11(const float* src, float* dst, int count) {

    int i = 0;
    for (; i < count - 3; i += 4) {

        __m128 x = _mm_loadu_ps(&src[i]);

        // split into mantissa and exponent
        __m128i k = _mm_srli_epi32(_mm_castps_si128(x), 23);
        x = _mm_and_ps(x, _mm_castsi128_ps(_mm_set1_epi32((1 << 23) - 1)));
        x = _mm_or_ps(x, _mm_castsi128_ps(_mm_set1_epi32(127 << 23)));

        // pow(2, e * 5/11) from table
        __m128 pow_e = _mm_setr_ps(
            pow_5_11_table[_mm_cvtsi128_si32(k)],
            pow_5_11_table[_mm_extract_epi16(k, 2)],
            pow_5_11_table[_mm_extract_epi16(k, 4)],
            pow_5_11_table[_mm_extract_epi16(k, 6)]
        );

        // polynomial for pow(m, 5/11) over m=[1,2)
        __m128 pow_m = _mm_set1_ps(-0.0110083047f);
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(0.0905038750f));
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(-0.324697506f));
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(0.876040946f));
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(0.369160989f));

        // recontruct the result
        _mm_storeu_ps(&dst[i], _mm_mul_ps(pow_e, pow_m));
    }

    for (; i < count; i++) {
        dst[i] = _powf_5_11(src[i]);
    }
}

void nv::powf_11_5(const float* src, float* dst, int count) {

    int i = 0;
    for (; i < count - 3; i += 4) {

        __m128 x = _mm_loadu_ps(&src[i]);

        // split into mantissa and exponent
        __m128i k = _mm_srli_epi32(_mm_castps_si128(x), 23);
        x = _mm_and_ps(x, _mm_castsi128_ps(_mm_set1_epi32((1 << 23) - 1)));
        x = _mm_or_ps(x, _mm_castsi128_ps(_mm_set1_epi32(127 << 23)));

        // pow(2, e * 11/5) from table
        __m128 pow_e = _mm_setr_ps(
            pow_11_5_table[_mm_cvtsi128_si32(k)],
            pow_11_5_table[_mm_extract_epi16(k, 2)],
            pow_11_5_table[_mm_extract_epi16(k, 4)],
            pow_11_5_table[_mm_extract_epi16(k, 6)]
        );

        // polynomial for pow(m, 11/5) over m=[1,2)
        __m128 pow_m = _mm_set1_ps(-0.00916587552f);
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(0.119315466f));
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(1.01847068f));
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(-0.158338739f));
        pow_m = _mm_add_ps(_mm_mul_ps(pow_m, x), _mm_set1_ps(0.0297184721f));

        // recontruct the result
        _mm_storeu_ps(&dst[i], _mm_mul_ps(pow_e, pow_m));
    }

    for (; i < count; i++) {
        dst[i] = _powf_11_5(src[i]);
    }
}

#else

void nv::powf_5_11(const float* src, float* dst, int count) {
    for (int i = 0; i < count; i++) {
        dst[i] = _powf_5_11(src[i]);
    }
}
void nv::powf_11_5(const float* src, float* dst, int count) {
    for (int i = 0; i < count; i++) {
        dst[i] = _powf_11_5(src[i]);
    }
}

#endif // SSE2
