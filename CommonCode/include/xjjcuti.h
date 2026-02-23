#pragma once

/* xjjcuti.h */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <string>
#include <cmath>
#include <vector>
#include <cxxabi.h>
#include <algorithm>
#include <regex>
#include <ctime>
#include <cctype>
#include <sstream>
#include <chrono>
#include <random>

#define MAKE_HAS_METHOD(method)                                         \
  template <typename T, typename = void>                                \
  struct has_method_##method : std::false_type {};                      \
  template <typename T>                                                 \
  struct has_method_##method<T, std::void_t<decltype(std::declval<const T&>().method())>> \
    : std::true_type {};

namespace xjjc
{
  const std::vector<std::string> speciallist = { " ", "/", "(", ")", "^", "#", "%", "$", ",", ".", "*", "&", ":", "{", "}", ";", "|" };

  template<typename T, size_t N> void init_array(T (&array)[N], T value = T{});

  template<typename T, size_t N> int find_ibin(const T (&array)[N], T value); // overflow: -1
  template<typename T> int find_ibin(const std::vector<T> &array, T value); // overflow: -1
  template<typename T, size_t N> int find_iedge(const T (&array)[N], T value); // overflow: -1
  template<typename T> int find_iedge(const std::vector<T> &array, T value); // overflow: -1

  template<typename T> std::string number_to_string(T param);
  float string_to_number(const std::string& param);
  template<typename T> std::string number_remove_zero(T param);
  template<typename T> std::string number_range_string(T val1, T val2, const std::string& var, const std::string& opt);
  template<typename T> std::string number_range_string(T val1, T val2, const std::string& var, T over1=0, T over2=1.e+3, const std::string& unit="");
  int number_digit(int i, int n);
  template<typename T> std::string to_string(const T& s);
  template<typename T> T str_convert(const std::string& s);
  template<typename T> std::vector<T> str_convert_vector(const std::string& s, const std::string& divider = ",");

  void progressbar(int index, const int& total, int step=10000, int morespace=0);
  void progressslide(int index, const int& total, int step=10000, char done='=', char yet='.', char arrow='>');
  void progressbar_summary(const int& total);

  template<typename T> char* gettype(T exp);

  template<class T> bool sortbydes(const T &a, const T &b) { return a > b; }
  template<class T1, class T2> bool sortbyfirst_des(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.first > b.first; }
  template<class T1, class T2> bool sortbyfirst_as(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.first < b.first; }
  template<class T1, class T2> bool sortbysecond_des(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.second > b.second; }
  template<class T1, class T2> bool sortbysecond_as(const std::pair<T1,T2> &a, const std::pair<T1,T2> &b) { return a.second < b.second; }
  // template<class T1, class T2> std::map<T1, T2>::iterator findbysecond(std::map<T1, T2>& map_, T2 value_);

  template<class T> void vec_append(std::vector<T>& a, const std::vector<T>& b) { a.insert(a.end(), b.begin(), b.end()); }
  template<class T1, class T2> std::vector<T2> vec_cast(const std::vector<T1>& a);
  
  std::string str_replaceall(const std::string& strs, const std::string& sub, const std::string& newsub);
  std::string str_replaceallspecial(const std::string& strs, const std::string& newsub = "_");
  std::string str_eraseall(const std::string& strs, const std::string& sub) { return str_replaceall(strs, sub, ""); }
  std::string str_erasestar(const std::string& strs, const std::string& sub); // e.g. sub = */ or .*
  std::string str_trim(const std::string& strs);
  std::vector<std::string> str_trim(const std::vector<std::string>& strs);
  bool str_contains(const std::string& str1, const std::string& str2) { return str1.find(str2) != std::string::npos; }
  bool str_startsby(const std::string& str1, const std::string& str2) { return str1.find(str2) == 0; }
  bool str_endsby(const std::string& str1, const std::string& str2) { return str1.ends_with(str2); }
  bool str_isnumber(const std::string& strs) { return (std::regex_match(strs, std::regex("-?[0-9]+([.][0-9]*)?")) || std::regex_match(strs, std::regex("-?[0-9]*[.][0-9]+"))); }
  bool str_isinteger(const std::string& strs) { return std::regex_match(strs, std::regex("-?[0-9]+")); }
  std::vector<std::string> str_divide(const std::string& str, const std::string& div);
  std::vector<std::string> str_divide_trim(const std::string& str, const std::string& div);
  std::vector<std::string> str_divide_once(const std::string& str, const std::string& div);
  std::vector<std::string> str_divide_once_back(const std::string& str, const std::string& div);

