/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2019 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Simd/SimdMemory.h"
#include "Simd/SimdStore.h"
#include "Simd/SimdWinograd.h"
#include "Simd/SimdBase.h"
#include "Simd/SimdSet.h"

namespace Simd
{
#ifdef SIMD_NEON_ENABLE  
    namespace Neon
    {
        SIMD_INLINE void Load4(const float * src, size_t step, float32x4_t * dst)
        {
            float32x4_t a0 = Load<false>(src + 0 * step);
            float32x4_t a1 = Load<false>(src + 1 * step);
            float32x4_t a2 = Load<false>(src + 2 * step);
            float32x4_t a3 = Load<false>(src + 3 * step);
            float32x4x2_t b0 = vzipq_f32(a0, a2);
            float32x4x2_t b1 = vzipq_f32(a1, a3);
            *(float32x4x2_t*)(dst + 0) = vzipq_f32(b0.val[0], b1.val[0]);
            *(float32x4x2_t*)(dst + 2) = vzipq_f32(b0.val[1], b1.val[1]);
        }

        SIMD_INLINE void Winograd2x3SetFilter4n(const float * src, float * dst, size_t stride)
        {
            const float32x4_t r2 = vdupq_n_f32(1.0f / 2.0f);
            const float32x4_t r4 = vdupq_n_f32(1.0f / 4.0f);

            float32x4_t s[9];
            Load4(src + 0, 9, s + 0);
            Load4(src + 4, 9, s + 4);
            s[8] = SetF32(src[8], src[17], src[26], src[35]);

            Store<false>(dst + 0 * stride, s[0]);
            float32x4_t _0a2 = vaddq_f32(s[0], s[2]);
            Store<false>(dst + 1 * stride, vmulq_f32(vaddq_f32(_0a2, s[1]), r2));
            Store<false>(dst + 2 * stride, vmulq_f32(vsubq_f32(_0a2, s[1]), r2));
            Store<false>(dst + 3 * stride, s[2]);

            float32x4_t _0a6a3 = vaddq_f32(vaddq_f32(s[0], s[6]), s[3]);
            Store<false>(dst + 4 * stride, vmulq_f32(_0a6a3, r2));
            float32x4_t _2a8a5 = vaddq_f32(vaddq_f32(s[2], s[8]), s[5]);
            float32x4_t _1a7a4 = vaddq_f32(vaddq_f32(s[1], s[7]), s[4]);
            Store<false>(dst + 5 * stride, vmulq_f32(vaddq_f32(vaddq_f32(_0a6a3, _2a8a5), _1a7a4), r4));
            Store<false>(dst + 6 * stride, vmulq_f32(vsubq_f32(vaddq_f32(_0a6a3, _2a8a5), _1a7a4), r4));
            Store<false>(dst + 7 * stride, vmulq_f32(_2a8a5, r2));

            float32x4_t _0a6s3 = vsubq_f32(vaddq_f32(s[0], s[6]), s[3]);
            Store<false>(dst + 8 * stride, vmulq_f32(_0a6s3, r2));
            float32x4_t _2a8s5 = vsubq_f32(vaddq_f32(s[2], s[8]), s[5]);
            float32x4_t _1a7s4 = vsubq_f32(vaddq_f32(s[1], s[7]), s[4]);
            Store<false>(dst + 9 * stride, vmulq_f32(vaddq_f32(vaddq_f32(_0a6s3, _2a8s5), _1a7s4), r4));
            Store<false>(dst + 10 * stride, vmulq_f32(vsubq_f32(vaddq_f32(_0a6s3, _2a8s5), _1a7s4), r4));
            Store<false>(dst + 11 * stride, vmulq_f32(_2a8s5, r2));

            Store<false>(dst + 12 * stride, s[6]);
            float32x4_t _6a8 = vaddq_f32(s[6], s[8]);
            Store<false>(dst + 13 * stride, vmulq_f32(vaddq_f32(_6a8, s[7]), r2));
            Store<false>(dst + 14 * stride, vmulq_f32(vsubq_f32(_6a8, s[7]), r2));
            Store<false>(dst + 15 * stride, s[8]);
        }

        SIMD_INLINE void Winograd2x3SetFilter4t(const float * src, float * dst, size_t stride)
        {
            const float32x4_t r2 = vdupq_n_f32(1.0f / 2.0f);
            const float32x4_t r4 = vdupq_n_f32(1.0f / 4.0f);

            float32x4_t s[9];
            s[0] = Load<false>(src + 0 * stride);
            s[1] = Load<false>(src + 1 * stride);
            s[2] = Load<false>(src + 2 * stride);
            s[3] = Load<false>(src + 3 * stride);
            s[4] = Load<false>(src + 4 * stride);
            s[5] = Load<false>(src + 5 * stride);
            s[6] = Load<false>(src + 6 * stride);
            s[7] = Load<false>(src + 7 * stride);
            s[8] = Load<false>(src + 8 * stride);

            Store<false>(dst + 0 * stride, s[0]);
            float32x4_t _0a2 = vaddq_f32(s[0], s[2]);
            Store<false>(dst + 1 * stride, vmulq_f32(vaddq_f32(_0a2, s[1]), r2));
            Store<false>(dst + 2 * stride, vmulq_f32(vsubq_f32(_0a2, s[1]), r2));
            Store<false>(dst + 3 * stride, s[2]);

            float32x4_t _0a6a3 = vaddq_f32(vaddq_f32(s[0], s[6]), s[3]);
            Store<false>(dst + 4 * stride, vmulq_f32(_0a6a3, r2));
            float32x4_t _2a8a5 = vaddq_f32(vaddq_f32(s[2], s[8]), s[5]);
            float32x4_t _1a7a4 = vaddq_f32(vaddq_f32(s[1], s[7]), s[4]);
            Store<false>(dst + 5 * stride, vmulq_f32(vaddq_f32(vaddq_f32(_0a6a3, _2a8a5), _1a7a4), r4));
            Store<false>(dst + 6 * stride, vmulq_f32(vsubq_f32(vaddq_f32(_0a6a3, _2a8a5), _1a7a4), r4));
            Store<false>(dst + 7 * stride, vmulq_f32(_2a8a5, r2));

            float32x4_t _0a6s3 = vsubq_f32(vaddq_f32(s[0], s[6]), s[3]);
            Store<false>(dst + 8 * stride, vmulq_f32(_0a6s3, r2));
            float32x4_t _2a8s5 = vsubq_f32(vaddq_f32(s[2], s[8]), s[5]);
            float32x4_t _1a7s4 = vsubq_f32(vaddq_f32(s[1], s[7]), s[4]);
            Store<false>(dst + 9 * stride, vmulq_f32(vaddq_f32(vaddq_f32(_0a6s3, _2a8s5), _1a7s4), r4));
            Store<false>(dst + 10 * stride, vmulq_f32(vsubq_f32(vaddq_f32(_0a6s3, _2a8s5), _1a7s4), r4));
            Store<false>(dst + 11 * stride, vmulq_f32(_2a8s5, r2));

            Store<false>(dst + 12 * stride, s[6]);
            float32x4_t _6a8 = vaddq_f32(s[6], s[8]);
            Store<false>(dst + 13 * stride, vmulq_f32(vaddq_f32(_6a8, s[7]), r2));
            Store<false>(dst + 14 * stride, vmulq_f32(vsubq_f32(_6a8, s[7]), r2));
            Store<false>(dst + 15 * stride, s[8]);
}

        void Winograd2x3SetFilter(const float * src, size_t size, float * dst, SimdBool trans)
        {
            size_t size4 = AlignLo(size, 4), i = 0;
            if (trans)
            {
                for (; i < size4; i += 4)
                    Winograd2x3SetFilter4t(src + i, dst + i, size);
                for (; i < size; i += 1)
                    Base::Winograd2x3SetFilter1t(src + i, dst + i, size);
            }
            else
            {
                for (; i < size4; i += 4, src += 36, dst += 4)
                    Winograd2x3SetFilter4n(src, dst, size);
                for (; i < size; i += 1, src += 9, dst += 1)
                    Base::Winograd2x3SetFilter1n(src, dst, size);
            }
        }

        SIMD_INLINE void Winograd2x3SetInputLoad4n(const float * src, float32x4_t * dst)
        {
            *(float32x4x2_t*)(dst + 0) = Load2<false>(src + 0);
            *(float32x4x2_t*)(dst + 2) = Load2<false>(src + 2);
        }

