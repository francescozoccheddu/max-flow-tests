#ifndef INCLUDED_MAX_FLOW_REFERENCE_EQUATABLE
#define INCLUDED_MAX_FLOW_REFERENCE_EQUATABLE

#include <memory>

namespace MaxFlow::Utils
{

#pragma region Declaration

	class ReferenceComparable
	{

	protected:

		// Construction

		ReferenceComparable () = default;

	public:

		// Comparison

		virtual bool operator==(const ReferenceComparable& _other) const;

	};

#pragma endregion

#pragma region Implementation

	bool MaxFlow::Utils::ReferenceComparable::operator==(const ReferenceComparable& _other) const
	{
		return this == std::addressof (_other);
	}

#pragma endregion


}

#endif