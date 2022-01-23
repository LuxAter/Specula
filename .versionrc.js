const VERSION_REGEX = /project\(([^)]+)VERSION ([0-9\.]+)([^)]+)\)/s;
const cmake = {
  filename: "CMakeLists.txt",
  updater: {
    readVersion: (contents) => {
      if ((m = contents.match(VERSION_REGEX)) !== null) {
        return m[2];
      }
    },
    writeVersion: (contents, version) => {
      return contents.replace(
        VERSION_REGEX,
        (_m, pre, _ver, post) => `project(${pre}VERSION ${version}${post})`
      );
    },
  },
};

module.exports = {
  bumpFiles: [cmake],
  packageFiles: [cmake],
  types: [
    {
      type: "build",
      section: "Features",
    },
    {
      type: "ci",
      hidden: true,
    },
    {
      type: "chore",
      hidden: true,
    },
    {
      type: "docs",
      hidden: true,
    },
    {
      type: "feat",
      hidden: true,
    },
    {
      type: "fix",
      section: "Fix",
    },
    {
      type: "perf",
      hidden: true,
    },
    {
      type: "refactor",
      hidden: true,
    },
    {
      type: "revert",
      hidden: true,
    },
    {
      type: "style",
      hidden: true,
    },
    {
      type: "test",
      hidden: true,
    },
  ],
};
