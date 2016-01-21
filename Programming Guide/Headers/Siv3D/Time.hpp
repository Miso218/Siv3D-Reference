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
# include <ctime>
# include "Fwd.hpp"

namespace s3d
{
	/// <summary>
	/// 時間
	/// </summary>
	/// <remarks>
	/// 時間に関する機能を提供します。
	/// </remarks>
	namespace Time
	{
		/// <summary>
		/// コンピューターが起動してからの経過時間をミリ秒で返します。
		/// </summary>
		/// <returns>
		/// コンピューターが起動してからの経過時間（ミリ秒）
		/// </returns>
		uint32 GetMillisec();

		/// <summary>
		/// コンピューターが起動してからの経過時間をミリ秒で返します。
		/// </summary>
		/// <returns>
		/// コンピューターが起動してからの経過時間（ミリ秒）
		/// </returns>
		uint64 GetMillisec64();

		/// <summary>
		/// コンピューターが起動してからの経過時間をマイクロ秒で返します。
		/// </summary>
		/// <returns>
		/// コンピューターが起動してからの経過時間（マイクロ秒）
		/// </returns>
		uint64 GetMicrosec();

		/// <summary>
		/// コンピューターが起動してからの経過時間をナノ秒で返します。
		/// </summary>
		/// <returns>
		/// コンピューターが起動してからの経過時間（ナノ秒）
		/// </returns>
		uint64 GetNanosec();

		/// <summary>
		/// 協定世界時 (UTC)  1970 年 1 月 1 日午前 0 時からの経過時間を秒で返します。
		/// </summary>
		/// <returns>
		/// 1970 年 1 月 1 日午前 0 時からの経過秒数
		/// </returns>
		inline int64 SecSince1970()
		{
			return ::_time64(nullptr);
		}

		/// <summary>
		/// 協定世界時 (UTC)  1601 年 1 月 1 日午前 0 時からの経過時間をミリ秒で返します。
		/// </summary>
		/// <returns>
		/// 1601 年 1 月 1 日午前 0 時からの経過ミリ秒
		/// </returns>
		uint64 MillisecSince1601();

		/// <summary>
		/// 現在の協定世界時 (UTC) との時差を分で返します。
		/// </summary>
		/// <returns>
		/// 現在の協定世界時 (UTC) との時差（分）
		/// </returns>
		int32 UtcOffsetMinutes();
	}
}
