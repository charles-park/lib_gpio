# lib_gpio
gpio control lib

```
Usage: ./lib_gpio [-p:gpio no] [-d:out|in] [-o:1|0]

  -p --gpio number        Control Device node
  -d --gpio direction     out:output, in:input (default input)
  -o --gpio set value     if direction is out, set data (1 or 0)

  e.g) gpio 250 set high.
       lib_gpio -p 250 -d out -o 1
```