  std::string str_getdir(const std::string& filename);
  std::string str_tag_from_file(const std::string& filename) { return str_erasestar(str_erasestar(filename, "*/"), ".*"); }
  std::string str_tolower(const std::string& str);
  std::string str_toupper(const std::string& str);
  std::vector<std::string> str_wrap(const std::string& str, size_t n);
  std::vector<std::string> str_wrap(const std::vector<std::string>& str, size_t n);
  
  template <class T> using array2D = std::vector<std::vector<T>>;
  template <class T> using array3D = std::vector<std::vector<std::vector<T>>>;
  template<class T> std::vector<std::vector<T>> array2d(int n1, int n2);
  template<class T> std::vector<std::vector<std::vector<T>>> array3d(int n1, int n2, int n3);

  std::string current_time();
  std::string unique_str();

  void print_tab_base(const std::vector<std::vector<std::string>>& vstrs, uint8_t opt = 3);
  template<typename T> void print_tab(const std::vector<std::vector<T>>& vstrs, uint8_t opt = 3);
  template<typename T1, typename T2> void xjjc::print_tab(const std::map<T1, T2>& vstrs, uint8_t opt = 3);
  template<typename T> void print_vec_v(const std::vector<T>& vstrs, uint8_t opt = 1);
  template<typename T> void print_vec_h(const std::vector<T>& vstrs, uint8_t opt = 1);

  void prt_divider(const std::string& color="\e[0m", int len=35) { std::cout<<color<<std::string(len, '-')<<"\e[0m"<<std::endl; }
}

/* ---------- */

template<typename T, size_t N>
void xjjc::init_array(T (&array)[N], T value/* = T{}*/) {
  for(std::size_t i = 0; i < N; ++i)
    array[i]  = value;
}

template<typename T, size_t N>
int xjjc::find_ibin(const T (&array)[N], T value) {
  for (std::size_t i = 0; i + 1 < N; ++i) {
    if (value >= array[i] && value < array[i+1])
      return static_cast<int>(i);
  }
  return -1;
}

template<typename T>
int xjjc::find_ibin(const std::vector<T> &array, T value) {
  for(std::size_t i = 0; i < (array.size()-1); ++i) {
    if(value >= array[i] && value < array[i+1])
      return static_cast<int>(i);
  }
  return -1;
}

template<typename T, size_t N>
int xjjc::find_iedge(const T (&array)[N], T value) {
  for (std::size_t i = 0; i < N; ++i) {
    if (value == array[i])
      return static_cast<int>(i);
  }
  return -1;
}

template<typename T>
int xjjc::find_iedge(const std::vector<T> &array, T value) {
  for (std::size_t i = 0; i < array.size(); ++i) {
    if (value == array[i])
      return static_cast<int>(i);
  }
  return -1;
}

template<typename T>
std::string xjjc::number_to_string(T param) {
  std::string str;
  if(param<0) { str += "M"; param = std::abs(param); }
  str += std::to_string(param);
  std::size_t found = str.find('.');
  if(found==std::string::npos)
    return str;
  str.replace(found, 1, "p");
  while(*(str.end()-1)=='0' && *(str.end()-2)!='p' && !str.empty()) str.erase(str.end()-1);
  if(*(str.end()-1)=='0' && *(str.end()-2)=='p') str.erase(str.size()-2, 2);
  return str;
}

