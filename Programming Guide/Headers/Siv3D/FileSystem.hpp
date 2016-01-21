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
# include "Array.hpp"
# include "String.hpp"
# include "Optional.hpp"
# include "DateTime.hpp"

namespace s3d
{
	/// <summary>
	/// ファイル作成時のオープンモード
	/// </summary>
	/// <remarks>
	/// 書き込み用ファイル作成時に同名の既存ファイルがある場合の動作を示します。
	/// </remarks>
	enum class OpenMode
	{
		/// <summary>
		/// 同名のファイルが存在する場合、それを破棄する。
		/// </summary>
		Trunc,

		/// <summary>
		/// 同名のファイルが存在する場合、それに追加書き込みする。
		/// </summary>
		Append,
	};

	/// <summary>
	/// テキストファイルのエンコーディング形式
	/// </summary>
	/// <remarks>
	/// テキストファイルの文字エンコーディング形式を示します。
	/// </remarks>
	enum class TextEncoding
	{
		/// <summary>
		/// ANSI
		/// </summary>
		ANSI,

		/// <summary>
		/// UTF-8
		/// </summary>
		UTF8,

		/// <summary>
		/// UTF-16 リトルエンディアン
		/// </summary>
		UTF16LE,

		/// <summary>
		/// UTF-16 ビッグエンディアン
		/// </summary>
		UTF16BE,

		/// <summary>
		/// デフォルト [UTF-8]
		/// </summary>
		Default = UTF8,
	};

	/// <summary>
	/// ファイルコピー時の動作
	/// </summary>
	/// <remarks>
	/// ファイルをコピーする際の動作を示します。
	/// </remarks>
	enum class CopyOption
	{
		/// <summary>
		/// ファイル名が既に使われていた場合、コピーを失敗させる。
		/// </summary>
		Fail_if_exists,

		/// <summary>
		/// ファイル名が既に使われていた場合、ファイルを上書きする。
		/// </summary>
		Overwrite_if_exists,

		/// <summary>
		/// ファイル名が既に使われていた場合、別名でコピーする。
		/// </summary>
		Rename_if_exists,
	};

	/// <summary>
	/// ファイルとディレクトリ
	/// </summary>
	/// <remarks>
	/// ファイルとディレクトリを操作する機能を提供します。
	/// </remarks>
	namespace FileSystem
	{
		/// <summary>
		/// 指定したパスのファイルまたはディレクトリが存在するかを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 存在する場合は true, それ以外の場合は false
		/// </returns>
		bool Exists(const FilePath& path);

		/// <summary>
		/// 指定したパスがディレクトリであるかを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ディレクトリである場合は true, それ以外の場合は false
		/// </returns>
		bool IsDirectory(const FilePath& path);

		/// <summary>
		/// 指定したパスがファイルであるかを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ファイルである場合は true, それ以外の場合は false
		/// </returns>
		bool IsFile(const FilePath& path);

		/// <summary>
		/// 指定したパスが exe に埋め込まれたリソースであるかを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// リソースである場合は true, それ以外の場合は false
		/// </returns>
		bool IsResource(const FilePath& path);

		/// <summary>
		/// 指定したファイルの絶対パスを返します。（例: L"C:/Users/Siv/Desktop/picture.png"）
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ファイルの絶対パス。失敗した場合は空の文字列
		/// </returns>
		FilePath FullPath(const FilePath& path);

		/// <summary>
		/// 指定したファイルの .を含まない拡張子を小文字にして返します。（例: L"png"）
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 小文字の拡張子。失敗した場合は空の文字列
		/// </returns>
		String Extension(const FilePath& path);

		/// <summary>
		/// 指定したファイルの、親ディレクトリを含まずに、拡張子を含んだ名前を返します。（例: L"picture.png"）
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ファイル名。失敗した場合は空の文字列
		/// </returns>
		String FileName(const FilePath& path);

