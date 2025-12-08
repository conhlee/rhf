#ifndef NW4R_UT_LINK_LIST_H
#define NW4R_UT_LINK_LIST_H

/*******************************************************************************
 * headers
 */

#include <revolution/types.h>

#include <nw4r/ut/inlines.h> // NonCopyable

#include <nw4r/db.h>

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace ut { namespace detail { class LinkListImpl; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace ut
{
	struct LinkListNode : private NonCopyable
	{
	// methods
	public:
		// cdtors
		LinkListNode() : mNext(NULL), mPrev(NULL) {}
		~LinkListNode() {}

		// methods
		LinkListNode *GetNext() const { return mNext; }
		LinkListNode *GetPrev() const { return mPrev; }

	// members
	private:
		/* base NonCopyable */	// size 0x00, offset 0x00
		LinkListNode	*mNext;	// size 0x04, offset 0x00
		LinkListNode	*mPrev;	// size 0x04, offset 0x04

	// friends
	private:
		friend class detail::LinkListImpl;
	}; // size 0x08

	namespace detail
	{
		template <typename Iterator>
		class ReverseIterator
		{
		// methods
		public:
			// cdtors
			ReverseIterator(Iterator it) : mCurrent(it) {}

			// operators
			ReverseIterator &operator ++()
			{
				--mCurrent;
				return *this;
			}

			typename Iterator::Element const *operator->() const
			{
				return &operator *();
			}

			typename Iterator::Element &operator*() const
			{
				Iterator it = mCurrent;
				--it;
				return *it;
			}

			friend bool operator==(ReverseIterator const &r1,
			                       ReverseIterator const &r2)
			{
				return r1.mCurrent == r2.mCurrent;
			}

			friend bool operator!=(ReverseIterator const &r1,
			                       ReverseIterator const &r2)
			{
				return !(r1 == r2);
			}

			// methods
			Iterator GetBase() const { return mCurrent; }

		// members
		private:
			Iterator	mCurrent;	// size Iterator, offset 0x00
		}; // size Iterator

		namespace dummy { class Iterator {}; }

		class LinkListImpl : private NonCopyable
		{
		// nested classes
		public:
			// forward declarations
			class ConstIterator;

			class Iterator : public dummy::Iterator // what does this base do?
			{
			// methods
			public:
				// cdtors
				Iterator(LinkListNode *pNode) : mPointer(pNode) {}

				// operators
				friend bool operator==(Iterator it1, Iterator it2)
				{
					return it1.mPointer == it2.mPointer;
				}

				Iterator &operator++()
				{
					mPointer = mPointer->GetNext();
					return *this;
				}

				Iterator &operator--()
				{
					mPointer = mPointer->GetPrev();
					return *this;
				}

				LinkListNode *operator->() const { return mPointer; }

			// members
			private:
				/* base Iterator */			// size 0x00, offset 0x00
				LinkListNode	*mPointer;	// size 0x04, offset 0x00

			// friends
			private:
				friend class LinkListImpl;
				friend class ConstIterator;
			}; // size 0x04

			class ConstIterator : public dummy::Iterator
			{
			// methods
			public:
				// cdtors
				ConstIterator(LinkListImpl::Iterator it) :
					mPointer(it.mPointer)
				{
				}

				// operators
				ConstIterator &operator++()
				{
					mPointer = mPointer->GetNext();
					return *this;
				}

				ConstIterator &operator--()
				{
					mPointer = mPointer->GetPrev();
					return *this;
				}

				LinkListNode const *operator->() const { return mPointer; }

				friend bool operator==(ConstIterator it1, ConstIterator it2)
				{
					return it1.mPointer == it2.mPointer;
				}

			// members
			private:
				/* base dummy::Iterator */	// size 0x00, offset 0x00
				LinkListNode	*mPointer;	// size 0x04, offset 0x00

			// friends
			private:
				friend class LinkListImpl;
			}; // size 0x04

		// methods
		public:
			// cdtors
			LinkListImpl() : mNode() { Initialize_(); }
			~LinkListImpl();

			// methods
			Iterator GetBeginIter() { return Iterator(mNode.GetNext()); }
			Iterator GetEndIter() { return Iterator(&mNode); }
			u32 GetSize() const { return mSize; }

			Iterator Insert(Iterator it, LinkListNode *p);

			Iterator Erase(LinkListNode *p);
			Iterator Erase(Iterator it);
			Iterator Erase(Iterator itFirst, Iterator itLast);

			void PopFront() { Erase(GetBeginIter()); }
			void PopBack() { Erase(--GetEndIter()); }
			void Clear();
			bool IsEmpty() const { return mSize == 0; }

			static Iterator GetIteratorFromPointer(LinkListNode *p)
			{
				// specifically not the header variant
				NW4R_ASSERT_PTR_NULL(p, 328);

				return p;
			}

		private:
			void Initialize_()
			{
				mSize = 0;
				mNode.mNext = &mNode;
				mNode.mPrev = &mNode;
			}

		// members
		private:
			u32				mSize;	// size 0x04, offset 0x00
			LinkListNode	mNode;	// size 0x08, offset 0x04
		}; // size 0x0c
	} // namespace detail

	namespace dummy { template <typename, int> class Iterator {}; }

	template <typename T, int I>
	class LinkList : private detail::LinkListImpl
	{
	// iterator typedefs
	public:
		class Iterator;
		class ConstIterator;
		typedef class detail::ReverseIterator<Iterator> ReverseIterator;
		typedef class detail::ReverseIterator<ConstIterator> ReverseConstIterator;

	// nested classes
	public:
		class Iterator : public dummy::Iterator<T, I> // similar thing here idk
		{
		// typedefs
		public:
			typedef T Element;

		// methods
		public:
			// cdtors
			Iterator(detail::LinkListImpl::Iterator it) : it_(it) {}

			// operators
			friend bool operator!=(Iterator it1, Iterator it2)
			{
				return !(it1 == it2);
			}

			friend bool operator==(Iterator it1, Iterator it2)
			{
				return it1.it_ == it2.it_;
			}

			Iterator &operator++()
			{
				++it_;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator it = *this;
				++*this;
				return it;
			}

			Iterator &operator--()
			{
				--it_;
				return *this;
			}

			T &operator*() const
			{
				T *p = this->operator->();

				return *p;
			}

			T *operator->() const
			{
				return GetPointerFromNode(it_.operator->());
			}

		// members
		private:
			/* base Iterator */						// size 0x00, offset 0x00
			detail::LinkListImpl::Iterator	it_;	// size 0x04, offset 0x00

		// friends
		private:
			template <typename, int> friend class LinkList;
		}; // size 0x04

		class ConstIterator : public dummy::Iterator<T, I>
		{
		// typedefs
		public:
			typedef T Element;

		// methods
		public:
			// cdtors
			ConstIterator(LinkListImpl::Iterator it) : it_(it) {}
			ConstIterator(Iterator it) : it_(it.mIterator) {}

			// operators
			friend bool operator==(ConstIterator it1, ConstIterator it2)
			{
				return it1.it_ == it2.it_;
			}

			friend bool operator!=(ConstIterator it1, ConstIterator it2)
			{
				return !(it1 == it2);
			}

			ConstIterator &operator++()
			{
				++it_;
				return *this;
			}

			ConstIterator &operator--()
			{
				--it_;
				return *this;
			}

			ConstIterator operator++(int)
			{
				ConstIterator ret = *this;
				++*this;
				return ret;
			}

			T const *operator->() const
			{
				return GetPointerFromNode(it_.operator->());
			}

			T const &operator*() const { return *this->operator->(); }

		// members
		private:
			/* base Iterator */					// size 0x00, offset 0x00
			LinkListImpl::ConstIterator	it_;	// size 0x04, offset 0x00
		}; // size 0x04

	// methods
	public:
		// cdtors
		LinkList() {}
		~LinkList() {}

		// get methods
		Iterator GetBeginIter() { return LinkListImpl::GetBeginIter(); }
		Iterator GetEndIter() { return LinkListImpl::GetEndIter(); }

		// Aren't you supposed to do it the other way?
		ConstIterator GetBeginIter() const
		{
			return const_cast<LinkList *>(this)->LinkListImpl::GetBeginIter();
		}

		ConstIterator GetEndIter() const
		{
			return const_cast<LinkList *>(this)->LinkListImpl::GetEndIter();
		}

		ReverseIterator GetBeginReverseIter() { return GetBeginIter(); }
		ReverseIterator GetEndReverseIter() { return GetEndIter(); }
		ReverseConstIterator GetBeginReverseIter() const
		{
			return GetBeginIter();
		}
		ReverseConstIterator GetEndReverseIter() const { return GetEndIter(); }

		T &GetFront()
		{
			return *GetBeginIter();
		}

		T &GetBack()
		{
			return *--GetEndIter();
		}

		// methods
		Iterator Insert(Iterator it, T *p)
		{
			return LinkListImpl::Insert(it.it_, GetNodeFromPointer(p));
		}

		void PushBack(T *p) { Insert(GetEndIter(), p); }
		Iterator Erase(T *p) { return LinkListImpl::Erase(GetNodeFromPointer(p)); }
		Iterator Erase(Iterator it) { return LinkListImpl::Erase(it.it_); }

		// TODO: fake (should be direct calls to the LinkListImpl versions)
		void Clear() { LinkListImpl::Clear(); }
		u32 GetSize() const { return LinkListImpl::GetSize(); }
		bool IsEmpty() const { return LinkListImpl::IsEmpty(); }
		void PopFront() { LinkListImpl::PopFront(); }
		void PopBack() { LinkListImpl::PopBack(); }

		// static methods
		static Iterator GetIteratorFromPointer(LinkListNode *p)
		{
			// specifically not the header variant
			NW4R_ASSERT_PTR_NULL(p, 542);

			return LinkListImpl::GetIteratorFromPointer(p);
		}

		static Iterator GetIteratorFromPointer(T *p)
		{
			// specifically not the header variant
			NW4R_ASSERT_PTR_NULL(p, 552);

			return GetIteratorFromPointer(GetNodeFromPointer(p));
		}

		static LinkListNode *GetNodeFromPointer(T *p)
		{
			return reinterpret_cast<LinkListNode *>(reinterpret_cast<u32>(p)
			                                        + I);
		}

		static T *GetPointerFromNode(LinkListNode *p)
		{
			return reinterpret_cast<T *>(reinterpret_cast<u32>(p) - I);
		}

		static T const *GetPointerFromNode(LinkListNode const *p)
		{
			return reinterpret_cast<T *>(reinterpret_cast<u32>(p) - I);
		}

	// members
	private:
		/* base LinkListImpl */	// size 0x0c, offset 0x00
	}; // size 0x0c
}} // namespace nw4r::ut

#endif // NW4R_UT_LINK_LIST_H
