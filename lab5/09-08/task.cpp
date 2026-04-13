#include <cstddef>
#include <new>
#include <print>

template < typename D > class Entity
{
public :

	static auto operator new(std::size_t size) -> void *
	{
		std::print("Entity::operator new\n");
		return ::operator new(size);
	}

	static void operator delete(void * x)
	{
		std::print("Entity::operator delete\n");
		::operator delete(x);
	}

	static auto operator new[](std::size_t size) -> void *
	{
		std::print("Entity::operator new[]\n");
		return ::operator new[](size);
	}

	static void operator delete[](void * x)
	{
		std::print("Entity::operator delete[]\n");
		::operator delete[](x);
	}

	static auto operator new(std::size_t size, const std::nothrow_t& tag) noexcept -> void *
	{
		std::print("Entity::operator new(nothrow)\n");
		return ::operator new(size, tag);
	}

	static void operator delete(void * x, const std::nothrow_t& tag) noexcept
	{
		std::print("Entity::operator delete(nothrow)\n");
		::operator delete(x);
	}

	static auto operator new[](std::size_t size, const std::nothrow_t& tag) noexcept -> void *
	{
		std::print("Entity::operator new[](nothrow)\n");
		return ::operator new[](size, tag);
	}

	static void operator delete[](void * x, const std::nothrow_t& tag) noexcept
	{
		std::print("Entity::operator delete[](nothrow)\n");
		::operator delete[](x);
	}

protected :

    Entity() = default;
};

class Client : private Entity < Client >
{
public :

	Client() { std::print("Client::Client\n"); }

   ~Client() { std::print("Client::~Client\n"); }

	using Entity < Client > ::operator new;
	using Entity < Client > ::operator delete;
	using Entity < Client > ::operator new[];
	using Entity < Client > ::operator delete[];
};

int main()
{
	std::print("=== Test 1: new/delete ===\n");
	delete new Client;

	std::print("\n=== Test 2: new[]/delete[] ===\n");
	delete[] new Client[3];

	std::print("\n=== Test 3: new(nothrow)/delete ===\n");
	delete new(std::nothrow) Client;

	std::print("\n=== Test 4: new[](nothrow)/delete[] ===\n");
	delete[] new(std::nothrow) Client[2];

	return 0;
}