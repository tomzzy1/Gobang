#pragma once
#include <future>
#include <queue>
#include <vector>
#include <type_traits>

template<typename T>
class async_queue
{
public:
	async_queue(size_t c = 10) :size(c), count(c) {}
	async_queue(async_queue<T>&) = delete;
	async_queue& operator=(const async_queue<T>&) = delete;	
	async_queue& operator=(async_queue<T>&& aq)noexcept
	{
		if (this != &aq)
		{
			future_queue = aq.future_queue;
			results = aq.results;
			count = aq.count;
		}
		return *this;
	}
	async_queue(async_queue<T>&& aq)noexcept
	{
		*this = std::move(aq);
	}
	~async_queue() = default;

	template<typename Fn, typename...args, typename = std::enable_if_t<
		std::is_same<std::result_of_t<
		std::decay_t<Fn>(std::decay_t<args>...)>, T>::value>>
		void produce(Fn&& function, args&&... Args)
	{
		if (count == 0)
		{
			for (int i = 0; i < size / 5; ++i)
			{
				results.push_back(future_queue.front().get());
				future_queue.pop();
			}
			count += size / 5;
			auto future = std::async(std::launch::async, std::forward<Fn>(function), std::forward<args>(Args)...);
			future_queue.push(std::move(future));		
		}
		else
		{
			auto future = std::async(std::launch::async, std::forward<Fn>(function), std::forward<args>(Args)...);
			--count;
			future_queue.push(std::move(future));
		}
	}

	T consume()
	{
		if (!results.empty())
		{
			auto result = results.back();
			results.pop_back();
			++count;
			return result;
		}
		return future_queue.back().get();
	}

	std::vector<T> consume_all()
	{
		while (!future_queue.empty())
		{
			results.push_back(future_queue.front().get());
			future_queue.pop();
			++count;
		}
		return results;
	}

private:
	const size_t size;
	std::queue<std::future<T>> future_queue;
	std::vector<T> results;
	size_t count;
};

