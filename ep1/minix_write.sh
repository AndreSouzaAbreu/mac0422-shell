#!/bin/bash

function show_help() {
  cat <<EOF
Description: Attach files to a MINIX VBox VM
Usage: $0 <filename>
EOF
}

# input validation
if [[ -z $1 ]]; then
  show_help
  exit 1;
fi

# settings
controller="IDE"
machine_name="MINIX-3.1.2a"
if="$1"
of="$if.iso"

# remove old iso files
rm *.iso

# make iso filesystem to transfer containg input file
mkisofs -o "$of" "$if"

# attach iso file to the machine
VBoxManage storageattach $machine_name \
  --storagectl $controller \
  --port 0 --device 1 \
  --type dvddrive --medium "$of"
