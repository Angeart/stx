#pragma once
#include <memory>
#include <boost/noncopyable.hpp>

namespace stx
{
	template<class target_type>
	class singleton : public boost::noncopyable
	{
	private:
		using self_type = singleton<target_type>;
	public:
		using pointer_type = std::shared_ptr<target_type>;
		~singleton() = default;
		static pointer_type get_instance();
	protected:
		singleton() = default;
	};

	template <class target_type>
	typename singleton<target_type>::pointer_type singleton<target_type>::get_instance()
	{
		static auto instance = pointer_type(new target_type());
		return instance;
	}
}
