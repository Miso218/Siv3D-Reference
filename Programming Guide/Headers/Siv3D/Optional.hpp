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
# include <type_traits>
# include <stdexcept>
# include <cassert>
# include <functional>

//////////////////////////////////////////////////////////////////////////////
//
//	Copyright (C) 2011 - 2012 Andrzej Krzemienski.
//
//	Use, modification, and distribution is subject to the Boost Software
//	License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//	http://www.boost.org/LICENSE_1_0.txt)
//

# define OPTIONAL_REQUIRES(...) typename std::enable_if_t<__VA_ARGS__::value, bool> = false

namespace s3d
{
	template <class Type> class Optional;

	template <class Type> class Optional<Type&>;

	// workaround: std utility functions aren't constexpr yet
	template <class Type>
	inline constexpr Type&& constexpr_forward(typename std::remove_reference<Type>::type& t) noexcept
	{
		return static_cast<Type&&>(t);
	}

	template <class Type>
	inline constexpr Type&& constexpr_forward(typename std::remove_reference<Type>::type&& t) noexcept
	{
		static_assert(!std::is_lvalue_reference<Type>::value, "!!");
		return static_cast<Type&&>(t);
	}

	template <class Type>
	inline constexpr typename std::remove_reference<Type>::type&& constexpr_move(Type&& t) noexcept
	{
		return static_cast<typename std::remove_reference<Type>::type&&>(t);
	}

	namespace detail_
	{
		template <class Type>
		struct has_overloaded_addressof
		{
			template <class U>
			static constexpr bool has_overload(...) { return false; }

			template <class U, size_t S = sizeof(std::declval<U&>().operator&())>
			static constexpr bool has_overload(bool) { return true; }

			constexpr static bool value = has_overload<Type>(true);
		};

		template <class Type, OPTIONAL_REQUIRES(!has_overloaded_addressof<Type>)>
		constexpr Type* static_addressof(Type& ref)
		{
			return &ref;
		}

		template <class Type, OPTIONAL_REQUIRES(has_overloaded_addressof<Type>)>
		Type* static_addressof(Type& ref)
		{
			return std::addressof(ref);
		}

		template <class U>
		U convert(U v)
		{
			return v;
		}
	}

	constexpr struct trivial_init_t {} trivial_init{};

	constexpr struct in_place_t {} in_place{};

	struct nullopt_t
	{
		struct init {};

		constexpr explicit nullopt_t(init) {}
	};

	constexpr nullopt_t nullopt{ nullopt_t::init() };

	/// <summary>
	/// 無効値
	/// </summary>
	constexpr nullopt_t none{ nullopt };

	class bad_optional_access : public std::logic_error
	{
	public:
		explicit bad_optional_access(const std::string& what_arg)
			: logic_error{ what_arg } {}

		explicit bad_optional_access(const char* what_arg)
			: logic_error{ what_arg } {}
	};

	template <class Type>
	union storage_t
	{
		unsigned char dummy_;
		
		Type value_;

		constexpr storage_t(trivial_init_t) noexcept
			: dummy_() {};

		template <class... Args>
		constexpr storage_t(Args&&... args)
			: value_(constexpr_forward<Args>(args)...) {}

		~storage_t() {}
	};

	template <class Type>
	union constexpr_storage_t
	{
		unsigned char dummy_;
		
		Type value_;

		constexpr constexpr_storage_t(trivial_init_t) noexcept
			: dummy_() {};

		template <class... Args>
		constexpr constexpr_storage_t(Args&&... args)
			: value_(constexpr_forward<Args>(args)...) {}

		~constexpr_storage_t() = default;
	};

	template <class Type>
	struct Optional_base
	{
		bool init_;
		
		storage_t<Type> storage_;

		constexpr Optional_base() noexcept
			: init_(false)
			, storage_(trivial_init) {};

		explicit constexpr Optional_base(const Type& v)
			: init_(true)
			, storage_(v) {}

		explicit constexpr Optional_base(Type&& v)
			: init_(true)
			, storage_(constexpr_move(v)) {}

		template <class... Args> explicit Optional_base(in_place_t, Args&&... args)
			: init_(true)
			, storage_(constexpr_forward<Args>(args)...) {}

