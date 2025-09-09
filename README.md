# esp-davang

Custom classes to wrap around esp-idf. The main idea is not make it faster neither lighter but try to use `c++` features to ease future implementations.
For example compile checking validity of peripherals configurations. Avoid using raw pointers and use the Standard Template Library and custom classes as parameters.

As with esp-idf an export script is given to ease its use. It exports two variables, `DAVANG_ESP` and `DAVANG_COMPONENT_DIRS`.

* `DAVANG_ESP` : the path to the esp-davang directory base of the repository.
* `DAVANG_COMPONENT_DIRS` : the path to the components directory base of the.

It also add the tools directory to you path so you may run any tool within it.

I know writting a warp over an already HAL/BSP libary makes no sense, as for that reason you may use the existing one already.
My classes remove some funciontality, will ad some overhead to the code, and not be as tested as esp-idf.
So why did I do it? beacuase I do not use all the functionlay mos of the time, and run time errors are a pain in the ass.
I used this classes for a few time, and they are helpfull to me, I hope they to you too.

## Components

This directory has all componets, each have a unique folder with _dvng\__ as prefix this way they may nor result in conflict with any other component.
Check [esp-idf explanantion](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32/migration-guides/release-5.x/5.0/build-system.html#setting-component-dirs-and-extra-component-dirs-variables) on how to add extra directories for component search. Then it is possible to list any of this components as it were one from the esp-idf.

## Style

I use uncrustify for styling. Check the [configuration file](tools/etc/uncrustify.cfg). Run [`dvng_format.sh`](tools/dvng_format.sh) for formatting the files.

## Linux

Source [export.sh](./export.sh). Then try to compile one of the [examples](examples/). 

ESP-IDF recommends setting the [get_idf alias](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32/get-started/linux-macos-setup.html#step-3-set-up-the-tools). It can expanded to export all required developement enviroments.

```bash
alias get_idf='
. $HOME/esp/esp-idf/export.sh
. $HOME/esp/esp-davang/export.sh
'
```

---

Davang