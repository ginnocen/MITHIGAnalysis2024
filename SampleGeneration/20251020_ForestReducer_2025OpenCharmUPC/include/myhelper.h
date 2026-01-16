
namespace myhelper {

  std::string find_tree_from_list(TFile* inf, const std::vector<std::string>& candidates);
  std::string to_lower(const std::string& str);
}

std::string myhelper::find_tree_from_list(TFile* inf,
                                          const std::vector<std::string>& candidates) {
  for (const auto& t : candidates) {
    if (inf->Get(t.c_str())) {
      std::cout<<__FUNCTION__<<": \e[1;4m"<<t<<"\e[0m is used."<<std::endl;
      return t;
    }
  }
  std::cout<<"\e[31m"<<__FUNCTION__<<": no tree is found among ";
  for (const auto& t : candidates) std::cout<<"\e[4m"<<t<<"\e[31;0m ";
  std::cout<<"\e[0m"<<std::endl;
  return "";
}

std::string myhelper::to_lower(const std::string& str) {
  std::string lowerStr = str;
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
  return lowerStr;
}
