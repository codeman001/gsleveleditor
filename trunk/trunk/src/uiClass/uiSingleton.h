#ifndef _UISINGLETON_H_
#define _UISINGLETON_H_

namespace uiClass
{
	template <class T> class uiSingleton
	{
		private:
			uiSingleton(const uiSingleton&);
			uiSingleton& operator = (const uiSingleton&);
		protected:
			uiSingleton() {}
			virtual ~uiSingleton() { s_instance = 0; }
		public:
			static T* s_instance;

			static T* createGetInstance()
			{                    
				if (!s_instance)				
					s_instance = new T();
				
				return s_instance;
			};

			inline static T* getInstance()
			{
				return s_instance;
			}

			static void releaseInstance()
			{
				if (s_instance)
				{
					delete s_instance;					
				}
			}
	};
	
	template <class T>
	T* uiSingleton<T>::s_instance = 0;
}

#endif