		/// <summary>
		/// 指定したファイルの、親ディレクトリと拡張子を含まない名前を返します。（例: L"picture"）
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ファイル名。失敗した場合は空の文字列
		/// </returns>
		String BaseName(const FilePath& path);

		/// <summary>
		/// 指定したファイルの親ディレクトリを返します。（例: L"C:/Users/Siv/Desktop/"）
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 親ディレクトリ。失敗した場合は空の文字列
		/// </returns>
		FilePath ParentPath(const FilePath& path);

		/// <summary>
		/// 指定したファイルのドライブのパスを返します。（例: L"C:/"）
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ドライブのパス。失敗した場合は空の文字列
		/// </returns>
		FilePath VolumePath(const FilePath& path);

		/// <summary>
		/// 指定したファイルのパスを正規化します
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 正規化したパス
		/// </returns>
		FilePath NormalizedPath(const FilePath& path);

		/// <summary>
		/// 指定したファイルかディレクトリが空であるかを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 成功の場合は true, それ以外の場合は false
		/// </returns>
		bool IsEmpty(const FilePath& path);

		/// <summary>
		/// 指定したファイルかディレクトリのサイズを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <remarks>
		/// ファイルやディレクトリが存在しなかったり、空である場合は 0 を返します。
		/// </remarks>
		/// <returns>
		/// サイズ
		/// </returns>
		int64 Size(const FilePath& path);

		/// <summary>
		/// 指定したファイルのサイズを返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <remarks>
		/// ファイルが存在しなかったり、空である場合は 0 を返します。
		/// </remarks>
		/// <returns>
		/// サイズ
		/// </returns>
		int64 FileSize(const FilePath& path);

		/// <summary>
		/// ファイルまたはディレクトリの作成日時を返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 作成日時。ファイルが存在しない場合 none
		/// </returns>
		Optional<DateTime> CreationTime(const FilePath& path);

		/// <summary>
		/// ファイルまたはディレクトリの更新日時を返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 更新日時。ファイルが存在しない場合 none
		/// </returns>
		Optional<DateTime> WriteTime(const FilePath& path);

		/// <summary>
		/// ファイルまたはディレクトリのアクセス日時を返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// アクセス日時。ファイルが存在しない場合 none
		/// </returns>
		Optional<DateTime> AccessTime(const FilePath& path);

		/// <summary>
		/// ファイルまたはディレクトリの中身をコピーします。
		/// </summary>
		/// <param name="from">
		/// コピーするパス
		/// </param>
		/// <param name="to">
		/// コピー先のパス
		/// </param>
		/// <param name="copyOption">
		/// 名前衝突時のふるまい
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool Copy(const FilePath& from, const FilePath& to, CopyOption copyOption = CopyOption::Fail_if_exists);

		/// <summary>
		/// ディレクトリを作成します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool CreateDirectories(const FilePath& path);

		/// <summary>
		/// 指定したパスまでの親ディレクトリを作成します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool CreateParentDirectories(const FilePath& path);

		/// <summary>
		/// ファイルまたはディレクトリを削除します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <param name="allowUndo">
		/// ごみ箱に送る場合は true, それ以外の場合は false
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool Remove(const FilePath& path, bool allowUndo = false);

		/// <summary>
		/// ディレクトリの中身を削除します。
		/// </summary>
		/// <param name="path">
		/// ディレクトリのパス
		/// </param>
		/// <param name="allowUndo">
		/// ごみ箱に送る場合は true, それ以外の場合は false
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool RemoveContents(const FilePath& path, bool allowUndo = false);

		/// <summary>
		/// ファイルまたはディレクトリの名前を変更します。
		/// </summary>
		/// <param name="from">
		/// 変更前のパス
		/// </param>
		/// <param name="to">
		/// 変更後のパス
		/// </param>
		/// <param name="copyOption">
		/// 名前衝突時のふるまい
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool Rename(const FilePath& from, const FilePath& to, CopyOption copyOption = CopyOption::Fail_if_exists);

