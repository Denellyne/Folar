#!/bin/bash
cd "$(dirname "$0")"
mkdir ./Rules/Expressions/ >/dev/null 2>&1
cd ./Rules/Expressions
rules="../rules.h"
echo "#pragma once" > $rules

while IFS= read -r line
do
  
  echo "#include \"Expressions/"$file"\"" >> $rules
  file="$line"".h"
  #if test -f $file; then
   # file=/dev/null 2>&1
  #fi
  touch $file
  echo "#pragma once" > $file
  echo "#include \"../../../Tokens/tokens.h\"" >> $file
  echo "struct "$line"{" >> $file
  echo $line"(token tk,"; >> $file
  IFS= read -r args
  if [ args == 2 ]; then
    IFS= read -r arg1 arg2
  else  
    IFS= read -r arg1
  fi
  echo "};" >> $file
  echo $line" expression header generated"
done < ../rules.txt