        SIMD_INLINE void Winograd2x3SetInputLoad4n(const float * src, float32x4_t * dst, PadType pad)
        {
            float32x4_t a0 = (pad == PadNose1 ? LoadPadZeroNose1(src + 0) : Load<false>(src + 0));
            float32x4_t a1 = Load<false>(src + 2);
            float32x4_t a2 = Load<false>(src + 4);
            float32x4_t a3 = (pad == PadTail2 ? LoadPadZeroTail2(src + 6) : (pad == PadTail1 ? LoadPadZeroTail1(src + 6) : Load<false>(src + 6)));
            *(float32x4x2_t*)(dst + 0) = vuzpq_f32(a0, a2);
            *(float32x4x2_t*)(dst + 2) = vuzpq_f32(a1, a3);
        }

        SIMD_INLINE void Winograd2x3SetInputLoad4z(float32x4_t * dst)
        {
            dst[0] = vdupq_n_f32(0.0f);
            dst[1] = vdupq_n_f32(0.0f);
            dst[2] = vdupq_n_f32(0.0f);
            dst[3] = vdupq_n_f32(0.0f);
        }

        SIMD_INLINE void Winograd2x3SetInput4Store(const float32x4_t * src, float * dst, size_t stride)
        {
            Store<false>(dst + 0 * stride, vsubq_f32(vsubq_f32(src[0], src[8]), vsubq_f32(src[2], src[10])));
            Store<false>(dst + 1 * stride, vaddq_f32(vsubq_f32(src[1], src[9]), vsubq_f32(src[2], src[10])));
            Store<false>(dst + 2 * stride, vsubq_f32(vsubq_f32(src[2], src[10]), vsubq_f32(src[1], src[9])));
            Store<false>(dst + 3 * stride, vsubq_f32(vsubq_f32(src[1], src[9]), vsubq_f32(src[3], src[11])));
            Store<false>(dst + 4 * stride, vsubq_f32(vaddq_f32(src[4], src[8]), vaddq_f32(src[6], src[10])));
            Store<false>(dst + 5 * stride, vaddq_f32(vaddq_f32(src[5], src[9]), vaddq_f32(src[6], src[10])));
            Store<false>(dst + 6 * stride, vsubq_f32(vaddq_f32(src[6], src[10]), vaddq_f32(src[5], src[9])));
            Store<false>(dst + 7 * stride, vsubq_f32(vaddq_f32(src[5], src[9]), vaddq_f32(src[7], src[11])));
            Store<false>(dst + 8 * stride, vsubq_f32(vsubq_f32(src[8], src[4]), vsubq_f32(src[10], src[6])));
            Store<false>(dst + 9 * stride, vaddq_f32(vsubq_f32(src[9], src[5]), vsubq_f32(src[10], src[6])));
            Store<false>(dst + 10 * stride, vsubq_f32(vsubq_f32(src[10], src[6]), vsubq_f32(src[9], src[5])));
            Store<false>(dst + 11 * stride, vsubq_f32(vsubq_f32(src[9], src[5]), vsubq_f32(src[11], src[7])));
            Store<false>(dst + 12 * stride, vsubq_f32(vsubq_f32(src[4], src[12]), vsubq_f32(src[6], src[14])));
            Store<false>(dst + 13 * stride, vaddq_f32(vsubq_f32(src[5], src[13]), vsubq_f32(src[6], src[14])));
            Store<false>(dst + 14 * stride, vsubq_f32(vsubq_f32(src[6], src[14]), vsubq_f32(src[5], src[13])));
            Store<false>(dst + 15 * stride, vsubq_f32(vsubq_f32(src[5], src[13]), vsubq_f32(src[7], src[15])));
        }

        SIMD_INLINE void Winograd2x3SetInput4n(const float * src, size_t srcStride, float * dst, size_t dstStride)
        {
            float32x4_t t[16];
            Winograd2x3SetInputLoad4n(src + 0 * srcStride, t + 0);
            Winograd2x3SetInputLoad4n(src + 1 * srcStride, t + 4);
            Winograd2x3SetInputLoad4n(src + 2 * srcStride, t + 8);
            Winograd2x3SetInputLoad4n(src + 3 * srcStride, t + 12);
            Winograd2x3SetInput4Store(t, dst, dstStride);
        }

        SIMD_INLINE void Winograd2x3SetInput4n(const float * src, size_t srcStride, PadType rowPad, PadType colPad, float * dst, size_t dstStride)
        {
            float32x4_t t[16];
            if (rowPad == PadNose1)
                Winograd2x3SetInputLoad4z(t + 0);
            else
                Winograd2x3SetInputLoad4n(src + 0 * srcStride, t + 0, colPad);
            Winograd2x3SetInputLoad4n(src + 1 * srcStride, t + 4, colPad);
            if (rowPad == PadTail2)
                Winograd2x3SetInputLoad4z(t + 8);
            else
                Winograd2x3SetInputLoad4n(src + 2 * srcStride, t + 8, colPad);
            if (rowPad >= PadTail1)
                Winograd2x3SetInputLoad4z(t + 12);
            else
                Winograd2x3SetInputLoad4n(src + 3 * srcStride, t + 12, colPad);
            Winograd2x3SetInput4Store(t, dst, dstStride);
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcS, size_t srcC, float32x4_t dst[16])
        {
            dst[0] = Load<false>(src + 0 * srcS + 0 * srcC);
            dst[1] = Load<false>(src + 0 * srcS + 1 * srcC);
            dst[2] = Load<false>(src + 0 * srcS + 2 * srcC);
            dst[3] = Load<false>(src + 0 * srcS + 3 * srcC);
            dst[4] = Load<false>(src + 1 * srcS + 0 * srcC);
            dst[5] = Load<false>(src + 1 * srcS + 1 * srcC);
            dst[6] = Load<false>(src + 1 * srcS + 2 * srcC);
            dst[7] = Load<false>(src + 1 * srcS + 3 * srcC);
            dst[8] = Load<false>(src + 2 * srcS + 0 * srcC);
            dst[9] = Load<false>(src + 2 * srcS + 1 * srcC);
            dst[10] = Load<false>(src + 2 * srcS + 2 * srcC);
            dst[11] = Load<false>(src + 2 * srcS + 3 * srcC);
            dst[12] = Load<false>(src + 3 * srcS + 0 * srcC);
            dst[13] = Load<false>(src + 3 * srcS + 1 * srcC);
            dst[14] = Load<false>(src + 3 * srcS + 2 * srcC);
            dst[15] = Load<false>(src + 3 * srcS + 3 * srcC);
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcW, size_t srcC, float * dst, size_t dstStride)
        {
            size_t srcS = srcW * srcC;
            size_t srcCF = AlignLo(srcC, F);
            for (size_t c = 0; c < srcCF; c += F)
            {
                float32x4_t tmp[16];
                Winograd2x3SetInput4t(src + c, srcS, srcC, tmp);
                Winograd2x3SetInput4Store(tmp, dst + c, dstStride);
            }
            if (srcCF < srcC)
            {
                float32x4_t tmp[16];
                Winograd2x3SetInput4t(src + srcC - F, srcS, srcC, tmp);
                Winograd2x3SetInput4Store(tmp, dst + srcC - F, dstStride);
            }
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcS, size_t srcC, size_t rowB, size_t rowE, size_t colB, size_t colE, float32x4_t dst[16])
        {
            for (size_t i = 0; i < 16; ++i)
                dst[i] = vdupq_n_f32(0.0f);
            for (size_t row = rowB; row < rowE; ++row)
                for (size_t col = colB; col < colE; ++col)
                    dst[row * 4 + col] = Load<false>(src + row * srcS + col * srcC);
        }

        SIMD_INLINE void Winograd2x3SetInput4t(const float * src, size_t srcW, size_t srcC, size_t rowB, size_t rowE, size_t colB, size_t colE, float * dst, size_t dstStride)
        {
            size_t srcS = srcW * srcC;
            size_t srcCF = AlignLo(srcC, F);
            for (size_t c = 0; c < srcCF; c += F)
            {
                float32x4_t tmp[16];
                Winograd2x3SetInput4t(src + c, srcS, srcC, rowB, rowE, colB, colE, tmp);
                Winograd2x3SetInput4Store(tmp, dst + c, dstStride);
            }
            if (srcCF < srcC)
            {
                float32x4_t tmp[16];
                Winograd2x3SetInput4t(src + srcC - F, srcS, srcC, rowB, rowE, colB, colE, tmp);
                Winograd2x3SetInput4Store(tmp, dst + srcC - F, dstStride);
            }
        }

