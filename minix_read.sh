#!/bin/sh
input="/dev/c0d1"
output="work"

mkdir -p $output

isodir -l $input | while read line; do 
  f=$(echo $line | awk '{print $NF}')
  if [ $f = "." ] || [ $f = ".." ] || [ $f = "_iso." ]; then
    continue
  fi
  isoread $input $f > "$output/$f"
done

echo "transferred files:"
ls $output

if [[ -f $output/Makfile ]]; then
  cd $output && make
fi