float xjjc::string_to_number(const std::string& param) {
  if (param.empty()) 
    throw std::invalid_argument("Empty string cannot be converted to number");
  std::string str = param;
  bool is_negative = false;
  if (str.front() == 'M') {
    is_negative = true;
    str.erase(0, 1);
  }
  std::replace(str.begin(), str.end(), 'p', '.');
  float result = 0.f;
  try {
    result = std::stof(str);
  } catch (const std::exception& e) {
    throw std::invalid_argument("Invalid numeric string: " + param);
  }
  if (is_negative) result = -result;
  return result;
}

template<typename T>
std::string xjjc::number_remove_zero(T param) {
  bool negative = param<0;
  if(negative) param = 0-param;
  std::string str = std::to_string(param);
  std::size_t found = str.find('.');
  if(found==std::string::npos) return str;
  while(*(str.end()-1)=='0') str.erase(str.end()-1);
  if(*(str.end()-1)=='.') str.erase(str.end()-1);
  if(negative) str = "-"+str;
  return str;
}

template<typename T>
std::string xjjc::number_range_string(T val1, T val2, const std::string& var, const std::string& opt) {
  std::string str = "";
  if(val1 == val2) { return str; }
  if(val1 < val2 && str_tolower(opt) == "fmax") { T valtmp_ = val1; val1 = val2; val2 = valtmp_; }
  if(val1 > val2 && str_tolower(opt) == "fmin") { T valtmp_ = val1; val1 = val2; val2 = valtmp_; }
  std::string sign = val1 < val2 ? "<" : ">";
  if(str_tolower(opt) != "fmax" && str_tolower(opt) != "fmin") { str += (number_remove_zero(val1) + " " + sign + " "); }
  str += var;
  str += (" "+sign+" "+number_remove_zero(val2)); 
  return str;
}

template<typename T>
std::string xjjc::number_range_string(T val1, T val2, const std::string& var, T over1/*=0*/, T over2/*=1.e+3*/, const std::string& unit/*=""*/) {
  auto str_unit = (unit.empty() ? "" : " "+unit);
  if (val2 >= over2) return number_range_string(val1, val2, var, "fmax") + str_unit;
  else if (val1 <= over1) return number_range_string(val1, val2, var, "fmin") + str_unit;
  return number_range_string(val1, val2, var, "") + str_unit;
}


int xjjc::number_digit(int i, int n) {
  if(n < 0) return 0;
  int i1 = i%((int)std::pow(10, n+1));
  if(n==0) return i1;
  int i2 = std::floor(float(i1/std::pow(10, n-1)));
  return i2;
}

template<typename T>
std::string xjjc::to_string(const T& s) {
  if constexpr (std::is_same_v<T, std::string>) {
    return s;
  } else if constexpr (std::is_same_v<T, char*[]>) {
    return std::string(s);
  } else if constexpr (std::is_same_v<T, bool>) {
    return (s ? "true" : "false");
  } else if constexpr (std::is_same_v<T, std::string>) {
    return s;
  } else if constexpr (std::is_arithmetic_v<T>) {
    return std::to_string(s);
  } else {
    static_assert(std::is_arithmetic_v<T>,
                  "xjjc::to_string only supports arithmetic types");
  }
  return "";
}

template<typename T>
T xjjc::str_convert(const std::string& s) {
  if constexpr (std::is_same_v<T, std::string>) {
    return s;
  } else if constexpr (std::is_same_v<T, bool>) {
    if (str_tolower(s) == "true") {
      return true;
    } else if (str_tolower(s) == "false") {
      return false;
    } else if (str_isinteger(s)) {
      return std::stoi(s);
    }
    return false;
  } else if constexpr (std::is_same_v<T, int>) {
    return s.empty() ? 0 : std::stoi(s);
  } else if constexpr (std::is_same_v<T, float>) {
    return s.empty() ? 0.f : std::stof(s);
  } else {
    static_assert(!sizeof(T*),
                  "error: unsupported type for xjjc::str_convert.");
  }
  return (T)0;
}

