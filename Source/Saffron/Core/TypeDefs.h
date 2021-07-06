#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <optional>
#include <bitset>
#include <initializer_list>
#include <set>
#include <unordered_set>
#include <deque>
#include <stack>
#include <list>
#include <complex>

namespace Se
{
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
#if defined(_MSC_VER)
typedef unsigned __int64 ulong;
#else
typedef unsigned long long ulong;
#endif

template <typename T>
using List = std::vector<T>;
template <class _Ty, size_t _size>
using Array = std::array<_Ty, _size>;
template <typename Key, typename Value>
using TreeMap = std::map<Key, Value>;
template <typename Key, typename Value>
using MultiTreeMap = std::multimap<Key, Value>;
template <typename Key, typename Value>
using MultiHashMap = std::unordered_multimap<Key, Value>;
template <typename Key, typename Value, typename Hasher = std::hash<Key>>
using HashMap = std::unordered_map<Key, Value, Hasher>;
template <class T>
using TreeSet = std::set<T>;
template <typename T>
using HashSet = std::unordered_set<T>;
template <typename T>
using Deque = std::deque<T>;
template <typename T>
using Stack = std::stack<T>;
template <typename T>
using LinkedList = std::list<T>;

template <class FunctionType>
using Function = std::function<FunctionType>;
template <class... Types>
using Tuple = std::tuple<Types...>;
template <typename ClassType>
using Atomic = std::atomic<ClassType>;
template <typename FirstClass, typename SecondClass>
using Pair = std::pair<FirstClass, SecondClass>;
template <typename T>
using Optional = std::optional<T>;
template <size_t t_NumBits>
using Bitset = std::bitset<t_NumBits>;
template <class t_ElemType>
using InitializerList = std::initializer_list<t_ElemType>;
template <typename T>
using Complex = std::complex<T>;

// Memory
template <typename T>
using Unique = std::unique_ptr<T>;
template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T>
using Weak = std::weak_ptr<T>;

using String = std::string;
using WString = std::wstring;
using Path = std::filesystem::path;
using DirEntry = std::filesystem::directory_entry;
using OFileStream = std::ofstream;
using IFileStream = std::ifstream;
using OStream = std::ostream;
using IStream = std::istream;
using StringStream = std::stringstream;
using OStringStream = std::ostringstream;
using IStringStream = std::istringstream;
using Thread = std::thread;
using Mutex = std::mutex;
using ConditionVariable = std::condition_variable;


template <class MoveClass>
constexpr auto Move(MoveClass&& moveClass) -> auto&&
{
	return static_cast<std::remove_reference_t<MoveClass>&&>(moveClass);
}

template <class FirstClass, class SecondClass>
constexpr auto CreatePair(FirstClass&& first, SecondClass&& second)
{
	return std::make_pair(first, second);
}


// Memory

template <typename T, typename ... Args>
constexpr auto CreateUnique(Args&& ... args) -> Unique<T>
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
constexpr auto CreateShared(Args&& ... args) -> std::shared_ptr<T>
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}
}
