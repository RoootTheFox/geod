#pragma once

#include <cstdint>
#include <array>

namespace geode::stl {
	class StringImplAdapter;

	struct StringImpl;

#if defined(GEODE_IS_WINDOWS)
	struct StringImpl {
		union {
			std::array<char, 16> m_smallStorage;
			char* m_bigStorage;
		};

		size_t m_size;
		size_t m_capacity;
	};
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID)
	struct StringImpl {
		struct Internal {
			size_t m_size;
			size_t m_capacity;
			int m_refcount;
		};
		Internal* m_data;
	};
#elif defined(GEODE_IS_IOS)
	struct StringImpl {
		struct Short {
			uint8_t sizex2;
			std::array<char, 23> shortStorage;
		};

		struct Long {
			size_t capacitym1;
			size_t size;
			char* longStorage;
		};

		union {
			Short m_short;
			Long m_long;
		};
	};
#else
	using StringImpl = void;
#endif
}

namespace gd {
	class GEODE_DLL string {
		geode::stl::StringImpl m_impl;
		friend geode::stl::StringImplAdapter;
	public:
		string();
		string(string const&);
		string(string&&);
		string(char const*);
		string(std::string const&);
		// tried to add a string_view ctor, but got overload errors :(
		~string();

		string& operator=(string const&);
		string& operator=(string&&);
		string& operator=(char const*);
		string& operator=(std::string const&);

		void clear();
		
		char& at(size_t pos);
		char const& at(size_t pos) const;

		char& operator[](size_t pos);
		char const& operator[](size_t pos) const;

		char* data();
		char const* data() const;
		char const* c_str() const;

		size_t size() const;
		size_t capacity() const;
		bool empty() const;

		std::strong_ordering operator<=>(const std::string_view other) const;

		operator std::string() const;
		operator std::string_view() const;
	};
}