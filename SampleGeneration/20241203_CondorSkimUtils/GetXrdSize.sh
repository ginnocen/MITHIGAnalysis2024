#!/bin/bash

# Default values (will be used if no args given)
XROOTD_SERVER_DEF="root://xrootd.cmsaf.mit.edu"
REMOTE_DIR_DEF="/store/user/jdlang/run3_2023ReReco_Skims/"

# Overwrite if run with args
XROOTD_SERVER=${1:-$XROOTD_SERVER_DEF}
REMOTE_DIR=${2:-$REMOTE_DIR_DEF}

# Make dated list of all files in given directory
DATE=$(date +%Y%m%d)
FILE_LIST="filelist_$(basename $REMOTE_DIR)_${DATE}.txt"
rm $FILE_LIST
xrdfs $XROOTD_SERVER ls -R -l $REMOTE_DIR >> $FILE_LIST
if [ -z "$FILE_LIST" ]; then
    echo "No files found in the remote directory: $REMOTE_DIR"
    exit 1
fi
# Temp file for valid paths
TEMP_FILE=$(mktemp)
# Keep only paths ending with '.root'
while IFS= read -r line; do
  if [[ "$line" =~ \.root$ ]]; then
    echo "$line" >> "$TEMP_FILE"
  fi
done < $FILE_LIST
# Replace the original file with the temp file
mv "$TEMP_FILE" "$FILE_LIST"

# Calculate the total size, file by file
TOTAL_SIZE=0
LINE_COUNTER=0
while read -r LINE; do
    # Extract the size field (4th column from `xrdfs ls -R -l`)
    SIZE=$(echo "$LINE" | awk '{print $4}')
    if [[ "$SIZE" =~ ^[0-9]+$ ]]; then
      TOTAL_SIZE=$((TOTAL_SIZE + SIZE))
    fi
    if ! (( $LINE_COUNTER % 1000 )); then
      echo "$LINE_COUNTER: $TOTAL_SIZE"
    fi
    LINE_COUNTER=$((LINE_COUNTER + 1))
done < $FILE_LIST

# Convert to human-readable format
HUMAN_SIZE=$(numfmt --to=iec-i --suffix=B $TOTAL_SIZE)
echo "Total size of the folder $REMOTE_DIR: $HUMAN_SIZE ($TOTAL_SIZE)"
