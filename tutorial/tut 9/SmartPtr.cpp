https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#include <iostream>

using namespace std;

template<class T>
class SmartPtr {
	private:
		T* obj;
		int* ref_count;

		/* This private method decreases the reference count of the object, and
		 * deletes it if the reference count reaches zero. This functionality is
		 * needed by both the destructor and the overloaded assignment operator, so
		 * we place it in its own function. 
		 */

		void release() {
			if (!(obj == 0)) {
				(*ref_count)--;
				if (*ref_count == 0) {
					delete obj;
					delete ref_count;
				}
			}
		}

	/*
	 * This method copies the reference from another smart pointer. This
	 * functionality is used by both the copy constructor and the overloaded
	 * assignment operator, so we place it in its own function.
	 */
		
		void copy(const SmartPtr<T>& from) {
			// Copy the pointers to the referenced object and its reference count
			obj = from.obj;
			ref_count = from.ref_count;

			// If we do not have a null pointer, increment the reference count
			if (!(obj == 0)) {
				(*ref_count)++;
			}
		}

	public:
		// The default constructor
		SmartPtr() {
			obj = 0;
			ref_count = 0;
		}

		// Constructor from an existing (standard) pointer
		SmartPtr(T* obj) {
			this->obj = obj;
			ref_count = new int(1);	// Initialise the reference count
		}

		// The copy constructor
		SmartPtr(const SmartPtr<T>& src) {
			copy(src);
		}

		// The Destructor calls the release method to decrement the reference count
		~SmartPtr() {
			release();
		}

		/*
		 * The assignment operator releases current reference makes a copy of the
		 * new refernce
		 */

		SmartPtr<T>& operator=(const SmartPtr<T>& src) {
			if (!(&src == this))
			{
				release();
				copy(src);
			}
			return *this;
		}

		T& operator*() {
			return *obj;
		}

		T* operator->() {
			return obj;
		}

		int getRefCount() {
			if (!(ref_count == 0)) {
				return *ref_count;
			}
			return 0;
		}
};

class TestClass1 {
	public:
		virtual void print() { cout << "TestClass1 at address " << this << '\n'; }
};

class DerivedClass : public TestClass1 {
	public:
		virtual void print() { cout << "DerivedClass at address " << this << '\n'; }
};

class TestClass2 {
	public:
		int number;
		void print() { cout << "TestClass2 at address " << this << '\n'; }
};

class PtrLoop {
	public:
		int number;
		SmartPtr<PtrLoop> self_ptr;
		void print() { cout << "PtrLoop at address " << this << '\n'; }
};

int main() {
	// Smart Pointers work with primitive datatypes
	SmartPtr<int> sp_int1(new int(5));
	SmartPtr<int> sp_int2 = sp_int1;

	cout << *sp_int1 << '\n';    // Output: 5
	cout << *sp_int2 << '\n';    // Output: 5

	*sp_int1 = 3;

	cout << *sp_int1 << '\n';    // Output: 3
	cout << *sp_int2 << '\n';    // Output: 3

	cout << sp_int1.getRefCount() << '\n';    // Output: 2
	cout << sp_int2.getRefCount() << '\n';    // Output: 2

	// Check the assigning a smart pointer to itself does not increase the
	// reference count
	sp_int1 = sp_int1;
	cout << sp_int1.getRefCount() << '\n';    // Output: 2

	// Smart Pointers also work with object (class) types
	SmartPtr<TestClass1> sp_obj_test1(new TestClass1());
	SmartPtr<TestClass1> sp_obj_test2(new TestClass1());

	sp_obj_test1->print();    // Output: TestClass1 at address 0x4d8590
	sp_obj_test2->print();    // Output: TestClass1 at address 0x4d85b0

	sp_obj_test1 = sp_obj_test2;

	// We can call methods on objects stored in smart pointers
	sp_obj_test1->print();    // Output: TestClass1 at address 0x4d85b0

	// We even have virtual functions behaving correctly
	sp_obj_test1 = new DerivedClass();
	sp_obj_test1->print();    // Output: DerivedClass at address 0x4d85d0

	// Our smart pointers are typesafe
	TestClass1* test3 = new TestClass1();
	TestClass2* test4 = new TestClass2();

	// test3 = test4
	// this gives a compiler error since TestClass1 and TestClass 2 are NOT
	// compatible types

	SmartPtr<TestClass1> sp_test3(test3);
	SmartPtr<TestClass2> sp_test4(test4);

	// This should also give a compiler error: no match for operator=
	// sp_test3 = sp_test4;

	// Now we test that memory is reclaimed
	test4->number = 4;
	cout << "Number: " << test4->number << endl;
	{
		SmartPtr<TestClass2> sp_test5(test4);
	} // 'last' reference to TestClass2 object goes out of scope here

	// This should give a runtime error or print a different number because the
	// object at this memory address has been freed
	//cout << "Number: " << test4->number << endl;

	// A handle to a PtrLoop object
	PtrLoop* loop = new PtrLoop();
	loop->number = 8;
	cout << "Number: " << loop->number << endl;

	/*
	 * A new scope block: in here we will create a smart pointer reference to the
	 * loop object and make it point to itself so that even when our last reference
	 * to the object in the main body of the program is lost the object is not
	 * deleted from memory
	 */
	
	{
		// Create the object and reference it by a smart pointer
		SmartPtr<PtrLoop> sp_loop(loop);

		// Make it point to itself
		sp_loop->self_ptr = sp_loop;
	}

	// This does NOT cause a runtime error or prints a different number as the
	// object has not been freed by the smart pointer
	cout << "Number: " << loop->number << endl;

	return 0;
}
