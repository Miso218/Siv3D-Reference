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
# include <memory>
# include "Fwd.hpp"
# include "IReader.hpp"

namespace s3d
{
	/// <summary>
	/// 読み込み用バイナリファイル
	/// </summary>
	class BinaryReader : public IReader
	{
	private:

		class CBinaryReader;

		std::shared_ptr<CBinaryReader> pImpl;

	public:

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		BinaryReader();

		/// <summary>
		/// バイナリファイルを開きます。
		/// </summary>
		/// <param name="path">
		/// ファイルパス
		/// </param>
		explicit BinaryReader(const FilePath& path);

		/// <summary>
		/// バイナリファイルを開きます。
		/// </summary>
		/// <param name="path">
		/// ファイルパス
		/// </param>
		/// <returns>
		/// ファイルのオープンに成功した場合 true, それ以外の場合は false
		/// </returns>
		_Check_return_
		bool open(const FilePath& path);

		/// <summary>
		/// バイナリファイルをクローズします。
		/// </summary>
		/// <returns>
		/// なし
		/// </returns>
		void close();

		/// <summary>
		/// バイナリファイルがオープンされているかを返します。
		/// </summary>
		/// <returns>
		/// ファイルがオープンされている場合 true, それ以外の場合は false
		/// </returns>
		_Check_return_
		bool isOpened() const override;

		/// <summary>
		/// バイナリファイルがオープンされているかを返します。
		/// </summary>
		/// <returns>
		/// ファイルがオープンされている場合 true, それ以外の場合は false
		/// </returns>
		_Check_return_
		explicit operator bool() const { return isOpened(); }

		/// <summary>
		/// バイナリファイルのサイズを返します。
		/// </summary>
		/// <returns>
		/// バイナリファイルのサイズ（バイト）
		/// </returns>
		_Check_return_
		int64 size() const override;

		/// <summary>
		/// 現在の読み込み位置を返します。
		/// </summary>
		/// <returns>
		/// 現在の読み込み位置（バイト）
		/// </returns>
		_Check_return_
		int64 getPos() const override;

		/// <summary>
		/// 読み込み位置を変更します。
		/// </summary>
		/// <param name="pos">
		/// 新しい読み込み位置（バイト）
		/// </param>
		/// <returns>
		/// 読み込み位置の変更に成功した場合 true, それ以外の場合は false
		/// </returns>
		_Check_return_
		bool setPos(int64 pos) override;

		/// <summary>
		/// ファイルを読み飛ばし、読み込み位置を変更します。
		/// </summary>
		/// <param name="offset">
		/// 読み飛ばすサイズ（バイト）
		/// </param>
		/// <returns>
		/// 新しい読み込み位置
		/// </returns>
		_Check_return_
		int64 skip(int64 offset) override;

		/// <summary>
		/// ファイルからデータを読み込みます。
		/// </summary>
		/// <param name="buffer">
		/// 読み込み先
		/// </param>
		/// <param name="size">
		/// 読み込むサイズ（バイト）
		/// </param>
		/// <returns>
		/// 実際に読み込んだサイズ（バイト）
		/// </returns>
		_Check_return_
		int64 read(_Out_writes_bytes_all_(size) void* buffer, int64 size) override;

		/// <summary>
		/// ファイルからデータを読み込みます。
		/// </summary>
		/// <param name="buffer">
		/// 読み込み先
		/// </param>
		/// <param name="pos">
		/// 先頭から数えた読み込み開始位置（バイト）
		/// </param>
		/// <param name="size">
		/// 読み込むサイズ（バイト）
		/// </param>
		/// <returns>
		/// 実際に読み込んだサイズ（バイト）
		/// </returns>
		_Check_return_
		int64 read(_Out_writes_bytes_all_(size) void* buffer, int64 pos, int64 size) override;

		/// <summary>
		/// ファイルからデータを読み込みます。
		/// </summary>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功したら true, それ以外の場合は false
		/// </returns>
		template <class Type>
		_Check_return_ bool read(Type& to)
		{
			static_assert(std::is_trivially_copyable<Type>::value, "BinaryReader::read(): Type must be trivially copyable");

			return IReader::read(to);
		}

		/// <summary>
		/// 読み込み位置を変更しないデータ読み込みをサポートしているかを返します。
		/// </summary>
		/// <returns>
		/// つねに true
		/// </returns>
		_Check_return_
		bool supportsLookahead() const override { return true; }

		/// <summary>
		/// 読み込み位置を変更しないでファイルからデータを読み込みます。
		/// </summary>
		/// <param name="buffer">
		/// 読み込み先
		/// </param>
		/// <param name="size">
		/// 読み込むサイズ（バイト）
		/// </param>
		/// <returns>
		/// 実際に読み込んだサイズ（バイト）
		/// </returns>
		_Check_return_
		int64 lookahead(_Out_writes_bytes_all_(size) void* buffer, int64 size) const override;

		/// <summary>
		/// 読み込み位置を変更しないでファイルからデータを読み込みます。
		/// </summary>
		/// <param name="buffer">
		/// 読み込み先
		/// </param>
		/// <param name="pos">
		/// 先頭から数えた読み込み開始位置（バイト）
		/// </param>
		/// <param name="size">
		/// 読み込むサイズ（バイト）
		/// </param>
		/// <returns>
		/// 実際に読み込んだサイズ（バイト）
		/// </returns>
		_Check_return_
		int64 lookahead(_Out_writes_bytes_all_(size) void* buffer, int64 pos, int64 size) const override;

		/// <summary>
		/// 読み込み位置を変更しないでファイルからデータを読み込みます。
		/// </summary>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功したら true, それ以外の場合は false
		/// </returns>
		template <class Type>
		_Check_return_ bool lookahead(Type& to)
		{
			static_assert(std::is_trivially_copyable<Type>::value, "BinaryReader::lookahead(): Type must be trivially copyable");

			return IReader::lookahead(to);
		}

		/// <summary>
		/// 現在の読み込み位置以降のすべてのデータを読み込み、ByteArray として返します。
		/// </summary>
		/// <returns>
		/// 現在の読み込み位置以降のすべてのデータ
		/// </returns>
		_Check_return_
		ByteArray readWhole();

		/// <summary>
		/// 現在の読み込み位置から指定したサイズのデータを読み込み、ByteArray として返します。
		/// </summary>
		/// <param name="size">
		/// 読み込むサイズ（バイト）
		/// </param>
		/// <returns>
		/// 読み込んだデータ
		/// </returns>
		_Check_return_
		ByteArray readSubset(int64 size);

		/// <summary>
		/// 読み込み範囲を指定してデータを読み込み、ByteArray として返します。
		/// </summary>
		/// <param name="pos">
		/// 先頭から数えた読み込み開始位置（バイト）
		/// </param>
		/// <param name="size">
		/// 読み込むサイズ（バイト）
		/// </param>
		/// <returns>
		/// 読み込んだデータ
		/// </returns>
		_Check_return_
		ByteArray readSubset(int64 pos, int64 size);

		/// <summary>
		/// オープンしているファイルのパスを返します。
		/// </summary>
		/// <remarks>
		/// クローズしている場合は空の文字列です。
		/// </remarks>
		_Check_return_
		FilePath path() const;
	};
}
