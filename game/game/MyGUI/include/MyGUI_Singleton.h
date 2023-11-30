/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SINGLETON_H_
#define MYGUI_SINGLETON_H_

#include "MyGUI_Diagnostic.h"

namespace MyGUI
{

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
	template <class T>
	class Singleton
#else
	template <class T>
	class MYGUI_EXPORT MYGUI_OBSOLETE("Singleton class is deprecated. Do not use singletons.") Singleton
#endif
	{
	public:

		#if defined(__clang__)
			// This constructor is called before the `T` object is fully constructed, and
			// pointers are not dereferenced anyway, so UBSan shouldn't check vptrs.
			__attribute__((no_sanitize("vptr")))
		#endif
		Singleton()
		{
			MYGUI_ASSERT(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			if (nullptr == msInstance)
				MYGUI_LOG(Critical, "Destroying Singleton instance " << getClassTypeName() << " before constructing it.");
			msInstance = nullptr;
		}

		static T& getInstance()
		{
			MYGUI_ASSERT(nullptr != getInstancePtr(), "Singleton instance " << getClassTypeName() << " was not created");
			return (*getInstancePtr());
		}

		static T* getInstancePtr()
		{
			return msInstance;
		}

		static const char* getClassTypeName()
		{
			return mClassTypeName;
		}

	private:
		static T* msInstance;
		static const char* mClassTypeName;
	};

/*
    // Template Singleton class was replaces with a set of macroses, because there were too many issues with it,
    // all appearing in different compilers:
    //   - incorrect exporting template class;
    //   - static variables in headers issues;
    //   - explicit specialization/implicit instantiation issues;
    //   - many other related compile errors.
    // It is possible to move all template definitions into cpp code, but result looked even worse.

    // Usage:

	// in header
	class MyClass
	{
		MYGUI_SINGLETON_DECLARATION(MyClass);

		MyClass()
		// ...
	};

	// in cpp
	MYGUI_SINGLETON_DEFINITION(MyClass);

 	MyClass() : mSingletonHolder(this)
 	{
 		// ...
 	}
*/

	// proxy class to avoid calling initialiseSingleton/shutdownSingleton in constructor/destructor
	template<class T>
	class SingletonHandler
	{
	public:
		SingletonHandler(T* instance) :
			mInstance(instance)
		{
			mInstance->initialiseSingleton();
		}
		~SingletonHandler()
		{
			mInstance->shutdownSingleton();
		}
	private:
		T* mInstance;
	};

#define MYGUI_SINGLETON_DECLARATION(ClassName) \
	private: \
	friend MyGUI::SingletonHandler<ClassName>; \
	MyGUI::SingletonHandler<ClassName> mSingletonHolder; \
	void initialiseSingleton(); \
	void shutdownSingleton(); \
	 \
	public: \
	static ClassName& getInstance(); \
	static ClassName* getInstancePtr(); \
	static const char* getClassTypeName()

#define MYGUI_SINGLETON_DEFINITION(ClassName) \
	static ClassName* msInstance = nullptr; \
	static const char* mClassTypeName = #ClassName; \
	 \
	void ClassName::initialiseSingleton() \
	{ \
		MYGUI_ASSERT(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist"); \
		msInstance = this; \
	} \
	 \
	void ClassName::shutdownSingleton() \
	{ \
		if (nullptr == msInstance) \
			MYGUI_LOG(Critical, "Destroying Singleton instance " << getClassTypeName() << " before constructing it."); \
		msInstance = nullptr; \
	} \
	 \
	ClassName& ClassName::getInstance() \
	{ \
		MYGUI_ASSERT(nullptr != getInstancePtr(), "Singleton instance " << getClassTypeName() << " was not created"); \
		return (*getInstancePtr()); \
	} \
	 \
	ClassName* ClassName::getInstancePtr() \
	{ \
		return msInstance; \
	} \
	 \
	const char* ClassName::getClassTypeName() \
	{ \
		return mClassTypeName; \
	} \
	static_assert(true, "require semicolon")

} // namespace MyGUI

#endif // MYGUI_SINGLETON_H_
