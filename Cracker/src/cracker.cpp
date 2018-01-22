#include <crypt.h>
#include <iostream>
#include <cstring>
#include <algorithm>


bool stringIsValid(const std::string& str)
{
  return std::find_if(str.begin(), str.end(), isalnum) != str.end();
}

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

  auto salt = target.substr(0, 2).c_str();

  while (str != end) {
    std::string hashed = crypt(str.c_str(), salt);
    if (hashed == target){
      std::cout << str << ' ' << hashed << '\n';
      return 0;
    }
    incrementString(str);
  }
  return 1;
}
