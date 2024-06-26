# esp-davang

Custom classes to wrap around esp-idf. The main idea is not make it faster neither lighter but try to use `c++` features to ease future implementations.
For example compile checking validity of peripherals configurations. Improve memory safety like using `std::string` instead of `char*`.

As with esp-idf an export script is given to ease its use. It exports two variables, `DAVANG_ESP` and `DAVANG_COMPONENT_DIRS`.

* `DAVANG_ESP` : the path to the esp-davang directory base of the repository.
* `DAVANG_COMPONENT_DIRS` : the path to the components directory base of the.

I know writting a warp over an already HAL/BSP libary makes no sense, as for that reason you may use the existing one already.
My classes remove some funciontality, will ad some overhead to the code, and not be as tested as esp-idf.
So why did I do it? beacuase I do not use all the functionlay mos of the time, and run time errors are a pain in the ass.
I used this classes for a few time, and they are helpfull to me, I hope they to you too.

## Components

This directory has all componets, each have a unique folder with _dvng_ as prefix this way they may nor result in conflict with any other component.
Add `list(APPEND EXTRA_COMPONENT_DIRS "${DAVANG_COMPONENT_DIRS}")` to your project.

## Examples

Just what the name says it is, examples.

---

Davang