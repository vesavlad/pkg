#include "pkg/status.h"

#include <sstream>
#include <string>

#include "pkg/exec.h"
#include "pkg/git.h"

namespace fs = boost::filesystem;

namespace pkg {

struct git_status {
  explicit git_status(dep const* d) {
    std::stringstream ss;
    ss.str(exec(d->path_, "git status --short").out_);

    std::string line;
    while (std::getline(ss, line)) {
      changed_.emplace_back(line);
    }
  }
  bool clean() const { return changed_.empty(); }
  std::vector<std::string> changed_;
};

std::map<dep*, status> get_status(std::vector<dep*> const& deps) {
  std::map<dep*, status> dep_status;
  for (auto const& d : deps) {
    auto const s = git_status(d);
    auto const current_commit = get_commit(d->path_);
    auto const commited = current_commit != d->commit_;
    if (!s.clean() || commited) {
      dep_status[d].commited_change_ = commited;
      dep_status[d].uncommited_change_ = !s.clean();
      if (commited) {
        dep_status[d].new_commit_ = current_commit;
        for (auto const& pred : d->recursive_preds()) {
          dep_status[pred].recursive_change_ = true;
        }
      }
    } else {
      dep_status[d];
    }
  }
  return dep_status;
}

}  // namespace pkg