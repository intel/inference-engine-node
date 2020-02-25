# Coding Style

Use `clang-format` and follow [Chromium coding style](https://chromium.googlesource.com/chromium/src/+/master/styleguide/c++/c++.md).

```sh
$ npm install -g clang-format
```

For C++ code, please refer to [Using clang-format on C++ Code](https://electronjs.org/docs/development/clang-format).
```sh
$ clang-format -i src/binding.cc
```

For JS code, please refer to [Formatting .js with clang-format](https://github.com/google/closure-library/wiki/Formatting-.js-with-clang-format).

```sh
$ clang-format -i -style=Google test/network.js
```