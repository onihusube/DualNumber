#pragma once

#include <complex>
#include <iostream>

namespace DualNumbers {

	/**
	* @brief 双対数（二重数）のtratis
	* @tparam T 任意の二重数の実装型
	*/
	template<typename T>
	struct dual_number_traits {
		static constexpr auto a(const T& val) {
			return val.a();
		}

		static constexpr auto b(const T& val) {
			return val.b();
		}
	};

	/**
	* @brief std::complexを対象としたtratis実装
	*/
	template<typename T>
	struct dual_number_traits<std::complex<T>> {

		static constexpr auto a(const std::complex<T>& val) {
			return val.real();
		}

		static constexpr auto b(const std::complex<T>& val) {
			return val.imag();
		}
	};


	/**
	* @brief 双対数（二重数）の実装
	* @tparam T 値型、doubleと同じ操作ができる型
	*/
	template<typename T>
	struct dual {
		using this_type  = dual<T>;
		using value_type = T;

		/**
		* 双対数の零元
		* @return 0 + 0ε
		*/
		static constexpr this_type Zero() {
			return this_type{ T(0.0), T(0.0) };
		}

		/**
		* 双対数の加法単位元
		* @return 0 + 0ε
		*/
		static constexpr this_type ID_Add() {
			return Zero();
		}

		/**
		* 双対数の乗法単位元
		* @return 1 + 0ε
		*/
		static constexpr this_type ID_Mul() {
			return this_type{ T(0.0), T(0.0) };
		}

		/**
		* デフォルトコンストラクタ
		*/
		constexpr dual()
			: m_a{ 0.0 }
			, m_b{ 0.0 }
		{}

		/**
		* 基本コンストラクタ、値を入れて構築
		*/
		constexpr dual(T a, T b = T(0.0))
			: m_a{ a }
			, m_b{ b }
		{}

		/**
		* 他の双対数実装からの変換コンストラクタ
		* @brief その型に適合したdual_number_traits<T>の特殊化が必要
		* @param other 任意の双対数
		*/
		template<typename OtherDual>
		constexpr dual(const OtherDual& other)
			: m_a{ T{dual_number_traits<OtherDual>::a(other)} }
			, m_b{ T{dual_number_traits<OtherDual>::b(other)} }
		{}

		constexpr dual(const this_type& other) = default;
		constexpr dual(this_type&& other) = default;
		
		constexpr this_type& operator=(const this_type& other) & = default;
		constexpr this_type& operator=(this_type&& other) & = default;

		constexpr operator T() const {
			return m_a;
		}

		constexpr this_type operator+() const {
			return *this;
		}

		constexpr this_type operator-() const {
			return this_type{ -m_a,-m_b };
		}

		constexpr bool operator==(const this_type& rhs) const {
			//return m_a == rhs.m_a && m_b == rhs.m_b;
			return std::tie(m_a, m_b) == std::tie(rhs.m_a, rhs.m_b);
		}

		constexpr bool operator<(const this_type& rhs) const {
			return std::tie(m_a, m_b) < std::tie(rhs.m_a, rhs.m_b);
		}

		constexpr this_type& operator++() {
			++m_a;
			return *this;
		}

		constexpr this_type& operator--() {
			--m_a;
			return *this;
		}

		constexpr this_type& operator+=(const this_type& rhs) {
			m_a += rhs.m_a;
			m_b += rhs.m_b;

			return *this;
		}

		constexpr this_type& operator+=(const T rhs) {
			m_a += rhs;

			return *this;
		}

		constexpr this_type& operator-=(const this_type& rhs) {
			m_a -= rhs.m_a;
			m_b -= rhs.m_b;

			return *this;
		}

		constexpr this_type& operator-=(const T rhs) {
			m_a -= rhs;

			return *this;
		}

		constexpr this_type& operator*=(const this_type& rhs) {
			//(a+bε)*(c+dε) = ac + (ad + bc)ε
			m_b *= rhs.m_a;
			m_b += m_a * rhs.m_b;
			m_a *= rhs.m_a;

			return *this;
		}

		constexpr this_type& operator*=(const T rhs) {
			m_b *= rhs;
			m_a *= rhs;

			return *this;
		}

		constexpr this_type& operator/=(const this_type& rhs) {
			// (a+bε)/(c+dε) = a/c + (-ad + bc)ε/c^2
			m_b *= rhs.m_a;
			m_b -= m_a * rhs.m_b;
			m_b /= rhs.m_a * rhs.m_a;
			m_a /= rhs.m_a;

			return *this;
		}

