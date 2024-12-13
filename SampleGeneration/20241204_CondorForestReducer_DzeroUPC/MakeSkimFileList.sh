#!/bin/bash

XROOTD_SERVER=$1
T2_INPUT_DIR=$2
FILELIST=$3

# WARNING! ONLY USE THIS FOR xrdfs FUNCTIONS, NEVER FOR TRANSFERS!!!
XROOTD_FS=(
  "root://xrootd3.cmsaf.mit.edu/"
  "root://xrootd4.cmsaf.mit.edu/"
  "root://xrootd5.cmsaf.mit.edu/"
  "root://xrootd6.cmsaf.mit.edu/"
  "root://xrootd7.cmsaf.mit.edu/"
  "root://xrootd8.cmsaf.mit.edu/"
  "root://xrootd9.cmsaf.mit.edu/"
)
RND=$((RANDOM % 7))

# Make list of all files in parent dir
rm $FILELIST
xrdfs ${XROOTD_FS[$RND]} ls -R $T2_INPUT_DIR >> $FILELIST
wait
if [ -z "$FILELIST" ]; then
  echo "No files found in remote directory: $T2_INPUT_DIR"
  exit 1
else
  echo "Building file list for remote directory: $T2_INPUT_DIR"
fi
# Temp file for valid paths
TEMPLIST=$(mktemp)
FILE_COUNTER=0
# Keep only paths ending with '.root'
while read -r LINE; do
  if [[ "$LINE" =~ \.root$ ]]; then
    echo "$XROOTD_SERVER/$LINE" >> "$TEMPLIST"
    wait
    FILE_COUNTER=$((FILE_COUNTER + 1))
    if ! (( $FILE_COUNTER % 100 )); then
      echo "Found $FILE_COUNTER files..."
    fi
  else
    continue
  fi
done < $FILELIST
# Replace the original file with the temp file
mv "$TEMPLIST" "$FILELIST"
wait

echo "Found $FILE_COUNTER root files in $T2_INPUT_DIR"
