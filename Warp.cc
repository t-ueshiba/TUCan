/*
 *  平成14-19年（独）産業技術総合研究所 著作権所有
 *  
 *  創作者：植芝俊夫
 *
 *  本プログラムは（独）産業技術総合研究所の職員である植芝俊夫が創作し，
 *  （独）産業技術総合研究所が著作権を所有する秘密情報です．著作権所有
 *  者による許可なしに本プログラムを使用，複製，改変，第三者へ開示する
 *  等の行為を禁止します．
 *  
 *  このプログラムによって生じるいかなる損害に対しても，著作権所有者お
 *  よび創作者は責任を負いません。
 *
 *  Copyright 2002-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the copyright holder are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holder or the creator are not responsible for any
 *  damages caused by using this program.
 *  
 *  $Id: Warp.cc,v 1.15 2009-09-11 05:56:13 ueshiba Exp $
 */
#if defined(__INTEL_COMPILER)
#  undef SSE4
#  undef SSSE3
#  undef SSE3
#  undef SSE2
#  define SSE
//#  define SSE2
//#  define SSE3
#endif
#include "TU/Warp.h"

namespace TU
{
#if defined(SSE)
  static inline mmInt16
  mmLinearInterpolate(mmInt16 x, mmInt16 y, mmInt16 d)
  {
      return x + ((d * (y - x)) >> 7);
  }

  template <class T> static inline mmInt16
  mmBilinearInterpolate(const Image<T>& in,
			mmInt16 us, mmInt16 vs, mmInt16 du, mmInt16 dv)
  {
      const mmInt16	ue = us + mmSetAll<mmInt16>(1);
#  if defined(SSE2)
      mmUInt8	uc = mmSet<mmUInt8>(*(int*)&in[mmNth<1>(vs)][mmNth<1>(ue)],
				    *(int*)&in[mmNth<0>(vs)][mmNth<0>(ue)],
				    *(int*)&in[mmNth<1>(vs)][mmNth<1>(us)],
				    *(int*)&in[mmNth<0>(vs)][mmNth<0>(us)]);
#  else
      mmUInt8	uc = mmSet<mmUInt8>(*(int*)&in[mmNth<0>(vs)][mmNth<0>(ue)],
				    *(int*)&in[mmNth<0>(vs)][mmNth<0>(us)]);
#  endif
      const mmInt16	ss = mmLinearInterpolate(mmCvt<mmInt16>(uc),
						 mmCvtH<mmInt16>(uc), du);
      vs = vs + mmSetAll<mmInt16>(1);
#  if defined(SSE2)
      uc = mmSet<mmUInt8>(*(int*)&in[mmNth<1>(vs)][mmNth<1>(ue)],
			  *(int*)&in[mmNth<0>(vs)][mmNth<0>(ue)],
			  *(int*)&in[mmNth<1>(vs)][mmNth<1>(us)],
			  *(int*)&in[mmNth<0>(vs)][mmNth<0>(us)]);
#  else
      uc = mmSet<mmUInt8>(*(int*)&in[mmNth<0>(vs)][mmNth<0>(ue)],
			  *(int*)&in[mmNth<0>(vs)][mmNth<0>(us)]);
#  endif
      return mmLinearInterpolate(ss,
				 mmLinearInterpolate(mmCvt<mmInt16>(uc),
						     mmCvtH<mmInt16>(uc), du),
				 dv);
  }
    
