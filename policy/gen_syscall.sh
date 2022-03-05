#!/bin/bash

outname=syscall.polib

cat > $outname << EOF
EOF

while read -r line; do
  if [[ $line =~ ^([0-9]*)[[:space:]]*([a-z0-9][a-z0-9_]*)$ ]]; then
    printf "#define ${BASH_REMATCH[2]} ${BASH_REMATCH[1]}\n" >> $outname
  else 
    printf "ignore: ${line}\n"
  fi
done < <(ausyscall --dump)