		template <class U, class... Args, OPTIONAL_REQUIRES(std::is_constructible<Type, std::initializer_list<U>>)>
		explicit Optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
			: init_(true)
			, storage_(il, std::forward<Args>(args)...) {}

		~Optional_base()
		{
			if (init_)
			{
				storage_.value_.~Type();
			}
		}
	};

	template <class Type>
	struct constexpr_Optional_base
	{
		bool init_;

		constexpr_storage_t<Type> storage_;

		constexpr constexpr_Optional_base() noexcept
			: init_(false)
			, storage_(trivial_init) {};

		explicit constexpr constexpr_Optional_base(const Type& v)
			: init_(true)
			, storage_(v) {}

		explicit constexpr constexpr_Optional_base(Type&& v)
			: init_(true)
			, storage_(constexpr_move(v)) {}

		template <class... Args> explicit constexpr constexpr_Optional_base(in_place_t, Args&&... args)
			: init_(true)
			, storage_(constexpr_forward<Args>(args)...) {}

		template <class U, class... Args, OPTIONAL_REQUIRES(std::is_constructible<Type, std::initializer_list<U>>)>
		constexpr explicit constexpr_Optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
			: init_(true)
			, storage_(il, std::forward<Args>(args)...) {}

		~constexpr_Optional_base() = default;
	};

	template <class Type>
	using OptionalBase = typename std::conditional<std::is_trivially_destructible<Type>::value, constexpr_Optional_base<Type>, Optional_base<Type>>::type;

	/// <summary>
	/// Optional
	/// </summary>
	template <class Type>
	class Optional : private OptionalBase<Type>
	{
		static_assert(!std::is_same<typename std::decay<Type>::type, nullopt_t>::value, "bad T");
		static_assert(!std::is_same<typename std::decay<Type>::type, in_place_t>::value, "bad T");

		constexpr bool initialized() const noexcept
		{
			return OptionalBase<Type>::init_;
		}

		Type* dataptr()
		{
			return std::addressof(OptionalBase<Type>::storage_.value_);
		}
		
		constexpr const Type* dataptr() const
		{
			return detail_::static_addressof(OptionalBase<Type>::storage_.value_);
		}

		constexpr const Type& contained_val() const&
		{
			return OptionalBase<Type>::storage_.value_;
		}
		
		Type& contained_val() &
		{
			return OptionalBase<Type>::storage_.value_;
		}
		
		Type&& contained_val() &&
		{
			return std::move(OptionalBase<Type>::storage_.value_);
		}

		void clear() noexcept
		{
			if (initialized())
			{
				dataptr()->~Type();
			}

			OptionalBase<Type>::init_ = false;
		}

		template <class... Args>
		void initialize(Args&&... args) noexcept(noexcept(Type(std::forward<Args>(args)...)))
		{
			assert(!OptionalBase<Type>::init_);
			::new (static_cast<void*>(dataptr())) Type(std::forward<Args>(args)...);
			OptionalBase<Type>::init_ = true;
		}

		template <class U, class... Args>
		void initialize(std::initializer_list<U> il, Args&&... args) noexcept(noexcept(Type(il, std::forward<Args>(args)...)))
		{
			assert(!OptionalBase<Type>::init_);
			::new (static_cast<void*>(dataptr())) Type(il, std::forward<Args>(args)...);
			OptionalBase<Type>::init_ = true;
		}

	public:

		using value_type = Type;

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		constexpr Optional() noexcept
			: OptionalBase<Type>() {};
		
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param>
		/// 無効値
		/// </param>
		constexpr Optional(nullopt_t) noexcept
			: OptionalBase<Type>() {};

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		Optional(const Optional& rhs)
			: OptionalBase<Type>()
		{
			if (rhs.initialized()) {
				::new (static_cast<void*>(dataptr())) Type(*rhs);
				OptionalBase<Type>::init_ = true;
			}
		}

		/// <summary>
		/// ムーブコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		Optional(Optional&& rhs) noexcept(std::is_nothrow_move_constructible<Type>::value)
			: OptionalBase<Type>()
		{
			if (rhs.initialized())
			{
				::new (static_cast<void*>(dataptr())) Type(std::move(*rhs));
				OptionalBase<Type>::init_ = true;
			}
		}

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		constexpr Optional(const Type& v)
			: OptionalBase<Type>(v) {}