  template <> inline mmInt16
  mmBilinearInterpolate(const Image<u_char>& in,
			mmInt16 us, mmInt16 vs, mmInt16 du, mmInt16 dv)
  {
      const mmInt16	ue = us + mmSetAll<mmInt16>(1);
#  if defined(SSE2)
      mmUInt8		uc = mmSet<mmUInt8>(in[mmNth<7>(vs)][mmNth<7>(ue)],
					    in[mmNth<6>(vs)][mmNth<6>(ue)],
					    in[mmNth<5>(vs)][mmNth<5>(ue)],
					    in[mmNth<4>(vs)][mmNth<4>(ue)],
					    in[mmNth<3>(vs)][mmNth<3>(ue)],
					    in[mmNth<2>(vs)][mmNth<2>(ue)],
					    in[mmNth<1>(vs)][mmNth<1>(ue)],
					    in[mmNth<0>(vs)][mmNth<0>(ue)],
					    in[mmNth<7>(vs)][mmNth<7>(us)],
					    in[mmNth<6>(vs)][mmNth<6>(us)],
					    in[mmNth<5>(vs)][mmNth<5>(us)],
					    in[mmNth<4>(vs)][mmNth<4>(us)],
					    in[mmNth<3>(vs)][mmNth<3>(us)],
					    in[mmNth<2>(vs)][mmNth<2>(us)],
					    in[mmNth<1>(vs)][mmNth<1>(us)],
					    in[mmNth<0>(vs)][mmNth<0>(us)]);
#  else
      mmUInt8		uc = mmSet<mmUInt8>(in[mmNth<3>(vs)][mmNth<3>(ue)],
					    in[mmNth<2>(vs)][mmNth<2>(ue)],
					    in[mmNth<1>(vs)][mmNth<1>(ue)],
					    in[mmNth<0>(vs)][mmNth<0>(ue)],
					    in[mmNth<3>(vs)][mmNth<3>(us)],
					    in[mmNth<2>(vs)][mmNth<2>(us)],
					    in[mmNth<1>(vs)][mmNth<1>(us)],
					    in[mmNth<0>(vs)][mmNth<0>(us)]);
#  endif
      const mmInt16	ss = mmLinearInterpolate(mmCvt<mmInt16>(uc),
						 mmCvtH<mmInt16>(uc), du);
      vs = vs + mmSetAll<mmInt16>(1);
#  if defined(SSE2)
      uc = mmSet<mmUInt8>(in[mmNth<7>(vs)][mmNth<7>(ue)],
			  in[mmNth<6>(vs)][mmNth<6>(ue)],
			  in[mmNth<5>(vs)][mmNth<5>(ue)],
			  in[mmNth<4>(vs)][mmNth<4>(ue)],
			  in[mmNth<3>(vs)][mmNth<3>(ue)],
			  in[mmNth<2>(vs)][mmNth<2>(ue)],
			  in[mmNth<1>(vs)][mmNth<1>(ue)],
			  in[mmNth<0>(vs)][mmNth<0>(ue)],
			  in[mmNth<7>(vs)][mmNth<7>(us)],
			  in[mmNth<6>(vs)][mmNth<6>(us)],
			  in[mmNth<5>(vs)][mmNth<5>(us)],
			  in[mmNth<4>(vs)][mmNth<4>(us)],
			  in[mmNth<3>(vs)][mmNth<3>(us)],
			  in[mmNth<2>(vs)][mmNth<2>(us)],
			  in[mmNth<1>(vs)][mmNth<1>(us)],
			  in[mmNth<0>(vs)][mmNth<0>(us)]);
#  else
      uc = mmSet<mmUInt8>(in[mmNth<3>(vs)][mmNth<3>(ue)],
			  in[mmNth<2>(vs)][mmNth<2>(ue)],
			  in[mmNth<1>(vs)][mmNth<1>(ue)],
			  in[mmNth<0>(vs)][mmNth<0>(ue)],
			  in[mmNth<3>(vs)][mmNth<3>(us)],
			  in[mmNth<2>(vs)][mmNth<2>(us)],
			  in[mmNth<1>(vs)][mmNth<1>(us)],
			  in[mmNth<0>(vs)][mmNth<0>(us)]);
#  endif
      return mmLinearInterpolate(ss,
				 mmLinearInterpolate(mmCvt<mmInt16>(uc),
						     mmCvtH<mmInt16>(uc), du),
				 dv);
  }
#endif
    
/************************************************************************
*  static functions							*
************************************************************************/
template <class T> static inline T
bilinearInterpolate(const Image<T>& in, int us, int vs, int du, int dv)
{
    T		in00 = in[vs][us],   in01 = in[vs][us+1],
		in10 = in[vs+1][us], in11 = in[vs+1][us+1];
    int		tmp0, tmp1;
    T		out;
    tmp0 = int(in00.r) + ((du * (int(in01.r) - int(in00.r))) >> 7);
    tmp1 = int(in10.r) + ((du * (int(in11.r) - int(in10.r))) >> 7);
    out.r = tmp0 + ((dv * (tmp1 - tmp0)) >> 7);
    tmp0 = int(in00.g) + ((du * (int(in01.g) - int(in00.g))) >> 7);
    tmp1 = int(in10.g) + ((du * (int(in11.g) - int(in10.g))) >> 7);
    out.g = tmp0 + ((dv * (tmp1 - tmp0)) >> 7);
    tmp0 = int(in00.b) + ((du * (int(in01.b) - int(in00.b))) >> 7);
    tmp1 = int(in10.b) + ((du * (int(in11.b) - int(in10.b))) >> 7);
    out.b = tmp0 + ((dv * (tmp1 - tmp0)) >> 7);
    tmp0 = int(in00.a) + ((du * (int(in01.a) - int(in00.a))) >> 7);
    tmp1 = int(in10.a) + ((du * (int(in11.a) - int(in10.a))) >> 7);
    out.a = tmp0 + ((dv * (tmp1 - tmp0)) >> 7);

    return out;
}

template <> inline u_char
bilinearInterpolate(const Image<u_char>& in, int us, int vs, int du, int dv)
{
    int		in00 = in[vs][us],   in01 = in[vs][us+1],
		in10 = in[vs+1][us], in11 = in[vs+1][us+1];
    int		out0 = in00 + ((du * (in01 - in00)) >> 7),
		out1 = in10 + ((du * (in11 - in10)) >> 7);
    
    return out0 + ((dv * (out1 - out0)) >> 7);
}

/************************************************************************
*  class Warp								*
************************************************************************/
//! 画像を射影変換するための行列を設定する．
/*!
  入力画像点uは射影変換
  \f[
    \TUbeginarray{c} \TUvec{v}{} \\ 1 \TUendarray \simeq
    \TUvec{H}{} \TUbeginarray{c} \TUvec{u}{} \\ 1 \TUendarray
  \f]
  によって出力画像点vに写される．
  \param Htinv		変形を指定する3x3射影変換行列の逆行列の転置，すなわち
			\f$\TUtinv{H}{}\f$
  \param inWidth	入力画像の幅
  \param inHeight	入力画像の高さ
  \param outWidth	出力画像の幅
  \param outWidth	出力画像の高さ
*/
void
Warp::initialize(const Matrix33d& Htinv,
		 u_int inWidth,  u_int inHeight,
		 u_int outWidth, u_int outHeight)
{
    initialize(Htinv, CameraWithDistortion::Intrinsic(),
	       inWidth, inHeight, outWidth, outHeight);
}

//! 画像の非線形歪みを除去した後に射影変換を行うための行列とカメラ内部パラメータを設定する．
/*!

  canonical座標xから画像座標uへの変換が\f$\TUvec{u}{} = {\cal
  K}(\TUvec{x}{})\f$ と表されるカメラ内部パラメータについて，その線形変
  換部分を表す3x3上半三角行列をKとすると，
  \f[
    \TUbeginarray{c} \TUbar{u}{} \\ 1 \TUendarray =
    \TUvec{K}{}
    \TUbeginarray{c} {\cal K}^{-1}(\TUvec{u}{}) \\ 1 \TUendarray
  \f]
  によって画像の非線形歪みだけを除去できる．本関数は，この歪みを除去した画像点を
  射影変換Hによって出力画像点vに写すように変形パラメータを設定する．すなわち，
  全体の変形は
  \f[
    \TUbeginarray{c} \TUvec{v}{} \\ 1 \TUendarray \simeq
    \TUvec{H}{}\TUvec{K}{}
    \TUbeginarray{c} {\cal K}^{-1}(\TUvec{u}{}) \\ 1 \TUendarray
  \f]
  となる．
  \param Htinv		変形を指定する3x3射影変換行列の逆行列の転置
  \param Intrinsic	入力画像に加えれられている放射歪曲を表すカメラ内部パラメータ
  \param inWidth	入力画像の幅
  \param inHeight	入力画像の高さ
  \param outWidth	出力画像の幅
  \param outWidth	出力画像の高さ
*/
void
Warp::initialize(const Matrix33d& Htinv,
		 const CameraBase::Intrinsic& intrinsic,
		 u_int inWidth,  u_int inHeight,
		 u_int outWidth, u_int outHeight)
{
    _fracs.resize(outHeight);
    _width = outWidth;
    
  /* Compute frac for each pixel. */
    const Matrix<double>&	HKtinv = Htinv * intrinsic.Ktinv();
    Vector<double>		leftmost = HKtinv[2];
    for (u_int v = 0; v < height(); ++v)
    {
	Vector<double>	xc = leftmost;
	FracArray	frac(width());
	u_int		n = 0;
	for (u_int u = 0; u < width(); ++u)
	{
	    const Point2d&
		m = intrinsic(Point2d(xc[0]/xc[2], xc[1]/xc[2]));
	    if (0.0 <= m[0] && m[0] <= inWidth - 2 &&
		0.0 <= m[1] && m[1] <= inHeight - 2)
	    {
		if (n == 0)
		    frac.lmost = u;
		frac.us[n] = (short)floor(m[0]);
		frac.vs[n] = (short)floor(m[1]);
		frac.du[n] = (u_char)floor((m[0] - floor(m[0])) * 128.0);
		frac.dv[n] = (u_char)floor((m[1] - floor(m[1])) * 128.0);
		++n;
	    }
	    xc += HKtinv[0];
	}

	_fracs[v].resize(n);
	_fracs[v].lmost = frac.lmost;
	for (u_int u = 0; u < n; ++u)
	{
	    _fracs[v].us[u] = frac.us[u];
	    _fracs[v].vs[u] = frac.vs[u];
	    _fracs[v].du[u] = frac.du[u];
	    _fracs[v].dv[u] = frac.dv[u];
	}

	leftmost += HKtinv[1];
    }
}

//! 出力画像の範囲を指定して画像を変形する．
/*!
  \param in	入力画像
  \param out	出力画像
  \param vs	変形結果となる領域の最初の行を指定するindex
  \param ve	変形結果となる領域の最後の行の次を指定するindex．0ならば出力画像の
		最後の行まで変形結果によって埋められる
*/
template <class T> void
Warp::operator ()(const Image<T>& in, Image<T>& out, u_int vs, u_int ve) const
{
    out.resize(height(), width());
    if (ve == 0)
	ve = out.height();
    
    for (u_int v = vs; v < ve; ++v)
    {
	const short	*usp  = _fracs[v].us, *vsp  = _fracs[v].vs;
	const u_char	*dup  = _fracs[v].du, *dvp  = _fracs[v].dv;
	T		*outp = out[v] + _fracs[v].lmost;
	T* const	outq  = outp + _fracs[v].width();
#if defined(SSE)
	for (T* const outr = outq - mmUInt8::NElms; outp <= outr; )
	{
	    const u_int	NClrs16 = mmUInt16::NElms/4;
	    mmInt16	uu = mmLoad(usp), vv = mmLoad(vsp);
	    mmUInt8	du = mmLoad(dup), dv = mmLoad(dvp);
	    mmUInt8	du4 = mmQuad0(du), dv4 = mmQuad0(dv);
	    mmStoreU((u_char*)outp,
		     mmCvt<mmUInt8>(
			 mmBilinearInterpolate(
			     in, uu, vv,
			     mmCvt<mmInt16>(du4), mmCvt<mmInt16>(dv4)),
			 mmBilinearInterpolate(
			     in,
			     mmShiftElmR<NClrs16>(uu),
			     mmShiftElmR<NClrs16>(vv),
			     mmCvtH<mmInt16>(du4), mmCvtH<mmInt16>(dv4))));
	    outp += mmUInt8::NElms/4;

	    du4 = mmQuad1(du);
	    dv4 = mmQuad1(dv);
	    mmStoreU((u_char*)outp,
		     mmCvt<mmUInt8>(
			 mmBilinearInterpolate(
			     in,
			     mmShiftElmR<2*NClrs16>(uu),
			     mmShiftElmR<2*NClrs16>(vv),
			     mmCvt<mmInt16>(du4), mmCvt<mmInt16>(dv4)),
			 mmBilinearInterpolate(
			     in,
			     mmShiftElmR<3*NClrs16>(uu),
			     mmShiftElmR<3*NClrs16>(vv),
			     mmCvtH<mmInt16>(du4), mmCvtH<mmInt16>(dv4))));
	    outp += mmUInt8::NElms/4;
	    usp  += mmInt16::NElms;
	    vsp  += mmInt16::NElms;
	    
	    uu  = mmLoad(usp);
	    vv  = mmLoad(vsp);
	    du4 = mmQuad2(du);
	    dv4 = mmQuad2(dv);
	    mmStoreU((u_char*)outp,
		     mmCvt<mmUInt8>(
			 mmBilinearInterpolate(
			     in, uu, vv,
			     mmCvt<mmInt16>(du4), mmCvt<mmInt16>(dv4)),
			 mmBilinearInterpolate(
			     in,
			     mmShiftElmR<NClrs16>(uu),
			     mmShiftElmR<NClrs16>(vv),
			     mmCvtH<mmInt16>(du4), mmCvtH<mmInt16>(dv4))));
	    outp += mmUInt8::NElms/4;
	    
	    du4 = mmQuad3(du);
	    dv4 = mmQuad3(dv);
	    mmStoreU((u_char*)outp,
		     mmCvt<mmUInt8>(
			 mmBilinearInterpolate(
			     in,
			     mmShiftElmR<2*NClrs16>(uu),
			     mmShiftElmR<2*NClrs16>(vv),
			     mmCvt<mmInt16>(du4), mmCvt<mmInt16>(dv4)),
			 mmBilinearInterpolate(
			     in,
			     mmShiftElmR<3*NClrs16>(uu),
			     mmShiftElmR<3*NClrs16>(vv),
			     mmCvtH<mmInt16>(du4), mmCvtH<mmInt16>(dv4))));
	    outp += mmUInt8::NElms/4;
	    usp  += mmInt16::NElms;
	    vsp  += mmInt16::NElms;

	    dup  += mmUInt8::NElms;
	    dvp  += mmUInt8::NElms;
	}
#endif
      	while (outp < outq)
	    *outp++ = bilinearInterpolate(in, *usp++, *vsp++, *dup++, *dvp++);
	out[v].setLimits(_fracs[v].lmost, _fracs[v].lmost + _fracs[v].width());
    }
#if defined(SSE)
    mmEmpty();
#endif	
}

//! 出力画像の範囲を指定して画像を変形する．
/*!
  \param in	入力画像
  \param out	出力画像
  \param vs	変形結果となる領域の最初の行を指定するindex
  \param ve	変形結果となる領域の最後の行の次を指定するindex．0ならば出力画像の
		最後の行まで変形結果によって埋められる
*/
template <> __PORT void
Warp::operator ()(const Image<u_char>& in, Image<u_char>& out,
		  u_int vs, u_int ve) const
{
    out.resize(height(), width());
    if (ve == 0)
	ve = out.height();
    
    for (u_int v = vs; v < ve; ++v)
    {
	const short	*usp  = _fracs[v].us, *vsp = _fracs[v].vs;
	const u_char	*dup  = _fracs[v].du, *dvp = _fracs[v].dv;
	u_char		*outp = out[v] + _fracs[v].lmost;
	u_char* const	outq  = outp + _fracs[v].width();
#if defined(SSE)
	for (u_char* const outr = outq - mmUInt8::NElms; outp <= outr; )
	{
	    mmUInt8	du = mmLoad(dup), dv = mmLoad(dvp);
	    mmInt16	out0 = mmBilinearInterpolate(in,
						     mmLoad(usp), mmLoad(vsp),
						     mmCvt<mmInt16>(du),
						     mmCvt<mmInt16>(dv));
	    usp += mmInt16::NElms;
	    vsp += mmInt16::NElms;
	    mmStoreU(outp, mmCvt<mmUInt8>(out0,
					  mmBilinearInterpolate(
					      in,
					      mmLoad(usp), mmLoad(vsp),
					      mmCvtH<mmInt16>(du),
					      mmCvtH<mmInt16>(dv))));
	    usp  += mmInt16::NElms;
	    vsp  += mmInt16::NElms;
	    dup  += mmUInt8::NElms;
	    dvp  += mmUInt8::NElms;
	    outp += mmUInt8::NElms;
	}
#endif
      	while (outp < outq)
	    *outp++ = bilinearInterpolate(in, *usp++, *vsp++, *dup++, *dvp++);
	out[v].setLimits(_fracs[v].lmost, _fracs[v].lmost + _fracs[v].width());
    }
#if defined(SSE)
    mmEmpty();
#endif	
}

template __PORT void
Warp::operator ()(const Image<RGBA>& in,
		  Image<RGBA>& out, u_int vs, u_int ve) const;
template __PORT void
Warp::operator ()(const Image<ABGR>& in,
		  Image<ABGR>& out, u_int vs, u_int ve) const;
}
