#!/bin/bash

# xrootd server (e.g. "root://xrootd.cmsaf.mit.edu/")
XROOTD_SERVER=$1
# Path to dir with files on server (can have subdirs)
SOURCE_DIR=$2
# Filename for output list
FILE_LIST=$3

# Make list of all files in parent dir
rm $FILE_LIST
wait
xrdfs $XROOTD_SERVER ls -R $SOURCE_DIR >> $FILE_LIST
wait
if [ -z "$FILE_LIST" ]; then
  echo "No files found in remote directory: $SOURCE_DIR"
  exit 1
else
  echo "Building file list for remote directory: $SOURCE_DIR"
fi
# Temp file for valid paths
TEMP_LIST=$(mktemp)
FILE_COUNTER=0
# Keep only paths ending with '.root'
while read -r LINE; do
  if [[ "$LINE" =~ \.root$ ]]; then
    echo "$XROOTD_SERVER/$LINE" >> "$TEMP_LIST"
    wait
    FILE_COUNTER=$((FILE_COUNTER + 1))
    if ! (( $FILE_COUNTER % 100 )); then
      echo "Found $FILE_COUNTER files..."
    fi
  else
    continue
  fi
done < $FILE_LIST
# Replace the original file with the temp file
rm $FILE_LIST
mv "$TEMP_LIST" "$FILE_LIST"
wait

echo "Found $FILE_COUNTER root files in $SOURCE_DIR"