template<typename T>
std::vector<T> xjjc::str_convert_vector(const std::string& s, const std::string& divider) {
  std::vector<T> r;
  for (const auto& st : str_divide_trim(s, divider)) {
    r.push_back(str_convert<T>(st));
  }
  return r;
}

void xjjc::progressbar(int index, const int& total, int step, int morespace/*=0*/) {
  if(index%step==0 || index == total-1) {
    if(total > 0)
      std::cout << std::setiosflags(std::ios::left)
                << "  [ \e[36m" << std::setw(10+morespace) << index << "\e[0m"
                << " / " << std::setw(10+morespace) << total << " ] "
                << "\e[36m" << round(100.*index/total) << "%\e[0m" << "\r" << std::flush;
    else
      std::cout << std::setiosflags(std::ios::left)
                << "  [ \e[36m" << std::setw(10+morespace) << index << "\e[0m ]" << "\r" << std::flush;
  }
}

void xjjc::progressslide(int index, const int& total, int step, char done, char yet, char arrow) {
  if (index%step==0 || index == total-1) {
    if (index == total-1) index++;
    std::cout << std::setiosflags(std::ios::left) << "  [ \e[36m"
              << std::setw(21) << std::string(round(20.*index/total), done) + std::string(1, arrow)+std::string(20-round(20.*index/total), yet)
              << "\e[0m ] " << "\e[36m" << index << "\e[0m" << " / " << total << "\r" << std::flush;
  }
}

void xjjc::progressbar_summary(const int& total) {
  std::cout << std::endl << "  Processed " << "\e[1;31m" << total << "\e[0m event(s)." << std::endl;
}

template<typename T>
char* xjjc::gettype(T exp) {
  const std::type_info &ti = typeid(exp);
  int status = 0;
  char* humantypename = abi::__cxa_demangle(ti.name(), 0, 0, &status);
  return humantypename;
}

template<class T1, class T2>
std::vector<T2> xjjc::vec_cast(const std::vector<T1>& a) {
  std::vector<T2> result;
  result.reserve(a.size());
  for (const auto& i : a) {
    result.push_back(static_cast<T2>(i));
  }
  return result;
}

std::string xjjc::str_erasestar(const std::string& strs, const std::string& sub) {
  std::string realsub = str_replaceall(sub, "*", "");
  std::string result(strs), str(strs);
  if(sub.front() == '*') {
    size_t pos = str.find(realsub);
    while (str.find(realsub, pos+1) != std::string::npos) {
      pos = str.find(realsub, pos+1);
    }
    if(pos != std::string::npos)
      result.erase(0, pos+realsub.size());
  }
  else if(sub.back() == '*') {
    size_t pos = str.find(realsub); 
    if(pos != std::string::npos)
      result.erase(pos, str.size() - pos);
  }
  return result;
}

std::string xjjc::str_trim(const std::string& strs) {
  std::string result(strs), str(strs);
  size_t pos_front = str.find_first_not_of(" ");
  if(pos_front != std::string::npos && pos_front != 0)
    result.erase(0, pos_front);
  size_t pos_back = result.find_last_not_of(" ");
  if(pos_back != std::string::npos && pos_back != result.size()-1)
    result.erase(pos_back+1, str.size() - (pos_back+1));
  if(pos_front == std::string::npos && pos_back == std::string::npos)
    result = "";
  return result;
}

std::vector<std::string> xjjc::str_trim(const std::vector<std::string>& strs) {
  std::vector<std::string> result;
  for (auto& tt : strs) {
    result.push_back(str_trim(tt));
  }
  return result;
}

