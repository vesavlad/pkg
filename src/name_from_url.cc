#include "pkg/name_from_url.h"

#include "utl/parser/util.h"

#include "pkg/dep.h"

namespace pkg {

std::string name_from_url(std::string const& url) {
  if (url == ROOT) {
    return ROOT;
  }

  auto const slash_pos = url.find_last_of('/');
  auto const dot_pos = url.find_last_of('.');
  verify(slash_pos != std::string::npos, "no slash in url");
  verify(dot_pos != std::string::npos, "no dot in url");
  verify(slash_pos < dot_pos, "slash and dot in wrong order");
  return url.substr(slash_pos + 1, dot_pos - slash_pos - 1);
}

}  // namespace pkg