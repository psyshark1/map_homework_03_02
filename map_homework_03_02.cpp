#include <iostream>
#include <future>
#include <thread>
#include <algorithm>
#include <functional>

class Sum
{
public:
	Sum() {};
	Sum(int&& v) : sum(std::move(v)) {};
	int sum{ 0 };
	void operator()(const int& n) { sum += n; }
	Sum operator+(const Sum& rsum) { return Sum(this->sum + rsum.sum); }
};

template<typename it, typename func, int size = 8>
func async_sum(it begin, it end, func f)
{
	size_t cur_sz = std::distance(begin, end);
	if (cur_sz <= size)
	{
		return std::for_each(begin, end, f);
	}
	auto mid = begin;
	std::advance(mid, cur_sz / 2);
	auto fut_res = std::async(async_sum<it, func>, begin, mid, f);
	return fut_res.get() + async_sum(mid, end, f);
 }

int main()
{
	int arr[]{ 15,68,79,55,2,63,98,69,889,129,44,338,681,4,275 };

	Sum s = async_sum(std::begin(arr), std::end(arr), Sum());

	std::cout << "Summ array is: " << s.sum << std::endl;
	return 0;
}
