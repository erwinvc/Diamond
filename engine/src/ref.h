#pragma once
#include "atomic"

namespace Diamond {
	template<typename T>
	class AssetRef {
	private:
		T* m_object = nullptr;
	public:
		AssetRef() {
			Set(nullptr);
		}

		AssetRef(T* object) {
			Set(object);
		}

		AssetRef(const AssetRef& other) {
			this->operator=(other);
		}

		AssetRef& operator=(const AssetRef& other) {
			Set(other.Get());
			return *this;
		}

		operator T* () { return m_object; }

		~AssetRef() {
			Set(nullptr);
		}

		T* Get()const {
			return m_object;
		}

		void Set(T* object) {
			m_object = object;
		}

		T* operator ->() { return m_object; }
		const T* operator ->() const { return m_object; }

		T& operator *() { return *m_object; }
		const T& operator *() const { return *m_object; }

		bool operator==(const AssetRef<T>& other) const {
			return m_object == other.m_object;
		}
	};

	template<typename T>
	class ManagedRef;

	class Ref {
	public:
		std::atomic<uint32_t> m_refCount;
		Ref() : m_refCount(0) {}
	};

	template<typename T>
	class ManagedRef {
	private:
		T* m_pointer;
	public:
		T* operator->() { return m_pointer; }
		const T* operator->() const { return m_pointer; }
		T& operator*() { return *m_pointer; }
		const T& operator*() const { return *m_pointer; }
		T* Raw() { return  m_pointer; }
		const T* Raw() const { return  m_pointer; }

		ManagedRef() : m_pointer(nullptr) {}
		ManagedRef(ManagedRef const& managedRef) : m_pointer(managedRef.m_pointer) { ++m_pointer->m_refCount; }
		ManagedRef(T* pointer) : m_pointer(pointer) { if (m_pointer) ++m_pointer->m_refCount; }
		~ManagedRef() { if (--m_pointer->m_refCount == 0) delete m_pointer; }
		ManagedRef& operator= (ManagedRef const& managedRef) {
			T* const old = m_pointer;
			m_pointer = managedRef.m_pointer;
			++m_pointer->m_refCount;
			if (--old->m_refCount == 0) delete old;
			return *this;
		}

		ManagedRef& operator= (T* const& pointer) {
			T* const old = m_pointer;
			m_pointer = pointer;
			++m_pointer->m_refCount;
			if (old && --old->m_refCount == 0) delete old;
			return *this;
		}
	};
}