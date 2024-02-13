#include <iostream>
#include <vector>

int num_of_new_int = 0;
int num_of_delete = 0;
int num_of_const_d = 0;
int num_of_const_c = 0;
int num_of_const_m = 0;
int num_of_assig_c = 0;
int num_of_assig_m = 0;
int num_of_dest = 0;

void reset_stats()
{
	num_of_new_int = 0;
	num_of_delete = 0;
	num_of_const_d = 0;
	num_of_const_c = 0;
	num_of_const_m = 0;
	num_of_assig_c = 0;
	num_of_assig_m = 0;
	num_of_dest = 0;
}

void print_stats()
{
	std::cout << "--------------" << std::endl;
	std::cout << "new int: " << num_of_new_int << std::endl;
	std::cout << "del int: " << num_of_delete << std::endl;
	std::cout << "const_d: " << num_of_const_d << std::endl;
	std::cout << "const_c: " << num_of_const_c << std::endl;
	std::cout << "const_m: " << num_of_const_m << std::endl;
	std::cout << "assig_c: " << num_of_assig_c << std::endl;
	std::cout << "assig_m: " << num_of_assig_m << std::endl;
	std::cout << "--------------" << std::endl;
}

class A
{
public:
	int* p;

	A(int n)
	{
		p = new int(n);
		num_of_new_int++;
		num_of_const_d++;
		std::cout << "Def. const. *p=" << *p << std::endl;
	}

	A(const A& o)
	{
		p = new int(*o.p);
		num_of_new_int++;
		num_of_const_c++;
		std::cout << "Copy. const. *p=" << *p << ", copied " << &o << " to "
			<< this << std::endl;
	}

	A(A&& o) noexcept
	{
		p = o.p;
		o.p = nullptr;
		num_of_const_m++;
		std::cout << "Move. const. *p=" << *p << ", moved " << &o << " to "
			<< this << std::endl;
	}

	A& operator=(const A& o)
	{
		if (p != nullptr)
		{
			// we have something in p, we should release it first
			delete p;
			num_of_delete++;
		}

		if (o.p == nullptr)
			p = nullptr;
		else
			p = new int(*o.p);

		num_of_new_int++;
		num_of_assig_c++;
		std::cout << "Copy. assign. *p=" << *p << ", copied " << &o << " to "
			<< this << std::endl;
		return *this;
	}

	A& operator=(A&& o) noexcept
	{
		if (p != nullptr)
		{
			// we have something in p, we should release it first
			delete p;
			num_of_delete++;
		}
		p = o.p;
		o.p = nullptr;
		num_of_assig_m++;
		std::cout << "Move. assign. *p=" << *p << ", moved " << &o << " to "
			<< this << std::endl;
		return *this;
	}

	~A()
	{
		std::cout << "Dtor." << std::endl;
		num_of_dest++;
		if (p != nullptr)
		{
			num_of_delete++;
			delete p;
		}
	}

	operator int() const
	{
		return *p;
	}
};

template <typename T>
void print_vector(const std::vector<T>& v)
{
	for (const T& a : v)
	{
		std::cout << a << " ";
	}
	std::cout << std::endl;
}

void test0()
{
	std::cout << std::endl << "PRIMERO" << std::endl;
	std::vector<A> v1;
	v1.push_back(A(1));
	print_stats();
	reset_stats();

	std::cout << std::endl << "SEGUNDO" << std::endl;
	std::vector<A> v2;
	A x1(1);
	v2.push_back(x1);
	print_stats();
	reset_stats();

	std::cout << std::endl << "TERCERO" << std::endl;
	std::vector<A> v3;
	A x2(1);
	v3.push_back(std::move(x2));
	print_stats();
	reset_stats();

	std::cout << std::endl << "CUARTO" << std::endl;
	std::vector<A> v4;
	v4.emplace_back(1);
	//print_stats();
}

int main(int, char**)
{
	reset_stats();
	test0();
	print_stats();

	return 0;
}
