# pwm-fan-ctrl

## build & flash

```shell
mkdir build && cd build
cmake ..
cmake --build . --target flash
```

## fuse bits

```shell
avrdude -p t85 -c usbtiny -U lfuse:w:0xce:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
```
