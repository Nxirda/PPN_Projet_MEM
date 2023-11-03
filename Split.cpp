#include <string_view>
#include <iostream>
#include <cstring>
#include <vector>

std::vector<std::string> split(const std::string& str, char split_char) {

  const char* cur = str.c_str();
  const char* str_end = str.c_str() + str.size();

  std::vector<std::string> chunks;
  while (cur < str_end)
  {
    const char* start = cur;
    while (start < str_end and *start == split_char)
        start++;

    const char* end = start + 1;
    while (end < str_end and *end != split_char)
        end++;

    cur = end + 1;
    chunks.push_back(std::string(start, end - start));
  }
  return chunks;
}

int main() {
    for (auto c : split("1,2,3,4", ',')) std::cout << c << std::endl;
}

 

std::string_view next_chunk(const char*& cur, const char* line_end, char split_char) {

  const char* start = cur;
  while (start < line_end and *start == split_char)
    start++;

  const char* end = start + 1;
  while (end < line_end and *end != split_char)
    end++;

  cur = end + 1;
  return std::string_view(start, end - start);
}

int main() {

    const char* str = "1,2,3,4";
    const char* end = str + strlen(str);
    const char* it = str;
    while (it < end) {
        std::cout << next_chunk(it, end, ',') << std::endl;
    }

}