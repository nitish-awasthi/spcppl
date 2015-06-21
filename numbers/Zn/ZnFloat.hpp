#pragma once

#include <iostream>
#include <assert.h>
#include "../extended_gcd.hpp"
#include "../../assert.hpp"
#include "../../identity.hpp"
#include "Normalizator.hpp"

template <>
struct impl__Normalizator<false> {
	static void softUp(long long&, long long) {
	}

	static void softDown(long long&, long long) {
	}

	static void hardDown(long long&, long long) {
	}

	static void hard(long long&, long long) {
	}
};

template <bool autoNormalize = true>
class ZnFloat {
public:

	/**
	* Not to be used
	*/
	ZnFloat(): mod(0), value(0) {
	}

	explicit ZnFloat(long long mod): mod(mod), value(0) {

	}

	/**
	* Instead of ctor, to allow not to notmalize in ctor
	*/
	static ZnFloat valueOf(long long mod, long long value) {
		impl__Normalizator<true>::hard(value, mod);
		return ZnFloat(mod, value);
	}

	ZnFloat& operator += (const ZnFloat& rhs) {
		SPCPPL_ASSERT(mod == rhs.mod);
		value += rhs.value;
		impl__Normalizator<autoNormalize>::softDown(value, mod);
		return *this;
	}

	ZnFloat& operator += (long long rhs) {
		return *this += ZnFloat::ZnFloat(mod, rhs);
	}

	ZnFloat& operator -= (const ZnFloat& rhs) {
		SPCPPL_ASSERT(mod == rhs.mod);
		value -= rhs.value;
		impl__Normalizator<autoNormalize>::softUp(value, mod);
		return *this;
	}

	ZnFloat& operator -= (long long rhs) {
		return *this -= ZnFloat::ZnFloat(mod, rhs);
	}

	ZnFloat& operator *= (const ZnFloat& rhs) {
		SPCPPL_ASSERT(mod == rhs.mod);
		value *= rhs.value;
		impl__Normalizator<autoNormalize>::hardDown(value, mod);
		return *this;
	}

	ZnFloat& operator *= (long long rhs) {
		return *this *= ZnFloat::ZnFloat(mod, rhs);
	}

	/**
	* Changes internal state, but doesn't change logical state
	*/
	void normalize() const {
		long long& value = const_cast<long long&>(this->value);
		impl__Normalizator<!autoNormalize>::hard(value, mod);
	}

	ZnFloat operator - () const {
		ZnFloat result(mod, mod - value);
		impl__Normalizator<autoNormalize>::softDown(result.value, mod);
		return result;
	}

	ZnFloat& operator /= (const ZnFloat& rhs) {
		return *this *= rhs.inversed();
	}

	ZnFloat& operator /= (long long rhs) {
		return *this /= ZnFloat::ZnFloat(mod, rhs);
	}

	bool operator == (const ZnFloat& rhs) const {
		SPCPPL_ASSERT(mod == rhs.mod);
		normalize();
		rhs.normalize();
		return value == rhs.value;
	}

	ZnFloat inversed() const {
		SPCPPL_ASSERT(value != 0);
		normalize();

		long long x, y;
		long long gcd = extended_gcd(value, mod, x, y);
		(void)gcd;
		SPCPPL_ASSERT(gcd == 1);

		impl__Normalizator<autoNormalize>::softUp(x, mod);
		return ZnFloat(mod, x);
	}
	template <bool aF>
	friend std::ostream& operator << (std::ostream&, const ZnFloat<aF>& zn);
	template <bool aF>
	friend std::istream& operator >> (std::istream&, ZnFloat<aF>& zn);

	long long longValue() const {
		return value;
	}

	long long mod;
private:

	/**
	* No normalization performed
	*/
	ZnFloat(long long mod, long long value): mod(mod), value(value) {
		SPCPPL_ASSERT(mod > 0);
	}

	long long value;
};

template <bool a>
ZnFloat<a> operator + (const ZnFloat<a>& lhs, const ZnFloat<a>& rhs) {
	ZnFloat<a> copy = lhs;
	return copy += rhs;
}

template <bool a>
ZnFloat<a> operator + (const ZnFloat<a>& lhs, long long rhs) {
	ZnFloat<a> copy = lhs;
	return copy += rhs;
}

template <bool a>
ZnFloat<a> operator - (const ZnFloat<a>& lhs, const ZnFloat<a>& rhs) {
	ZnFloat<a> copy = lhs;
	return copy -= rhs;
}

template <bool a>
ZnFloat<a> operator - (const ZnFloat<a>& lhs, long long rhs) {
	ZnFloat<a> copy = lhs;
	return copy -= rhs;
}

template <bool a>
ZnFloat<a> operator * (const ZnFloat<a>& lhs, const ZnFloat<a>& rhs) {
	ZnFloat<a> copy = lhs;
	return copy *= rhs;
}

template <bool a>
ZnFloat<a> operator * (const ZnFloat<a>& lhs, long long rhs) {
	ZnFloat<a> copy = lhs;
	return copy *= rhs;
}

template <bool a>
ZnFloat<a> operator / (const ZnFloat<a>& lhs, const ZnFloat<a>& rhs) {
	ZnFloat<a> copy = lhs;
	return copy /= rhs;
}

template <bool a>
ZnFloat<a> operator / (const ZnFloat<a>& lhs, long long rhs) {
	ZnFloat<a> copy = lhs;
	return copy /= rhs;
}

template <bool a>
std::ostream& operator <<(std::ostream& stream, const ZnFloat<a>& zn) {
	zn.normalize();
	return stream << zn.value;
}

template <bool a>
std::istream& operator >>(std::istream& stream, ZnFloat<a>& zn) {
	stream >> zn.value;
	impl__Normalizator<a>::hard(zn.value, zn.mod);
	return stream;
}

template <bool a>
struct impl__SampleIdentityHelper<ZnFloat<a>> {
	static ZnFloat<a> identity(const ZnFloat<a>& sample) {
		return ZnFloat<a>::ZnFloat(sample.mod, 1);
	}
};