std::string xjjc::str_replaceall(const std::string& strs, const std::string& sub, const std::string& newsub) {
  std::string result(strs), str(strs);
  auto pos = str.find(sub, 0);
  size_t thispos = 0;
  while (pos != std::string::npos) {
    result.replace(pos + thispos, sub.size(), newsub);
    thispos += (newsub.size() - sub.size());
    pos = str.find(sub, pos+1);
  }
  return result;
}

std::string xjjc::str_replaceallspecial(const std::string& strs, const std::string& newsub) {
  std::string result(strs);
  for(auto& isp : speciallist) { result = xjjc::str_replaceall(result, isp, newsub); }
  return result;
}

std::vector<std::string> xjjc::str_divide(const std::string& str, const std::string& div) {
  std::vector<std::string> token;
  size_t lastpos = 0;
  size_t pos = str.find(div, 0);
  while (pos != std::string::npos) {
    std::string thistoken = str.substr(lastpos, pos-lastpos);
    token.push_back(thistoken);
    lastpos = pos + div.size();
    pos = str.find(div, pos + div.size());
  }
  if (lastpos != 0) { token.push_back(str.substr(lastpos, str.size()-lastpos)); }
  if (token.empty()) token.push_back(str);
  return token;
}

std::vector<std::string> xjjc::str_divide_trim(const std::string& str, const std::string& div) {
  auto token = str_divide(str, div);
  for (auto& tt : token) {
    tt = str_trim(tt);
  }
  return token;
}

std::vector<std::string> xjjc::str_divide_once(const std::string& str, const std::string& div) {
  std::vector<std::string> token;
  size_t pos = str.find(div);
  if (pos == std::string::npos) {
    token.push_back(str);
  } else {
    token.push_back(str.substr(0, pos));
    token.push_back(str.substr(pos + div.size()));
  }
  return token;
}

std::vector<std::string> xjjc::str_divide_once_back(const std::string& str, const std::string& div) {
  std::vector<std::string> token;
  size_t pos = str.find(div);
  if (pos == std::string::npos) {
    token.push_back(str);
  } else {
    token.push_back(str.substr(0, pos));
    token.push_back(str.substr(pos + div.size()));
  }
  return token;
}

std::string xjjc::str_getdir(const std::string& filename) {
  return str_replaceall(filename, str_divide(filename, "/").back(), "");
}

std::string xjjc::str_tolower(const std::string& str) {
  std::string newstr(str);
  std::transform(newstr.begin(), newstr.end(), newstr.begin(),
                 [](unsigned char c){ return std::tolower(c); });
  return newstr;
}

std::string xjjc::str_toupper(const std::string& str) {
  std::string newstr(str);
  std::transform(newstr.begin(), newstr.end(), newstr.begin(),
                 [](unsigned char c){ return std::toupper(c); });
  return newstr;
}

std::vector<std::string> xjjc::str_wrap(const std::string& str, size_t n) {
  std::vector<std::string> result;
  if (n == 0) return { str };
  for (std::size_t i = 0; i < str.size(); i += n) {
    result.emplace_back(str.substr(i, n));
  }
  return result;
}

std::vector<std::string> xjjc::str_wrap(const std::vector<std::string>& str, size_t n) {
  std::vector<std::string> result;
  if (n == 0) return str;
  for (const auto& s : str) {
    xjjc::vec_append(result, str_wrap(s, n));
  }
  return result;
}

std::string xjjc::current_time() {
  std::time_t t = std::time(0);   // get time now
  std::tm* now = std::localtime(&t);
  char chartime[29]; // yyyymmdd-hhmmss
  snprintf(chartime, 29, "%d%s%d%d-%d%d%d", now->tm_year+1900, (now->tm_mon>=9?"":"0"), now->tm_mon+1, now->tm_mday,
           now->tm_hour, now->tm_min, now->tm_sec);
  return std::string(chartime);
}

