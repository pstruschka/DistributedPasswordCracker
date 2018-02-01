#include "threaded_crpyt.h"
#include "crypt_data.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>

static const unsigned int num_threads = std::thread::hardware_concurrency();

constexpr char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

constexpr unsigned long length(const char* str)
{
  return *str ? 1ul + length(str + 1) : 0;
}

constexpr auto base = length(charset);

void incrementString(std::string& str)
{
  for (auto i = str.rbegin(); i != str.rend(); i++) {
    if (*i == 'Z')
      *i = 'a';
    else if (*i == 'z')
      *i = '0';
    else if (*i != '9')
      *i = (*i) + (char)1;
    else {
      *i = 'A';
      continue;
    }
    return;
  }
  str.insert(0, 1, 'A');
}

template<int I>
constexpr unsigned long getMax()
{
  static_assert(I > 0 && I < 5, "too many permutations");
  return base * getMax<I - 1>();
}

template<>
constexpr unsigned long getMax<0>()
{
  return 1ul;
}

unsigned long getMax(int n)
{
  switch(n) {
  case 1:
  {
    constexpr auto res = getMax<1>();
    return res;
  }
  case 2:
  {
    constexpr auto res = getMax<2>();
    return res;
  }
  case 3:
  {
    constexpr auto res = getMax<3>();
    return res;
  }
  case 4:
  {
    constexpr auto res = getMax<4>();
    return res;
  }
  default:
    throw std::runtime_error("length too long");
  }

}

template<int I>
std::string getPermutation(const unsigned long num)
{
  static_assert(I > 0 && I < 5, "too many permutations");
  constexpr unsigned long max = getMax<I>();
  unsigned long n = std::min(num, max-1);
  unsigned int idx = 0;
  std::string str;
  while (n > 0) {
    str.insert(0, 1, charset[n%base]);
    n /= base;
    idx++;
  }
  while (idx < I) {
    str.insert(0,1,'A');
    idx++;
  }
  return str;
}

std::string getPermutation(const unsigned long num, const int length)
{
  switch(length) {
  case 1:
    return getPermutation<1>(num);
  case 2:
    return getPermutation<2>(num);
  case 3:
    return getPermutation<3>(num);
  case 4:
    return getPermutation<4>(num);
  default:
    throw std::runtime_error("length too long");
  }
}

bool stringIsValid(const std::string& str)
{
  return std::find_if(str.begin(), str.end(), isalnum) != str.end();
}

std::atomic<bool> isComplete(false);

bool crack(std::string str, const std::string& end, const std::string& target, const std::string& salt) {

  crypt_data data = crypt_data();
  while (str != end && !isComplete.load()) {

    auto hashed_c = crypt_threaded(str.c_str(), salt.c_str(), data.get_address());
    std::string hashed(hashed_c);
    if (hashed == target) {
      isComplete.store(true);
      std::cout << str << ' ' << hashed << '\n';
      return true;
    }
    incrementString(str);
  }
  return false;
}

typedef std::vector<std::pair<std::string, std::string>> RangeList;
RangeList getRanges()
{
  RangeList ranges{};
  ranges.reserve(num_threads);

  auto maxVal = getMax(4);
  auto partitionSize = (maxVal/num_threads);
  unsigned long partition = 0ul;

  for (unsigned int i = 0; i < num_threads; ++i) {
    ranges.emplace_back("h" + getPermutation(partition, 4), "h" + getPermutation(partition+partitionSize-1, 4));
    std::cout << "h" + getPermutation(partition, 4) << "h" + getPermutation(partition+partitionSize-1, 4) << '\n';
    partition += partitionSize;
  }
  return ranges;
}

int main(const int argc, const char* argv[])
{
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " <begin> <end> <hash>\n";
    return 1;
  }
  std::string str = argv[1];
  const std::string end = argv[2], target = argv[3];

  if (!stringIsValid(str)) {
    std::cout << "<begin> must be an alphanumeric string\n";
    return 1;
  } else if (!stringIsValid(end)) {
    std::cout << "<end> must be an alphanumeric string\n";
    return 1;
  } else if (target.size() != 13) {
    std::cout << "<hash> must be a valid DES hash\n";
    return 1;
  }

  auto salt = target.substr(0, 2);

  RangeList ranges = getRanges();
  std::cout << ranges.size() << '\n';

  std::vector<std::thread> threads(num_threads);

  for (unsigned int i = 0; i < num_threads; ++i) {
    threads.at(i) = std::thread(crack, ranges[i].first, ranges[i].second, target, salt);
    std::cout << "Started Thread " << i << " with " << ranges[i].first << " & " << ranges[i].second << "\n";
  }

  for (auto& t : threads)
    t.join();

  std::cout << isComplete.load() << '\n';
  return 0;
}
