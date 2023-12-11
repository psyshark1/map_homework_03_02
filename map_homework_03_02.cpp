#include <iostream>
#include <future>
#include <thread>
//#include <algorithm>
//#include <functional>

class Sum
{
public:
	Sum() {};
	Sum(int&& v) : sum(std::move(v)) {};
	int sum{ 0 };
	void operator()(const int& n){ sum += n; }
	Sum operator+(const Sum& rsum) { return Sum(this->sum + rsum.sum); }
};

template<typename it, class func, int size = 4>
void parallel_for_each(const it* begin, const it* end, func& f)
{
	size_t cur_sz = std::distance(begin, end);
	if (cur_sz <= size)
	{
		f = std::for_each(begin, end, f);
		return;
	}
	auto mid = begin;
	std::advance(mid, cur_sz / 2);
	auto fut_res = std::async(parallel_for_each<it, func>, begin, mid, std::ref(f));
	parallel_for_each(mid, end, f);
	fut_res.get();
 }

int main()
{
	int arr[]{ 15,68,79,55,2,63,98,69,889,129,44,338,681,4,275 };
	Sum s;
	parallel_for_each(std::begin(arr), std::end(arr), s);

	std::cout << "Summ array is: " << s.sum << std::endl;
	return 0;
}