		constexpr this_type& operator/=(const T rhs) {
			m_a /= rhs;
			m_b /= rhs;

			return *this;
		}

		/**
		* 現在の値をその逆数にする
		* @brief 現在保持する値をその積の逆元に更新する
		* @detail 実部(a)がゼロでない事
		*/
		constexpr void inverse() {
			this->inverted();
		}

		/**
		* 現在の値をその共役にする
		* @brief 現在保持する値をその共役双対数にする
		*/
		constexpr void conjugate() {
			this->conjugated();
		}

		/**
		* 双対数の逆数を得る
		* @brief 指定された双対数の逆数を求める
		* @param dual 実部(a)がゼロでない双対数
		* @return 逆数(1/a - b/a^2ε)
		*/
		constexpr this_type& inverted() {
			//d^-1 = 1/a - b/a^2
			m_a = T(1.0) / m_a;
			m_b /= -(m_a* m_a);
			return *this;
		}

		/**
		* 双対数の共役を得る
		* @brief 指定された双対数の共役を求める
		* @param dual 実部(a)がゼロでない双対数
		* @return 共役双対数(a - bε)
		*/
		constexpr this_type& conjugated() {
			m_b = -m_b;
			return *this;
		}
		
		/**
		* 実部を取得する
		* @return 実部の値
		*/
		constexpr T a() const {
			return m_a;
		}

		/**
		* 虚部を取得する
		* @return 虚部の値
		*/
		constexpr T b() const {
			return m_b;
		}

	private:
		value_type m_a;
		value_type m_b;
	};


	template<typename T>
	constexpr bool operator!=(const dual<T>& lhs, const dual<T>& rhs) {
		return !(lhs == rhs);
	}

	template<typename T>
	constexpr bool operator<=(const dual<T>& lhs, const dual<T>& rhs) {
		return (lhs == rhs) || (lhs < rhs);
	}

	template<typename T>
	constexpr bool operator>(const dual<T>& lhs, const dual<T>& rhs) {
		return rhs < lhs;
	}

	template<typename T>
	constexpr bool operator>=(const dual<T>& lhs, const dual<T>& rhs) {
		return (lhs == rhs) || (lhs > rhs);
	}

	template<typename T>
	constexpr auto operator++(dual<T>& dual, int) {
		auto copy = dual;
		++dual;
		return copy;
	}

	template<typename T>
	constexpr auto operator--(dual<T>& dual, int) {
		auto copy = dual;
		--dual;
		return copy;
	}

