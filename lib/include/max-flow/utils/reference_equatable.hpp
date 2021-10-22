#ifndef INCLUDED_MAX_FLOW_REFERENCE_EQUATABLE
#define INCLUDED_MAX_FLOW_REFERENCE_EQUATABLE

#include <memory>

namespace MaxFlow::Utils
{

	class ReferenceEquatable
	{

	protected:

		// Construction

		ReferenceEquatable () = default;

	public:

		// Comparison

		bool operator==(const ReferenceEquatable& _other) const;
		bool operator!=(const ReferenceEquatable& _other) const;

	};

#pragma region Implementation

	inline bool ReferenceEquatable::operator==(const ReferenceEquatable& _other) const
	{
		return this == std::addressof (_other);
	}

	inline bool ReferenceEquatable::operator!=(const ReferenceEquatable& _other) const
	{
		return this != std::addressof (_other);
	}

#pragma endregion

}

#endif