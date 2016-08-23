#ifndef SINGLETON_H
#define SINGLETON_H

template<class T> 
class Singleton {

public:
	static T* Instance() {
		static T instance;
		return &instance;
		}
	};

#endif // SINGLETON_H