	template<typename T>
	constexpr auto operator+(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} += rhs;
	}

	template<typename T>
	constexpr auto operator+(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} += rhs;
	}

	template<typename T>
	constexpr auto operator+(const T lhs, const dual<T>& rhs) {
		return dual<T>{rhs} += lhs;
	}

	template<typename T>
	constexpr auto operator*(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} *= rhs;
	}

	template<typename T>
	constexpr auto operator*(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} *= rhs;
	}

	template<typename T>
	constexpr auto operator*(const T lhs, const dual<T>& rhs) {
		return dual<T>{rhs} *= lhs;
	}

	template<typename T>
	constexpr auto operator-(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} -= rhs;
	}

	template<typename T>
	constexpr auto operator-(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} -= rhs;
	}

	template<typename T>
	constexpr auto operator-(const T lhs, const dual<T>& rhs) {
		return dual<T>{lhs} -= rhs;
	}
	
	template<typename T>
	constexpr auto operator/(const dual<T>& lhs, const dual<T>& rhs) {
		return dual<T>{lhs} /= rhs;
	}

	template<typename T>
	constexpr auto operator/(const dual<T>& lhs, const T rhs) {
		return dual<T>{lhs} /= rhs;
	}

	template<typename T>
	constexpr auto operator/(const T lhs, const dual<T>& rhs) {
		return dual<T>{lhs} /= rhs;
	}

	template<typename Char, typename Traits, typename T>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<char, Traits>& ostream, const dual<T>& rhs) {
		ostream << rhs.a() << " + " << rhs.b() << "e";
		return ostream;
	}

	constexpr dual<double> operator"" _d(long double real) {
		return {static_cast<double>(real), 0.0};
	}

	constexpr dual<double> operator"" _eps(long double eps) {
		return {0.0, static_cast<double>(eps)};
	}

	/**
	* 双対数の逆数を得る
	* @brief 指定された双対数の逆数を求める
	* @param dual 実部(a)がゼロでない双対数
	* @return 逆数(1/a - b/a^2ε)
	*/
	template<typename T>
	constexpr auto inverted(dual<T>& dual) {
		dual.inverse();
		return dual;
	}

	/**
	* 双対数の共役を得る
	* @brief 指定された双対数の共役を求める
	* @param dual 実部(a)がゼロでない双対数
	* @return 共役双対数(a - bε)
	*/
	template<typename T>
	constexpr auto conjugated(dual<T>& dual) {
		dual.conjugate();
		return dual;
	}

	using dual_f  = dual<float>;
	using dual_d  = dual<double>;
	using dual_ld = dual<long double>;

	inline namespace cmath {

		namespace Constant {
			template<typename T>
			constexpr T loge_2 = static_cast<T>(0.693147180559945309417232121458);

			template<typename T>
			constexpr T loge_10 = static_cast<T>(2.30258509299404568401799145468);
		}

		template<typename T>
		auto atan2(const dual<T>& y, const dual<T>& x) {
			using std::atan2;

			auto sumsq_inv = T(1.0) / (x.a() * x.a() + y.a() * y.a());
			return dual<T>{atan2(y.a(), x.a()), sumsq_inv*(-y.a()*x.b() + x.a()*y.b())};
		}

		template<typename T>
		auto pow(const dual<T>& f, const dual<T>& y) {
			using std::pow;

			auto real = pow(f.a(), y.b());
			auto dpow_y = y.a() * pow(f.a(), y.a() - T(1.0));
			auto dpow_f = real * log(f.a());
			return dual<T>{real, dpow_y * f.b() + dpow_f * y.b()};
		}

		template<typename Exponent, typename T>
		auto pow(Exponent f, const dual<T>& y) {
			using std::pow;
			using std::log;

			auto tmp = pow(f, y.a());
			return dual<T>{tmp, y.b() * tmp * log(f)};
		}

		template<typename T, typename Exponent>
		auto pow(const dual<T>& d, Exponent y) {
			using std::pow;
			return dual<T>{pow(d.a(), y), static_cast<T>(y)*d.b()*pow(d.a(), y - T(1.0))};
		}

		template<typename T>
		auto sqrt(const dual<T>& d) {
			using std::sqrt;
			auto sqrt_a = sqrt(d.a());
			return dual<T>{sqrt_a, d.b() / (sqrt_a + sqrt_a)};
		}

		template<typename T>
		auto cbrt(const dual<T>& d) {
			using std::cbrt;
			auto cbrt_a = cbrt(d.a());
			cbrt_a *= cbrt_a;
			return dual<T>{cbrt_a, d.b() / (cbrt_a + cbrt_a + cbrt_a)};
		}

		template<typename T>
		auto sin(const dual<T>& d) {
			using std::sin;
			using std::cos;

			return dual<T>{sin(d.a()), d.b()*cos(d.a())};
		}

		template<typename T>
		auto cos(const dual<T>& d) {
			using std::sin;
			using std::cos;

			return dual<T>{cos(d.a()), -d.b()*sin(d.a())};
		}

		template<typename T>
		auto tan(const dual<T>& d) {
			using std::cos;
			using std::tan;

			auto cos_a = cos(d.a());
			return dual<T>{tan(d.a()), d.b() / (cos_a*cos_a)};
		}

		template<typename T>
		auto asin(const dual<T>& d) {
			using std::asin;
			using std::sqrt;

			return dual<T>{asin(d.a()), d.b() / sqrt(T(1.0) -d.a()*d.a())};
		}

		template<typename T>
		auto acos(const dual<T>& d) {
			using std::acos;
			using std::sqrt;

			return dual<T>{acos(d.a()), -d.b() / sqrt(T(1.0) -d.a()*d.a())};
		}

		template<typename T>
		auto atan(const dual<T>& d) {
			using std::atan;

			return dual<T>{atan(d.a()), d.b() / (T(1.0) + d.a()*d.a())};
		}

		template<typename T>
		auto sinh(const dual<T>& d) {
			using std::sinh;
			using std::cosh;

			return dual<T>{sinh(d.a()), d.b()* cosh(d.a()) };
		}

		template<typename T>
		auto cosh(const dual<T>& d) {
			using std::sinh;
			using std::cosh;

			return dual<T>{cosh(d.a()), d.b()* sinh(d.a()) };
		}

		template<typename T>
		auto tanh(const dual<T>& d) {
			using std::tanh;
			using std::cosh;

			auto cosh_a = cosh(d.a());
			return dual<T>{tanh(d.a()), d.b() / (cosh_a*cosh_a)};
		}

		template<typename T>
		auto asinh(const dual<T>& d) {
			using std::asinh;
			using std::sqrt;

			return dual<T>{asinh(d.a()), d.b() / sqrt(T(1.0) +d.a()*d.a())};
		}

		template<typename T>
		auto acosh(const dual<T>& d) {
			using std::acosh;
			using std::sqrt;

			return dual<T>{acosh(d.a()), d.b() / sqrt(d.a()*d.a() - T(1.0))};
		}

		template<typename T>
		auto atanh(const dual<T>& d) {
			using std::atanh;

			return dual<T>{atanh(d.a()), d.b() / (T(1.0) -d.a()*d.a())};
		}

		template<typename T>
		auto exp(const dual<T>& d) {
			using std::exp;

			auto f = exp(d.a());
			return dual<T>{f, d.b()*f};
		}

		template<typename T>
		auto exp2(const dual<T>& d) {
			using std::exp2;

			auto f = exp2(d.a());
			return dual<T>{f, d.b()*f*Constant::loge_2<T>};
		}

		template<typename T>
		auto expm1(const dual<T>& d) {
			using std::exp;
			using std::expm1;

			return dual<T>{expm1(d.a()), d.b()*exp(d.a())};
		}

		template<typename T>
		auto log(const dual<T>& d) {
			using std::log;

			return dual<T>{log(d.a()), d.b() / d.a()};
		}

		template<typename T>
		auto log1p(const dual<T>& d) {
			using std::log1p;

			return dual<T>{log1p(d.a()), d.b() / (T(1.0) + d.a())};
		}

		template<typename T>
		auto log10(const dual<T>& d) {
			using std::log10;

			return dual<T>{log10(d.a()), d.b() / (d.a() * Constant::loge_10<T>)};
		}

		template<typename T>
		auto log2(const dual<T>& d) {
			using std::log10;

			return dual<T>{log2(d.a()), d.b() / (d.a() * Constant::loge_2<T>)};
		}


		/**
		* 各種ベッセル関数とその微分を計算する
		* @tparam T nuの型
		* @tparam T2 入力双対数の内部値型
		* @tparam BesselFunctor 各種ベッセル関数をラップしたファンクタ
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		* @param bessel 各種ベッセル関数をラップしたファンクタの実態
		*/
		template<typename T, typename T2, typename BesselFunctor>
		static auto calculateBesselFunctions(T nu, const dual<T2>& x, BesselFunctor&& bessel) {
			using ReturnType = decltype(bessel(T(0.0), x.a()));

			if (nu == T{ 0.0 }) {
				// Z0'(x) = -Z1(x)、Z=任意の円柱関数（ベッセル、ノイマン、ハンケル）
				return dual<ReturnType>{bessel(T(0.0), x.a()), -x.b() * bessel(T(0.0), x.a())};
			}
			else {
				// Zn'(x) = 0.5*(Zn-1(x) - Zn+1(x))
				return dual<ReturnType>{bessel(nu, x.a()), T2(0.5) *x.b() * (bessel(nu - T(1.0), x.a()) - bessel(nu + T(1.0), x.a()))};
			}
		}

		/**
		* 各種変形ベッセル関数とその微分を計算する
		* @tparam T nuの型
		* @tparam T2 入力双対数の内部値型
		* @tparam BesselFunctor 各種変形ベッセル関数をラップしたファンクタ
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		* @param bessel 各種変形ベッセル関数をラップしたファンクタの実態
		* @return 変形ベッセル関数に通した双対数、第二種の場合は微分の符号が異なる
		*/
		template<typename T, typename T2, typename BesselFunctor>
		static auto calculateModifiedBesselFunctions(T nu, const dual<T2>& x, BesselFunctor&& bessel) {
			using ReturnType = decltype(bessel(T(0.0), x.a()));

			if (nu == T(0.0)) {
				// Z0'(x) = Z1(x)、Z=任意の変形ベッセル関数
				return dual<ReturnType>{bessel(T(0.0), x.a()), x.b() * bessel(T(1.0), x.a())};
			}
			else {
				// Zn'(x) = 0.5*(Zn-1(x) + Zn+1(x))
				return dual<ReturnType>{bessel(nu, x.a()), T2(0.5) *x.b() * (bessel(nu - T(1.0), x.a()) + bessel(nu + T(1.0), x.a()))};
			}
		}

		/**
		* 第一種ベッセル関数、float用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_jf(float nu, const dual<T>& x) {
			using std::cyl_bessel_jf;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_jf(nu, x); });
		}

		/**
		* 第一種ベッセル関数、double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_j(double nu, const dual<T>& x) {
			using std::cyl_bessel_j;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_j(nu, x); });
		}

		/**
		* 第一種ベッセル関数、long double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_jl(long double nu, const dual<T>& x) {
			using std::cyl_bessel_jl;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_jl(nu, x); });
		}

		/**
		* 第二種ベッセル関数(ノイマン関数)、float用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_neumannf(float nu, const dual<T>& x) {
			using std::cyl_neumannf;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_neumannf(nu, x); });
		}

		/**
		* 第二種ベッセル関数(ノイマン関数)、double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_neumann(double nu, const dual<T>& x) {
			using std::cyl_neumann;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_neumann(nu, x); });
		}

		/**
		* 第二種ベッセル関数(ノイマン関数)、long double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_neumannl(long double nu, const dual<T>& x) {
			using std::cyl_neumannl;

			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_neumannl(nu, x); });
		}

		/**
		* 第三種ベッセル関数(ハンケル関数の一つ、虚部の符号がプラス)、float用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		auto cyl_hankel_1f(float nu, const dual<float>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<float>{std::cyl_bessel_jf(nu, x), std::cyl_neumannf(nu, x) }; });
		}

		/**
		* 第三種ベッセル関数(ハンケル関数の一つ、虚部の符号がマイナス)、float用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		auto cyl_hankel_2f(float nu, const dual<float>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<float>{std::cyl_bessel_jf(nu, x), -std::cyl_neumannf(nu, x) }; });
		}

		/**
		* 第三種ベッセル関数(ハンケル関数の一つ、虚部の符号がプラス)、double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		auto cyl_hankel_1(double nu, const dual<double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<double>{std::cyl_bessel_j(nu, x), std::cyl_neumann(nu, x) }; });
		}

		/**
		* 第三種ベッセル関数(ハンケル関数の一つ、虚部の符号がマイナス)、double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		auto cyl_hankel_2(double nu, const dual<double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<double>{std::cyl_bessel_j(nu, x), -std::cyl_neumann(nu, x) }; });
		}

		/**
		* 第三種ベッセル関数(ハンケル関数の一つ、虚部の符号がプラス)、long double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		auto cyl_hankel_1l(long double nu, const dual<long double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<long double>{std::cyl_bessel_jl(nu, x), std::cyl_neumannl(nu, x) }; });
		}

		/**
		* 第三種ベッセル関数(ハンケル関数の一つ、虚部の符号がマイナス)、long double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		auto cyl_hankel_2l(long double nu, const dual<long double>& x) {
			return calculateBesselFunctions(nu, x, [](auto nu, auto x) {return std::complex<long double>{std::cyl_bessel_jl(nu, x), -std::cyl_neumannl(nu, x) }; });
		}

		/**
		* 第一種変形ベッセル関数、float用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_if(float nu, const dual<T>& x) {
			using std::cyl_bessel_if;

			return calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_if(nu, x); });
		}

		/**
		* 第一種変形ベッセル関数、double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_i(double nu, const dual<T>& x) {
			using std::cyl_bessel_i;

			return calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_i(nu, x); });
		}

		/**
		* 第一種変形ベッセル関数、long double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_il(long double nu, const dual<T>& x) {
			using std::cyl_bessel_il;

			return calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_il(nu, x); });
		}

		/**
		* 第二種変形ベッセル関数、float用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_kf(float nu, const dual<T>& x) {
			using std::cyl_bessel_kf;

			dual<T> d = calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_kf(nu, x); });
			return dual<T>{d.a(), -d.b()};
		}

		/**
		* 第二種変形ベッセル関数、double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_k(double nu, const dual<T>& x) {
			using std::cyl_bessel_k;

			dual<T> d = calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_k(nu, x); });
			return dual<T>{d.a(), -d.b()};
		}

		/**
		* 第二種変形ベッセル関数、long double用
		* @tparam T 入力双対数の内部値型
		* @param nu 各種円柱関数のα(v)
		* @param x 入力双対数
		*/
		template<typename T>
		auto cyl_bessel_kl(long double nu, const dual<T>& x) {
			using std::cyl_bessel_kl;

			dual<T> d = calculateModifiedBesselFunctions(nu, x, [](auto nu, auto x) {return cyl_bessel_kl(nu, x); });
			return dual<T>{d.a(), -d.b()};
		}
	}
}