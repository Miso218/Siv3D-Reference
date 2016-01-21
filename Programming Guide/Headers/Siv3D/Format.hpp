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
# include <array>
# include <sstream>
# include "Fwd.hpp"
# include "Optional.hpp"
# include "Number.hpp"
# include "Array.hpp"
# include "String.hpp"
# include "PyFmt.hpp"

namespace s3d
{
	/// <summary>
	/// 値を指定した文字数の文字列に変換します。
	/// </summary>
	/// <param name="value">
	/// 変換する値
	/// </param>
	/// <param name="padding">
	/// 文字数と詰め文字のペア
	/// </param>
	/// <returns>
	/// 変換した文字列
	/// </returns>
	template <class Type>
	inline String Pad(const Type& value, const std::pair<int32, wchar>& padding)
	{
		return Format(value).padLeft(padding.first, padding.second);
	}

	/// <summary>
	/// Format の内部で使用するデータ
	/// </summary>
	struct FormatData
	{
		String string;

		struct DecimalPlace
		{
			constexpr DecimalPlace(int32 v = 5) : value(v) {}
			int32 value;
		} decimalPlace;
	};

	/// <summary>
	/// Format の内部で使用する関数
	/// </summary>
	inline void Format(const FormatData&)
	{
		return;
	}

	/// <summary>
	/// Format の内部で使用する関数
	/// </summary>
	template <class Type, class ... Args>
	inline void Format(FormatData& formatData, const Type& value, const Args& ... args)
	{
		Formatter(formatData, value);

		Format(formatData, args...);
	}

	/// <summary>
	/// Format の内部で使用するクラス
	/// </summary>
	template <class...>
	struct format_validation : std::true_type {};

	/// <summary>
	/// Format の内部で使用するクラス
	/// </summary>
	template <class T, class... TT>
	struct format_validation<T, TT...>
		: std::integral_constant<bool, !std::is_same<std::decay_t<T>, char*>::value && format_validation<TT...>::value> {};

	/// <summary>
	/// 一連の引数を文字列に変換します。
	/// </summary>
	/// <param name="args">
	/// 変換する値
	/// </param>
	/// <returns>
	/// 引数を文字列に変換して連結した文字列
	/// </returns>
	template <class ... Args>
	inline String Format(const Args& ... args)
	{
		static_assert(format_validation<Args...>::value, "type \"char*\" cannot be used in Format()");

		FormatData formatData;

		Format(formatData, args...);

		return std::move(formatData.string);
	}

	/// <summary>
	/// 引数を文字列に変換します。
	/// </summary>
	/// <param name="ch">
	/// 変換する値
	/// </param>
	/// <returns>
	/// 引数を文字列に変換した文字列
	/// </returns>
	inline String Format(wchar ch)
	{
		return String(1, ch);
	}

	/// <summary>
	/// 引数を文字列に変換します。
	/// </summary>
	/// <param name="str">
	/// 変換する文字列
	/// </param>
	/// <returns>
	/// 引数を文字列に変換した文字列
	/// </returns>
	inline String Format(const wchar* const str)
	{
		return String(str);
	}

	/// <summary>
	/// 引数を文字列に変換します。
	/// </summary>
	/// <param name="str">
	/// 変換する文字列
	/// </param>
	/// <returns>
	/// 引数を文字列に変換した文字列
	/// </returns>
	inline String Format(const String& str)
	{
		return str;
	}

	/// <summary>
	/// 引数を文字列に変換します。
	/// </summary>
	/// <param name="str">
	/// 変換する文字列
	/// </param>
	/// <returns>
	/// 引数を文字列に変換した文字列
	/// </returns>
	inline String Format(String&& str)
	{
		return std::move(str);
	}

	/// <summary>
	/// 浮動小数点数を変換する際の小数点以下の最大桁数を指定するマニピュレータを返します。
	/// </summary>
	/// <param name="width">
	/// 小数点以下の最大桁数
	/// </param>
	/// <remarks>
	/// この関数が返すマニピュレータを Format の引数にすると、
	/// 該当 Format() 内のそれ以降の浮動小数点数の変換に適用されます。
	/// </remarks>
	/// <returns>
	/// Format に渡すマニピュレータ
	/// </returns>
	inline constexpr FormatData::DecimalPlace DecimalPlace(int32 width)
	{
		return FormatData::DecimalPlace(width);
	}

	namespace literals
	{
		inline namespace formatting_literals
		{
			inline constexpr FormatData::DecimalPlace operator "" _dp(unsigned long long width)
			{
				return DecimalPlace(static_cast<int32>(width));
			}
		}
	}

	inline void Formatter(FormatData& formatData, int32 value)
	{
		const detail::FormatInt buffer(value);
		formatData.string.append(buffer.c_str(), buffer.size());
	}

	inline void Formatter(FormatData& formatData, uint32 value)
	{
		const detail::FormatInt buffer(value);
		formatData.string.append(buffer.c_str(), buffer.size());
	}

	inline void Formatter(FormatData& formatData, int64 value)
	{
		const detail::FormatInt buffer(value);
		formatData.string.append(buffer.c_str(), buffer.size());
	}

	inline void Formatter(FormatData& formatData, uint64 value)
	{
		const detail::FormatInt buffer(value);
		formatData.string.append(buffer.c_str(), buffer.size());
	}

	inline void Formatter(FormatData& formatData, double value)
	{
		formatData.string.append(ToString(value, formatData.decimalPlace.value));
	}

