﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (C) 2008-2016 Ryo Suzuki
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <functional>
# include <cmath>
# include "MathConstants.hpp"
# include "Lerp.hpp"

# ifndef S3D_EASING_DEFINED
#	define S3D_EASING_DEFINED
# endif

namespace s3d
{
	/// <summary>
	/// イージング
	/// </summary>
	/// <remarks>
	/// イージング関数を提供します。
	/// </remarks>
	namespace Easing
	{
		inline constexpr double Linear(double t)
		{
			return t;
		}

		inline double Sine(double t)
		{
			return 1.0 - std::cos(t * Math::HalfPi);
		}

		inline constexpr double Quad(double t)
		{
			return t * t;
		}

		inline constexpr double Cubic(double t)
		{
			return t * t * t;
		}

		inline constexpr double Quart(double t)
		{
			return (t * t) * (t * t);
		}

		inline constexpr double Quint(double t)
		{
			return (t * t) * (t * t) * t;
		}

		inline double Expo(double t)
		{
			return t == 0.0 ? 0.0 : std::exp2(10.0*(t - 1.0));
		}

		inline double Circ(double t)
		{
			return 1.0 - std::sqrt(1.0 - t * t);
		}

		inline constexpr double Back(double t)
		{
			return t * t * (2.70158 * t - 1.70158);
		}

		inline double ElasticAP(double t, double a, double p)
		{
			if (t == 0)
				return 0.0;
			else if (t == 1)
				return 1.0;

			double s;

			if (a < 1.0)
			{
				a = 1.0;

				s = p / 4.0f;
			}
			else
			{
				s = p / Math::TwoPi * std::asin(1.0 / a);
			}

			t -= 1.0;

			return -(a * std::exp2(10.0 * t) * std::sin((t - s) * Math::TwoPi / p));
		}

		inline double Elastic(double t)
		{
			return ElasticAP(t, 1.0, 0.3);
		}

		inline double Bounce(double t)
		{
			t = 1.0 - t;

			if (t < 1 / 2.75)
			{
				return  1.0 - (7.5625*t*t);
			}
			else if (t < 2 / 2.75)
			{
				t -= 1.5 / 2.75;

				return  1.0 - (7.5625*t*t + 0.75);
			}
			else if (t < 2.5 / 2.75)
			{
				t -= 2.25 / 2.75;

				return  1.0 - (7.5625*t*t + 0.9375);
			}
			else
			{
				t -= 2.625 / 2.75;

				return  1.0 - (7.5625*t*t + 0.984375);
			}
		}

		inline double EaseIn(std::function<double(double)> f, double t)
		{
			return f(t);
		}

		inline double EaseOut(std::function<double(double)> f, double t)
		{
			return 1.0 - f(1.0 - t);
		}

		inline double EaseInOut(std::function<double(double)> f, double t)
		{
			return (t < 0.5) ? f(2.0 * t) * 0.5 : 0.5 + EaseOut(f, 2.0 * t - 1.0) * 0.5;
		}

		template <double Func(double)>
		inline constexpr double EaseIn(double t)
		{
			return Func(t);
		}

		template <double Func(double)>
		inline constexpr double EaseOut(double t)
		{
			return 1.0 - Func(1.0 - t);
		}

		template <double Func(double)>
		inline constexpr double EaseInOut(double t)
		{
			return (t < 0.5) ? Func(2.0 * t) * 0.5 : 0.5 + EaseOut<Func>(2.0 * t - 1.0) * 0.5;
		}

		template <class Type>
		inline Type EaseIn(const Type& start, const Type& end, std::function<double(double)> f, double t)
		{
			return Math::Lerp(start, end, f(t));
		}

		template <class Type>
		inline Type EaseOut(const Type& start, const Type& end, std::function<double(double)> f, double t)
		{
			return Math::Lerp(start, end, EaseOut(f, t));
		}

		template <class Type>
		inline Type EaseInOut(const Type& start, const Type& end, std::function<double(double)> f, double t)
		{
			return Math::Lerp(start, end, EaseInOut(f, t));
		}

		template <double Func(double), class Type>
		inline constexpr Type EaseIn(const Type& start, const Type& end, double t)
		{
			return Math::Lerp(start, end, Func(t));
		}

		template <double Func(double), class Type>
		inline constexpr Type EaseOut(const Type& start, const Type& end, double t)
		{
			return Math::Lerp(start, end, EaseOut<Func>(t));
		}

		template <double Func(double), class Type>
		inline constexpr Type EaseInOut(const Type& start, const Type& end, double t)
		{
			return Math::Lerp(start, end, EaseInOut<Func>(t));
		}
	}
}