		/// <summary>
		/// ムーブコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		constexpr Optional(Type&& v)
			: OptionalBase<Type>(constexpr_move(v)) {}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param="args">
		/// 値のコンストラクタ引数
		/// </param>
		template <class... Args>
		constexpr explicit Optional(in_place_t, Args&&... args)
			: OptionalBase<Type>(in_place_t{}, constexpr_forward<Args>(args)...) {}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param="args">
		/// 値のコンストラクタ引数
		/// </param>
		template <class U, class... Args, OPTIONAL_REQUIRES(std::is_constructible<Type, std::initializer_list<U>>)>
		constexpr explicit Optional(in_place_t, std::initializer_list<U> il, Args&&... args)
			: OptionalBase<Type>(in_place_t{}, il, constexpr_forward<Args>(args)...) {}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Optional() = default;

		/// <summary>
		/// 代入演算子
		/// </summary>
		/// <returns>
		/// *this
		/// </returns>
		Optional& operator = (nullopt_t) noexcept
		{
			clear();
			return *this;
		}

		/// <summary>
		/// 代入演算子
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		/// <returns>
		/// *this
		/// </returns>
		Optional& operator = (const Optional& rhs)
		{
			if (initialized() == true && rhs.initialized() == false) clear();
			else if (initialized() == false && rhs.initialized() == true)  initialize(*rhs);
			else if (initialized() == true && rhs.initialized() == true)  contained_val() = *rhs;
			return *this;
		}

		/// <summary>
		/// ムーブ代入演算子
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		/// <returns>
		/// *this
		/// </returns>
		Optional& operator = (Optional&& rhs)
			noexcept(std::is_nothrow_move_assignable<Type>::value && std::is_nothrow_move_constructible<Type>::value)
		{
			if (initialized() == true && rhs.initialized() == false) clear();
			else if (initialized() == false && rhs.initialized() == true)  initialize(std::move(*rhs));
			else if (initialized() == true && rhs.initialized() == true)  contained_val() = std::move(*rhs);
			return *this;
		}

		/// <summary>
		/// ムーブ代入演算子
		/// </summary>
		/// <param="v">
		/// 他のオブジェクト
		/// </param>
		/// <returns>
		/// *this
		/// </returns>
		template <class U>
		auto operator = (U&& v)
			-> typename std::enable_if<std::is_same<typename std::decay<U>::type, Type>::value, Optional&>::type
		{
			if (initialized()) { contained_val() = std::forward<U>(v); }
			else { initialize(std::forward<U>(v)); }
			return *this;
		}