	void Formatter(FormatData& formatData, const void* value);

	void Formatter(FormatData& formatData, const char* const str) = delete;

	inline void Formatter(FormatData& formatData, std::nullptr_t)
	{
		formatData.string.append(L"null");
	}

	inline void Formatter(FormatData& formatData, bool value)
	{
		static const String sBool[2] = { L"false", L"true" };
		formatData.string.append(sBool[value]);
	}

	inline void Formatter(FormatData& formatData, int8 value)
	{
		Formatter(formatData, static_cast<int32>(value));
	}

	inline void Formatter(FormatData& formatData, uint8 value)
	{
		Formatter(formatData, static_cast<uint32>(value));
	}

	inline void Formatter(FormatData& formatData, char value)
	{
		Formatter(formatData, static_cast<int32>(value));
	}

	inline void Formatter(FormatData& formatData, int16 value)
	{
		Formatter(formatData, static_cast<int32>(value));
	}

	inline void Formatter(FormatData& formatData, uint16 value)
	{
		Formatter(formatData, static_cast<uint32>(value));
	}

	inline void Formatter(FormatData& formatData, long value)
	{
		Formatter(formatData, static_cast<int32>(value));
	}

	inline void Formatter(FormatData& formatData, unsigned long value)
	{
		Formatter(formatData, static_cast<uint32>(value));
	}

	inline void Formatter(FormatData& formatData, wchar value)
	{
		formatData.string.push_back(value);
	}

	inline void Formatter(FormatData& formatData, float value)
	{
		Formatter(formatData, static_cast<double>(value));
	}

	inline void Formatter(FormatData& formatData, const FormatData::DecimalPlace decimalPlace)
	{
		formatData.decimalPlace = decimalPlace;
	}

	inline void Formatter(FormatData& formatData, const wchar* const str)
	{
		formatData.string.append(str);
	}

	inline void Formatter(FormatData& formatData, const String& str)
	{
		formatData.string.append(str);
	}

	template <class Type>
	inline void Formatter(FormatData& formatData, const Optional<Type>& opt)
	{
		if (opt)
		{
			formatData.string.append(L"Optional ");

			Formatter(formatData, static_cast<Type>(opt.value()));
		}
		else
		{
			formatData.string.append(L"none");
		}
	}

	inline void Formatter(FormatData& formatData, nullopt_t)
	{
		formatData.string.append(L"none");
	}

	template <class Iterator>
	inline void Formatter(FormatData& formatData, Iterator begin, Iterator end)
	{
		formatData.string.push_back(L'{');

		for (bool b = false; begin != end; ++begin)
		{
			if (std::exchange(b, true))
			{
				formatData.string.push_back(L',');
			}

			Formatter(formatData, *begin);
		}

		formatData.string.push_back(L'}');
	}

	template <class Type, size_t N>
	inline void Formatter(FormatData& formatData, const Type(&values)[N])
	{
		Formatter(formatData, std::begin(values), std::end(values));
	}

	template <class Type, size_t N>
	inline void Formatter(FormatData& formatData, const std::array<Type, N>& v)
	{
		Formatter(formatData, v.begin(), v.end());
	}

	template <class Type>
	inline void Formatter(FormatData& formatData, const std::vector<Type>& v)
	{
		Formatter(formatData, v.begin(), v.end());
	}

	template <class Type>
	inline void Formatter(FormatData& formatData, const std::initializer_list<Type>& ilist)
	{
		Formatter(formatData, ilist.begin(), ilist.end());
	}

	inline void Formatter(FormatData& formatData, const BoolArray& v)
	{
		Formatter(formatData, v.begin(), v.end());
	}

	template <class Type>
	inline void Formatter(FormatData& formatData, const Type& value)
	{
		std::wostringstream wos;

		wos << value;

		formatData.string.append(wos.str());
	}

	void Formatter(FormatData& formatData, const Color& value);
	void Formatter(FormatData& formatData, const ColorF& value);
	void Formatter(FormatData& formatData, const HSV& value);
	void Formatter(FormatData& formatData, const Point& value);
	void Formatter(FormatData& formatData, const Float2& value);
	void Formatter(FormatData& formatData, const Vec2& value);
	void Formatter(FormatData& formatData, const Float3& value);
	void Formatter(FormatData& formatData, const Vec3& value);
	void Formatter(FormatData& formatData, const Float4& value);
	void Formatter(FormatData& formatData, const Vec4& value);
	void Formatter(FormatData& formatData, const __m128& value);
	void Formatter(FormatData& formatData, const Circular& value);
	void Formatter(FormatData& formatData, const LineInt& value);
	void Formatter(FormatData& formatData, const Line& value);
	void Formatter(FormatData& formatData, const Rect& value);
	void Formatter(FormatData& formatData, const RectF& value);
	void Formatter(FormatData& formatData, const Circle& value);
	void Formatter(FormatData& formatData, const Ellipse& value);
	void Formatter(FormatData& formatData, const Triangle& value);
	void Formatter(FormatData& formatData, const Quad& value);
	void Formatter(FormatData& formatData, const RoundRect& value);
	void Formatter(FormatData& formatData, const Mat3x2& value);
	void Formatter(FormatData& formatData, const Quaternion& value);
	void Formatter(FormatData& formatData, const Mat4x4& value);
	void Formatter(FormatData& formatData, const Ray& value);
}
