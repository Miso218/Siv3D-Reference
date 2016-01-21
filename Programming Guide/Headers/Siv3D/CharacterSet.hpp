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
# include "String.hpp"

namespace s3d
{
	/// <summary>
	/// 文字セット
	/// </summary>
	/// <remarks>
	/// 文字セットを変換する機能を提供します。
	/// </remarks>
	namespace CharacterSet
	{
		/// <summary>
		/// マルチバイト ASCII 文字列をワイド文字列に変換します。
		/// </summary>
		/// <param name="asciiStr">
		/// ASCII 文字で構成されたマルチバイト文字列
		/// </param>
		/// <remarks>
		/// Widen() 関数より高速に動作します。
		/// </remarks>
		/// <returns>
		/// 変換されたワイド文字列
		/// </returns>
		String WidenAscii(const std::string& asciiStr);

		/// <summary>
		/// ワイド ASCII 文字列をマルチバイト文字列に変換します。
		/// </summary>
		/// <param name="asciiStr">
		/// ASCII 文字で構成されたワイド文字列
		/// </param>
		/// <remarks>
		/// Narrow() 関数より高速に動作します。
		/// </remarks>
		/// <returns>
		/// 変換されたマルチバイト文字列
		/// </returns>
		std::string NarrowAscii(StringView asciiStr);

		/// <summary>
		/// マルチバイト文字列をワイド文字列に変換します。
		/// </summary>
		/// <param name="str">
		/// マルチバイト文字列
		/// </param>
		/// <returns>
		/// 変換されたワイド文字列
		/// </returns>
		String Widen(const std::string& str);

		/// <summary>
		/// ワイド文字列をマルチバイト文字列に変換します。
		/// </summary>
		/// <param name="str">
		/// ワイド文字列
		/// </param>
		/// <returns>
		/// 変換されたマルチバイト文字列
		/// </returns>
		std::string Narrow(StringView str);

		/// <summary>
		/// UTF-8 文字列をワイド文字列に変換します。
		/// </summary>
		/// <param name="str">
		/// UTF-8 文字列
		/// </param>
		/// <returns>
		/// 変換されたワイド文字列
		/// </returns>
		String FromUTF8(const std::string& str);

		/// <summary>
		/// ワイド文字列を UTF-8 文字列に変換します。
		/// </summary>
		/// <param name="str">
		/// ワイド文字列
		/// </param>
		/// <returns>
		/// 変換された UTF-8 文字列
		/// </returns>
		std::string ToUTF8(StringView str);

		/// <summary>
		/// UTF-32 文字列をワイド文字列に変換します。
		/// </summary>
		/// <param name="str">
		/// UTF-32 文字列
		/// </param>
		/// <returns>
		/// 変換されたワイド文字列
		/// </returns>
		String FromUTF32(const std::u32string& str);

		/// <summary>
		/// ワイド文字列を UTF-32 文字列に変換します。
		/// </summary>
		/// <param name="str">
		/// ワイド文字列
		/// </param>
		/// <returns>
		/// 変換された UTF-32 文字列
		/// </returns>
		std::u32string ToUTF32(StringView str);

		/// <summary>
		/// 文字列をパーセントエンコードします。
		/// </summary>
		/// <param name="str">
		/// パーセントエンコードする文字列
		/// </param>
		/// <returns>
		/// 変換された文字列
		/// </returns>
		String PercentEncode(StringView str);
	}
}