std::string xjjc::unique_str() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  
  // Generate a random number
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(1000, 9999);
  int randomNumber = distribution(generator);

  // Combine time and random number into a unique string
  std::ostringstream uniqueString;
  uniqueString << millis << "_" << randomNumber;

  return uniqueString.str();
}

void xjjc::print_tab_base(const std::vector<std::vector<std::string>>& vstrs, uint8_t opt) {
  //
  if (vstrs.empty()) return;
  bool has_hline = opt == 1 || opt == 3,
    has_vline = opt == 2 || opt == 3;

  size_t ncols = 0;
  for (const auto& row : vstrs)
    ncols = std::max(ncols, row.size());

  std::vector<size_t> widths(ncols, 0);
  for (const auto& row : vstrs) {
    for (size_t c = 0; c < row.size(); ++c) {
      widths[c] = std::max(widths[c], row[c].size());
    }
  }

  auto print_border = [&]() {
    std::cout << '+';
    for (size_t c = 0; c < ncols; ++c) {
      std::cout << std::string(widths[c] + 2, '-');
      if (c + 1 < ncols) std::cout << (has_vline?'+':'-');
    }
    std::cout << "+" << std::endl;
  };

  print_border();
  for (const auto& row : vstrs) {
    std::cout << '|';
    for (size_t c = 0; c < ncols; ++c) {
      std::string cell = (c < row.size()) ? row[c] : "";
      std::cout << ' ' << cell
                << std::string(widths[c] - cell.size(), ' ')
                << ' ';
      if (c + 1 < ncols) std::cout << (has_vline?'|':' ');
    }
    std::cout << "|" << std::endl;
    if (has_hline) print_border();
  }
  if (!has_hline) print_border();
}

template<typename T>
void xjjc::print_tab(const std::vector<std::vector<T>>& vstrs, uint8_t opt) {
  std::vector<std::vector<std::string>> a2d;
  for (const auto& str : vstrs) {
    a2d.push_back({});
    for (const auto& s : str)
      a2d.back().push_back(to_string(s));
  }
  print_tab_base(a2d, opt);
}

template<typename T1, typename T2>
void xjjc::print_tab(const std::map<T1, T2>& vstrs, uint8_t opt) {
  std::vector<std::vector<std::string>> a2d;
  for (const auto& str : vstrs) {
    a2d.push_back({ to_string(str.first), to_string(str.second) });
  }
  print_tab_base(a2d, opt);
}

template<typename T>
void xjjc::print_vec_v(const std::vector<T>& vstrs, uint8_t opt) {
  std::vector<std::vector<std::string>> a2d;
  for (const auto& str : vstrs) {
    a2d.push_back({ to_string(str) });
  }
  print_tab_base(a2d, opt?3:0);
}

template<typename T>
void xjjc::print_vec_h(const std::vector<T>& vstrs, uint8_t opt) {
  std::vector<std::vector<std::string>> a2d(1);
  for (const auto& str : vstrs) {
    a2d.back().push_back(to_string(str));
  }
  print_tab_base(a2d, opt?3:0);
}

template<class T> std::vector<std::vector<T>> xjjc::array2d(int n1, int n2) {
  std::vector<std::vector<T>> v(n1);
  for(auto& vi : v)
    vi.resize(n2);
  return v;
}

template<class T> std::vector<std::vector<std::vector<T>>> xjjc::array3d(int n1, int n2, int n3) {
  std::vector<std::vector<std::vector<T>>> v(n1);
  for(auto& vi : v) {
    vi.resize(n2);
    for(auto& vj : vi)
      vj.resize(n3);
  }
  return v;
}

#ifndef __PRMYERR
#define __PRMYERR(info) { std::cout<<"\e[42m("<<__FUNCTION__<<")\e[0m \e[31;1merror: \e[0m"<<#info<<std::endl; }
#endif
