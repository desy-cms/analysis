#ifndef Analysis_BackgroundModel_TreeContainer_h
#define Analysis_BackgroundModel_TreeContainer_h 1

#include <string>
#include <vector>
#include <memory>
#include "TTree.h"


namespace analysis {
  namespace backgroundmodel {

    class TreeContainer {
    public:
      TreeContainer(const std::string& input);
      virtual ~TreeContainer();

      std::unique_ptr<TTree> data() const;
      void show() const;

    private:
      std::unique_ptr<TTree> getTree_(const std::string& name) const;
      static std::unique_ptr<TTree> uniqueClone_(const TTree& original);

      const std::string treeFileName_;
      std::unique_ptr<TTree> data_;
    };

  }
}

#endif  // Analysis_BackgroundModel_TreeContainer_h