		/// <summary>
		/// Optional オブジェクトを初期化します。
		/// </summary>
		/// <param="args">
		/// 値のコンストラクタ引数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		template <class... Args>
		void emplace(Args&&... args)
		{
			clear();
			initialize(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Optional オブジェクトを初期化します。
		/// </summary>
		/// <param="il">
		/// 値のコンストラクタ引数
		/// </param>
		/// <param="args">
		/// 値のコンストラクタ引数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		template <class U, class... Args>
		void emplace(std::initializer_list<U> il, Args&&... args)
		{
			clear();
			initialize<U, Args...>(il, std::forward<Args>(args)...);
		}

		/// <summary>
		/// 別の Optional オブジェクトと中身を入れ替えます。
		/// </summary>
		/// <param name="another">
		/// 別の Optional オブジェクト
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void swap(Optional<Type>& rhs) noexcept(std::is_nothrow_move_constructible<Type>::value && noexcept(std::swap(std::declval<Type&>(), std::declval<Type&>())))
		{
			if (initialized() == true && rhs.initialized() == false) { rhs.initialize(std::move(**this)); clear(); }
			else if (initialized() == false && rhs.initialized() == true) { initialize(std::move(*rhs)); rhs.clear(); }
			else if (initialized() == true && rhs.initialized() == true) { using std::swap; swap(**this, *rhs); }
		}

		/// <summary>
		/// 中身があるかを返します。
		/// </summary>
		/// <returns>
		/// 中身が none ではない場合 true, それ以外の場合は false
		/// </returns>
		constexpr explicit operator bool() const noexcept
		{
			return initialized();
		}

		/// <summary>
		/// 中身があるかを返します。
		/// </summary>
		/// <returns>
		/// 中身が none ではない場合 true, それ以外の場合は false
		/// </returns>
		constexpr bool has_value() const noexcept
		{
			return initialized();
		}

		/// <summary>
		/// 中身へのポインタを返します。
		/// </summary>
		/// <remarks>
		/// 中身が none の場合の動作は未定義です。
		/// </remarks>
		/// <returns>
		/// 中身へのポインタ
		/// </returns>
		constexpr Type const* operator -> () const
		{
			assert(initialized());
			return dataptr();
		}

		/// <summary>
		/// 中身へのポインタを返します。
		/// </summary>
		/// <remarks>
		/// 中身が none の場合の動作は未定義です。
		/// </remarks>
		/// <returns>
		/// 中身へのポインタ
		/// </returns>
		Type* operator -> ()
		{
			assert(initialized());
			return dataptr();
		}

		/// <summary>
		/// 中身の参照を返します。
		/// </summary>
		/// <remarks>
		/// 中身が none の場合の動作は未定義です。
		/// </remarks>
		/// <returns>
		/// 中身の参照
		/// </returns>
		constexpr Type const& operator * () const
		{
			assert(initialized());
			return contained_val();
		}

		/// <summary>
		/// 中身の参照を返します。
		/// </summary>
		/// <remarks>
		/// 中身が none の場合の動作は未定義です。
		/// </remarks>
		/// <returns>
		/// 中身の参照
		/// </returns>
		Type& operator * ()
		{
			assert(initialized());
			return contained_val();
		}

		/// <summary>
		/// 中身の参照を返します。
		/// </summary>
		/// <exception cref="bad_optional_access">
		/// 中身が none の場合 throw されます。
		/// </exception>
		/// <returns>
		/// 中身の参照
		/// </returns>
		constexpr Type const& value() const
		{
			if (!initialized())
			{
				throw bad_optional_access("bad Optional access");
			}

			return contained_val();
		}

		/// <summary>
		/// 中身の参照を返します。
		/// </summary>
		/// <exception cref="bad_optional_access">
		/// 中身が none の場合 throw されます。
		/// </exception>
		/// <returns>
		/// 中身の参照
		/// </returns>
		Type& value()
		{
			if (!initialized())
			{
				throw bad_optional_access("bad Optional access");
			}

			return contained_val();
		}

		/// <summary>
		/// 中身がある場合はその値を、それ以外の場合は v を返します。
		/// </summary>
		/// <param name="v">
		/// 中身が none の場合に返ってくる値
		/// </param>
		/// <returns>
		/// 中身がある場合はその値、それ以外の場合は v
		/// </returns>
		template <class V>
		constexpr Type value_or(V&& v) const&
		{
			return *this ? **this : detail_::convert<Type>(constexpr_forward<V>(v));
		}

		/// <summary>
		/// 中身がある場合はその値を、それ以外の場合は v を返します。
		/// </summary>
		/// <param name="v">
		/// 中身が none の場合に返ってくる値
		/// </param>
		/// <returns>
		/// 中身がある場合はその値、それ以外の場合は v
		/// </returns>
		template <class V>
		Type value_or(V&& v) &&
		{
			return *this ? constexpr_move(const_cast<Optional<Type>&>(*this).contained_val()) : detail_::convert<Type>(constexpr_forward<V>(v));
		}

		/// <summary>
		/// 中身を none にします。
		/// </summary>
		/// <returns>
		/// なし
		/// </returns>
		void reset() noexcept
		{
			clear();
		}

		/// <summary>
		/// 中身に別の値を代入します。
		/// </summary>
		/// <param name="v">
		/// コピーする値
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void reset(const Type& v)
		{
			emplace(v);
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f を呼びます。
		/// </summary>
		/// <param name="f">
		/// 中身の値と同じ型を引数にとる関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(Type&)> f)
		{
			if (initialized())
			{
				f(value());
			}
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f を呼びます。
		/// </summary>
		/// <param name="f">
		/// 中身があるときに呼び出す関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(const Type&)> f) const
		{
			if (initialized())
			{
				f(value());
			}
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f1 を呼び、それ以外の場合に関数 f2 を呼びます。
		/// </summary>
		/// <param name="f1">
		/// 中身があるときに呼び出す関数
		/// </param>
		/// </summary>
		/// <param name="f2">
		/// 中身が無いときに呼び出す関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(Type&)> f1, std::function<void()> f2)
		{
			if (initialized())
			{
				f1(value());
			}
			else
			{
				f2();
			}
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f1 を呼び、それ以外の場合に関数 f2 を呼びます。
		/// </summary>
		/// <param name="f1">
		/// 中身があるときに呼び出す関数
		/// </param>
		/// </summary>
		/// <param name="f2">
		/// 中身が無いときに呼び出す関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(const Type&)> f1, std::function<void()> f2) const
		{
			if (initialized())
			{
				f1(value());
			}
			else
			{
				f2();
			}
		}
	};

	/// <summary>
	/// Optional
	/// </summary>
	template <class Type>
	class Optional<Type&>
	{
		static_assert(!std::is_same<Type, nullopt_t>::value, "bad Type");
		static_assert(!std::is_same<Type, in_place_t>::value, "bad Type");
			
		Type* ref;

	public:

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		constexpr Optional() noexcept
			: ref(nullptr) {}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param>
		/// 無効値
		/// </param>
		constexpr Optional(nullopt_t) noexcept
			: ref(nullptr) {}

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		constexpr Optional(Type& v) noexcept
			: ref(detail_::static_addressof(v)) {}

		Optional(Type&&) = delete;

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		constexpr Optional(const Optional& rhs) noexcept
			: ref(rhs.ref) {}

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		explicit constexpr Optional(in_place_t, Type& v) noexcept
			: ref(detail_::static_addressof(v)) {}

		explicit Optional(in_place_t, Type&&) = delete;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Optional() = default;

		/// <summary>
		/// 代入演算子
		/// </summary>
		/// <returns>
		/// *this
		/// </returns>
		Optional& operator = (nullopt_t) noexcept
		{
			ref = nullptr;
			return *this;
		}

		//	Optional& operator = (const Optional& rhs) noexcept
		//	{
		//		ref = rhs.ref;
		//		return *this;
		//	}

		//	Optional& operator = (Optional&& rhs) noexcept
		//	{
		//		ref = rhs.ref;
		//		return *this;
		//	}

		/// <summary>
		/// 代入演算子
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		/// <returns>
		/// *this
		/// </returns>
		template <class U>
		auto operator = (U&& rhs) noexcept
			-> typename std::enable_if<std::is_same<typename std::decay<U>::type, Optional<Type&>>::value, Optional&>::type
		{
			ref = rhs.ref;
			return *this;
		}

		/// <summary>
		/// ムーブ代入演算子
		/// </summary>
		/// <param="rhs">
		/// 他の Optional オブジェクト
		/// </param>
		/// <returns>
		/// *this
		/// </returns>
		template <class U>
		auto operator =(U&& rhs) noexcept
			-> typename std::enable_if<!std::is_same<typename std::decay<U>::type, Optional<Type&>>::value, Optional&>::type = delete;

		/// <summary>
		/// Optional オブジェクトを初期化します。
		/// </summary>
		/// <param="v">
		/// 新しい値
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void emplace(Type& v) noexcept
		{
			ref = detail_::static_addressof(v);
		}

		void emplace(Type&&) = delete;

		/// <summary>
		/// 別の Optional オブジェクトと中身を入れ替えます。
		/// </summary>
		/// <param name="another">
		/// 別の Optional オブジェクト
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void swap(Optional<Type&>& rhs) noexcept
		{
			std::swap(ref, rhs.ref);
		}

		/// <summary>
		/// 中身があるかを返します。
		/// </summary>
		/// <returns>
		/// 中身が none ではない場合 true, それ以外の場合は false
		/// </returns>
		explicit constexpr operator bool() const noexcept
		{
			return ref != nullptr;
		}

		/// <summary>
		/// 中身があるかを返します。
		/// </summary>
		/// <returns>
		/// 中身が none ではない場合 true, それ以外の場合は false
		/// </returns>
		constexpr bool has_value() const noexcept
		{
			return ref != nullptr;
		}

		/// <summary>
		/// 中身へのポインタを返します。
		/// </summary>
		/// <remarks>
		/// 中身が none の場合の動作は未定義です。
		/// </remarks>
		/// <returns>
		/// 中身へのポインタ
		/// </returns>
		constexpr Type* operator -> () const
		{
			assert(ref);
			return ref;
		}

		/// <summary>
		/// 中身の参照を返します。
		/// </summary>
		/// <remarks>
		/// 中身が none の場合の動作は未定義です。
		/// </remarks>
		/// <returns>
		/// 中身の参照
		/// </returns>
		constexpr Type& operator * () const
		{
			assert(ref);
			return ref;
		}

		/// <summary>
		/// 中身の参照を返します。
		/// </summary>
		/// <exception cref="bad_optional_access">
		/// 中身が none の場合 throw されます。
		/// </exception>
		/// <returns>
		/// 中身の参照
		/// </returns>
		constexpr Type& value() const
		{
			if (!ref)
			{
				throw bad_optional_access("bad Optional access");		
			}

			return *ref;
		}

		/// <summary>
		/// 中身がある場合はその値を、それ以外の場合は v を返します。
		/// </summary>
		/// <param name="v">
		/// 中身が none の場合に返ってくる値
		/// </param>
		/// <returns>
		/// 中身がある場合はその値、それ以外の場合は v
		/// </returns>
		template <class V>
		constexpr typename std::decay<Type>::type value_or(V&& v) const
		{
			return *this ? **this : detail_::convert<typename std::decay<Type>::type>(constexpr_forward<V>(v));
		}

		/// <summary>
		/// 中身を none にします。
		/// </summary>
		/// <returns>
		/// なし
		/// </returns>
		void reset() noexcept
		{
			*this = nullopt;
		}

		/// <summary>
		/// 中身に別の値を代入します。
		/// </summary>
		/// <param name="v">
		/// コピーする値
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void reset(Type& v) noexcept
		{
			emplace(v);
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f を呼びます。
		/// </summary>
		/// <param name="f">
		/// 中身の値と同じ型を引数にとる関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(Type&)> f)
		{
			if (static_cast<bool>(*this))
			{
				f(value());
			}
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f を呼びます。
		/// </summary>
		/// <param name="f">
		/// 中身があるときに呼び出す関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(const Type&)> f) const
		{
			if (static_cast<bool>(*this))
			{
				f(value());
			}
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f1 を呼び、それ以外の場合に関数 f2 を呼びます。
		/// </summary>
		/// <param name="f1">
		/// 中身があるときに呼び出す関数
		/// </param>
		/// </summary>
		/// <param name="f2">
		/// 中身が無いときに呼び出す関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(Type&)> f1, std::function<void()> f2)
		{
			if (static_cast<bool>(*this))
			{
				f1(value());
			}
			else
			{
				f2();
			}
		}

		/// <summary>
		/// 中身がある場合に、その値を引数に関数 f1 を呼び、それ以外の場合に関数 f2 を呼びます。
		/// </summary>
		/// <param name="f1">
		/// 中身があるときに呼び出す関数
		/// </param>
		/// </summary>
		/// <param name="f2">
		/// 中身が無いときに呼び出す関数
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		void then(std::function<void(const Type&)> f1, std::function<void()> f2) const
		{
			if (static_cast<bool>(*this))
			{
				f1(value());
			}
			else
			{
				f2();
			}
		}
	};

	template <class Type>
	class Optional<Type&&>
	{
		static_assert(sizeof(Type) == 0, "Optional rvalue referencs disallowed");
	};

	template <class Type>
	constexpr bool operator == (const Optional<Type>& x, const Optional<Type>& y)
	{
		return static_cast<bool>(x) != static_cast<bool>(y) ? false : static_cast<bool>(x) == false ? true : *x == *y;
	}

	template <class Type>
	constexpr bool operator != (const Optional<Type>& x, const Optional<Type>& y)
	{
		return !(x == y);
	}

	template <class Type>
	constexpr bool operator < (const Optional<Type>& x, const Optional<Type>& y)
	{
		return (!y) ? false : (!x) ? true : *x < *y;
	}

	template <class Type>
	constexpr bool operator > (const Optional<Type>& x, const Optional<Type>& y)
	{
		return (y < x);
	}

	template <class Type>
	constexpr bool operator <= (const Optional<Type>& x, const Optional<Type>& y)
	{
		return !(y < x);
	}

	template <class Type>
	constexpr bool operator >= (const Optional<Type>& x, const Optional<Type>& y)
	{
		return !(x < y);
	}

	template <class Type>
	constexpr bool operator == (const Optional<Type>& x, nullopt_t) noexcept
	{
		return (!x);
	}

	template <class Type>
	constexpr bool operator == (nullopt_t, const Optional<Type>& x) noexcept
	{
		return (!x);
	}

	template <class Type>
	constexpr bool operator != (const Optional<Type>& x, nullopt_t) noexcept
	{
		return static_cast<bool>(x);
	}

	template <class Type>
	constexpr bool operator != (nullopt_t, const Optional<Type>& x) noexcept
	{
		return static_cast<bool>(x);
	}

	template <class Type>
	constexpr bool operator < (const Optional<Type>&, nullopt_t) noexcept
	{
		return false;
	}

	template <class Type>
	constexpr bool operator < (nullopt_t, const Optional<Type>& x) noexcept
	{
		return static_cast<bool>(x);
	}

	template <class Type>
	constexpr bool operator <= (const Optional<Type>& x, nullopt_t) noexcept
	{
		return (!x);
	}

	template <class Type>
	constexpr bool operator <= (nullopt_t, const Optional<Type>&) noexcept
	{
		return true;
	}

	template <class Type>
	constexpr bool operator > (const Optional<Type>& x, nullopt_t) noexcept
	{
		return static_cast<bool>(x);
	}

	template <class Type>
	constexpr bool operator > (nullopt_t, const Optional<Type>&) noexcept
	{
		return false;
	}

	template <class Type>
	constexpr bool operator >= (const Optional<Type>&, nullopt_t) noexcept
	{
		return true;
	}

	template <class Type>
	constexpr bool operator >= (nullopt_t, const Optional<Type>& x) noexcept
	{
		return (!x);
	}

	template <class Type>
	constexpr bool operator == (const Optional<Type>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x == v : false;
	}

	template <class Type>
	constexpr bool operator == (const Type& v, const Optional<Type>& x)
	{
		return static_cast<bool>(x) ? v == *x : false;
	}

	template <class Type>
	constexpr bool operator != (const Optional<Type>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x != v : true;
	}

	template <class Type>
	constexpr bool operator != (const Type& v, const Optional<Type>& x)
	{
		return static_cast<bool>(x) ? v != *x : true;
	}

	template <class Type>
	constexpr bool operator < (const Optional<Type>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x < v : true;
	}

	template <class Type>
	constexpr bool operator > (const Type& v, const Optional<Type>& x)
	{
		return static_cast<bool>(x) ? v > *x : true;
	}

	template <class Type>
	constexpr bool operator > (const Optional<Type>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x > v : false;
	}

	template <class Type>
	constexpr bool operator < (const Type& v, const Optional<Type>& x)
	{
		return static_cast<bool>(x) ? v < *x : false;
	}

	template <class Type>
	constexpr bool operator >= (const Optional<Type>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x >= v : false;
	}

	template <class Type>
	constexpr bool operator <= (const Type& v, const Optional<Type>& x)
	{
		return static_cast<bool>(x) ? v <= *x : false;
	}

	template <class Type>
	constexpr bool operator <= (const Optional<Type>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x <= v : true;
	}

	template <class Type>
	constexpr bool operator >= (const Type& v, const Optional<Type>& x)
	{
		return static_cast<bool>(x) ? v >= *x : true;
	}

	template <class Type>
	constexpr bool operator == (const Optional<Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x == v : false;
	}

	template <class Type>
	constexpr bool operator == (const Type& v, const Optional<Type&>& x)
	{
		return static_cast<bool>(x) ? v == *x : false;
	}

	template <class Type>
	constexpr bool operator != (const Optional<Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x != v : true;
	}

	template <class Type>
	constexpr bool operator !=(const Type& v, const Optional<Type&>& x)
	{
		return static_cast<bool>(x) ? v != *x : true;
	}

	template <class Type>
	constexpr bool operator < (const Optional<Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x < v : true;
	}

	template <class Type>
	constexpr bool operator > (const Type& v, const Optional<Type&>& x)
	{
		return static_cast<bool>(x) ? v > *x : true;
	}

	template <class Type>
	constexpr bool operator > (const Optional<Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x > v : false;
	}

	template <class Type>
	constexpr bool operator < (const Type& v, const Optional<Type&>& x)
	{
		return static_cast<bool>(x) ? v < *x : false;
	}

	template <class Type>
	constexpr bool operator >= (const Optional<Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x >= v : false;
	}

	template <class Type>
	constexpr bool operator <=(const Type& v, const Optional<Type&>& x)
	{
		return static_cast<bool>(x) ? v <= *x : false;
	}

	template <class Type>
	constexpr bool operator <= (const Optional<Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x <= v : true;
	}

	template <class Type>
	constexpr bool operator >= (const Type& v, const Optional<Type&>& x)
	{
		return static_cast<bool>(x) ? v >= *x : true;
	}

	template <class Type>
	constexpr bool operator == (const Optional<const Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x == v : false;
	}

	template <class Type>
	constexpr bool operator == (const Type& v, const Optional<const Type&>& x)
	{
		return static_cast<bool>(x) ? v == *x : false;
	}

	template <class Type>
	constexpr bool operator != (const Optional<const Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x != v : true;
	}

	template <class Type>
	constexpr bool operator != (const Type& v, const Optional<const Type&>& x)
	{
		return static_cast<bool>(x) ? v != *x : true;
	}

	template <class Type>
	constexpr bool operator < (const Optional<const Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x < v : true;
	}

	template <class Type>
	constexpr bool operator > (const Type& v, const Optional<const Type&>& x)
	{
		return static_cast<bool>(x) ? v > *x : true;
	}

	template <class Type>
	constexpr bool operator > (const Optional<const Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x > v : false;
	}

	template <class Type>
	constexpr bool operator < (const Type& v, const Optional<const Type&>& x)
	{
		return static_cast<bool>(x) ? v < *x : false;
	}

	template <class Type>
	constexpr bool operator >= (const Optional<const Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x >= v : false;
	}

	template <class Type>
	constexpr bool operator <= (const Type& v, const Optional<const Type&>& x)
	{
		return static_cast<bool>(x) ? v <= *x : false;
	}

	template <class Type>
	constexpr bool operator <= (const Optional<const Type&>& x, const Type& v)
	{
		return static_cast<bool>(x) ? *x <= v : true;
	}

	template <class Type>
	constexpr bool operator >= (const Type& v, const Optional<const Type&>& x)
	{
		return static_cast<bool>(x) ? v >= *x : true;
	}

	template <class Type>
	constexpr Optional<typename std::decay<Type>::type> make_Optional(Type&& v)
	{
		return Optional<typename std::decay<Type>::type>(constexpr_forward<Type>(v));
	}

	template <class U>
	constexpr Optional<U&> make_Optional(std::reference_wrapper<U> v)
	{
		return Optional<U&>(v.get());
	}

	template <class CharType, class Type>
	inline std::basic_ostream<CharType>& operator << (std::basic_ostream<CharType>& os, const Optional<Type>& x)
	{
		if (x)
		{
			const CharType opt[] = { 'O','p','t','i','o','n','a','l', ' ', '\0' };

			return os << opt << x.value();
		}
		else
		{
			const CharType no[] = { 'n','o','n','e','\0' };

			return os << no;
		}
	}
}

namespace std
{
	template <class Type>
	void swap(s3d::Optional<Type>& x, s3d::Optional<Type>& y) noexcept(noexcept(x.swap(y)))
	{
		x.swap(y);
	}

	template <class Type>
	struct hash<s3d::Optional<Type>>
	{
		using result_type = typename hash<Type>::result_type;
		using argument_type = s3d::Optional<Type>;

		constexpr result_type operator()(argument_type const& arg) const {
			return arg ? std::hash<Type>{}(*arg) : result_type{};
		}
	};

	template <class Type>
	struct hash<s3d::Optional<Type&>>
	{
		using result_type = typename hash<Type>::result_type;
		using argument_type = s3d::Optional<Type&>;

		constexpr result_type operator()(argument_type const& arg) const
		{
			return arg ? std::hash<Type>{}(*arg) : result_type{};
		}
	};
}

# undef OPTIONAL_REQUIRES

//
//////////////////////////////////////////////////////////////////////////////
