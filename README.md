# robosys_2016_task
driver to talk  
[![](http://img.youtube.com/vi/0EjzGRgsp0E/0.jpg)](https://www.youtube.com/watch?v=0EjzGRgsp0E)  

### Requirements
 * linux kernel source
 * download kernel source into `/usr/src/linux`
 * kernel build scripts : [https://github.com/ryuichiueda/raspberry_pi_kernel_build_scripts](https://github.com/ryuichiueda/raspberry_pi_kernel_build_scripts)
* tested only on Raspberry Pi 3

### Software
First, download this repository.

```
git clone https://github.com/masamasa9841/robosys_2016_task
```

Next, move into robosys2016_task directory and run make command.

```
cd robosys2016_task
make && make install
```
### Usage
```
./scripts/saito.sh
```
### License

This repository is licensed under the GPLv3 license, see [LICENSE](./LICENSE).