        void Winograd2x3SetInput(const float * src, size_t srcChannels, size_t srcHeight, size_t srcWidth, float * dst, size_t dstStride, SimdBool pad, SimdBool trans)
        {
            if (trans ? (srcChannels < 4) : (srcHeight < 4 || srcWidth < 10))
            {
                Base::Winograd2x3SetInput(src, srcChannels, srcHeight, srcWidth, dst, dstStride, pad, trans);
                return;
            }
            size_t dstH = pad ? srcHeight : srcHeight - 2;
            size_t dstW = pad ? srcWidth : srcWidth - 2;
            size_t tileH = (dstH + 1) / 2;
            size_t tileW = (dstW + 1) / 2;
            size_t dstH2 = AlignLo(dstH, 2);
            size_t dstW2 = AlignLo(dstW, 2);
            if (trans)
            {
                size_t noseW = Simd::Min<size_t>(4, dstW + 1);
                size_t noseH = Simd::Min<size_t>(4, dstH + 1);
                size_t start = pad ? 2 : 0;
                if (pad)
                {
                    if (dstH == dstH2)
                        dstH2 -= 2;
                    if (dstW == dstW2)
                        dstW2 -= 2;
                    src -= (srcWidth + 1)*srcChannels;
                }
                size_t tailW = dstW - dstW2 + (pad ? 1 : 2);
                size_t tailH = dstH - dstH2 + (pad ? 1 : 2);
                size_t row = 0, col = 0;
                if (pad)
                {
                    if (pad)
                        Winograd2x3SetInput4t(src, srcWidth, srcChannels, 1, noseH, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW2; col += 2)
                        Winograd2x3SetInput4t(src + col * srcChannels, srcWidth, srcChannels, 1, noseH, 0, 4, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd2x3SetInput4t(src + col * srcChannels, srcWidth, srcChannels, 1, noseH, 0, tailW, dst, dstStride), dst += srcChannels;
                }
                for (row = start; row < dstH2; row += 2)
                {
                    if (pad)
                        Winograd2x3SetInput4t(src + row * srcWidth * srcChannels, srcWidth, srcChannels, 0, 4, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW2; col += 2)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, 4, 0, tailW, dst, dstStride), dst += srcChannels;
                }
                if (row < dstH)
                {
                    if (pad)
                        Winograd2x3SetInput4t(src + row * srcWidth* srcChannels, srcWidth, srcChannels, 0, tailH, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW2; col += 2)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, tailH, 0, 4, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd2x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, tailH, 0, tailW, dst, dstStride), dst += srcChannels;
                }
            }
            else
            {
                size_t dstW8 = AlignLo(dstW, 8);
                if (pad && dstW8 == dstW)
                    dstW8 -= 8;
                PadType rowPad = dstH2 < dstH ? PadTail1 : PadNone;
                PadType colPad = dstW2 < dstW ? PadTail1 : PadNone;
                size_t tailCol = dstW2 < dstW ? dstW - 7 : dstW - 8;
                size_t tailRow = dstH2 < dstH ? dstH - 1 : dstH - 2;
                bool specialColTail = dstW8 < dstW || pad;
                bool specialRowTail = dstH2 < dstH || pad;
                if (pad)
                {
                    src -= srcWidth + 1;
                    rowPad = dstH2 < dstH ? PadTail2 : PadTail1;
                    colPad = dstW2 < dstW ? PadTail2 : PadTail1;
                    if (dstH2 == dstH)
                        dstH2 -= 2;
                }
                for (size_t c = 0; c < srcChannels; ++c)
                {
                    size_t row = 0, tileY = 0;
                    if (pad)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + row * srcWidth;
                        float * d = dst + tileY * tileW;
                        if (pad)
                            Winograd2x3SetInput4n(s + col, srcWidth, PadNose1, PadNose1, d + tileX, dstStride), col += 8, tileX += 4;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetInput4n(s + col, srcWidth, PadNose1, PadNone, d + tileX, dstStride);
                        if (specialColTail)
                            Winograd2x3SetInput4n(s + tailCol, srcWidth, PadNose1, colPad, d + tileW - 4, dstStride);
                        row += 2, tileY += 1;
                    }
                    for (; row < dstH2; row += 2, tileY += 1)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + row * srcWidth;
                        float * d = dst + tileY * tileW;
                        if (pad)
                            Winograd2x3SetInput4n(s + col, srcWidth, PadNone, PadNose1, d + tileX, dstStride), col += 8, tileX += 4;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetInput4n(s + col, srcWidth, d + tileX, dstStride);
                        if (specialColTail)
                            Winograd2x3SetInput4n(s + tailCol, srcWidth, PadNone, colPad, d + tileW - 4, dstStride);
                    }
                    if (specialRowTail)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + tailRow * srcWidth;
                        float * d = dst + (tileH - 1) * tileW;
                        if (pad)
                            Winograd2x3SetInput4n(s + col, srcWidth, rowPad, PadNose1, d + tileX, dstStride), col += 8, tileX += 4;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetInput4n(s + col, srcWidth, rowPad, PadNone, d + tileX, dstStride);
                        if (specialColTail)
                            Winograd2x3SetInput4n(s + tailCol, srcWidth, rowPad, colPad, d + tileW - 4, dstStride);
                    }
                    src += srcWidth * srcHeight;
                    dst += tileW * tileH;
                }
            }
        }

        SIMD_INLINE void Winograd2x3SetOutputLoad4(const float * src, size_t stride, float32x4_t * dst)
        {
            float32x4_t s0 = Load<false>(src + 0 * stride);
            float32x4_t s1 = Load<false>(src + 1 * stride);
            float32x4_t s2 = Load<false>(src + 2 * stride);
            float32x4_t s3 = Load<false>(src + 3 * stride);
            dst[0] = vaddq_f32(vaddq_f32(s0, s1), s2);
            dst[1] = vsubq_f32(vsubq_f32(s1, s2), s3);
        }

        SIMD_INLINE void Winograd2x3SetOutputLoad16(const float * src, size_t stride, float32x4_t * dst)
        {
            float32x4_t tmp[8];
            Winograd2x3SetOutputLoad4(src + 0 * stride, stride, tmp + 0);
            Winograd2x3SetOutputLoad4(src + 4 * stride, stride, tmp + 2);
            Winograd2x3SetOutputLoad4(src + 8 * stride, stride, tmp + 4);
            Winograd2x3SetOutputLoad4(src + 12 * stride, stride, tmp + 6);
            dst[0] = vaddq_f32(vaddq_f32(tmp[0], tmp[2]), tmp[4]);
            dst[1] = vaddq_f32(vaddq_f32(tmp[1], tmp[3]), tmp[5]);
            dst[2] = vsubq_f32(vsubq_f32(tmp[2], tmp[4]), tmp[6]);
            dst[3] = vsubq_f32(vsubq_f32(tmp[3], tmp[5]), tmp[7]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4n(const float * src, size_t srcStride, float * dst, size_t dstStride)
        {
            float32x4x2_t tmp[2];
            Winograd2x3SetOutputLoad16(src, srcStride, (float32x4_t*)tmp);
            Store2<false>(dst + 0 * dstStride, tmp[0]);
            Store2<false>(dst + 1 * dstStride, tmp[1]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4n(const float * src, size_t srcStride, float * dst, size_t dstStride, bool lastRow, bool lastCol, const uint32x4_t & mask)
        {
            float32x4_t tmp[4];
            Winograd2x3SetOutputLoad16(src, srcStride, tmp);
            float32x4x2_t zip0 = vzipq_f32(tmp[0], tmp[1]);
            Store<false>(dst + 0, zip0.val[0]);
            if (lastCol)
                Store<false>(dst + 4, zip0.val[1]);
            else
                StoreMasked<false>(dst + 4, zip0.val[1], mask);
            if (lastRow)
            {
                float32x4x2_t zip1 = vzipq_f32(tmp[2], tmp[3]);
                dst += dstStride;
                Store<false>(dst + 0, zip1.val[0]);
                if (lastCol)
                    Store<false>(dst + 4, zip1.val[1]);
                else
                    StoreMasked<false>(dst + 4, zip1.val[1], mask);
            }
        }

        SIMD_INLINE void Winograd2x3SetOutputStore4(const float32x4_t src[4], float * dst, size_t dstS, size_t dstC)
        {
            Store<false>(dst + 0 * dstS + 0 * dstC, src[0]);
            Store<false>(dst + 0 * dstS + 1 * dstC, src[1]);
            Store<false>(dst + 1 * dstS + 0 * dstC, src[2]);
            Store<false>(dst + 1 * dstS + 1 * dstC, src[3]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4t(const float * src, size_t srcStride, float * dst, size_t dstW, size_t dstC)
        {
            size_t dstS = dstW * dstC, dstCF = AlignLo(dstC, F);
            for (size_t d = 0; d < dstCF; d += F)
            {
                float32x4_t tmp[4];
                Winograd2x3SetOutputLoad16(src + d, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + d, dstS, dstC);
            }
            if (dstCF < dstC)
            {
                float32x4_t tmp[4];
                Winograd2x3SetOutputLoad16(src + dstC - F, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + dstC - F, dstS, dstC);
            }
        }

        SIMD_INLINE void Winograd2x3SetOutputStore4(const float32x4_t src[4], float * dst, size_t dstS, size_t dstC, size_t rowE, size_t colE)
        {
            for (size_t row = 0; row < rowE; ++row)
                for (size_t col = 0; col < colE; ++col)
                    Store<false>(dst + row * dstS + col * dstC, src[row * 2 + col]);
        }

        SIMD_INLINE void Winograd2x3SetOutput4t(const float * src, size_t srcStride, float * dst, size_t dstW, size_t dstC, size_t rowE, size_t colE)
        {
            size_t dstS = dstW * dstC, dstCF = AlignLo(dstC, F);
            for (size_t d = 0; d < dstCF; d += F)
            {
                float32x4_t tmp[4];
                Winograd2x3SetOutputLoad16(src + d, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + d, dstS, dstC, rowE, colE);
            }
            if (dstCF < dstC)
            {
                float32x4_t tmp[4];
                Winograd2x3SetOutputLoad16(src + dstC - F, srcStride, tmp);
                Winograd2x3SetOutputStore4(tmp, dst + dstC - F, dstS, dstC, rowE, colE);
            }
        }

        void Winograd2x3SetOutput(const float * src, size_t srcStride, float * dst, size_t dstChannels, size_t dstHeight, size_t dstWidth, SimdBool trans)
        {
            if (trans ? (dstChannels < 4) : (dstHeight < 2 || dstWidth < 8))
            {
                Base::Winograd2x3SetOutput(src, srcStride, dst, dstChannels, dstHeight, dstWidth, trans);
                return;
            }
            size_t tileH = (dstHeight + 1) / 2;
            size_t tileW = (dstWidth + 1) / 2;
            size_t dstH2 = AlignLo(dstHeight, 2);
            size_t dstW2 = AlignLo(dstWidth, 2);
            if (trans)
            {
                size_t row, col;
                for (row = 0; row < dstH2; row += 2)
                {
                    for (col = 0; col < dstW2; col += 2)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels), src += dstChannels;
                    if (col < dstWidth)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, 2, dstWidth - col), src += dstChannels;
                }
                if (row < dstHeight)
                {
                    for (col = 0; col < dstW2; col += 2)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, dstHeight - row, 2), src += dstChannels;
                    if (col < dstWidth)
                        Winograd2x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, dstHeight - row, dstWidth - col), src += dstChannels;
                }
            }
            else
            {
                size_t dstW8 = AlignLo(dstWidth, 8);
                uint32x4_t tailMask = vreinterpretq_u32_f32(LeftNotZero(4 + dstW2 - dstWidth));
                size_t tailCol = dstW2 < dstWidth ? dstWidth - 7 : dstWidth - 8;
                for (size_t c = 0; c < dstChannels; ++c)
                {
                    size_t row = 0, tileY = 0;
                    for (; row < dstH2; row += 2, tileY += 1)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + tileY * tileW;
                        float * d = dst + row * dstWidth;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetOutput4n(s + tileX, srcStride, d + col, dstWidth);
                        if (col < dstWidth)
                            Winograd2x3SetOutput4n(s + tileW - 4, srcStride, d + tailCol, dstWidth, true, false, tailMask);
                    }
                    if (row < dstHeight)
                    {
                        size_t col = 0, tileX = 0;
                        const float * s = src + (tileH - 1) * tileW;
                        float * d = dst + (dstHeight - 1) * dstWidth;
                        for (; col < dstW8; col += 8, tileX += 4)
                            Winograd2x3SetOutput4n(s + tileX, srcStride, d + col, dstWidth, false, true, tailMask);
                        if (col < dstWidth)
                            Winograd2x3SetOutput4n(s + tileW - 4, srcStride, d + tailCol, dstWidth, false, false, tailMask);
                    }
                    src += tileW * tileH;
                    dst += dstHeight * dstWidth;
                }
            }
        }

        SIMD_INLINE void Winograd4x3SetFilter4Row(const float32x4_t * t, float * dst, size_t stride)
        {
            const float32x4_t r4 = vdupq_n_f32(1.0f / 4.0f);
            const float32x4_t r6 = vdupq_n_f32(1.0f / 6.0f);
            const float32x4_t mr6 = vdupq_n_f32(-1.0f / 6.0f);
            const float32x4_t r12 = vdupq_n_f32(1.0f / 12.0f);
            const float32x4_t r24 = vdupq_n_f32(1.0f / 24.0f);
            Store<false>(dst + 0 * stride, vmulq_f32(r4, t[0]));
            float32x4_t t0 = vaddq_f32(t[0], t[2]);
            Store<false>(dst + 1 * stride, vmulq_f32(mr6, vaddq_f32(t0, t[1])));
            Store<false>(dst + 2 * stride, vmulq_f32(mr6, vsubq_f32(t0, t[1])));
            float32x4_t t1 = vaddq_f32(vmulq_f32(r24, t[0]), vmulq_f32(r6, t[2]));
            float32x4_t t2 = vmulq_f32(r12, t[1]);
            Store<false>(dst + 3 * stride, vaddq_f32(t1, t2));
            Store<false>(dst + 4 * stride, vsubq_f32(t1, t2));
            Store<false>(dst + 5 * stride, t[2]);
        }

        SIMD_INLINE void Winograd4x3SetFilter4All(const float32x4_t * s, float * dst, size_t stride)
        {
            const float32x4_t r4 = vdupq_n_f32(1.0f / 4.0f);
            const float32x4_t r6 = vdupq_n_f32(1.0f / 6.0f);
            const float32x4_t mr6 = vdupq_n_f32(-1.0f / 6.0f);
            const float32x4_t r12 = vdupq_n_f32(1.0f / 12.0f);
            const float32x4_t r24 = vdupq_n_f32(1.0f / 24.0f);

            float32x4_t t[3];
            t[0] = vmulq_f32(r4, s[0]);
            t[1] = vmulq_f32(r4, s[1]);
            t[2] = vmulq_f32(r4, s[2]);
            Winograd4x3SetFilter4Row(t, dst + 0 * stride, stride);

            t[0] = vmulq_f32(mr6, vaddq_f32(vaddq_f32(s[0], s[3]), s[6]));
            t[1] = vmulq_f32(mr6, vaddq_f32(vaddq_f32(s[1], s[4]), s[7]));
            t[2] = vmulq_f32(mr6, vaddq_f32(vaddq_f32(s[2], s[5]), s[8]));
            Winograd4x3SetFilter4Row(t, dst + 6 * stride, stride);

            t[0] = vmulq_f32(mr6, vaddq_f32(vsubq_f32(s[0], s[3]), s[6]));
            t[1] = vmulq_f32(mr6, vaddq_f32(vsubq_f32(s[1], s[4]), s[7]));
            t[2] = vmulq_f32(mr6, vaddq_f32(vsubq_f32(s[2], s[5]), s[8]));
            Winograd4x3SetFilter4Row(t, dst + 12 * stride, stride);

            t[0] = vaddq_f32(vaddq_f32(vmulq_f32(r24, s[0]), vmulq_f32(r12, s[3])), vmulq_f32(r6, s[6]));
            t[1] = vaddq_f32(vaddq_f32(vmulq_f32(r24, s[1]), vmulq_f32(r12, s[4])), vmulq_f32(r6, s[7]));
            t[2] = vaddq_f32(vaddq_f32(vmulq_f32(r24, s[2]), vmulq_f32(r12, s[5])), vmulq_f32(r6, s[8]));
            Winograd4x3SetFilter4Row(t, dst + 18 * stride, stride);

            t[0] = vaddq_f32(vsubq_f32(vmulq_f32(r24, s[0]), vmulq_f32(r12, s[3])), vmulq_f32(r6, s[6]));
            t[1] = vaddq_f32(vsubq_f32(vmulq_f32(r24, s[1]), vmulq_f32(r12, s[4])), vmulq_f32(r6, s[7]));
            t[2] = vaddq_f32(vsubq_f32(vmulq_f32(r24, s[2]), vmulq_f32(r12, s[5])), vmulq_f32(r6, s[8]));
            Winograd4x3SetFilter4Row(t, dst + 24 * stride, stride);

            Winograd4x3SetFilter4Row(s + 6, dst + 30 * stride, stride);
        }


        SIMD_INLINE void Winograd4x3SetFilter4n(const float * src, float * dst, size_t stride)
        {
            float32x4_t s[9];
            Load4(src + 0, 9, s + 0);
            Load4(src + 4, 9, s + 4);
            s[8] = SetF32(src[8], src[17], src[26], src[35]);
            Winograd4x3SetFilter4All(s, dst + 0 * stride, stride);
        }

        SIMD_INLINE void Winograd4x3SetFilter4t(const float * src, float * dst, size_t stride)
        {
            float32x4_t s[9];
            s[0] = Load<false>(src + 0 * stride);
            s[1] = Load<false>(src + 1 * stride);
            s[2] = Load<false>(src + 2 * stride);
            s[3] = Load<false>(src + 3 * stride);
            s[4] = Load<false>(src + 4 * stride);
            s[5] = Load<false>(src + 5 * stride);
            s[6] = Load<false>(src + 6 * stride);
            s[7] = Load<false>(src + 7 * stride);
            s[8] = Load<false>(src + 8 * stride);
            Winograd4x3SetFilter4All(s, dst + 0 * stride, stride);
        }

        void Winograd4x3SetFilter(const float * src, size_t size, float * dst, SimdBool trans)
        {
            size_t size4 = AlignLo(size, 4), i = 0;
            if (trans)
            {
                for (; i < size4; i += 4)
                    Winograd4x3SetFilter4t(src + i, dst + i, size);
                for (; i < size; i += 1)
                    Base::Winograd4x3SetFilter1t(src + i, dst + i, size);
            }
            else
            {
                for (; i < size4; i += 4, src += 36, dst += 4)
                    Winograd4x3SetFilter4n(src, dst, size);
                for (; i < size; i += 1, src += 9, dst += 1)
                    Base::Winograd4x3SetFilter1n(src, dst, size);
            }
        }

        SIMD_INLINE void Winograd4x3SetInput4Store(const float32x4_t src[36], float * dst, size_t stride)
        {
            float32x4_t _2 = vdupq_n_f32(2.0f);
            float32x4_t _4 = vdupq_n_f32(4.0f);
            float32x4_t _5 = vdupq_n_f32(5.0f);
            float32x4_t tmp[36];
            tmp[0] = vmlaq_f32(vmlsq_f32(src[24], _5, src[12]), _4, src[0]);
            tmp[1] = vmlaq_f32(vmlsq_f32(src[25], _5, src[13]), _4, src[1]);
            tmp[2] = vmlaq_f32(vmlsq_f32(src[26], _5, src[14]), _4, src[2]);
            tmp[3] = vmlaq_f32(vmlsq_f32(src[27], _5, src[15]), _4, src[3]);
            tmp[4] = vmlaq_f32(vmlsq_f32(src[28], _5, src[16]), _4, src[4]);
            tmp[5] = vmlaq_f32(vmlsq_f32(src[29], _5, src[17]), _4, src[5]);
            tmp[6] = vmlsq_f32(vaddq_f32(src[18], src[24]), _4, vaddq_f32(src[6], src[12]));
            tmp[7] = vmlsq_f32(vaddq_f32(src[19], src[25]), _4, vaddq_f32(src[7], src[13]));
            tmp[8] = vmlsq_f32(vaddq_f32(src[20], src[26]), _4, vaddq_f32(src[8], src[14]));
            tmp[9] = vmlsq_f32(vaddq_f32(src[21], src[27]), _4, vaddq_f32(src[9], src[15]));
            tmp[10] = vmlsq_f32(vaddq_f32(src[22], src[28]), _4, vaddq_f32(src[10], src[16]));
            tmp[11] = vmlsq_f32(vaddq_f32(src[23], src[29]), _4, vaddq_f32(src[11], src[17]));
            tmp[12] = vmlaq_f32(vsubq_f32(src[24], src[18]), _4, vsubq_f32(src[6], src[12]));
            tmp[13] = vmlaq_f32(vsubq_f32(src[25], src[19]), _4, vsubq_f32(src[7], src[13]));
            tmp[14] = vmlaq_f32(vsubq_f32(src[26], src[20]), _4, vsubq_f32(src[8], src[14]));
            tmp[15] = vmlaq_f32(vsubq_f32(src[27], src[21]), _4, vsubq_f32(src[9], src[15]));
            tmp[16] = vmlaq_f32(vsubq_f32(src[28], src[22]), _4, vsubq_f32(src[10], src[16]));
            tmp[17] = vmlaq_f32(vsubq_f32(src[29], src[23]), _4, vsubq_f32(src[11], src[17]));
            tmp[18] = vmlaq_f32(vsubq_f32(src[24], src[12]), _2, vsubq_f32(src[18], src[6]));
            tmp[19] = vmlaq_f32(vsubq_f32(src[25], src[13]), _2, vsubq_f32(src[19], src[7]));
            tmp[20] = vmlaq_f32(vsubq_f32(src[26], src[14]), _2, vsubq_f32(src[20], src[8]));
            tmp[21] = vmlaq_f32(vsubq_f32(src[27], src[15]), _2, vsubq_f32(src[21], src[9]));
            tmp[22] = vmlaq_f32(vsubq_f32(src[28], src[16]), _2, vsubq_f32(src[22], src[10]));
            tmp[23] = vmlaq_f32(vsubq_f32(src[29], src[17]), _2, vsubq_f32(src[23], src[11]));
            tmp[24] = vmlaq_f32(vsubq_f32(src[24], src[12]), _2, vsubq_f32(src[6], src[18]));
            tmp[25] = vmlaq_f32(vsubq_f32(src[25], src[13]), _2, vsubq_f32(src[7], src[19]));
            tmp[26] = vmlaq_f32(vsubq_f32(src[26], src[14]), _2, vsubq_f32(src[8], src[20]));
            tmp[27] = vmlaq_f32(vsubq_f32(src[27], src[15]), _2, vsubq_f32(src[9], src[21]));
            tmp[28] = vmlaq_f32(vsubq_f32(src[28], src[16]), _2, vsubq_f32(src[10], src[22]));
            tmp[29] = vmlaq_f32(vsubq_f32(src[29], src[17]), _2, vsubq_f32(src[11], src[23]));
            tmp[30] = vmlaq_f32(vmlsq_f32(src[30], _5, src[18]), _4, src[6]);
            tmp[31] = vmlaq_f32(vmlsq_f32(src[31], _5, src[19]), _4, src[7]);
            tmp[32] = vmlaq_f32(vmlsq_f32(src[32], _5, src[20]), _4, src[8]);
            tmp[33] = vmlaq_f32(vmlsq_f32(src[33], _5, src[21]), _4, src[9]);
            tmp[34] = vmlaq_f32(vmlsq_f32(src[34], _5, src[22]), _4, src[10]);
            tmp[35] = vmlaq_f32(vmlsq_f32(src[35], _5, src[23]), _4, src[11]);

            Store<false>(dst + 0 * stride, vmlaq_f32(vmlsq_f32(tmp[4], _5, tmp[2]), _4, tmp[0]));
            Store<false>(dst + 1 * stride, vmlsq_f32(vaddq_f32(tmp[3], tmp[4]), _4, vaddq_f32(tmp[1], tmp[2])));
            Store<false>(dst + 2 * stride, vmlaq_f32(vsubq_f32(tmp[4], tmp[3]), _4, vsubq_f32(tmp[1], tmp[2])));
            Store<false>(dst + 3 * stride, vmlaq_f32(vsubq_f32(tmp[4], tmp[2]), _2, vsubq_f32(tmp[3], tmp[1])));
            Store<false>(dst + 4 * stride, vmlaq_f32(vsubq_f32(tmp[4], tmp[2]), _2, vsubq_f32(tmp[1], tmp[3])));
            Store<false>(dst + 5 * stride, vmlaq_f32(vmlsq_f32(tmp[5], _5, tmp[3]), _4, tmp[1]));
            Store<false>(dst + 6 * stride, vmlaq_f32(vmlsq_f32(tmp[10], _5, tmp[8]), _4, tmp[6]));
            Store<false>(dst + 7 * stride, vmlsq_f32(vaddq_f32(tmp[9], tmp[10]), _4, vaddq_f32(tmp[7], tmp[8])));
            Store<false>(dst + 8 * stride, vmlaq_f32(vsubq_f32(tmp[10], tmp[9]), _4, vsubq_f32(tmp[7], tmp[8])));
            Store<false>(dst + 9 * stride, vmlaq_f32(vsubq_f32(tmp[10], tmp[8]), _2, vsubq_f32(tmp[9], tmp[7])));
            Store<false>(dst + 10 * stride, vmlaq_f32(vsubq_f32(tmp[10], tmp[8]), _2, vsubq_f32(tmp[7], tmp[9])));
            Store<false>(dst + 11 * stride, vmlaq_f32(vmlsq_f32(tmp[11], _5, tmp[9]), _4, tmp[7]));
            Store<false>(dst + 12 * stride, vmlaq_f32(vmlsq_f32(tmp[16], _5, tmp[14]), _4, tmp[12]));
            Store<false>(dst + 13 * stride, vmlsq_f32(vaddq_f32(tmp[15], tmp[16]), _4, vaddq_f32(tmp[13], tmp[14])));
            Store<false>(dst + 14 * stride, vmlaq_f32(vsubq_f32(tmp[16], tmp[15]), _4, vsubq_f32(tmp[13], tmp[14])));
            Store<false>(dst + 15 * stride, vmlaq_f32(vsubq_f32(tmp[16], tmp[14]), _2, vsubq_f32(tmp[15], tmp[13])));
            Store<false>(dst + 16 * stride, vmlaq_f32(vsubq_f32(tmp[16], tmp[14]), _2, vsubq_f32(tmp[13], tmp[15])));
            Store<false>(dst + 17 * stride, vmlaq_f32(vmlsq_f32(tmp[17], _5, tmp[15]), _4, tmp[13]));
            Store<false>(dst + 18 * stride, vmlaq_f32(vmlsq_f32(tmp[22], _5, tmp[20]), _4, tmp[18]));
            Store<false>(dst + 19 * stride, vmlsq_f32(vaddq_f32(tmp[21], tmp[22]), _4, vaddq_f32(tmp[19], tmp[20])));
            Store<false>(dst + 20 * stride, vmlaq_f32(vsubq_f32(tmp[22], tmp[21]), _4, vsubq_f32(tmp[19], tmp[20])));
            Store<false>(dst + 21 * stride, vmlaq_f32(vsubq_f32(tmp[22], tmp[20]), _2, vsubq_f32(tmp[21], tmp[19])));
            Store<false>(dst + 22 * stride, vmlaq_f32(vsubq_f32(tmp[22], tmp[20]), _2, vsubq_f32(tmp[19], tmp[21])));
            Store<false>(dst + 23 * stride, vmlaq_f32(vmlsq_f32(tmp[23], _5, tmp[21]), _4, tmp[19]));
            Store<false>(dst + 24 * stride, vmlaq_f32(vmlsq_f32(tmp[28], _5, tmp[26]), _4, tmp[24]));
            Store<false>(dst + 25 * stride, vmlsq_f32(vaddq_f32(tmp[27], tmp[28]), _4, vaddq_f32(tmp[25], tmp[26])));
            Store<false>(dst + 26 * stride, vmlaq_f32(vsubq_f32(tmp[28], tmp[27]), _4, vsubq_f32(tmp[25], tmp[26])));
            Store<false>(dst + 27 * stride, vmlaq_f32(vsubq_f32(tmp[28], tmp[26]), _2, vsubq_f32(tmp[27], tmp[25])));
            Store<false>(dst + 28 * stride, vmlaq_f32(vsubq_f32(tmp[28], tmp[26]), _2, vsubq_f32(tmp[25], tmp[27])));
            Store<false>(dst + 29 * stride, vmlaq_f32(vmlsq_f32(tmp[29], _5, tmp[27]), _4, tmp[25]));
            Store<false>(dst + 30 * stride, vmlaq_f32(vmlsq_f32(tmp[34], _5, tmp[32]), _4, tmp[30]));
            Store<false>(dst + 31 * stride, vmlsq_f32(vaddq_f32(tmp[33], tmp[34]), _4, vaddq_f32(tmp[31], tmp[32])));
            Store<false>(dst + 32 * stride, vmlaq_f32(vsubq_f32(tmp[34], tmp[33]), _4, vsubq_f32(tmp[31], tmp[32])));
            Store<false>(dst + 33 * stride, vmlaq_f32(vsubq_f32(tmp[34], tmp[32]), _2, vsubq_f32(tmp[33], tmp[31])));
            Store<false>(dst + 34 * stride, vmlaq_f32(vsubq_f32(tmp[34], tmp[32]), _2, vsubq_f32(tmp[31], tmp[33])));
            Store<false>(dst + 35 * stride, vmlaq_f32(vmlsq_f32(tmp[35], _5, tmp[33]), _4, tmp[31]));
        }

        SIMD_INLINE void Winograd4x3SetInput4t(const float * src, size_t srcS, size_t srcC, float32x4_t dst[36])
        {
            dst[0] = Load<false>(src + 0 * srcS + 0 * srcC);
            dst[1] = Load<false>(src + 0 * srcS + 1 * srcC);
            dst[2] = Load<false>(src + 0 * srcS + 2 * srcC);
            dst[3] = Load<false>(src + 0 * srcS + 3 * srcC);
            dst[4] = Load<false>(src + 0 * srcS + 4 * srcC);
            dst[5] = Load<false>(src + 0 * srcS + 5 * srcC);
            dst[6] = Load<false>(src + 1 * srcS + 0 * srcC);
            dst[7] = Load<false>(src + 1 * srcS + 1 * srcC);
            dst[8] = Load<false>(src + 1 * srcS + 2 * srcC);
            dst[9] = Load<false>(src + 1 * srcS + 3 * srcC);
            dst[10] = Load<false>(src + 1 * srcS + 4 * srcC);
            dst[11] = Load<false>(src + 1 * srcS + 5 * srcC);
            dst[12] = Load<false>(src + 2 * srcS + 0 * srcC);
            dst[13] = Load<false>(src + 2 * srcS + 1 * srcC);
            dst[14] = Load<false>(src + 2 * srcS + 2 * srcC);
            dst[15] = Load<false>(src + 2 * srcS + 3 * srcC);
            dst[16] = Load<false>(src + 2 * srcS + 4 * srcC);
            dst[17] = Load<false>(src + 2 * srcS + 5 * srcC);
            dst[18] = Load<false>(src + 3 * srcS + 0 * srcC);
            dst[19] = Load<false>(src + 3 * srcS + 1 * srcC);
            dst[20] = Load<false>(src + 3 * srcS + 2 * srcC);
            dst[21] = Load<false>(src + 3 * srcS + 3 * srcC);
            dst[22] = Load<false>(src + 3 * srcS + 4 * srcC);
            dst[23] = Load<false>(src + 3 * srcS + 5 * srcC);
            dst[24] = Load<false>(src + 4 * srcS + 0 * srcC);
            dst[25] = Load<false>(src + 4 * srcS + 1 * srcC);
            dst[26] = Load<false>(src + 4 * srcS + 2 * srcC);
            dst[27] = Load<false>(src + 4 * srcS + 3 * srcC);
            dst[28] = Load<false>(src + 4 * srcS + 4 * srcC);
            dst[29] = Load<false>(src + 4 * srcS + 5 * srcC);
            dst[30] = Load<false>(src + 5 * srcS + 0 * srcC);
            dst[31] = Load<false>(src + 5 * srcS + 1 * srcC);
            dst[32] = Load<false>(src + 5 * srcS + 2 * srcC);
            dst[33] = Load<false>(src + 5 * srcS + 3 * srcC);
            dst[34] = Load<false>(src + 5 * srcS + 4 * srcC);
            dst[35] = Load<false>(src + 5 * srcS + 5 * srcC);
        }

        SIMD_INLINE void Winograd4x3SetInput4t(const float * src, size_t srcW, size_t srcC, float * dst, size_t dstStride)
        {
            size_t srcS = srcW * srcC;
            size_t srcCF = AlignLo(srcC, F);
            for (size_t c = 0; c < srcCF; c += F)
            {
                float32x4_t tmp[36];
                Winograd4x3SetInput4t(src + c, srcS, srcC, tmp);
                Winograd4x3SetInput4Store(tmp, dst + c, dstStride);
            }
            if (srcCF < srcC)
            {
                float32x4_t tmp[36];
                Winograd4x3SetInput4t(src + srcC - F, srcS, srcC, tmp);
                Winograd4x3SetInput4Store(tmp, dst + srcC - F, dstStride);
            }
        }

        SIMD_INLINE void Winograd4x3SetInput4t(const float * src, size_t srcS, size_t srcC, size_t rowB, size_t rowE, size_t colB, size_t colE, float32x4_t dst[36])
        {
            for (size_t i = 0; i < 36; ++i)
                dst[i] = vdupq_n_f32(0.0f);
            for (size_t row = rowB; row < rowE; ++row)
                for (size_t col = colB; col < colE; ++col)
                    dst[row * 6 + col] = Load<false>(src + row * srcS + col * srcC);
        }

        SIMD_INLINE void Winograd4x3SetInput4t(const float * src, size_t srcW, size_t srcC, size_t rowB, size_t rowE, size_t colB, size_t colE, float * dst, size_t dstStride)
        {
            size_t srcS = srcW * srcC;
            size_t srcCF = AlignLo(srcC, F);
            for (size_t c = 0; c < srcCF; c += F)
            {
                float32x4_t tmp[36];
                Winograd4x3SetInput4t(src + c, srcS, srcC, rowB, rowE, colB, colE, tmp);
                Winograd4x3SetInput4Store(tmp, dst + c, dstStride);
            }
            if (srcCF < srcC)
            {
                float32x4_t tmp[36];
                Winograd4x3SetInput4t(src + srcC - F, srcS, srcC, rowB, rowE, colB, colE, tmp);
                Winograd4x3SetInput4Store(tmp, dst + srcC - F, dstStride);
            }
        }

        void Winograd4x3SetInput(const float * src, size_t srcChannels, size_t srcHeight, size_t srcWidth, float * dst, size_t dstStride, SimdBool pad, SimdBool trans)
        {
            if (trans ? (srcChannels < 4) : (srcHeight < 6 || srcWidth < 14))
            {
                Base::Winograd4x3SetInput(src, srcChannels, srcHeight, srcWidth, dst, dstStride, pad, trans);
                return;
            }
            size_t dstH = pad ? srcHeight : srcHeight - 2;
            size_t dstW = pad ? srcWidth : srcWidth - 2;
            size_t tileH = (dstH + 3) / 4;
            size_t tileW = (dstW + 3) / 4;
            size_t dstH4 = AlignLo(dstH, 4);
            size_t dstW4 = AlignLo(dstW, 4);
            if (trans)
            {
                size_t noseW = Simd::Min<size_t>(6, dstW + 1);
                size_t noseH = Simd::Min<size_t>(6, dstH + 1);
                size_t start = pad ? 4 : 0;
                if (pad)
                {
                    if (dstH == dstH4)
                        dstH4 -= 4;
                    if (dstW == dstW4)
                        dstW4 -= 4;
                    src -= (srcWidth + 1)*srcChannels;
                }
                size_t tailW = dstW - dstW4 + (pad ? 1 : 2);
                size_t tailH = dstH - dstH4 + (pad ? 1 : 2);
                size_t row = 0, col = 0;
                if (pad)
                {
                    if (pad)
                        Winograd4x3SetInput4t(src, srcWidth, srcChannels, 1, noseH, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW4; col += 4)
                        Winograd4x3SetInput4t(src + col * srcChannels, srcWidth, srcChannels, 1, noseH, 0, 6, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd4x3SetInput4t(src + col * srcChannels, srcWidth, srcChannels, 1, noseH, 0, tailW, dst, dstStride), dst += srcChannels;
                }
                for (row = start; row < dstH4; row += 4)
                {
                    if (pad)
                        Winograd4x3SetInput4t(src + row * srcWidth * srcChannels, srcWidth, srcChannels, 0, 6, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW4; col += 4)
                        Winograd4x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd4x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, 6, 0, tailW, dst, dstStride), dst += srcChannels;
                }
                if (row < dstH)
                {
                    if (pad)
                        Winograd4x3SetInput4t(src + row * srcWidth* srcChannels, srcWidth, srcChannels, 0, tailH, 1, noseW, dst, dstStride), dst += srcChannels;
                    for (col = start; col < dstW4; col += 4)
                        Winograd4x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, tailH, 0, 6, dst, dstStride), dst += srcChannels;
                    if (col < dstW)
                        Winograd4x3SetInput4t(src + (row * srcWidth + col) * srcChannels, srcWidth, srcChannels, 0, tailH, 0, tailW, dst, dstStride), dst += srcChannels;
                }
            }
            else
            {
                Base::Winograd4x3SetInput(src, srcChannels, srcHeight, srcWidth, dst, dstStride, pad, trans);
            }
        }

        SIMD_INLINE void Winograd4x3SetOutputLoad36(const float * src, size_t stride, float32x4_t dst[16])
        {
            float32x4_t s[36];
            s[0] = Load<false>(src + 0 * stride);
            s[1] = Load<false>(src + 1 * stride);
            s[2] = Load<false>(src + 2 * stride);
            s[3] = Load<false>(src + 3 * stride);
            s[4] = Load<false>(src + 4 * stride);
            s[5] = Load<false>(src + 5 * stride);
            s[6] = Load<false>(src + 6 * stride);
            s[7] = Load<false>(src + 7 * stride);
            s[8] = Load<false>(src + 8 * stride);
            s[9] = Load<false>(src + 9 * stride);
            s[10] = Load<false>(src + 10 * stride);
            s[11] = Load<false>(src + 11 * stride);
            s[12] = Load<false>(src + 12 * stride);
            s[13] = Load<false>(src + 13 * stride);
            s[14] = Load<false>(src + 14 * stride);
            s[15] = Load<false>(src + 15 * stride);
            s[16] = Load<false>(src + 16 * stride);
            s[17] = Load<false>(src + 17 * stride);
            s[18] = Load<false>(src + 18 * stride);
            s[19] = Load<false>(src + 19 * stride);
            s[20] = Load<false>(src + 20 * stride);
            s[21] = Load<false>(src + 21 * stride);
            s[22] = Load<false>(src + 22 * stride);
            s[23] = Load<false>(src + 23 * stride);
            s[24] = Load<false>(src + 24 * stride);
            s[25] = Load<false>(src + 25 * stride);
            s[26] = Load<false>(src + 26 * stride);
            s[27] = Load<false>(src + 27 * stride);
            s[28] = Load<false>(src + 28 * stride);
            s[29] = Load<false>(src + 29 * stride);
            s[30] = Load<false>(src + 30 * stride);
            s[31] = Load<false>(src + 31 * stride);
            s[32] = Load<false>(src + 32 * stride);
            s[33] = Load<false>(src + 33 * stride);
            s[34] = Load<false>(src + 34 * stride);
            s[35] = Load<false>(src + 35 * stride);

            float32x4_t _2 = vdupq_n_f32(2.0f);
            float32x4_t _4 = vdupq_n_f32(4.0f);
            float32x4_t _8 = vdupq_n_f32(8.0f);
            float32x4_t t[24];
            t[0] = vaddq_f32(vaddq_f32(vaddq_f32(s[0], s[6]), vaddq_f32(s[12], s[18])), s[24]);
            t[1] = vaddq_f32(vaddq_f32(vaddq_f32(s[1], s[7]), vaddq_f32(s[13], s[19])), s[25]);
            t[2] = vaddq_f32(vaddq_f32(vaddq_f32(s[2], s[8]), vaddq_f32(s[14], s[20])), s[26]);
            t[3] = vaddq_f32(vaddq_f32(vaddq_f32(s[3], s[9]), vaddq_f32(s[15], s[21])), s[27]);
            t[4] = vaddq_f32(vaddq_f32(vaddq_f32(s[4], s[10]), vaddq_f32(s[16], s[22])), s[28]);
            t[5] = vaddq_f32(vaddq_f32(vaddq_f32(s[5], s[11]), vaddq_f32(s[17], s[23])), s[29]);
            t[6] = vmlaq_f32(vsubq_f32(s[6], s[12]), _2, vsubq_f32(s[18], s[24]));
            t[7] = vmlaq_f32(vsubq_f32(s[7], s[13]), _2, vsubq_f32(s[19], s[25]));
            t[8] = vmlaq_f32(vsubq_f32(s[8], s[14]), _2, vsubq_f32(s[20], s[26]));
            t[9] = vmlaq_f32(vsubq_f32(s[9], s[15]), _2, vsubq_f32(s[21], s[27]));
            t[10] = vmlaq_f32(vsubq_f32(s[10], s[16]), _2, vsubq_f32(s[22], s[28]));
            t[11] = vmlaq_f32(vsubq_f32(s[11], s[17]), _2, vsubq_f32(s[23], s[29]));
            t[12] = vmlaq_f32(vaddq_f32(s[6], s[12]), _4, vaddq_f32(s[18], s[24]));
            t[13] = vmlaq_f32(vaddq_f32(s[7], s[13]), _4, vaddq_f32(s[19], s[25]));
            t[14] = vmlaq_f32(vaddq_f32(s[8], s[14]), _4, vaddq_f32(s[20], s[26]));
            t[15] = vmlaq_f32(vaddq_f32(s[9], s[15]), _4, vaddq_f32(s[21], s[27]));
            t[16] = vmlaq_f32(vaddq_f32(s[10], s[16]), _4, vaddq_f32(s[22], s[28]));
            t[17] = vmlaq_f32(vaddq_f32(s[11], s[17]), _4, vaddq_f32(s[23], s[29]));
            t[18] = vaddq_f32(vmlaq_f32(vsubq_f32(s[6], s[12]), _8, vsubq_f32(s[18], s[24])), s[30]);
            t[19] = vaddq_f32(vmlaq_f32(vsubq_f32(s[7], s[13]), _8, vsubq_f32(s[19], s[25])), s[31]);
            t[20] = vaddq_f32(vmlaq_f32(vsubq_f32(s[8], s[14]), _8, vsubq_f32(s[20], s[26])), s[32]);
            t[21] = vaddq_f32(vmlaq_f32(vsubq_f32(s[9], s[15]), _8, vsubq_f32(s[21], s[27])), s[33]);
            t[22] = vaddq_f32(vmlaq_f32(vsubq_f32(s[10], s[16]), _8, vsubq_f32(s[22], s[28])), s[34]);
            t[23] = vaddq_f32(vmlaq_f32(vsubq_f32(s[11], s[17]), _8, vsubq_f32(s[23], s[29])), s[35]);

            dst[0] = vaddq_f32(vaddq_f32(vaddq_f32(t[0], t[1]), vaddq_f32(t[2], t[3])), t[4]);
            dst[1] = vmlaq_f32(vsubq_f32(t[1], t[2]), _2, vsubq_f32(t[3], t[4]));
            dst[2] = vmlaq_f32(vaddq_f32(t[1], t[2]), _4, vaddq_f32(t[3], t[4]));
            dst[3] = vaddq_f32(vmlaq_f32(vsubq_f32(t[1], t[2]), _8, vsubq_f32(t[3], t[4])), t[5]);
            dst[4] = vaddq_f32(vaddq_f32(vaddq_f32(t[6], t[7]), vaddq_f32(t[8], t[9])), t[10]);
            dst[5] = vmlaq_f32(vsubq_f32(t[7], t[8]), _2, vsubq_f32(t[9], t[10]));
            dst[6] = vmlaq_f32(vaddq_f32(t[7], t[8]), _4, vaddq_f32(t[9], t[10]));
            dst[7] = vaddq_f32(vmlaq_f32(vsubq_f32(t[7], t[8]), _8, vsubq_f32(t[9], t[10])), t[11]);
            dst[8] = vaddq_f32(vaddq_f32(vaddq_f32(t[12], t[13]), vaddq_f32(t[14], t[15])), t[16]);
            dst[9] = vmlaq_f32(vsubq_f32(t[13], t[14]), _2, vsubq_f32(t[15], t[16]));
            dst[10] = vmlaq_f32(vaddq_f32(t[13], t[14]), _4, vaddq_f32(t[15], t[16]));
            dst[11] = vaddq_f32(vmlaq_f32(vsubq_f32(t[13], t[14]), _8, vsubq_f32(t[15], t[16])), t[17]);
            dst[12] = vaddq_f32(vaddq_f32(vaddq_f32(t[18], t[19]), vaddq_f32(t[20], t[21])), t[22]);
            dst[13] = vmlaq_f32(vsubq_f32(t[19], t[20]), _2, vsubq_f32(t[21], t[22]));
            dst[14] = vmlaq_f32(vaddq_f32(t[19], t[20]), _4, vaddq_f32(t[21], t[22]));
            dst[15] = vaddq_f32(vmlaq_f32(vsubq_f32(t[19], t[20]), _8, vsubq_f32(t[21], t[22])), t[23]);
        }

        SIMD_INLINE void Winograd4x3SetOutputStore16(const float32x4_t src[16], float * dst, size_t dstS, size_t dstC)
        {
            Store<false>(dst + 0 * dstS + 0 * dstC, src[0]);
            Store<false>(dst + 0 * dstS + 1 * dstC, src[1]);
            Store<false>(dst + 0 * dstS + 2 * dstC, src[2]);
            Store<false>(dst + 0 * dstS + 3 * dstC, src[3]);
            Store<false>(dst + 1 * dstS + 0 * dstC, src[4]);
            Store<false>(dst + 1 * dstS + 1 * dstC, src[5]);
            Store<false>(dst + 1 * dstS + 2 * dstC, src[6]);
            Store<false>(dst + 1 * dstS + 3 * dstC, src[7]);
            Store<false>(dst + 2 * dstS + 0 * dstC, src[8]);
            Store<false>(dst + 2 * dstS + 1 * dstC, src[9]);
            Store<false>(dst + 2 * dstS + 2 * dstC, src[10]);
            Store<false>(dst + 2 * dstS + 3 * dstC, src[11]);
            Store<false>(dst + 3 * dstS + 0 * dstC, src[12]);
            Store<false>(dst + 3 * dstS + 1 * dstC, src[13]);
            Store<false>(dst + 3 * dstS + 2 * dstC, src[14]);
            Store<false>(dst + 3 * dstS + 3 * dstC, src[15]);
        }

        SIMD_INLINE void Winograd4x3SetOutput4t(const float * src, size_t srcStride, float * dst, size_t dstW, size_t dstC)
        {
            size_t dstS = dstW * dstC, dstCF = AlignLo(dstC, F);
            for (size_t d = 0; d < dstCF; d += F)
            {
                float32x4_t tmp[16];
                Winograd4x3SetOutputLoad36(src + d, srcStride, tmp);
                Winograd4x3SetOutputStore16(tmp, dst + d, dstS, dstC);
            }
            if (dstCF < dstC)
            {
                float32x4_t tmp[16];
                Winograd4x3SetOutputLoad36(src + dstC - F, srcStride, tmp);
                Winograd4x3SetOutputStore16(tmp, dst + dstC - F, dstS, dstC);
            }
        }

        SIMD_INLINE void Winograd4x3SetOutputStore16(const float32x4_t src[16], float * dst, size_t dstS, size_t dstC, size_t rowE, size_t colE)
        {
            for (size_t row = 0; row < rowE; ++row)
                for (size_t col = 0; col < colE; ++col)
                    Store<false>(dst + row * dstS + col * dstC, src[row * 4 + col]);
        }

        SIMD_INLINE void Winograd4x3SetOutput4t(const float * src, size_t srcStride, float * dst, size_t dstW, size_t dstC, size_t rowE, size_t colE)
        {
            size_t dstS = dstW * dstC, dstCF = AlignLo(dstC, F);
            for (size_t d = 0; d < dstCF; d += F)
            {
                float32x4_t tmp[16];
                Winograd4x3SetOutputLoad36(src + d, srcStride, tmp);
                Winograd4x3SetOutputStore16(tmp, dst + d, dstS, dstC, rowE, colE);
            }
            if (dstCF < dstC)
            {
                float32x4_t tmp[16];
                Winograd4x3SetOutputLoad36(src + dstC - F, srcStride, tmp);
                Winograd4x3SetOutputStore16(tmp, dst + dstC - F, dstS, dstC, rowE, colE);
            }
        }

        void Winograd4x3SetOutput(const float * src, size_t srcStride, float * dst, size_t dstChannels, size_t dstHeight, size_t dstWidth, SimdBool trans)
        {
            if (trans ? (dstChannels < 4) : (dstHeight < 4 || dstWidth < 16))
            {
                Base::Winograd4x3SetOutput(src, srcStride, dst, dstChannels, dstHeight, dstWidth, trans);
                return;
            }
            size_t tileH = (dstHeight + 3) / 4;
            size_t tileW = (dstWidth + 3) / 4;
            size_t dstH4 = AlignLo(dstHeight, 4);
            size_t dstW4 = AlignLo(dstWidth, 4);
            if (trans)
            {
                size_t row, col;
                for (row = 0; row < dstH4; row += 4)
                {
                    for (col = 0; col < dstW4; col += 4)
                        Winograd4x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels), src += dstChannels;
                    if (col < dstWidth)
                        Winograd4x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, 4, dstWidth - col), src += dstChannels;
                }
                if (row < dstHeight)
                {
                    for (col = 0; col < dstW4; col += 4)
                        Winograd4x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, dstHeight - row, 4), src += dstChannels;
                    if (col < dstWidth)
                        Winograd4x3SetOutput4t(src, srcStride, dst + (row * dstWidth + col)*dstChannels, dstWidth, dstChannels, dstHeight - row, dstWidth - col), src += dstChannels;
                }
            }
            else
            {
                Base::Winograd4x3SetOutput(src, srcStride, dst, dstChannels, dstHeight, dstWidth, trans);
            }
        }
    }
#endif// SIMD_NEON_ENABLE
}
