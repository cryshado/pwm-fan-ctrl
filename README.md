## pwm-fan-ctrl

avrdude -p t85 -c usbtiny -U lfuse:w:0xE2:m

-U lfuse:w:0x4e:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

avrdude -p t85 -c usbtiny -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

avrdude -p t85 -c usbtiny -U lfuse:w:0xce:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
