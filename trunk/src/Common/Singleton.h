// Singleton Template file
#ifndef _Singleton_
#define _Singleton_


/*! This class is a template class in order for different singletons to be defined
  A singleton class is a class where one (and only one) instance of the class is created

*/

namespace Crystal
{
	template <typename T>
	class Singleton {
		static T* Instance;
		static int Busy;
		static int References;
		Singleton(Singleton &t)
		{}  // can't have copy constructor
	protected:
		Singleton()
		{}  // derived classes can use this
		virtual ~Singleton()
		{}
	public:
		static T* GetInstance()
		{
			Busy++;
			while (Busy>1) {
#ifdef __WIN32__
				_sleep(50);
#else

				struct timespec timeout,remains;
				timeout.tv_sec = 0;
				timeout.tv_nsec =500000000; /* 50 milliseconds */
				nanosleep(&timeout,&remains);
#endif

			}
			// we are sure we are the only thread here now
			if (Instance == 0)
				Instance = new T;
			Busy--;
			if (Instance==0)
				throw Exception("Initial Singleton Instance could not be created");
			References++;
			return Instance;
		}
		void ClearReferences()
		{
			References = 0;
		};
		void DeleteInstance()
		{
			if (References>0)
				References--;
			if (References==0) {
				delete Instance;
				Instance = 0;
			}
		}
	};

// default initialization
	template <typename T>
	T* Singleton<T>::Instance = 0;
	template <typename T>
	int Singleton<T>::Busy= 0;
	template <typename T>
	int Singleton<T>::References= 0;

#ifdef VALIDATING
// derived singleton class
	class MySingleton : public Singleton<MySingleton> {
		friend class Singleton<MySingleton>;
	private:
		MySingleton()
		{}
		;
		~MySingleton()
		{}
		;
	};

	class TestSingleton {
		MySingleton *First;
		MySingleton *Second;
	public:
		TestSingleton()
		{
			First = MySingleton::GetInstance();
			for (int i=0;i<1000;i++) {
				Second = MySingleton::GetInstance();
				if (Second != First)
					printf("Error--- two singleton's present");
			}
			Second->ClearReferences();
			First = MySingleton::GetInstance();
			Second = MySingleton::GetInstance();
		}
		~TestSingleton()
		{
			First->DeleteInstance();
			Second->DeleteInstance();
		};
	};
#endif	// VALIDATING
}
#endif // _Singleton_