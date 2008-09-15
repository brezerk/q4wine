#!/bin/sh

echo "Project WARNING: system(./config.sh) for generating config.h"
cat ./config.h.in > ./config.h
echo -e "#define APP_VERS \"$1\"" >> ./config.h
echo -e "#define APP_PREF \"$2\"\n" >> ./config.h
echo -e "#endif\n" >> ./config.h
