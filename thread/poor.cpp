#include <iostream>
#include <stdio.h>
#include <mutex>

/*
   class Test{
   private:
   static int data;
   public:   
   int * get_instance() {  return &data;  }
   }; 
*/

mutex lock;
class Test{
	private:
		volatile static int *data;
	public:
		int *get_instance(){
			if (data == NULL) {
				lock.lock();
				if (data == NULL){
					data = new int;
					*data = 4;
				}
				lock.unlock();
			}
			return data;
		}
};
int *Test::data = NULL;

//int Test::data = 10;

int main()
{
	Test a, b;

	printf("%d-%p\n", *a.get_instance(), a.get_instance());
	printf("%d-%p\n", *b.get_instance(), b.get_instance());
}
