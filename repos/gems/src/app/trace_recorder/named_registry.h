/*
 * \brief  Utility for finding objecs by name
 * \author Norman Feske
 * \date   2021-11-11
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _NAMED_REGISTRY_H_
#define _NAMED_REGISTRY_H_

#include <util/string.h>
#include <util/avl_tree.h>
#include <util/noncopyable.h>

namespace Trace_recorder {
	using namespace Genode;

	template <typename T> class Named_registry;

	template <size_t N1, size_t N2>
	static inline bool operator > (String<N1> const &s1, String<N2> const &s2)
	{
		return strcmp(s1.string(), s2.string()) > 0;
	}
}

template <typename T>
class Trace_recorder::Named_registry : Noncopyable
{
	private:

		Avl_tree<T> _tree { };

	public:

		class Element : private Avl_node<T>
		{
			public:

				using Name = Genode::String<64>;
				Name const name;

			private:

				Named_registry<T> &_registry;

				bool higher(T const *other) const { return name > other->name; }

				friend class Avl_tree<T>;
				friend class Avl_node<T>;
				friend class Named_registry<T>;

				static T *_matching_sub_tree(T &curr, Name const &name)
				{
					typename Avl_node<T>::Side side = (curr.name > name);

					return curr.Avl_node<T>::child(side);
				}

			public:

				Element(Named_registry &registry, Name const &name)
				:
					name(name), _registry(registry)
				{
					_registry._tree.insert(this);
				}

				~Element()
				{
					_registry._tree.remove(this);
				}
		};

		template <typename FN>
		void apply(typename Element::Name const &name, FN && fn)
		{
			T *curr_ptr = _tree.first();
			for (;;) {
				if (!curr_ptr)
					return;

				if (curr_ptr->name == name) {
					fn(*curr_ptr);
					return;
				}

				curr_ptr = Element::_matching_sub_tree(*curr_ptr, name);
			}
		}

		template <typename FN>
		void for_each(FN && fn) { _tree.for_each(fn); }
};

#endif /* _NAMED_REGISTRY_H_ */