		/// <summary>
		/// ファイルまたはディレクトリを移動します。
		/// </summary>
		/// <param name="from">
		/// 移動前のディレクトリのパス
		/// </param>
		/// <param name="to">
		/// 移動後のディレクトリのパス
		/// </param>
		/// <param name="copyOption">
		/// 名前衝突時のふるまい
		/// </param>
		/// <remarks>
		/// この関数は Rename() と同じです。
		/// </remarks>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool Move(const FilePath& from, const FilePath& to, CopyOption copyOption = CopyOption::Fail_if_exists);

		/// <summary>
		/// ディレクトリの中身を移動します。
		/// </summary>
		/// <param name="from">
		/// 移動前のディレクトリのパス
		/// </param>
		/// <param name="to">
		/// 移動後のディレクトリのパス
		/// </param>
		/// <param name="copyOption">
		/// 名前衝突時のふるまい
		/// </param>
		/// <returns>
		/// 成功した場合は true, それ以外の場合は false
		/// </returns>
		bool MoveContents(const FilePath& from, const FilePath& to, CopyOption copyOption = CopyOption::Fail_if_exists);

		/// <summary>
		/// 指定したディレクトリにあるファイルとディレクトリの一覧を返します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <returns>
		/// ファイルとディレクトリの一覧
		/// </returns>
		Array<FilePath> DirectoryContents(const FilePath& path);

		/// <summary>
		/// プログラムが起動したパスを返します。
		/// </summary>
		/// <returns>
		/// プログラムが起動したパス
		/// </returns>
		FilePath InitialPath();

		/// <summary>
		/// カレントパスを返します。
		/// </summary>
		/// <returns>
		/// カレントパス
		/// </returns>
		FilePath CurrentPath();

		/// <summary>
		/// 一時ファイル用のディレクトリのパスを返します。パスの末尾には L'\\' が付きます。
		/// </summary>
		/// <returns>
		/// 一時ファイル用のディレクトリのパス
		/// </returns>
		FilePath TemporaryPath();

		/// <summary>
		/// 一時ファイル用の固有なファイルパスを返します。拡張子は L".tmp" です。
		/// </summary>
		/// <returns>
		/// 一時ファイル用のファイルパス
		/// </returns>
		FilePath UniquePath();

		/// <summary>
		/// 現在のアプリケーションの実行可能ファイル (.exe) の完全パスを返します。
		/// </summary>
		/// <returns>
		/// 現在のアプリケーションの完全パス
		/// </returns>
		FilePath ModulePath();

		/// <summary>
		/// 指定したパスを相対パスに変換します。
		/// </summary>
		/// <param name="path">
		/// パス
		/// </param>
		/// <param name="start">
		/// 相対パスの基準位置
		/// </param>
		/// <returns>
		/// 相対パス
		/// </returns>
		FilePath Relative(const FilePath& path, const FilePath& start = FileSystem::CurrentPath());

		/// <summary>
		/// ファイルがテキストファイルである場合、そのエンコーディング形式を返します。
		/// </summary>
		/// <param name="path">
		/// ファイルパス
		/// </param>
		/// <param name="bomSize">
		/// [out] テキストファイルに含まれている BOM のサイズ
		/// </param>
		/// <returns>
		/// テキストファイルのエンコーディング形式
		/// </returns>
		TextEncoding GetEncoding(const FilePath& path, int32& bomSize);

		/// <summary>
		/// ファイルがテキストファイルである場合、そのエンコーディング形式を返します。
		/// </summary>
		/// <param name="reader">
		/// IReader
		/// </param>
		/// <param name="bomSize">
		/// [out] テキストファイルに含まれている BOM のサイズ
		/// </param>
		/// <returns>
		/// テキストファイルのエンコーディング形式
		/// </returns>
		TextEncoding GetEncoding(const IReader& reader, int32& bomSize);
	}
}
