// DualNumber.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include <iomanip>

#include"DualNumber.hpp"

template<typename T>
constexpr auto fabs_static(T x) {
	return (x < 0.0) ? (-x) : (x);
}

template<typename Func>
constexpr double newtonMethod(double x0, Func&& f) {
	using namespace DualNumbers;

	double xn = x0;
	double diff{};

	do {
		dual<double> d = f({ xn, 1.0 });
		diff = d.a() / d.b();
		xn -= diff;
	} while (1.0E-15 < fabs_static(diff));

	return xn;
}

struct squareroot {
	DualNumbers::dual<double> N;

	constexpr squareroot(double n)
		: N{ n, 1.0 }
	{}

	constexpr DualNumbers::dual<double> operator()(const DualNumbers::dual<double>& x) const {
		return x * x - N;
	}
};

constexpr double sqrt_newton(double n) {
	using namespace DualNumbers;

	//return newtonMethod(n, [N = dual<double>{ n, 1.0 }](const dual<double>& x) constexpr {return x * x - N; });
	return newtonMethod(n, squareroot{ n });
}


int main()
{
	using namespace DualNumbers;

	constexpr DualNumbers::dual<double> d1{ 1.0 };
	constexpr DualNumbers::dual<double> d2{ 3.0 };
	constexpr DualNumbers::dual<double> d3 = { 1.0, 1.0 };
	constexpr auto literal1 = 1.0_d;
	constexpr auto literal2 = 3.5_eps;
	constexpr auto literal3 = 1.25_d + 2.34_eps;

	constexpr auto d4 = d1 + d2;
	constexpr double a{ d4 };

	constexpr std::complex<double> z{ 1.0, 2.0 };
	constexpr DualNumbers::dual<double> fromz{ z };

	constexpr bool eq = d1 == d1;
	constexpr bool neq = d1 != d2;
	constexpr bool lt = d1 < d2;
	constexpr bool ltq = d1 <= d1;
	constexpr bool gt = d3 > d2;
	constexpr bool gtq = d2 >= d2;
	constexpr bool gtq2 = d3 >= d2;

	constexpr auto sqrt_n = sqrt_newton(10.0);
	auto sqrt_2 = std::sqrt(2.0);

	std::cout << std::setprecision(16) << sqrt_n << std::endl;
	std::cout << sqrt_2 << std::endl;


	//auto f = []() {return 1; };
	//constexpr auto t = f();
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
